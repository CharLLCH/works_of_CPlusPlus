/*****************************************
# File Name:alg_HeapSort.cpp
# Author:Charlley88
# Mail:charlley88@163.com
*****************************************/

/*
 * 堆排基本思想：
 * 将数组A创建成一个大顶堆，
 * 然后交换根和最后一个叶节点x，
 * 将x从堆中去掉，形成新的堆A1，
 * 然后重复以上动作直到堆中只剩下一个节点
 */

#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

template<typename Type>
class Heap{
    public:
        Heap(const vector<Type>& a_array){
            //关于assign函数，其是在不能使用赋值"="的情况下，可以将一个容器中的部分元素通过迭代器传递赋值到另一个容器中
            //但是在assign使用过程中，注意调用assign函数的容器必须有足够的空间来容纳复制过来的元素
            m_array.assign(a_array.begin(),a_array.end()); 
        }

        //Compare comp 传进来一个函数！！
        template<typename Compare>
        void sort(Compare comp);

        void printArray(const vector<Type>& a_array);

    private:
        vector<Type> m_array;

        //comp 为less<Type> 则大数下沉，创建最小堆
        //comp 为greater<Type> 则小数下沉，最大堆
        template<typename Compare>
        void createHeap(Compare comp); //建堆
        template<typename Compare>
        void downElement(int a_elem, Compare comp); //下沉元素
};

template<typename Type>
template<typename Compare>
void Heap<Type>::sort(Compare comp){
    //排序过程就是取出堆顶元素push到临时数组，
    //然后将堆顶元素和最后一个元素交换，再pop掉最后一个元素，直到堆中没有元素，
    //这样就获得了一个有序的数组，然后再复制到堆中
    printArray(m_array);
    createHeap(comp);
    vector<Type> array;
    for (int i = m_array.size() -1; i >= 0; i--){
        array.push_back(m_array[0]); //保留堆顶
        swap(m_array[0],m_array[i]); //交换
        m_array.pop_back(); //去掉最后一个元素
        downElement(0,comp); //新的首元素下沉
    }
    printArray(array);
    m_array.assign(array.begin(),array.end());
}

template<typename Type>
template<typename Compare>
void Heap<Type>::createHeap(Compare comp){
    //从最后一个非叶节点开始，将每个父节点都调整成最小堆
    //A[n/2+1,...,n]都是树的叶子节点（完全二叉树性质）
    //所以从ceiling(n/2)开始到1，对每个元素执行max/minHeapify
    for (int i = m_array.size() / 2 -1; i >= 0; i--){
        downElement(i,comp);
    }
}

template<typename Type>
template<typename Compare>
void Heap<Type>::downElement(int a_elem, Compare comp){
    //下沉元素
    int min; //最小元素下标
    int index = a_elem; //当前下沉元素下标
    while(index*2 + 1 < m_array.size()){
        //存在左节点
        min = index * 2 + 1;
        if (index * 2 + 2 < m_array.size()){
            //存在右节点
            if (comp(m_array[index*2+2],m_array[min])){
                //左右节点比较，选出最小的
                min = index *2 + 2;
            }
        }
        //同子节点比较，若父节点最小则结束
        if (comp(m_array[index],m_array[min])){
            break;
        }
        else{
            swap(m_array[index],m_array[min]);
            index = min;
        }
    }
}

template<typename Type>
void Heap<Type>::printArray(const vector<Type>& a_array){
    for(int i = 0; i < a_array.size(); i++){
        cout<<a_array[i]<<" ";
    }
    cout<<endl;
}

int main(){
    vector<int> array;
    for(int i = 10; i < 20; i++){
        array.push_back(i);
    }
    //打乱顺序
    random_shuffle(array.begin(),array.end());
    Heap<int> heap(array);
    heap.sort(less<int>());
    heap.sort(greater<int>());
    return 0;
}
