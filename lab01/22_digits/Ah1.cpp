#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

#define N 1024
#define SIZE 5

typedef struct Node{
	string actions;
	int gkey;
	char **status;
} *PNode;

typedef struct State {
	char **str;
	unsigned int hashcode;
	State *next;
} *PState;

struct HashNode {
	PState head;
};

HashNode* HTable = new HashNode[N];
int row, col;

int parent(int i) {
	return (i + 1) / 2 - 1;
}

int left(int i) {
	return 2 * i + 1;
}

int right(int i) {
	return 2 * i + 2;
}

template <typename T>
void myswap (T &a, T &b) {
	T tmp = a;
	a = b;
	b = tmp;
}

void MinHeapify(vector<PNode> &A, int i) {
	int l = left(i);
	int r = right(i);
	int smallest;
	if (l < A.size() && A[l]->gkey < A[i]->gkey) {
		smallest = l;
	}
	else {
		smallest = i;
	}
	if (r < A.size() && A[r]->gkey < A[smallest]->gkey) {
		smallest = r;
	}
	if (smallest != i) {
		myswap(A[i], A[smallest]);
		MinHeapify(A, smallest);
	}
}

void InsertNode(vector<PNode> &A, PNode e) {
	A.push_back(e);
	int i = A.size() - 1;
	while (i > 0 && A[parent(i)]->gkey > A[i]->gkey) {
		myswap(A[i], A[parent(i)]);
		i = parent(i);
	}
}

PNode ExtractMin(vector<PNode> &A) {
	if (A.size() < 1) {
		cout << "Heap Underflow!" << endl;
	}
	PNode min = A[0];
	int s = A.size();
	A[0] = A[s - 1];
	A.erase(A.end() - 1);
	MinHeapify(A, 0);
	return min;
}

void OutputQueue(vector<PNode> &A) {
	cout << "Queue" << endl;
	for (size_t i = 0; i < A.size(); i ++) {
		cout << "|" + A[i]->actions + " |" << A[i]->gkey << endl;
	}
}

bool StatusCompare(char **a, char **b) {
	for (int i = 0; i < SIZE; i ++) {
		for (int j = 0; j < SIZE; j++){
			if (a[i] != b[i])
			{
				return false;
			}
		}
	}
	return true;
}

unsigned int Hash(char *str)
{//BKDRHash
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

bool InsertHashTable(char **status) {
	unsigned int h = Hash(status[0]);
	int index = h % N;
	PState p = HTable[index].head, q;
	if (p == nullptr) {
		p = new State;
		p->str = status;
		p->hashcode = h;
	}
	else {
		if(p->hashcode < h) {
			q = p;
			p = p->next;
			while(p != nullptr) {
				if (p->hashcode < h) {
					q = p;
					p = p->next;
				}
				else if (p->hashcode == h) {
					if(StatusCompare(p->str, status)) {
						return false;
					}
					q = p;
					p = p->next;
				}
				else {
					break;	
				}
			}
			PState r = new State;
			r->str = status;
			r->hashcode = h;
			r->next = p;
			q->next = r;
		}
		else {
			PState r = new State;
			r->str = status;
			r->hashcode = h;
			r->next = p;
			HTable[index].head = r;
		}
	}
	return true;
}

void FreeHashTable() {
	for (int i = 0; i < N; i ++) {
		PState p = HTable[i].head, q;
		while(p != nullptr) {
			q = p;
			p = p->next;
			delete q;
		}
	}
}

bool GoalTest(char **digits, char **target) {
	for (int i = 0; i < SIZE; i ++) {
		for (int j = 0; j < SIZE; j ++) {
			if (digits[i][j] != target[i][j])
				return false;
		}
	}
	return true;
}

int Heuristic(char **digits, char **target) {
	int count = 0;
	for (int i = 0; i < SIZE; i ++) {
		for (int j = 0; j < SIZE; j ++) {
			if (digits[i][j] != target[i][j])
				count++;
		}
	}
	return count;
}

void FreeAllNode(vector<PNode> &Q) {
	PNode n;
	for (size_t i = 0; i < Q.size(); i ++) {
		delete Q[i];
	}
}

void LocatChess0(char **digitable){
	for (row = 0; row < SIZE; row ++) {
		for (col = 0; col < SIZE; col ++) {
			if(digitable[row][col] == 0) {
				return;
			}
		}
	}
	row = -1;
	return;
}

void Move(char **parent, char **child, char s) {
	for (int i = 0; i < SIZE; i ++){
		for (int j = 0; j < SIZE; j ++){
			child[i][j] = parent[i][j];
		}
	}
	//cout << s << endl;
/* 	for (int i = 0; i < 5; i ++){
		for (int j = 0; j < 5; j ++){
			cout << (int)child[i][j] << " ";
		}
		cout << endl;
	}  */
	if (s == 'R') {
		if (child[row][col + 1] == -1) {
			myswap(child[row][col], child[row][col + 2]);
		}
		else {
			myswap(child[row][col], child[row][col + 1]);
		}
	}
	else if (s == 'L') {
		if (child[row][col - 1] == -1) {
			myswap(child[row][col], child[row][col - 2]);
		}
		else {
			myswap(child[row][col], child[row][col - 1]);
		}
	}
	else if (s == 'U') {
		myswap(child[row][col], child[row - 1][col]);
	}
	else if (s == 'D') {
		myswap(child[row][col], child[row + 1][col]);
	}
}

PNode Astarh1(char **start, char ** target){
	vector<PNode> Q;
	PNode newNode;
	int cnt = 0;

	LocatChess0(start);
	if(row == -1) {
		cout << "input wrong file !" << endl;
		exit(0);
	}

	newNode = new Node;
	newNode->actions = "";
	newNode->status = start;
	newNode->gkey = 0 + Heuristic(newNode->status, target);
	InsertHashTable(start);
	InsertNode(Q, newNode);
	while(!Q.empty()) {
		PNode rootNode = ExtractMin(Q);
		char **digits = rootNode->status;
		cnt++;
		//cout << "#" << rootNode->actions;
		if(GoalTest(rootNode->status, target)) {
			cout << "Finish!" << endl;
			FreeAllNode(Q);
			FreeHashTable();
			return rootNode;
		}
		LocatChess0(rootNode->status);
		//cout << "	row:" << row << " col:" << col << endl;
		if (row < 4 && digits[row + 1][col] != -1) {
			PNode DNode = new Node;
			DNode->status = new char*[SIZE];
			for (int i = 0; i < SIZE; i ++){
				DNode->status[i] = new char[SIZE];
			}
			DNode->actions = rootNode->actions + "D";
			Move(digits, DNode->status, 'D');
			DNode->gkey = DNode->actions.size() + Heuristic(DNode->status, target);
			if (InsertHashTable(DNode->status)) {
				InsertNode(Q, DNode);
			}
			else {
				delete DNode->status;
				delete DNode;
			}
		}
		if (row > 0 && digits[row - 1][col] != -1) {
			PNode UNode = new Node;
			UNode->status = new char*[SIZE];
			for (int i = 0; i < SIZE; i ++){
				UNode->status[i] = new char[SIZE];
			}
			UNode->actions = rootNode->actions + "U";
			Move(digits, UNode->status, 'U');
			UNode->gkey = UNode->actions.size() + Heuristic(UNode->status, target);
			if (InsertHashTable(UNode->status)) {
				InsertNode(Q, UNode);
			}
			else {
				delete UNode->status;
				delete UNode;
			}
		}
		if (col < 4) {
			PNode RNode = new Node;
			RNode->status = new char*[SIZE];
			for (int i = 0; i < SIZE; i ++){
				RNode->status[i] = new char[SIZE];
			}
			RNode->actions = rootNode->actions + "R";
			Move(digits, RNode->status, 'R');
			RNode->gkey = RNode->actions.size() + Heuristic(RNode->status, target);
			if (InsertHashTable(RNode->status)) {
				InsertNode(Q, RNode);
			}
			else {
				delete RNode->status;
				delete RNode;
			}
		}
		if (col > 0) {
			PNode LNode = new Node;
			LNode->status = new char*[SIZE];
			for (int i = 0; i < SIZE; i ++){
				LNode->status[i] = new char[SIZE];
			}
			LNode->actions = rootNode->actions + "L";
			Move(digits, LNode->status, 'L');
			LNode->gkey = LNode->actions.size() + Heuristic(LNode->status, target);
			if (InsertHashTable(LNode->status)) {
				InsertNode(Q, LNode);
			}
			else {
				delete LNode->status;
				delete LNode;
			}
		}
		//OutputQueue(Q);
		delete rootNode;
		/* if (cnt == 2) {
			exit(0);
		} */
	}
	return nullptr;
}

int main(){
	string inputpath = "input/input.txt", targetpath = "input/target.txt";
	vector<vector<int> > start(5, vector<int>(5, 0)), target(5, vector<int>(5, 0));
	char **s, **t;
	PNode result;
	ifstream fin;

	fin.open(inputpath);
	for (int i = 0; i < 5; i ++){
		for (int j = 0; j < 5; j ++){
			fin >> start[i][j];
		}
	}
	s = new char*[SIZE];
	for (int i = 0; i < SIZE; i ++){
		s[i] = new char[SIZE];
		for (int j = 0; j < SIZE; j ++){
			s[i][j] = (char)start[i][j];
		}
	}
	fin.close();
	fin.open(targetpath);
	for (int i = 0; i < 5; i ++){
		for (int j = 0; j < 5; j ++){
			fin >> target[i][j];
		}
	}
	t = new char*[SIZE];
	for (int i = 0; i < SIZE; i ++){
		t[i] = new char[SIZE];
		for (int j = 0; j < SIZE; j ++){
			t[i][j] = (char)target[i][j];
		}
	}
	fin.close();
	/* for (int i = 0; i < 5; i ++){
		for (int j = 0; j < 5; j ++){
			cout << (int)s[i][j] << " ";
		}
		cout << endl;
	} */
	for (int i = 0; i < N; i ++) {
		HTable[i].head = nullptr;
	}
	result = Astarh1(s, t);
	if (result != nullptr){
		cout << result->actions << endl;
	}
	else {
		cout << "no result!" << endl;
	}

	delete result;
	return 0;
}