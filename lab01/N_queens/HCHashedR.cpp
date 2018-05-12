#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;

#define INF 0x7fffffff

typedef struct QNode {
	int row;
	int col;
	QNode *next;
} *Queen;

vector<Queen> hash_table;
vector<int> rowqnum, colqnum, mdiagqnum, bdiagqnum;

int Couples(int n) {
	if (n == 0 || n == 1) {
		return 0;
	}
	else {
		return n * (n - 1) / 2;
	}
}

int AttackCnt(vector<int> Node, int N, int M) {
	int attackcnt = 0;
	for (int i = 0; i < N; i ++) {
		attackcnt += Couples(rowqnum[i]);
		attackcnt += Couples(colqnum[i]);
		//cout << rowqnum[i] << " " << colqnum[i] << " " << mdiagqnum[i] << " " << bdiagqnum[i] << endl;
	}
	for (int i = 0; i < 2 * N - 1; i ++) {
		attackcnt += Couples(mdiagqnum[i]);
		attackcnt += Couples(bdiagqnum[i]);
	}
	//cout << endl;
	return attackcnt;
}

bool InitTables(vector<int> Node, int N) {
	for (int i = 0; i < N; i ++) {
		rowqnum.push_back(0);
		colqnum.push_back(0);
		hash_table.push_back(nullptr);
	}
	for (int i = 0; i < 2 * N - 1; i ++) {
		mdiagqnum.push_back(0);
		bdiagqnum.push_back(0);
	}
	for (int i = 0; i < N; i++)
	{
		int arow = Node[2 * i];
		int acol = Node[2 * i + 1];
		int amdiag = N + acol - arow - 1;
		int abdiag = arow + acol;
		rowqnum[arow]++;
		colqnum[acol]++;
		mdiagqnum[amdiag]++;
		bdiagqnum[abdiag]++;
		int hash_idx = (arow + acol) % N;
		Queen p = hash_table[hash_idx];
		Queen q = new QNode;
		q->row = arow;
		q->col = acol;
		q->next = nullptr;
		if (p == nullptr)
		{
			hash_table[hash_idx] = q;
		}
		else
		{
			if (p->row == q->row && p->col == q->col)
			{
				return false;
			}
			while (p->next != nullptr)
			{
				p = p->next;
				if (p->row == q->row && p->col == q->col)
				{
					return false;
				}
			}
			p->next = q;
		}
	}
	return true;
}

void FreeTables(int N) {
	rowqnum.clear();
	colqnum.clear();
	mdiagqnum.clear();
	bdiagqnum.clear();
	for (int i = 0; i < N; i ++) {
		Queen p = hash_table[i], q;
		while (p != nullptr) {
			q = p->next;
			delete p;
			p = q;
		}
	}
	hash_table.clear();
}

void RandStart(vector<int> &Node, int N) {
	for (int i = 0; i < N; i ++) {
		Node[2 * i] = (int)rand() % N;
		Node[2 * i + 1] = (int)rand() % N;
	}
	while (! InitTables(Node, N)) {
		FreeTables(N);
		for (int i = 0; i < N; i ++) {
			Node[2 * i] = (int)rand() % N;
			Node[2 * i + 1] = (int)rand() % N;
		}
	}
}

void OutputChess(ofstream &fout, vector<int> Node, int N, int M) {
	if (N >= 20) {
		return;
	}
	vector<vector<char>> chess(N, vector<char>(N, '*'));
	for (int i = 0; i < N; i ++) {
		chess[Node[2 * i]][Node[2 * i + 1]] = 'Q';
	}
	for (int i = 0; i < N; i ++) {
		for (int j = 0; j < N; j ++) {
			fout << chess[i][j] << "  ";
		}
		fout << endl;
	}
	fout << "row: ";
	for (int i = 0; i < N; i ++) {
		fout << rowqnum[i] << " ";
	}
	fout << endl;
	fout << "col: ";
	for (int i = 0; i < N; i ++) {
		fout << colqnum[i] << " ";
	}
	fout << endl;
	fout << "maindiag: ";
	for (int i = 0; i < 2 * N - 1; i ++) {
		fout << mdiagqnum[i] << " ";
	}
	fout << endl;
	fout << "backdiag: ";
	for (int i = 0; i < 2 * N - 1; i ++) {
		fout << bdiagqnum[i] << " ";
	}
	fout << endl;
	for (int i = 0; i < N; i ++) {
		fout << "(" << Node[2 * i] << ", " << Node[2 * i + 1] << ")" << " ";
	}
	fout << endl;
	fout << "| ";
	for (int i = 0; i < N; i ++) {
		Queen p = hash_table[i];
		while(p != nullptr) {
			fout << "(" << p->row << ", " << p->col << ")" << " ";
			p = p->next;
		}
		fout << "| ";
	}
	fout << endl;
	fout << "AttackCnt: " << AttackCnt(Node, N, M) << endl;
	fout << endl;
}

void OutputResult(ofstream &fout, vector<int> Node, int N) {
	vector<vector<char>> chess(N, vector<char>(N, '0'));
	for (int i = 0; i < N; i ++) {
		chess[Node[2 * i]][Node[2 * i + 1]] = 'Q';
	}
	for (int i = 0; i < N; i ++) {
		bool tag = false;
		for (int j = 0; j < N; j ++) {
			if (chess[i][j] == 'Q') {
				fout << j << " ";
				tag = true;
			}
		}
		if (!tag) {
			fout << -1;
		}
		fout << endl;
	}
}

bool QueenOccupied(int arow, int acol, int N) {
	int h = (arow + acol) % N;
	Queen p = hash_table[h];
	while (p != nullptr) {
		if (p->row == arow && p->col == acol) {
			return true;
		}
		p = p->next;
	}
	return false;
}

void MoveChess(vector<int> &Node, int loc, int newrow, int newcol, int N) {
	int oldrow = Node[2 * loc];
	int oldcol = Node[2 * loc + 1];
	int oldmdiag = N + oldcol - oldrow - 1;
	int oldbdiag = oldcol + oldrow;
	int newmdiag = N + newcol - newrow - 1;
	int newbdiag = newrow + newcol;
	rowqnum[oldrow]--;
	rowqnum[newrow]++;
	colqnum[oldcol]--;
	colqnum[newcol]++;
	mdiagqnum[oldmdiag]--;
	mdiagqnum[newmdiag]++;
	bdiagqnum[oldbdiag]--;
	bdiagqnum[newbdiag]++;
	Node[2 * loc] = newrow;
	Node[2 * loc + 1] = newcol;
	int oldh = (oldrow + oldcol) % N;
	int newh = (newrow + newcol) % N;
	Queen p = hash_table[oldh], q;
	q = p;
	while (p != nullptr) {
		if (p->row == oldrow && p->col == oldcol) {
			break;
		}
		q = p;
		p = p->next;
	}
	if (p == nullptr) {
		cout << "Move Chess: hash table cracked!" << endl;
		exit(0);
	}
	if (p == q) {
		hash_table[oldh] = p->next;
	}
	else {
		q->next = p->next;
	}
	p->row = newrow;
	p->col = newcol;
	p->next = nullptr;
	q = hash_table[newh];
	if (q == nullptr) {
		hash_table[newh] = p;
	}
	else {
		if (p->row == q->row && p->col == q->col) {
			cout << "MoveChess: QueenOccupied didn't work well!" << endl;
			exit(0);
		}
		while (q->next != nullptr) {
			q = q->next;
			if (p->row == q->row && p->col == q->col) {
				cout << "MoveChess: QueenOccupied didn't work well!" << endl;
				exit(0);
			}
		}
		p->next = q->next;
		q->next = p;
	}
}

bool RRHC(vector<int> &Node, int N, int M) {
	int cnt = 100;
	int side = 10;

	while (cnt > 0) {
		RandStart(Node, N);
		int curr_attack = AttackCnt(Node, N, M);
		int curr_eval = abs(curr_attack - M);
		int sidecnt = side;
		while (1) {
			int minrow = 0, mincol = 0, minloc = 0;
			int min_eval = INF;
			if (N < 500) {
				for (int i = 0; i < N; i ++) {
					int arow = Node[2 * i];
					int acol = Node[2 * i + 1];
					int amdiag = N + acol - arow - 1;
					int abdiag = arow + acol;
					for (int j = 0; j < N; j ++) {//行上移动
						if (j == acol) {
							continue;
						}
						if (! QueenOccupied(arow, j, N)) {
							int newmdiag = N + j - arow - 1;
							int newbdiag = arow + j;
							int eval_diff = 0;
							eval_diff += (Couples(colqnum[j] + 1) + Couples(colqnum[acol] - 1) - Couples(colqnum[j]) - Couples(colqnum[acol]));
							eval_diff += (Couples(mdiagqnum[newmdiag] + 1) + Couples(mdiagqnum[amdiag] - 1) - Couples(mdiagqnum[amdiag]) - Couples(mdiagqnum[newmdiag]));
							eval_diff += (Couples(bdiagqnum[newbdiag] + 1) + Couples(bdiagqnum[abdiag] - 1) - Couples(bdiagqnum[abdiag]) - Couples(bdiagqnum[newbdiag]));
							if (abs(curr_attack + eval_diff - M) < min_eval) {
								min_eval = abs(curr_attack + eval_diff - M);
								minloc = i;
								minrow = arow;
								mincol = j;
							}
						}
					}
					for (int j = 0; j < N; j ++) {//列上移动
						if (j == arow) {
							continue;
						}
						if (! QueenOccupied(j, acol, N)) {
							int newmdiag = N + acol - j - 1;
							int newbdiag = acol + j;
							int eval_diff = 0;
							eval_diff += (Couples(rowqnum[j] + 1) + Couples(rowqnum[arow] - 1) - Couples(rowqnum[j]) - Couples(rowqnum[arow]));
							eval_diff += (Couples(mdiagqnum[newmdiag] + 1) + Couples(mdiagqnum[amdiag] - 1) - Couples(mdiagqnum[amdiag]) - Couples(mdiagqnum[newmdiag]));
							eval_diff += (Couples(bdiagqnum[newbdiag] + 1) + Couples(bdiagqnum[abdiag] - 1) - Couples(bdiagqnum[abdiag]) - Couples(bdiagqnum[newbdiag]));
							if (abs(curr_attack + eval_diff - M) < min_eval) {
								min_eval = abs(curr_attack + eval_diff - M);
								minloc = i;
								minrow = j;
								mincol = acol;
							}
						}
					}
				}
			}
			else {
				for (int k = 0; k < 100; k ++) {
					int i = (int)rand() % N;
					int arow = Node[2 * i];
					int acol = Node[2 * i + 1];
					int amdiag = N + acol - arow - 1;
					int abdiag = arow + acol;
					for (int l = 0; l < 10; l ++) {//行上移动
						int j = (int)rand() % N;
						if (j == acol) {
							continue;
						}
						if (! QueenOccupied(arow, j, N)) {
							int newmdiag = N + j - arow - 1;
							int newbdiag = arow + j;
							int eval_diff = 0;
							eval_diff += (Couples(colqnum[j] + 1) + Couples(colqnum[acol] - 1) - Couples(colqnum[j]) - Couples(colqnum[acol]));
							eval_diff += (Couples(mdiagqnum[newmdiag] + 1) + Couples(mdiagqnum[amdiag] - 1) - Couples(mdiagqnum[amdiag]) - Couples(mdiagqnum[newmdiag]));
							eval_diff += (Couples(bdiagqnum[newbdiag] + 1) + Couples(bdiagqnum[abdiag] - 1) - Couples(bdiagqnum[abdiag]) - Couples(bdiagqnum[newbdiag]));
							if (abs(curr_attack + eval_diff - M) < min_eval) {
								min_eval = abs(curr_attack + eval_diff - M);
								minloc = i;
								minrow = arow;
								mincol = j;
							}
						}
					}
					for (int l = 0; l < 10; l ++) {//列上移动
						int j = (int)rand() % N;
						if (j == arow) {
							continue;
						}
						if (! QueenOccupied(j, acol, N)) {
							int newmdiag = N + acol - j - 1;
							int newbdiag = acol + j;
							int eval_diff = 0;
							eval_diff += (Couples(rowqnum[j] + 1) + Couples(rowqnum[arow] - 1) - Couples(rowqnum[j]) - Couples(rowqnum[arow]));
							eval_diff += (Couples(mdiagqnum[newmdiag] + 1) + Couples(mdiagqnum[amdiag] - 1) - Couples(mdiagqnum[amdiag]) - Couples(mdiagqnum[newmdiag]));
							eval_diff += (Couples(bdiagqnum[newbdiag] + 1) + Couples(bdiagqnum[abdiag] - 1) - Couples(bdiagqnum[abdiag]) - Couples(bdiagqnum[newbdiag]));
							if (abs(curr_attack + eval_diff - M) < min_eval) {
								min_eval = abs(curr_attack + eval_diff - M);
								minloc = i;
								minrow = j;
								mincol = acol;
							}
						}
					}
				}
			}
			if (min_eval < curr_eval) {
				MoveChess(Node, minloc, minrow, mincol, N);
				curr_attack = AttackCnt(Node, N, M);
				curr_eval = min_eval;
				sidecnt = side;
			}
			else if (min_eval == curr_eval && sidecnt > 0) {
				sidecnt--;
				MoveChess(Node, minloc, minrow, mincol, N);
				curr_attack = AttackCnt(Node, N, M);
				curr_eval = min_eval;
			}
			else {
				MoveChess(Node, minloc, minrow, mincol, N);
				break;
			}
		}
		if (curr_attack == M) {
			return true;
		}
		cnt--;
		FreeTables(N);
	}
	return false;
}

int main(){
	string inputpath = "input/input.txt", outputpath = "output/output_hill_climbing.txt";
	int N, M;
	ifstream fin;
	ofstream fout;
	clock_t starttime, finishtime;
	double totaltime;

	fin.open(inputpath);
	fin >> N >> M;
	fin.close();
	vector<int> Node(2 * N, 0);
	starttime = clock();
	bool tag = RRHC(Node, N, M);
	finishtime = clock();
	totaltime = (double)(finishtime-starttime)/(CLOCKS_PER_SEC / 1000);
	if (tag) {
		cout << "success!" << endl;
		fout.open(outputpath);
		OutputResult(fout, Node, N);
		fout << totaltime << "ms" << endl;
		fout.close();
	}
	else {
		cout << "fail!" << endl;
	}

	return 0;
}