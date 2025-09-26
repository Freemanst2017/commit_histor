
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <ctime>
 
 using std::cout;
 using std::endl;
 using std::string;
 
 struct Commit {
 int hash;
 std::string message;
 Commit* next;
 
 Commit(int h, const std::string& msg) : hash(h), message(msg), next(nullptr) {}
 };
 
 class CommitHistory {
 private:
 Commit* head;
 
 // Helper: deep copy from another list
 static Commit* copyList(const Commit* otherHead) {
 if (!otherHead) return nullptr;
 Commit* newHead = new Commit(otherHead->hash, otherHead->message);
 Commit* curNew = newHead;
 const Commit* curOld = otherHead->next;
 while (curOld) {
 curNew->next = new Commit(curOld->hash, curOld->message);
 curNew = curNew->next;
 curOld = curOld->next;
 }
 return newHead;
 }
 
 public:
 CommitHistory() : head(nullptr) {}
 
 // Rule of Three: copy ctor, assignment, destructor
 CommitHistory(const CommitHistory& other) {
 head = copyList(other.head);
 }
 
 CommitHistory& operator=(const CommitHistory& other) {
 if (this == &other) return *this;
 // delete existing
 this->~CommitHistory();
 head = copyList(other.head);
 return *this;
 }
 
 ~CommitHistory() {
 Commit* cur = head;
 while (cur) {
 Commit* nxt = cur->next;
 delete cur;
 cur = nxt;
 }
 head = nullptr;
 }
 
 // Append commit (simulates git commit)
 void commit(const std::string& message) {
 int hash = rand() % 100000; // simulated hash (0..99999)
 Commit* node = new Commit(hash, message);
 if (!head) {
 head = node;
 } else {
 Commit* cur = head;
 while (cur->next) cur = cur->next;
 cur->next = node;
 }
 cout << "Committed: [" << node->hash << "] " << node->message << endl;
 }
 
 // Display all commits as: [hash] message <- [hash] message <- ...
 void log() const {
 if (!head) {
 cout << "(no commits)" << endl;
 return;
 }
 const Commit* cur = head;
 // We'll print left-to-right with arrows pointing left as requested:
 // commit1 <-commit2 <- commit3 ...
 while (cur) {
 cout << "[" << cur->hash << "] " << cur->message;
 if (cur->next) cout << " <- ";
 cur = cur->next;
 }
 cout << endl;
 }
 
 // Delete last commit (simulates git reset --hard HEAD~1)
 void reset() {
 if (!head) {
 cout << "No commits to reset." << endl;
 return;
 }
 if (!head->next) {
 delete head;
 head = nullptr;
 cout << "Last commit removed (reset)." << endl;
 return;
 }
 Commit* cur = head;
 while (cur->next && cur->next->next) {
 cur = cur->next;
 }
 // cur->next is last
 delete cur->next;
 cur->next = nullptr;
 cout << "Last commit removed (reset)." << endl;
 }
 
 // Find commit by hash and print it (simulates git show)
 void gitShow(int hash) const {
 const Commit* cur = head;
 while (cur) {
 if (cur->hash == hash) {
 cout << "Commit found: [" << cur->hash << "] " << cur->message << endl;
 return;
 }
 cur = cur->next;
 }
 cout << "Commit with hash " << hash << " not found." << endl;
 }
 
 // Merge two histories: new list = copy(branch1) followed by copy(branch2).
 // Does not modify branch1 or branch2.
 static CommitHistory merge(const CommitHistory& branch1, const CommitHistory& branch2) {
 CommitHistory result;
 // Copy branch1
 if (branch1.head) {
 result.head = copyList(branch1.head);
 } else {
 result.head = nullptr;
 }
 
 // Find tail of result
 Commit* tail = result.head;
 if (tail) {
 while (tail->next) tail = tail->next;
 }
 
 // Copy branch2 and append
 Commit* branch2Copy = copyList(branch2.head);
 if (!result.head) {
 // result was empty; just adopt branch2 copy
 result.head = branch2Copy;
 } else if (branch2Copy) {
 tail->next = branch2Copy;
 }
 
 cout << "Branches merged." << endl;
 return result;
 }
 
 // Optional: duplicate (clone) current branch and return it
 CommitHistory duplicate() const {
 CommitHistory copy;
 copy.head = copyList(this->head);
 return copy;
 }
 };
 
 // Provided main from lab with slight additional demonstration
 int main() {
 std::srand((unsigned)std::time(nullptr));
 
 CommitHistory master;
 master.commit("Initial commit");
 master.commit("Add README");
 master.commit("Implement login system");
 
 CommitHistory featureBranch;
 featureBranch.commit("Start feature X");
 featureBranch.commit("Fix bug in feature X");
 
 cout << "\n== Master Branch ==\n";
 master.log();
 
 cout << "\n== Feature Branch ==\n";
 featureBranch.log();
 
 cout << "\n== Reset last commit on master ==\n";
 master.reset();
 master.log();
 
 cout << "\n== Merged History ==\n";
 CommitHistory merged = CommitHistory::merge(master, featureBranch);
 merged.reset();
 merged.log();
 
 cout << "\n== Master Branch Unchanged after merge ==\n";
 master.log();
 
 cout << "\n== Feature Branch Unchanged after merge ==\n";
 featureBranch.log();
 
 // Demonstrate gitShow (search by hash) -- search for a hash from master
 cout << "\n== Demonstrate gitShow ==\n";
 
 CommitHistory copyMaster = master.duplicate();
 
 cout << "(To test gitShow: call gitShow(hash) with a hash printed earlier from commit messages.)\n";
 
 return 0;
 }
 
 
 
