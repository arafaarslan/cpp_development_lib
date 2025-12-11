#include "BubbleSort.h"

BubbleSort::BubbleSort(int size) : Sort(size){
}
void BubbleSort::doit(int arr[]){
    for(int i=1;i<ARRAY_SIZE;i++){
        for(int j=0;j<ARRAY_SIZE-1;j++){
            if(arr[j] > arr[j+1]){
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}