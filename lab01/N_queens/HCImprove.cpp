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

typedef struct {
	int row;
	int col;
	Queen up;
	Queen down;
	Queen left;
	Queen right;
	Queen upleft;
	Queen upright;
	Queen downleft;
	Queen downright;
} QNode, *Queen;

typedef struct {
	int num;
	Queen head;
} ChessLine;

typedef struct {
	vector<ChessLine> row, col, maindiag, backdiag;
	vector<QNode> chess;
} ChessTable;

int Evaluation(ChessTable table, int N, int M) {
	int attackcnt = 0;
	for (int i = 0; i < N - 1; i ++) {
		/* for (int j = i + 1; j < N; j ++) {
			if (table.chess[i].row == table.chess[j].row && table.chess[i].col == table.chess[j].col) {
				return INF;
			}
			else if(table.chess[i].row == table.chess[j].row) {
				attackcnt++;
			}
			else if(table.chess[i].col == table.chess[j].col) {
				attackcnt++;
			}
			else if(abs(table.chess[i].row - table.chess[j].row) == abs(table.chess[i].col - table.chess[j].col)) {
				attackcnt ++;
			}
		} */
		attackcnt += table.row.num;
		attackcnt += table.col.num;
		attackcnt += table.maindiag.num;
		attackcnt += table.backdiag.num;
	}
	return abs(attackcnt - M);
}

void InitTable(ChessTable &table, int N) {
	table.chess = new QNode[N];
	table.row = new ChessLine[N];
	for (int i = 0; i < N; i ++) {
		table.row[i].num = 0;
		table.row[i].head = nullptr;
	}
	table.col = new ChessLine[N];
	for (int i = 0; i < N; i ++) {
		table.col[i].num = 0;
		table.col[i].head = nullptr;
	}
	table.maindiag = new ChessLine[N];
	for (int i = 0; i < N; i ++) {
		table.maindiag[i].num = 0;
		table.maindiag[i].head = nullptr;
	}
	table.backdiag = new ChessLine[N];
	for (int i = 0; i < N; i ++) {
		table.backdiag[i].num = 0;
		table.backdiag[i].head = nullptr;
	}
}

bool InsertQueen(ChessTable &table, int idx, int N, int newrow, int newcol) {
	Queen r, c;
	int newmaindiag, newbackdiag;
	Queen aqueen = &table.chess[idx];
	table.chess[idx].row = newrow;
	table.chess[idx].col = newcol;
	newmaindiag = N + newrow - newcol;
	newbackdiag = newrow + newcol;
	r = table.row[newrow].head;
	c = table.col[newcol].head;
	while (r != nullptr && r->right != nullptr && r->right.col < newcol) {
		r = r->right;
	}
	while (c != nullptr && c->down != nullptr && c->down.row < newrow) {
		c = c->down;
	}
	while (m != nullptr && m->downright )
	if (r == nullptr) {
		if (c == nullptr) {
			table.row[newrow].head = aqueen;
			table.col[newcol].head = aqueen;
			aqueen
		}
	}
	else if (r->right == nullptr){

	}
	else {

	}
}

void RandStart(ChessTable &table, int N) {
	int newrow, newcol;
	for (int i = 0; i < N; i ++) {
		newrow = (int)rand() % N;
		newcol = (int)rand() % N;
		table.chess[i].row = newrow;
		table.chess[i].col = newcol;
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
	fout << Evaluation(Node, N, M) << endl;
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

void CopyChess(vector<int> &na, vector<int> nb, int N) {
	for (int i = 0; i < 2 * N; i ++) {
		na[i] = nb[i];
	}
}

bool RRHC(vector<int> &Node, int N, int M) {
	int cnt = 10;
	int side = 1;
	vector<int> bestneighbor(2 * N, 0);
	//ofstream pfout;
	//string procedure = "procedure.txt";
	//pfout.open(procedure);

	while (cnt > 0) {
		RandStart(Node, N);
		while (Evaluation(Node, N, M) == INF) {
			RandStart(Node, N);
		}
		//OutputChess(pfout, Node, N, M);
		int curr_eval = Evaluation(Node, N, M);
		int min_eval = INF;
		int sidecnt = side;
		CopyChess(bestneighbor, Node, N);
		while (1) {
			for (int i = 0; i < 2 * N; i ++) {
				for (int j = 1; j <= Node[i]; j ++){
					if (Node[i] > 0)
					{
						Node[i] -= j;
						int neighbor_eval = Evaluation(Node, N, M);
						if (neighbor_eval < min_eval)
						{
							min_eval = neighbor_eval;
							CopyChess(bestneighbor, Node, N);
						}
						Node[i] += j;
					}
				}
				//OutputChess(pfout, bestneighbor, N, M);
				//pfout << min_eval << endl;
				for (int j = 1; j < N - Node[i]; j ++) {
					if (Node[i] < N - 1)
					{
						Node[i] += j;
						int neighbor_eval = Evaluation(Node, N, M);
						if (neighbor_eval < min_eval)
						{
							min_eval = neighbor_eval;
							CopyChess(bestneighbor, Node, N);
						}
						Node[i] -= j;
					}
				}
				//OutputChess(pfout, bestneighbor, N, M);
				//pfout << min_eval << endl;
			}
			if (min_eval < curr_eval) {
				CopyChess(Node, bestneighbor, N);
				curr_eval = min_eval;
				sidecnt = side;
			}
			else if (min_eval == curr_eval && sidecnt > 0) {
				sidecnt--;
				CopyChess(Node, bestneighbor, N);
				curr_eval = min_eval;
			}
			else {
				break;
			}
		}
		//OutputChess(pfout, Node, N, M);
		//pfout << min_eval << endl;
		//pfout << "###################" << endl;
		if (curr_eval == 0) {
			//OutputChess(pfout, Node, N, M);
			return true;
		}
		cnt--;
	}
	//pfout.close();
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
	ChessTable table;
	starttime = clock();
	bool tag = RRHC(Node, N, M);
	finishtime = clock();
	totaltime = (double)(finishtime-starttime)/(CLOCKS_PER_SEC / 1000);
	if (tag) {
		cout << "success!" << endl;
		fout.open(outputpath);
		OutputResult(fout, Node, N);
		fout << totaltime << endl;
		fout.close();
	}
	else {
		cout << "fail!" << endl;
	}

	return 0;
}