/*****************************************
# File Name:alg_Tree.cpp
# Author:Charlley88
# Mail:charlley88@163.com
*****************************************/

#include <iostream>
#include <stack>

using namespace std;

typedef struct tnode{
    public: int data;
    public: tnode *lchild, *rchild;

    tnode(){}
    tnode(int item, tnode *left, tnode *right):data(item),lchild(left),rchild(right){}
}*Tnode;

//递归遍历们
int preOrder(Tnode root){
    if(root){
        visit(root->data);
        preOrder(root->lchild);
        preOrder(root->rchild);
    }else{
        return 0;
    }
}

int midOrder(Tnode root){
    if(root){
        midOrder(root->lchild);
        visit(root->data);
        midOrder(root->rchild);
    }else{
        return 0;
    }
}

int posOrder(Tnode root){
    if(root){
        posOrder(root->lchild);
        posOrder(root->rchild);
        visit(root->data);
    }else{
        return 0;
    }
}

void visit(int data){
    cout<<data<<" ";
}

bool search(Tnode root, int val){
    if(root){
        if(root->data == val){
            return true;
        }else{
            return (search(root->lchild,val) || search(root->rchild,val));
        }
    }else{
        return false;
    }
}

Tnode insertTree(Tnode root, int insertVal){
    if(search(root,insertVal)){
        cout<<"已经存在"<<endl;
        return NULL;
    }else{
        if(root == NULL){
            root = new tnode(insertVal,NULL,NULL);
        }else{
            if(root->data > insertVal){
                root->lchild = insertTree(root->lchild,insertVal);
            }else{
                root->rchild = insertTree(root->rchild,insertVal);
            }
        }
        return root;
    }
}

//非递归实现
void preTraverse(Tnode root){
    stack<Tnode> stack;
    Tnode node = root;

    while(node != NULL || !stack.empty()){
        while(node != NULL){
            cout<<node->data<<" ";
            stack.push(node->lchild);
            node = node->lchild;
        }
        if(!stack.empty()){
            node = stack.top();
            node = node->rchild;
            stack.pop();
        }
    }
}

void inTraverse(Tnode root){
    stack<Tnode> stack;
    Tnode node = root;
    
    while(node != NULL || !stack.empty()){
        while(node != NULL){
            stack.push(node);
            node = node->lchild;
        }
        if(!stack.empty()){
            node = stack.top();
            cout<<node->data<<" ";
            stack.pop();
            node = node->rchild;
        }
    }
}

struct flag_node{
    Tnode node;
    bool flag;
}

void posTraverse(Tnode root){
    stack<flag_node> stack;
    Tnode node = root;
    flag_node = fnode;

    while(node != NULL || !stack.empty()){
        while(node != NULL){
            fnode->node = node;
            fnode->flag = false;
            stack.push(fnode);
            node = node->lchild;
        }
        if(!stack.empty()){
            fnode = stack.top();
            stack.pop();
            if(fnode->flag == false){
                fnode->flag = true;
                stack.push(fnode);
                node = fnode->node;
                node = node->rchild;
            }else{
                cout<<fnode->node->data<<endl;
            }
        }
    }
}

    


int main(){
    Tnode root = new tnode(1,NULL,NULL);
    return 1;
}
