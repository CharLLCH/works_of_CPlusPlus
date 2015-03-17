/*****************************************
# File Name:stl_map.cpp
# Author:Charlley88
# Mail:charlley88@163.com
*****************************************/

#include <iostream>
#include <map>

using namespace std;

/*
 * 1. 构造函数 map<string,int>mapstring; map<char,string>mapchar;
 *
 * 2. 添加数据 
 *      map<int, string>maplive;
 *      maplive.insert(pair<int,string>(102,"aclive"));
 *      maplive.insert(map<int,string>::value_type(321,"aclive"));
 *      maplive[112] = "april";   最简单的插入方式，没有就插入，有就覆盖
 *
 * 3. 查找
 *      find()函数返回一个迭代器指向键值为key的元素，如果没有找到返回指向尾部的迭代器
 *      map<int,string>maplive;
 *      map<int,string>::iterator l_it;
 *      l_it = maplive.find(112);
 *      if(l_it == maplive.end())
 *          cout<<"do not find the item of key 112"<<endl;
 *      else
 *          cout<<"found!"<<endl;
 *
 *      map<string,string>m;
 *      if(m["222"] == "")
 *          cout<<"do not find!"<<endl;
 *
 * 4. 删除
 *      map<int,string>maplive;
 *      map<int,string>::iterator l_it;
 *      l_it = maplive.find(112);
 *      if (l_it == maplive.end())
 *          cout<<"nothing to del";
 *      else
 *          maplive.erase(l_it);
 *
 * 5. map中的swap不是一个map中的元素互换，而是交换两个map！
 *      map<int,int>m1;
 *      map<int,int>m2;
 *      m1.insert(..);
 *      m2.insert(..);
 *      ...
 *      swap(m1.m2);
 *
 * 6. 因为map中的元素是自动按照key的升序排序，所以直接iterator遍历就是key有序的
 *      map<int,string>::iterator m1_iter;
 *      for(m1_iter = m1.begin(); m1_iter != m1.end(); m1_iter++)
 *          cout<<m1_iter->first<<" "<<m1_iter->second<<endl;
 *      ps: Key值是iterator的第一个值 iter->first ; value是iter的第二个值 iter->second;
 */

/*
 * begin() 返回第一个迭代器
 * end() 尾部的迭代器
 * clear() 删除所有元素
 * count(key) 统计该key出现次数，map中都是1吧，不是也被覆盖了
 * empty() 如果空则返回true
 * equal_range() 返回特殊条目的迭代器
 * iter.erase() 删除这个迭代器
 * find(key) 返回的是一个迭代器
 * insert() 插入
 * key_comp() 返回比较元素key的函数
 * value_comp() 返回比较元素value的函数
 * lower_bound() 返回键值大于等于给定元素的第一个位置
 * upper_bound() の，和上面对过来
 * max_size() 可以容纳的最大的元素个数
 * rbegin() 逆向迭代器
 * rend() 逆向迭代器
 * size() map中速度个数
 * swap() 交换两个map
 */ 

int main(){
    map<int,int>m1;
    m1[2] = 3;
    m1[3] = 2;
    m1[22] = 4;
    m1[5] = 2;
    m1[2] = 5;
    map<int,int>::iterator m1_iter;
    for(m1_iter=m1.begin();m1_iter!=m1.end();m1_iter++){
        cout<<m1_iter->second<<endl;
    }
    cout<<m1.count(2)<<endl;
    return 0;
}
