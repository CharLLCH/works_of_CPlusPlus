/*****************************************
# File Name:alg_MergeSort.cpp
# Author:Charlley88
# Mail:charlley88@163.com
*****************************************/

#include <iostream>

using namespace std;

template <typename T> void Merge(T *array, int low, int mid, int high){
    int i, j, k;
    i = low;
    j = mid + 1;
    k = 0;
    T* tmp = new T[high - low + 1];

    while( i <= mid && j <= high){
        if (*(array + i) <= *(array + j))
            *(tmp + k++) = *(array + i++);
        else
            *(tmp + k++) = *(array + j++);
    }

    while( i <= mid )
        *(tmp + k++) = *(array + i++);
    while( j <= high )
        *(tmp + k++) = *(array + j++);

    for ( i=low, k=0; i <= high; i++,k++){
        *(array + i) = *(tmp + k);
    }

    delete []tmp;
}

template <typename T> void MergeSort(T *array, int low, int high){
    int mid;
    if (low < high){
        mid = ( low + high ) / 2;
        MergeSort(array, low, mid);
        MergeSort(array, mid+1, high);
        Merge(array, low, mid, high);
    }
}


int main(){
    int i;
    int a[] = {5,5,2,6,1,7,9,8,3};
    MergeSort<int>(a,0,9);
    for(i=0;i<9;i++)
        cout<<a[i]<<endl;

    return 0;
}
