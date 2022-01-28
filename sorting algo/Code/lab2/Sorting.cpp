#include"Sorting.h"

#pragma region DATA_GENERATOR
void GenerateRandomData(int a[], int n)
{
	srand((unsigned int)time(NULL));

	for (int i = 0; i < n; i++)
	{
		a[i] = rand() % n;
	}
}

void GenerateSortedData(int a[], int n)
{
	for (int i = 0; i < n; i++){
		a[i] = i;
	}
}

void GenerateReverseData(int a[], int n)
{
	for (int i = 0; i < n; i++)
	{
		a[i] = n - 1 - i;
	}
}

void GenerateNearlySortedData(int a[], int n)
{
	for (int i = 0; i < n; i++)
	{
		a[i] = i;
	}
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 10; i++)
	{
		int r1 = rand() % n;
		int r2 = rand() % n;
		HoanVi(a[r1], a[r2]);
	}
}

void GenerateData(int a[], int n, int dataType)
{
	switch (dataType) {
	case 0:	// ngẫu nhiên
		GenerateRandomData(a, n);
		break;
	case 1:	// có thứ tự
		GenerateSortedData(a, n);
		break;
	case 2:	// có thứ tự ngược
		GenerateReverseData(a, n);
		break;
	case 3:	// gần như có thứ tự
		GenerateNearlySortedData(a, n);
		break;
	default:
		printf("Error: unknown data type!\n");
	}
}
#pragma endregion

#pragma region MERGE SORT 
void mergeSort	(int a[], int l, int r) {
	if (l < r) {
		//halve the aay (take the upper element if aay's size is odd)
		int mid = l + (r - l) / 2;

		//sort the 2 halves
		//Step 1: divide a huge aay into very small subaays
		mergeSort(a, l, mid);
		mergeSort(a, mid + 1, r);

		//Step 2: Merge 2 subaays (BASE CASE)
		merge(a, l, mid, r);
	}
}

void merge(int a[], int l, int m, int r) {
	int n1 = m - l + 1;
	int n2 = r - m; //not plus 1 cuz mid = 'upper elem' as above

	int* L = new int[n1];
	int* R = new int[n2];

	//copy a's contents to L & R
	for (int i = 0; i < n1; i++)
		L[i] = a[l + i];
	for (int i = 0; i < n2; i++)
		R[i] = a[m + i + 1];

	//merge the subaays back to a
	int i = 0, j = 0; //index of L[] & R[]
	int k = l; //index of the merged aay

	while (i < n1 && j < n2) {
		if (L[i] <= R[j])
			a[k++] = L[i++];
		else
			a[k++] = R[j++];
	}

	//Copy the remaining elems of L[] (if any)
	while (i < n1)
		a[k++] = L[i++];
	//Copy the remaining elems of R[] (if any)
	while (j < n2)
		a[k++] = R[j++];

	delete[] L;
	delete[] R;
}
#pragma endregion

#pragma region QUICK SORT
void quickSort(int a[], int l, int r) {
	int leftPtr = l, rightPtr = r;
	int tmp;
	int pivot = a[(l + r) / 2];

	/* partition */
	while (leftPtr <= rightPtr) {
		while (a[leftPtr] < pivot)
			leftPtr++;
		while (a[rightPtr] > pivot)
			rightPtr--;

		if (leftPtr <= rightPtr) {
			swap(a[leftPtr], a[rightPtr]);

			leftPtr++;
			rightPtr--;
		}
	};

	//now left & right pointers cross each other, creating 2 subarrays
	if (l < rightPtr)
		quickSort(a, l, rightPtr);
	if (leftPtr < r)
		quickSort(a, leftPtr, r);

	//CHOOSE LAST PIVOT
	//if (l < r) {
	//	//pi is the partitioning index, a[pi] is the pivot after after move down to middle (previously it's the last elem of a)
	//	int pi = PartitioningIndex(a, l, r);
	//	QuickSort(a, l, pi - 1); //fist halve
	//	QuickSort(a, pi + 1, r); //second halve
	//}
}

int partitioningIndex(int a[], int l, int r) {
	int pivot = a[r]; //store the pivot index
	int j = r; //keep track of pivot index, init it with last elem, then move it to left gradually 'til it's in right pos

	for (int i = 0; i < j;) {
		//instructions are in the notebook
		if (a[i] > pivot) {
			swap(a[i], a[j - 1]);
			swap(a[j], a[j - 1]);
			j--;
		}
		else
			i++;
	}

	return j;
}
#pragma endregion

#pragma region SELECTION SORT
void selectionSort(int a[], int n) {
	for (int i = 0; i < n; i++) {
		//find min in [i..n-1]
		int minIdx = i;

		for (int j = i + 1; j < n; j++) {
			if (a[minIdx] > a[j]) { //suppose min is a[i] (fisrt elem of the (sub)aay)
				minIdx = j;
			}
		}

		swap(a[i], a[minIdx]); //try to move min to top
	}
}
#pragma endregion

#pragma region HEAP SORT
//top-down approach because u are dealing w/ TREE (i.e cannot go backward)
//<=> heapify starts at nodeIdx and goes down
void heapify(int a[], int n, int nodeIdx) {
	int largest = nodeIdx; //keep track of the largest index
	int l = 2 * nodeIdx + 1; //left child's index of node
	int r = 2 * nodeIdx + 2;   //right child's index of node

	//try to find the larger child
	if (l < n && a[l] > a[largest]) //the first cond is for leaves
		largest = l;
	if (r < n && a[r] > a[largest])
		largest = r;

	//Base case
	if (largest != nodeIdx) { //if find the largest elem to swap
		swap(a[nodeIdx], a[largest]); //after swapping, largest become nodeIdx in next recursive call
		heapify(a, n, largest);
	}
}

void nonRecursiveHeapify(int a[], int n, int pos) {
	int k = pos;
	bool isHeap = false;

	while (!(isHeap && 2 * k + 1 < n)) {
		int l = 2 * k + 1; //left child

		//find the idx of the largest child bw 2 children
		if (l < n - 1) //right child exists !
			if (a[l] < a[l + 1])
				l = l + 1;

		if (a[k] >= a[l])
			isHeap = true;
		else {
			swap(a[k], a[l]);
			k = l; //heapify top-to-bot
		}
	}
}

void heapSort(int a[], int n) {
	//- Step 1: construct max heap, from right -> left, bot -> top
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(a, n, i);

	//- Step 2: "delete" element from heap ("delete" = swap deleted node with current last node & ignore that last node)
	for (int i = n - 1; i > 0; i--) {
		//move current root to end
		swap(a[0], a[i]);
		//heapify that newly created node
		heapify(a, i - 1, 0);
	}
}
#pragma endregion

#pragma region COUNTING SORT
int getMax(int a[], int n) {
	int max = a[0];

	for (int i = 1; i < n; i++)
		if (max < a[i])
			max = a[i];

	return max;
}

void countingSort(int a[], int n) {
	int m = getMax(a, n) + 1;
	int* counter = new int[m]; 
	int* res = new int[n];

	memset(counter, 0, m * sizeof(int));

	//compute each elem's frequency
	for (int i = 0; i < n; i++)
		++counter[a[i]];

	//update count[]
	for (int i = 1; i < m; i++)
		counter[i] += counter[i - 1];

	//sort the aay onto a new aay
	for (int i = n - 1; i >= 0; i--) 
		res[--counter[a[i]]] = a[i];
	
	//copy res[] to the original aay a[]
	for (int i = 0; i < n; i++)
		a[i] = res[i];

	delete[] counter;
	delete[] res;
}
#pragma endregion

#pragma region RADIX SORT
void radixSort(int a[], int n) {
	int max = getMax(a, n);

	//move from radix 1 -> radix 10^x; from right to left
	//the loop(max/radix) = the number of digits that need to countsort
	for (int radix = 1; max / radix > 0; radix *= 10) {
		countSort(a, n, radix);
	}
}

//countsort for each radix bucket (same as "countingSort" function
void countSort(int a[], int n, int radix) {
	int count[10] = { 0 };
	int* b = new int[n];

	//memset(count, 0, 10 * sizeof(int));

	//create count[]
	for (int i = 0; i < n; i++) {
		int digit = (a[i] / radix) % 10;
		count[digit]++;
	}

	//update count[]
	for (int i = 1; i < 10; i++)
		count[i] = count[i] + count[i - 1];

	//push each data to b[]
	//to maintain the stability, we need to go backward (n -> 0)
	for (int i = n - 1; i >= 0; i--) {
		int digit = (a[i] / radix) % 10;
		b[--count[digit]] = a[i];
	}

	//copy b[] to a[]
	for (int i = 0; i < n; i++)
		a[i] = b[i];

	delete[] b;
}

#pragma endregion

#pragma region BUBLE SORT
void bubbleSort(int a[], int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++)
			if (a[j] < a[i])
				swap(a[i], a[j]);
	}
}
#pragma endregion

#pragma region INSERTION SORT
void insertionSort(int a[], int n) {
	for (int i = 1; i < n; i++) {
		int value = a[i];
		int j = i;

		// Find the index j within the sorted subset a[0..i-1]
		// where element a[i] belongs
		while (j > 0 && a[j - 1] > value) {
			a[j] = a[j - 1];
			j--;
		}

		// Note that subaay a[j..i-1] is shifted to
		// the right by one position i.e. a[j+1..i]
		a[j] = value;
	}
}

//prerequisite: a[] must be sorted
int binarySearch(int a[], int item, int l, int r) {
	if (r <= l)
		return (item > a[l]) ? (l + 1) : l;

	int mid = (l + r) / 2;

	if (item == a[mid])
		return mid + 1;
	if (item > a[mid])
		return binarySearch(a, item, mid + 1, r);

	return binarySearch(a, item, l, mid - 1);
}

void binaryInsertionSort(int a[], int n) {
	//starting from 1 cuz {a[0]} is already sorted
	for (int i = 1; i < n; i++) {
		int rightBoundary = i - 1;
		int current = a[i];

		//find the location to insert /current/
		int idx = binarySearch(a, current, 0, rightBoundary);

		//Move all elements after location to create space
		while (idx <= rightBoundary) {
			a[rightBoundary + 1] = a[rightBoundary];
			rightBoundary--;
		}
		a[rightBoundary + 1] = current;
	}
}
#pragma endregion

#pragma region SHAKER SORT
void shakerSort(int a[], int n) {
	int left = 0;
	int right = n - 1;
	int newStart = 0;

	while (left < right) {
		//pass 1: go left to right
		for (int i = left; i < right; i++) {
			if (a[i] > a[i + 1]) {
				swap(a[i], a[i + 1]);
				newStart = i;
			}
		}

		//pass 2: go right to left
		right = newStart;

		for (int i = right; i > left; i--) {
			if (a[i] < a[i - 1]) {
				swap(a[i], a[i - 1]);
				newStart = i;
			}
		}

		left = newStart; //next loop
	}
}
#pragma endregion

#pragma region SHELL SORT 
void shellSort(int a[], int n){
	// Start with a big gap, then reduce the gap 
	for (int gap = n / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < n; i++) {
			// sort sub-lists created by applying gap 
			int temp = a[i];

			//shift sorted elements right-to-left 
			//until the correct location for a[i] is found 
			//This step contains INSERTION SORT
			int j;
			for (j = i; j >= gap && a[j - gap] > temp; j -= gap)
				a[j] = a[j - gap];

			//put temp (the original a[i]) in its correct location 
			a[j] = temp;
		}
	}
}
#pragma endregion

#pragma region FLASH SORT
void flashSort(int a[], int n) {
	//FIND MIN/MAX
	int min = a[0], maxIndex = 0;

	for (int i = 1; i < n; i++) {
		if (a[i] > a[maxIndex]) 
			maxIndex = i;
		if (a[i] < min) 
			min = a[i];
	}

	if (n == 1) 
		return;
	else if (n == 2) {
		a[0] = min;
		a[1] = a[maxIndex];
		return;
	}

	int max = a[maxIndex];
	//so cac phan lop
	int numClasses = 0.45 * n;
	//K[i] la phan lop cua a[i]
	int* classOf = new int[n];
	//store the upper bounds of each class, and the upper bound of a class 
	//is the number of elements in that class and every class before it
	int* lenClass = new int[numClasses];
	memset(lenClass, 0, numClasses * sizeof(int));

	//Find class (not exact location) for a[i]
	for (int i = 0; i < n; i++) {
		classOf[i] = (long long)(a[i] - min) * (numClasses - 1) / (max - min);
		lenClass[classOf[i]]++;
	}

	//store the upper bounds of each class
	for (int i = 1; i < numClasses; i++) 
		lenClass[i] += lenClass[i - 1];


	//ELEMENTS PERMUTATION
	//find exact location in a[i]'s class
	for (int i = 0; i < n; i++) {
		//if an elem is unclassified, i.e still below upper bound
		while (i < lenClass[classOf[i]]) {
			int temp = lenClass[classOf[i]];

			swap(a[i], a[temp - 1]);
			//now, class at index i & tmp-1 change due to the above change
			swap(classOf[i], classOf[temp - 1]);
			//after inserting, decrease l[] (upper bound/size) by 1
			lenClass[classOf[i]]--;
		}
	}

	//DO INSERTION SORT FOR EACH CLASS
	for (int i = 1; i < n; i++) {
		int j = i - 1;
		int t = a[i];

		while (a[j] > t && j >= 0) {
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = t;
	}

	delete[] lenClass;
	delete[] classOf;
}
#pragma endregion

void sortingAlgorithm(int a[], int n, string nameSoting) {
	if (nameSoting == "quick sort")
		quickSort(a, 0, n - 1);
	else if (nameSoting == "merge sort")
		mergeSort(a, 0, n - 1);
	else if (nameSoting == "selection sort")
		selectionSort(a, n);
	else if (nameSoting == "insertion sort")
		insertionSort(a, n);
	else if (nameSoting == "binary insertion sort")
		binaryInsertionSort(a, n);
	else if (nameSoting == "bubble sort")
		bubbleSort(a, n);
	else if (nameSoting == "shaker sort")
		shakerSort(a, n);
	else if (nameSoting == "shell sort")
		shellSort(a, n);
	else if (nameSoting == "heap sort")
		heapSort(a, n);
	else if (nameSoting == "counting sort")
		countingSort(a, n);
	else if (nameSoting == "radix sort")
		radixSort(a, n);
	else if (nameSoting == "flash sort")
		flashSort(a, n);
	else
		cout << "invalid sorting type";
}
