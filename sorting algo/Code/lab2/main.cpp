#include<iostream>
#include<string>
#include <chrono> 
#include<fstream>

#include"Sorting.h"

using namespace std;
using namespace std::chrono;

void printDataOrder(int a) {
	switch (a) {
	case 0:
		cout << "#For random data\n";
		break;
	case 1:
		cout << "#For already ascending sorted data\n";
		break;
	case 2:
		cout << "#For already descending sorted data\n";
		break;
	case 3:
		cout << "#For nearly sorted data\n";
		break;
	}
}

void printScreen_saveToFile() {
	string sortingNames[] = { "quick sort" , "merge sort", "selection sort", "bubble sort", "insertion sort", "binary insertion sort", "shaker sort", "shell sort", "heap sort", "radix sort", "counting sort","flash sort" };
	int sizeArr[] = { 3000, 10000, 30000, 100000, 300000 };
	string fileName[] = { "random_order.txt", "ascending_order.txt", "descending_order.txt", "nearly_ascending_order.txt" };
	int n = 0;

	cout << "Printing to screen & saving to 4 files ...\n";

	for (int sizeType = 0; sizeType < 4; sizeType++) {
		//0: ngau nhien, 1: already sorted, 2: descending sorted, 3: nearly sorted
		printDataOrder(sizeType);
		ofstream fout(fileName[sizeType]);

		for (int eachSize = 0; eachSize < 5; eachSize++) {
			//generate a[]
			n = sizeArr[eachSize];
			int* a = new int[n];
			GenerateData(a, n, sizeType);

			cout << "\tSize = " << n << endl;
			fout << "\nSize = " << n << endl;

			for (int i = 1; i < 12; i++) {
				//take a copy of a[]
				int* b = new int[n];
				memcpy(b, a, n * sizeof(int));

				//Apply sorting algorithms
				auto start = high_resolution_clock::now();
				sortingAlgorithm(b, n, sortingNames[i]);
				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);

				//print each time complexity to screen
				cout << "\t\t" << sortingNames[i] << ": " << duration.count() * 1e-3 << " milliseconds\n";
				fout << sortingNames[i] << "," << duration.count() * 1e-3 << endl;

				delete[] b;
			}
			delete[] a;
		}
		cout << "\n-------------------\n";
		fout.close();
	}
}

int main(int argc, char* argv[]) {
	//to print to screen & save to file
	printScreen_saveToFile();

	return 0;
}

