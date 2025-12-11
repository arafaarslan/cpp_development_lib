#ifndef _BUBBLE_SORT_
#define _BUBBLE_SORT_

class Sort{

    public : 
        Sort(int size) {
            ARRAY_SIZE = size;
        }
        virtual void doit(int arr[]) = 0;
    protected : 
        int ARRAY_SIZE;

};

#endif