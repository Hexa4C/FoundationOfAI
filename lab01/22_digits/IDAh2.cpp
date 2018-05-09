#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

#define N 1024
#define SIZE 5
#define INF 0x7fffffff

typedef struct Node{
	string actions;
	char hkey;
	char fkey;
	char **status;
} *PNode;

int row, col;
vector<vector<vector<int>>> HTable = {
	{{8, 7, 6, 5, 4, }, {7, 6, 5, 4, 3, }, {6, 5, 4, 3, 2, }, {5, 4, 3, 2, 1, }, {4, 3, 2, 1, 0, }, },
	{{0, 1, 2, 3, 4, }, {1, 2, 3, 4, 5, }, {2, 3, 4, 5, 6, }, {3, 4, 5, 6, 7, }, {4, 5, 6, 7, 8, }, },
	{{1, 0, 1, 2, 3, }, {2, 1, 2, 3, 4, }, {3, 2, 3, 4, 5, }, {4, 3, 4, 5, 6, }, {5, 4, 5, 6, 7, }, },
	{{2, 1, 0, 1, 2, }, {3, 2, 1, 2, 3, }, {4, 3, 2, 3, 4, }, {5, 4, 3, 4, 5, }, {6, 5, 4, 5, 6, }, },
	{{3, 2, 1, 0, 1, }, {4, 3, 2, 1, 2, }, {5, 4, 3, 2, 3, }, {6, 5, 4, 3, 4, }, {7, 6, 5, 4, 5, }, },
	{{4, 3, 2, 1, 0, }, {5, 4, 3, 2, 1, }, {6, 5, 4, 3, 2, }, {7, 6, 5, 4, 3, }, {8, 7, 6, 5, 4, }, },
	{{1, 2, 3, 4, 5, }, {0, 1, 2, 3, 4, }, {1, 2, 3, 4, 5, }, {2, 3, 4, 5, 6, }, {3, 4, 5, 6, 7, }, },
	{{2, 1, 2, 3, 4, }, {1, 0, 1, 2, 3, }, {2, 1, 2, 3, 4, }, {3, 2, 3, 4, 5, }, {4, 3, 4, 5, 6, }, },
	{{3, 2, 1, 2, 3, }, {2, 1, 0, 1, 2, }, {3, 2, 1, 2, 3, }, {4, 3, 2, 3, 4, }, {5, 4, 3, 4, 5, }, },
	{{4, 3, 2, 1, 2, }, {3, 2, 1, 0, 1, }, {4, 3, 2, 1, 2, }, {5, 4, 3, 2, 3, }, {6, 5, 4, 3, 4, }, },
	{{5, 4, 3, 2, 1, }, {4, 3, 2, 1, 0, }, {5, 4, 3, 2, 1, }, {6, 5, 4, 3, 2, }, {7, 6, 5, 4, 3, }, },
	{{2, 3, 4, 5, 6, }, {1, 2, 3, 4, 5, }, {0, 1, 2, 3, 4, }, {1, 2, 3, 4, 5, }, {2, 3, 4, 5, 6, }, },
	{{4, 3, 2, 3, 4, }, {3, 2, 1, 2, 3, }, {2, 1, 0, 1, 2, }, {3, 2, 1, 2, 3, }, {4, 3, 2, 3, 4, }, },
	{{6, 5, 4, 3, 2, }, {5, 4, 3, 2, 1, }, {4, 3, 2, 1, 0, }, {5, 4, 3, 2, 1, }, {6, 5, 4, 3, 2, }, },
	{{3, 4, 5, 6, 7, }, {2, 3, 4, 5, 6, }, {1, 2, 3, 4, 5, }, {0, 1, 2, 3, 4, }, {1, 2, 3, 4, 5, }, },
	{{4, 3, 4, 5, 6, }, {3, 2, 3, 4, 5, }, {2, 1, 2, 3, 4, }, {1, 0, 1, 2, 3, }, {2, 1, 2, 3, 4, }, },
	{{5, 4, 3, 4, 5, }, {4, 3, 2, 3, 4, }, {3, 2, 1, 2, 3, }, {2, 1, 0, 1, 2, }, {3, 2, 1, 2, 3, }, },
	{{6, 5, 4, 3, 4, }, {5, 4, 3, 2, 3, }, {4, 3, 2, 1, 2, }, {3, 2, 1, 0, 1, }, {4, 3, 2, 1, 2, }, },
	{{7, 6, 5, 4, 3, }, {6, 5, 4, 3, 2, }, {5, 4, 3, 2, 1, }, {4, 3, 2, 1, 0, }, {5, 4, 3, 2, 1, }, },
	{{4, 5, 6, 7, 8, }, {3, 4, 5, 6, 7, }, {2, 3, 4, 5, 6, }, {1, 2, 3, 4, 5, }, {0, 1, 2, 3, 4, }, },
	{{5, 4, 5, 6, 7, }, {4, 3, 4, 5, 6, }, {3, 2, 3, 4, 5, }, {2, 1, 2, 3, 4, }, {1, 0, 1, 2, 3, }, },
	{{6, 5, 4, 5, 6, }, {5, 4, 3, 4, 5, }, {4, 3, 2, 3, 4, }, {3, 2, 1, 2, 3, }, {2, 1, 0, 1, 2, }, },
	{{7, 6, 5, 4, 5, }, {6, 5, 4, 3, 4, }, {5, 4, 3, 2, 3, }, {4, 3, 2, 1, 2, }, {3, 2, 1, 0, 1, }, }
};

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

bool LessThan(PNode a, PNode b) {
	if (a->fkey < b->fkey) {
		return true;
	}
	else if(a->fkey == b->fkey && a->hkey < b->hkey) {
		return true;
	}
	else {
		return false;
	}
}

void MinHeapify(vector<PNode> &A, int i) {
	int l = left(i);
	int r = right(i);
	int smallest;
	if (l < A.size() && LessThan(A[l], A[i])) {
		smallest = l;
	}
	else {
		smallest = i;
	}
	if (r < A.size() && LessThan(A[r], A[smallest])) {
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
	while (i > 0 && LessThan(A[i], A[parent(i)])) {
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

void OutputQueue(ofstream &fout, vector<PNode> &A) {
	fout << "#Queue#" << endl;
	for (size_t i = 0; i < A.size(); i ++) {
		fout << "|" + A[i]->actions + " |" << A[i]->actions.size() << "|" << A[i]->hkey << "|" << A[i]->actions.size() + A[i]->hkey << "|" << endl;
	}
	fout << endl;
}

bool IsVisited(string a, char c) {
	if (a.size() == 0) {
		return false;
	}
	if (c == 'U') {
		for (size_t i = a.size() - 1; i >= 0; i --) {
			if (a[i] == 'D') {
				return true;
			}
			else if (a[i] == 'R' || a[i] == 'L') {
				return false;
			}
		}
	}
	else if (c == 'D') {
		for (size_t i = a.size() - 1; i >= 0; i --) {
			if (a[i] == 'U') {
				return true;
			}
			else if (a[i] == 'R' || a[i] == 'L') {
				return false;
			}
		}
	}
	else if (c == 'R') {
		for (size_t i = a.size() - 1; i >= 0; i --) {
			if (a[i] == 'L') {
				return true;
			}
			else if (a[i] == 'U' || a[i] == 'D') {
				return false;
			}
		}
	}
	else if (c == 'L') {
		for (size_t i = a.size() - 1; i >= 0; i --) {
			if (a[i] == 'R') {
				return true;
			}
			else if (a[i] == 'U' || a[i] == 'D') {
				return false;
			}
		}
	}
	return false;
}

bool GoalTest(char **digits, char **target) {
	for (int i = 0; i < SIZE; i ++) {
		for (int j = 0; j < SIZE; j ++) {
			if (digits[i][j] != target[i][j]) {
				return false;
			}
		}
	}
	return true;
}

int Heuristic(char **digits, char **target) {
	int h = 0, count = 0;
	for (int i = 0; i < SIZE; i ++) {
		for (int j = 0; j < SIZE; j ++) {
			if (digits[i][j] >= 0){
				int tmp = HTable[digits[i][j]][i][j];
				h += tmp;
				count++;
			} 
		}
	}
	return h / 2;
}

void FreeAllNode(vector<PNode> &Q) {
	PNode n;
	for (size_t i = 0; i < Q.size(); i ++) {
		for (int j = 0; j < SIZE; j ++) {
			delete Q[i]->status[j];
		}
		delete Q[i]->status;
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

PNode IDAstarh2(char **start, char ** target){
	vector<PNode> Q;
	PNode newNode;
	int cnt = 0;
	int d_limit;
	/* ofstream f1;
	f1.open("procedure1.txt"); */

	LocatChess0(start);
	if(row == -1) {
		cout << "input wrong file !" << endl;
		exit(0);
	}
	d_limit = 1;
	newNode = new Node;
	if (newNode == nullptr) {
		cout << "memory not enough!" << endl;
		exit(0);
	}
	newNode->actions = "";
	newNode->status = start;
	newNode->hkey = Heuristic(newNode->status, target);
	newNode->fkey = newNode->hkey;

	while (d_limit < INF) {
		int next_d_limit = INF;
		InsertNode(Q, newNode);
		while(!Q.empty()) {
			PNode rootNode = ExtractMin(Q);
			int ds = rootNode->fkey;
			if (ds > d_limit) {
				if (ds < next_d_limit) {
					next_d_limit = ds;
				} 
			}
			char **digits = rootNode->status;
			char last_dir = rootNode->actions[rootNode->actions.size() - 1];
			cnt++;
			if(GoalTest(rootNode->status, target)) {
				cout << "Finish!" << endl;
				FreeAllNode(Q);
				//f1.close();
				return rootNode;
			}
			LocatChess0(rootNode->status);
			if (row < 4 && digits[row + 1][col] != -1 && last_dir != 'U') {//D
				PNode DNode = new Node;
				if (DNode == nullptr) {
					cout << "memory not enough!" << endl;
					exit(0);
				}
				DNode->status = new char*[SIZE];
				if (DNode->status == nullptr) {
					cout << "memory not enough!" << endl;
					exit(0);
				}
				for (int i = 0; i < SIZE; i ++){
					DNode->status[i] = new char[SIZE];
					if (DNode->status[i] == nullptr) {
						cout << "memory not enough!" << endl;
						exit(0);
					}
				}
				DNode->actions = rootNode->actions + "D";
				Move(digits, DNode->status, 'D');
				DNode->hkey = Heuristic(DNode->status, target);
				DNode->fkey = DNode->hkey + DNode->actions.size();
				InsertNode(Q, DNode);
			}
			if (row > 0 && digits[row - 1][col] != -1 && last_dir != 'D') {//U
				PNode UNode = new Node;
				if (UNode == nullptr) {
					cout << "memory not enough!" << endl;
					exit(0);
				}
				UNode->status = new char*[SIZE];
				if (UNode->status == nullptr) {
					cout << "memory not enough!" << endl;
					exit(0);
				}
				for (int i = 0; i < SIZE; i ++){
					UNode->status[i] = new char[SIZE];
					if (UNode->status[i] == nullptr) {
						cout << "memory not enough!" << endl;
						exit(0);
					}
				}
				UNode->actions = rootNode->actions + "U";
				Move(digits, UNode->status, 'U');
				UNode->hkey = Heuristic(UNode->status, target);
				UNode->fkey = UNode->hkey + UNode->actions.size();
				InsertNode(Q, UNode);
			}
			if (col < 4 && last_dir != 'L') {//R
				PNode RNode = new Node;
				if (RNode == nullptr) {
					cout << "memory not enough!" << endl;
					exit(0);
				}
				RNode->status = new char*[SIZE];
				if (RNode->status == nullptr) {
					cout << "memory not enough!" << endl;
					exit(0);
				}
				for (int i = 0; i < SIZE; i ++){
					RNode->status[i] = new char[SIZE];
					if (RNode->status[i] == nullptr) {
						cout << "memory not enough!" << endl;
						exit(0);
					}
				}
				RNode->actions = rootNode->actions + "R";
				Move(digits, RNode->status, 'R');
				RNode->hkey = Heuristic(RNode->status, target);
				RNode->fkey = RNode->hkey + RNode->actions.size();
				InsertNode(Q, RNode);
			}
			if (col > 0 && last_dir != 'R') {//L
				PNode LNode = new Node;
				if (LNode == nullptr) {
					cout << "memory not enough!" << endl;
					exit(0);
				}
				LNode->status = new char*[SIZE];
				if (LNode->status == nullptr) {
					cout << "memory not enough!" << endl;
					exit(0);
				}
				for (int i = 0; i < SIZE; i ++){
					LNode->status[i] = new char[SIZE];
					if (LNode->status[i] == nullptr) {
						cout << "memory not enough!" << endl;
						exit(0);
					}
				}
				LNode->actions = rootNode->actions + "L";
				Move(digits, LNode->status, 'L');
				LNode->hkey = Heuristic(LNode->status, target);
				LNode->fkey = LNode->hkey + LNode->actions.size();
				InsertNode(Q, LNode);
			}
			//OutputQueue(f1, Q);
			for (int j = 0; j < SIZE; j ++) {
				delete rootNode->status[j];
			}
			delete rootNode->status;
			delete rootNode;
		}
		d_limit = next_d_limit;
	}
	return nullptr;
}


int main(){
	string inputpath = "input/input.txt", targetpath = "input/target.txt", outputpath = "output/output_IDAh2.txt";
	vector<vector<int> > start(5, vector<int>(5, 0)), target(5, vector<int>(5, 0));
	char **s, **t;
	PNode result;
	ifstream fin;
	ofstream fout;
	clock_t starttime, finishtime;
	double totaltime;

	fin.open(inputpath);
	for (int i = 0; i < 5; i ++){
		for (int j = 0; j < 5; j ++){
			fin >> start[i][j];
		}
	}
	s = new char*[SIZE];
	if (s == nullptr) {
		cout << "memory not enough!" << endl;
		exit(0);
	}
	for (int i = 0; i < SIZE; i ++){
		s[i] = new char[SIZE];
		if (s[i] == nullptr) {
			cout << "memory not enough!" << endl;
			exit(0);
		}
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
	if (t == nullptr) {
		cout << "memory not enough!" << endl;
		exit(0);
	}
	for (int i = 0; i < SIZE; i ++){
		t[i] = new char[SIZE];
		if (t[i] == nullptr) {
			cout << "memory not enough!" << endl;
			exit(0);
		}
		for (int j = 0; j < SIZE; j ++){
			t[i][j] = (char)target[i][j];
		}
	}
	fin.close();
	starttime = clock();
	result = IDAstarh2(s, t);
	finishtime = clock();
	totaltime = (double)(finishtime-starttime)/CLOCKS_PER_SEC;
	fout.open(outputpath);
	if (result != nullptr){
		fout << totaltime << endl;
		fout << result->actions << endl;
		fout << result->actions.size() << endl;
		for (int j = 0; j < SIZE; j ++) {
			delete result->status[j];
		}
		delete result->status;
		delete result;
	}
	else {
		cout << "no result!" << endl;
	}

	return 0;
}