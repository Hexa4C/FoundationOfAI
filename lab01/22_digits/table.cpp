#include<iostream>
#include<fstream>
#include<cstdlib>

using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;

int main() {
	int a[5][5] = {
		{1, 2, 3, 4, 5},
		{6, 7, 8, 9, 10},
		{11, -1, 12, -1, 13},
		{14, 15, 16, 17, 18},
		{19, 20, 21, 22, 0}};
	ofstream fout;
	int row, col;
	fout.open("table.txt");
	for (int k = 0; k < 23; k ++) {
		fout << "{";
		for (int i = 0; i < 5; i ++) {
			for (int j = 0; j < 5; j ++) {
				if (a[i][j] == k) {
					row = i;
					col = j;
				}
			}
		}
		for (int i = 0; i < 5; i ++) {
			fout << "{";
			for (int j = 0; j < 5; j ++) {
				fout << abs(i - row) + abs(j - col);
				if(j != 5) {
					fout << ", ";
				}
			}
			fout << "}";
			if(i != 5) {
				fout << ", ";
			}
		}
		fout << "}," << endl;
	}
}