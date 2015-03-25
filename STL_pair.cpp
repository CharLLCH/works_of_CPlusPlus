/*****************************************
# File Name:STL_pair.cpp
# Author:Charlley88
# Mail:charlley88@163.com
*****************************************/

#include <iostream>

using namespace std;

/*
 * pair一种模板类型，包含两个元素，类型可以相同也可以不同
 * 创建pair时如果不初始化的话，则调用默认构造函数初始化
 * pair<int,int> a;
 * pair<int,string> a(12,"james");
 *
 * 由于pair类型的使用比较繁琐，当需要定义多个pair类型时，可以typedef
 * typedef pair<string,string> author;
 * author pro("May","Lily");
 * author joye("James","joyce");
 *
 * pair类只有两个元素，first，second，直接使用普通的点操作符
 * pair<string,string> a("lily","poly");
 * string name;
 * name = pair.second;
 *
 * int a = 8;
 * string m = "james";
 * pair<int,string> newnode;
 * newnode = make_pair(a,m);
 *
 * map中添加的元素、迭代器返回的都是pair！
 */

int main(){
    return 0;
}
