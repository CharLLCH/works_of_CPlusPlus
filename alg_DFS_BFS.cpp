/*****************************************
# File Name:alg_DFS_BFS.cpp
# Author:Charlley88
# Mail:charlley88@163.com
*****************************************/

#include <iostream>
#include <vector>
#include <stack>
#include <deque>

using namespace std;

#define Element char

typedef struct Node{
    Element data;
    struct Node *lchild;
    struct Node *rchild;
    Node(){}
    Node(Element x){data = x; lchild = NULL; rchild = NULL;}
}*Tree;

int index = 0;

void treeNodeConstructor(Tree &root, Element data[]){
    Element e = data[index++];
    if (e == '#'){
        root = NULL;
    }else{
        root = new Node(e);
        root->data = e;
        treeNodeConstructor(root->lchild,data);
        treeNodeConstructor(root->rchild,data);
    }
}

void depthFistSearch(Tree root){
    stack<Tree> nodeStack;
    nodeStack.push(root);
    Tree node;
    while(!nodeStack.empty()){
        node = nodeStack.top();
        cout<<node->data<<endl;
        nodeStack.pop();
        if(node->rchild){
            nodeStack.push(node->rchild);
        }
        if(node->lchild){
            nodeStack.push(node->lchild);
        }
    }
}

void breadthFistSearch(Tree root){
    deque<Tree> nodeQueue;
    nodeQueue.push_back(root);
    Tree node;
    while(!nodeQueue.empty()){
        node = nodeQueue.front();
        nodeQueue.pop_front();
        cout<<node->data<<endl;
        if(node->lchild){
            nodeQueue.push_back(node->lchild);
        }
        if(node->rchild){
            nodeQueue.push_back(node->rchild);
        }
    }
}

int main(){
    Element data[15] = {'A', 'B', 'D', '#', '#', 'E', '#', '#', 'C', 'F','#', '#', 'G', '#', '#'};
    Tree tree;
    treeNodeConstructor(tree,data);
    cout<<"深度优先遍历结果： "<<endl;
    depthFistSearch(tree);
    cout<<"广度优先遍历结果： "<<endl;
    breadthFistSearch(tree);
    return 0;
}
