/*****************************************
# File Name:STL_vector.cpp
# Author:Charlley88
# Mail:charlley88@163.com
*****************************************/

#include <iostream>
#include <vector>

using namespace std;

/* 
 * vector动态数组，命名空间std;
 * 声明一个int向量替代一维的数组 vector<int> a;
 * 声明一个一维数组向量亦可代替二维数组，一个数组的名字其实代表的是它的首地址
 * so  vector<int *> a;
 * 三维 vector<int **> a;
 *
 * vec.push_back() 数组最后添加一个数据
 * vec.pop_back() 去掉数组最后一个数据
 * vec.back() 获得最后一个单元的引用
 * vec.front() 数组头的引用
 * vec.at() 获得编号位置的数据
 * vec.begin() 头位置的迭代器
 * vec.end() 尾+1位置迭代器
 * vec.size() 当前数据大小
 * vec.capacity() 当前vector分配的大小
 * vec.resize() 改变当前数据大小
 * vec.reserve() 改变当前分配空间大小
 * vec.erase(pos) 删除当前指针指向的数据项
 * vec.erase(beg,end)
 * vec.insert(pos,elem) 插入
 * vec.clear() 清空
 * vec.empty() 判断是否为空
 * swap 与另一个vector交换数据
 *
 * vector<int> (ivec).swap(ivec); 创建一个临时的(ivec)，它是ivec的一份拷贝，vector的拷贝函数只分配拷贝元素需要的内存
 * 所以这个临时的(ivec)没有多余的容量，然后让临时的vector和ivec交换数据就收缩了过剩的空间
 *
 * vec.assign(beg,end) 将[beg;end]数据空间的数据赋值给vec
 * vec.assign(n,elem) 将n个elem拷贝赋值给vec
 * vec.cat(idx) 传回idx所指数据，越界则out_of_range
 * vec.~vector<Elem>() 销毁数据，释放内存
 */

/*
 * vector<int> a;
 * int b = 5;
 * a.push_back(b);
 * 改变b不会影响a[0]
 *
 * vector<int *> a;
 * int *b = new int[4];
 * b[0] = 0; b[1] = 1;
 * a.push_back(b);
 * delete b;
 * cout<<a[0]<<endl;
 * 此时输出的不是b初始化时的值，而是未知的
 *
 * 因为第一个开始是压入值，a[0]和b的地址是不同的，释放b也没事
 * 而第二个a[0] = b 地址同，释放b就释放a[0]，则a[0]数组存放的数值也就不定了
 */

/*
 * 使用sort排序，需要添加 #include <algorithm>
 * sort(vec.begin(),vec.end());默认升序
 * 可以通过重写比较函数，按照降序排列
 *      bool Comp(const int &a; const int &b){
 *          return a > b;
 *      }
 * 调用时，sort(vec.begin(),vec.end(),Comp)
 *
 typedef struct rect
{
    int id;
    int length;
    int width;

　　//对于向量元素是结构体的，可在结构体内部定义比较函数，下面按照id,length,width升序排序。
　　bool operator< (const rect &a)  const
    {
        if(id!=a.id)
            return id<a.id;
        else
        {
            if(length!=a.length)
                return length<a.length;
            else
                return width<a.width;
        }
    }
}Rect;
*/

int main(){
    return 0;
}
