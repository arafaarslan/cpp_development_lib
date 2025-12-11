#include "QuickSort.h"

QuickSort::QuickSort(int size) : Sort(size){
}

int QuickSort::partition(int arr[], int low, int high) {
    int pivot = arr[high];  // choose last element as pivot
    int i = low - 1;        // index of smaller element

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {  // if current element is smaller than pivot
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);  // place pivot in correct position
    return i + 1;                 // return pivot index
}

void QuickSort::quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);  // partition index
        quickSort(arr, low, pi - 1);         // sort left side
        quickSort(arr, pi + 1, high);        // sort right side
    }
}

void QuickSort::swap(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}

void QuickSort::doit(int arr[]){
    quickSort(arr, 0, ARRAY_SIZE-1);
}