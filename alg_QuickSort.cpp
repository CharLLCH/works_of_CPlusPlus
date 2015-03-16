/*****************************************
# File Name:alg_QuickSort.cpp
# Author:Charlley88
# Mail:charlley88@163.com
*****************************************/

/*
 * 快排基本思想：
 * 序列中选出一个基准数，然后进行partition分区，再对左右两个区间重复选，partiton，直到只有一个数
 * 其实就是一个挖坑，填坑的过程：
 *  i = L j = R ， 找基准数k（一般选第一或第二个数），第一个坑A[k]
 *  j-- 由后向前找比A[k]小的数，找到后，将此数填到A[k]中去，新坑A[j]
 *  i++ 由前找比它大的数，找到后，填到新坑A[j]中去，产生新坑A[i]
 *  然后再从后找，直到i == j，将基准数填到A[i]中去
 */

#include <iostream>

using namespace std;

void QuickSort(int *array, int first, in end){
    int i = first;
    int j = end;
    int tmp = partition(array);

    while(i < j){
        while( i < j && array[j] >= tmp)
            j--;

        array[i] = array[j];

        while( i < j && array[i] <= tmp)
            i++;

        array[j] = array[i];
    }
    array[i] = tmp;

    if(first < i-1)
        QuickSort(array,first,i-1);
    if(end > i+1)
        QuickSort(array,i+1,end);
}

int partition(int *array){
    return array[0];
}
