#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <vector>

using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::queue;
using std::priority_queue;
using std::vector;

struct node {
	string actions;
	int f;
	friend bool operator < (node a, node b) {
		return a.f > b.f;
	}
};

bool GoalTest(vector<vector<int>> digits, vector<vector<int>> target) {
	for (int i = 0; i < digits.size(); i ++) {
		for (int j = 0; j < digits[i].size(); j ++) {
			if (digits[i][j] != target[i][j])
				return false;
		}
	}
	return true;
}

int Heuristic(vector<vector<int>> digits, vector<vector<int>> target) {
	int count = 0;
	for (int i = 0; i < digits.size(); i ++) {
		for (int j = 0; j < digits[i].size(); j ++) {
			if (digits[i][j] != target[i][j])
				count++;
		}
	}
	return count;
}

int Move()

int Astarh1(vector<vector<int>> digits, vector<vector<int>> target, priority_queue<node> Q){
	
}

int main(){
	priority_queue<node> que;
	string inputpath = "input/input.txt", targetpath = "input/target.txt";
	vector<vector<int>> digits(5, vector<int>(5, 0)), target(5, vector<int>(5, 0));
	ifstream fin;
	fin.open(inputpath);
	for (int i = 0; i < 5; i ++){
		for (int j = 0; j < 5; j ++){
			fin >> digits[i][j];
		}
	}
	fin.close();
	fin.open(targetpath);
	for (int i = 0; i < 5; i ++){
		for (int j = 0; j < 5; j ++){
			fin >> target[i][j];
		}
	}
	fin.close();
}