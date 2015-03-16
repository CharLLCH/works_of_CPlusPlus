/*****************************************
# File Name:alg_BinarySearch.cpp
# Author:Charlley88
# Mail:charlley88@163.com
*****************************************/

#include <iostream>

using namespace std;

/*
 * 用到二分的地方，一般数组是有序，查找某个key，如果key大于中间则查找mid到end右侧
 * 如果小于则查找左侧，如果相等则返回
 */

//非递归
int BinarySearch(int *array, int aSize, int key){
    if (array == NULL || aSize == 0){
        return -1;
    }
    int start = 0;
    int end = aSize - 1;
    int mid = 0;

    while (start <= end){
        mid = ( start + end) / 2;
        if (array[mid] < key){
            start = mid + 1;
        }else if ( array[mid] > key){
            end = mid - 1;
        }else{
            return mid;
        }
    }
    return -1;
}

//递归
int BinarySearchRecursive(int *array, int low, int high, int key){
    if ( low > high )
        return -1;
    int mid = ( low + high ) / 2;
    if (array[mid] == key)
        return mid;
    else if (array[mid] < key)
        return BinarySearchRecursive(array, mid+1, high, key);
    else
        return BinarySearchRecursive(array, low ,mid-1 ,key);
}

int main(){
    int array[10];
    for (int i = 0 ; i < 10 ; i++)
        array[i] = i;

    cout<<"No recursive:"<<endl;
    cout<<"position:"<<BinarySearch(array, 10, 6)<<endl;
    cout<<"recursive:"<<endl;
    cout<<"position:"<<BinarySearchRecursive(array,0,9,6)<<endl;
    
    return 0;
}


