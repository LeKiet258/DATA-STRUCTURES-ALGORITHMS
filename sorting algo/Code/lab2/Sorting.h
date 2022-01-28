#pragma once
#include<iostream>
#include<algorithm>
#include <cmath>
#include <time.h>
#include <fstream>

using namespace std;

//MERGE SORT 
void mergeSort(int a[], int l, int r);
void merge(int a[], int l, int m, int r);

//QUICK SORT <divide & conquer>
void quickSort(int a[], int low, int high);
int partitioningIndex(int[], int low, int high);

//SELECTION SORT
void selectionSort(int a[], int n);

//HEAP SORT
//this function will take O(n) to create max heap instead of O(nlogn)
void heapify(int a[], int n, int node); //node: the int node to be heapified
void nonRecursiveHeapify(int a[], int n, int pos);
void heapSort(int a[], int n);

//COUNTING SORT
void countingSort(int a[], int n);

//RADIX SORT: not using comparisons
int getMax(int a[], int n);
void radixSort(int a[], int n);
void countSort(int a[], int n, int pos);

//BUBLE SORT
void bubbleSort(int a[], int n);

//SHAKER SORT: 
void shakerSort(int a[], int n);

//INSERTION SORT
int binarySearch(int a[], int item, int l, int r);
void insertionSort(int a[], int n);
void binaryInsertionSort(int a[], int n);

//SHELL SORT: a variation of insertion sort
void shellSort(int arr[], int n);

//FLASH SORT: in-place sorting, no subarrays needed
void flashSort(int a[], int n);

//GENERAL FUNCTION FOR CALLING SORTING ALGORITHMS
void sortingAlgorithm(int a[], int n, string nameSoting);

//DATA GENERATOR
template <class T>
void HoanVi(T& a, T& b)
{
	T x = a;
	a = b;
	b = x;
}

// Hàm phát sinh mảng dữ liệu ngẫu nhiên
void GenerateRandomData(int a[], int n);

// Hàm phát sinh mảng dữ liệu có thứ tự tăng dần
void GenerateSortedData(int a[], int n);

// Hàm phát sinh mảng dữ liệu có thứ tự ngược (giảm dần)
void GenerateReverseData(int a[], int n);

// Hàm phát sinh mảng dữ liệu gần như có thứ tự
void GenerateNearlySortedData(int a[], int n);

void GenerateData(int a[], int n, int dataType);