#ifndef _QUICK_SORT_H_
#define _QUICK_SORT_H_

#include "Sort.h"

class QuickSort : public Sort {

    public : 
        QuickSort(int size);
        void doit(int arr[]);
    private :
        void quickSort(int arr[], int low, int high);
        int partition(int arr[], int low, int high);
        void swap(int& a, int& b);
};

#endif