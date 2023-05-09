#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

void Swap(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}

void Kruskal(int** maze) {
	cout << "Fix Kruskal" << endl;
	//sort the weights from lowest to highest
}

void Prim(int** maze) {
	cout << "fix Prim" << endl;
}


int main() {

	ifstream fin("input1.txt");

	int seedNum;
	string method;
	int count = 30;

	//fin >> seedNum >> method;
	seedNum = 10; //comment out later
	method = "kruskal"; // "prim";

	srand(seedNum);

	int** maze = new int* [count];

	for (int i = 0; i < count; ++i) {
		maze[i] = new int[count];
		for (int j = 0; j < count; ++j) {
			maze[i][j] = rand();
		}
	}

	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < count; ++j) {
			cout << maze[i][j] << " ";
		}
		cout << endl;
	}

	if (method == "kruskal") {
		
	}
}