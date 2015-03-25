/*****************************************
# File Name:stl_set.cpp
# Author:Charlley88
# Mail:charlley88@163.com
*****************************************/

#include <iostream>
#include <set>

using namespace std;

/*
 * set是有序的容器，里面的元素都是排序好的（map按照key也是排好序的），支持插入，删除，查找等操作
 * 所有操作都是在logN时间内完成
 * 删除，删除元素a，在定义比较关系下，和a相等的元素都会被删除
 *
 * set元素可以是任意类型的，但是由于需要排序，所有元素必须有一个序，int可以直接比较，不能的时候需要重载运算符
 * STL的比较关系都是用 < 来确定的，所以必须定义 < 
 *
 * 初始化：和vector很类似
 * set<int> first;
 * int myints[] = {10,11,13,14,15};
 * set<int> second(myints,myints+5);
 * set<int> third(second);
 * set<int> fourth(second.begin(),second.end());
 */

/*
typedef struct {
    int a,b;
    char s;
}newtype;

struct compare //there is no ()
{
    bool operator()(const newtype &a, const newtype &b) const{
        return a.s<b.s;
    }
};//the “; ” is here;

set<newtype,compare>element;

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
