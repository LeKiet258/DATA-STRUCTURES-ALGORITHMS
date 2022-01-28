#include<iostream>

using namespace std;

//DE 2

int recursiveBinarySearch(int* a, int left, int right, int key);
bool checkBelong(int N, int a);
int findMin3Digits(int N);


int main() {
	int n, num, digit;
	int key;
	//init
	n = num = digit = key = 0;

	cout << "Nhap so luong phan tu: ";
	cin >> n;

	int* a = new int[n];

	cout << "Nhap mang da sap xep: ";
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	
	cout << "Nhap key: ";
	cin >> key;

	cout << recursiveBinarySearch(a, 0, n - 1, key) << endl << endl;

	//Cau 2
	cout << "Cau 2\n";
	cout << "Nhap so: "; cin >> num;
	cout << "Nhap chu so can tim: "; cin >> digit;
	cout << (checkBelong(num, digit) ? "chu so ton tai" : "chu so ko ton tai") << endl;

	//Cau 3
	cout << "Cau 3:\n";
	cout << "Nhap so: "; cin >> num;
	//num = 543221;
	
	int res = findMin3Digits(num);
	
	if (res == -1)
		cout << "ko tim thay min";
	else
		cout << res;

	return 0;
}

//543221

int findMin3Digits(int N) {
	if (N < 100)
		return 0;
	else {
		int n = N % 1000;
		int min = findMin3Digits(N / 10);

		if (min > n)
			min = n;

		return min;
	}

	return -1;
}


//1,2,3  a = 0
bool checkBelong(int N, int a) {
	if (N != 0) {
		if (a == N % 10)
			return true;
		return checkBelong(N / 10, a);
	}

	return false;
}

//0 1 2 3 4 
//1 2 3 4 5 | 1
int recursiveBinarySearch(int* a, int left, int right, int key) {
	int mid = left + (right - left) / 2;

	if (a[mid] == key)
		return mid;
	else if (key < a[mid])
		return recursiveBinarySearch(a, left, mid - 1, key);
	else
		return recursiveBinarySearch(a, mid + 1, right, key);

	return -1;
}


