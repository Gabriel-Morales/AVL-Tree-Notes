//
//  main.c
//  CPlayground
//
//  Created by Gabe on 7/30/20.
//  Copyright © 2020 Gabe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX(a,b) (a > b ? a : b)


// List for purposes of integer tokenization.
// Doubly linked for easy reversal.
typedef struct LLNode {
    int data;
    struct LLNode* prev;
    struct LLNode* next;
} LLNode;

typedef struct List {
    LLNode* head;
    LLNode* tail;
} List;

typedef struct Node {
    
    int data;
    int height;
    int balanceFactor;
    
    
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    
} Node;

typedef struct Tree {
    Node* root;
} Tree;


void prettyPrintT (Node *pRoot, int iIndent)
{
    
    int i;
    if (pRoot == NULL)
        return;
    
    iIndent++;
    prettyPrintT (pRoot -> right, iIndent);
    for (i = 0; i < iIndent * 4; i++)
        printf(" ");
    printf("%d\n", pRoot -> data);
    prettyPrintT(pRoot -> left, iIndent);
    
}

void updateBals(Node* temp) {
    if (temp -> left == NULL && temp -> right == NULL) {
        temp -> balanceFactor = 0;
        temp -> height = 0;
    } else if (temp -> left == NULL) {
        temp -> height = temp -> right -> height + 1;
        temp -> balanceFactor = -1 - temp -> right -> height;
    } else if (temp -> right == NULL) {
        temp -> height = temp -> left -> height + 1;
        temp -> balanceFactor = temp -> left -> height + 1;
    } else {
        temp -> height = MAX(temp -> left -> height, temp -> right -> height) + 1;
        temp -> balanceFactor = temp -> left -> height - temp -> right -> height;
    }
}

List* newList(void);
LLNode* newLLNode(int data);

void insertToList(List* list, int data);
void freeList(List** list);

char* intToString(unsigned int  number);

Tree* newTree(void);
Node* newNode(int data);

void freeTree(Tree** tree);
void freeNodes(Node* root);
void insertToTree(Tree* tree, int data);
void insertToTreeReg(Tree* tree, int data);

void leftRotate(Tree* tree, Node* node);
void rightRotate(Tree* tree, Node* node);




int main(int argc, char * argv[]) {
    
    
    Tree* tree = newTree();
    
    srand((unsigned int) time(NULL));
    
    unsigned int limit =  50000000;
    
    int* nums = (int*) malloc(sizeof(int) * limit + 1);
    int i;
    printf("Starting to insert data...\r");
    for (i = 0; i <= limit; i ++) {
        int data = rand() % (limit + 1);
        nums[i] = data;
    }
    printf("Done preprocessing data.\n");
    
    unsigned long start = clock();
    for (i = 0; i <= limit; i ++) {
        insertToTree(tree, nums[i]);
    }
    unsigned long end = clock();
    double total_time = (end - start) / (double)(CLOCKS_PER_SEC);
    
    char* intStrRep = intToString(limit);
    
    printf("Time to insert %s number(s) to AVL Tree: %.20lf\n", intStrRep, total_time);
    
    //prettyPrintT(tree -> root, 0);
    
    
    Tree* bin = newTree();
    start = clock();
    for (i = 0; i <= limit; i ++) {
        insertToTreeReg(bin, nums[i]);
    }
    end = clock();
    total_time = (end - start) / (double)(CLOCKS_PER_SEC);
    
    
    printf("Time to insert %s number(s) to Binary Tree: %.20lf\n", intStrRep, total_time);
    
    free(nums);
    free(intStrRep);
    freeTree(&tree);
    freeTree(&bin);
    
     
    return 0;
}

char* intToString(unsigned int number) {
    
    // Get the number of digits in a number.
    unsigned int res = number;
    int divisor = 10;
    int cnt = 1;
    while (res % divisor != number) {
        cnt++;
        divisor *= 10;
    }
    
    
    // Get number of expected commas.
    int commas = 0;
    
    int i;
    for (i = 1; i < cnt; i++) {
        if (i % 3 == 0) {
            commas++;
        }
    }
    
    int buf_size = cnt + commas + 1;
    
    int temp = number;
    
    List* list = newList();
    List* tok = newList();
    
    // tokenize the number into the list.
    cnt = 1;
    while (temp != 0) {
        
        insertToList(list, temp % 10);
        if (cnt % 3 == 0) {
            insertToList(list, ',');
        }
        temp /= 10;
        cnt++;
    }
    
    // remove fencepost issue
    if ( list -> tail -> data == ',' ) {
        LLNode* temp = list -> tail;
        list -> tail = list -> tail -> prev;
        temp -> prev = NULL;
        list -> tail -> next = NULL;
        free(temp);
    }
    
    
    // reverse the list into a new one, adding the commas.
    LLNode* tail = list -> tail;
    while (tail != NULL) {
        
        insertToList(tok, tail -> data);
        tail = tail -> prev;
    }
    
    freeList(&list);
    
    
    
    
    char* buff = (char*) malloc(sizeof(char) * buf_size);
    buff[buf_size - 1] = '\0';
    int size = 0;
    
    // Translate characters in list and fill the buffer.
    LLNode* head = tok -> head;
    while (head != NULL) {

        switch (head -> data) {
            case 0:
                buff[size] = '0';
                break;
            case 1:
                buff[size] = '1';
                break;
            case 2:
                buff[size] = '2';
                break;
            case 3:
                buff[size] = '3';
                break;
            case 4:
                buff[size] = '4';
                break;
            case 5:
                buff[size] = '5';
                break;
            case 6:
                buff[size] = '6';
                break;
            case 7:
                buff[size] = '7';
                break;
            case 8:
                buff[size] = '8';
                break;
            case 9:
                buff[size] = '9';
                break;
            default:
                buff[size] = ',';
        }
        
        head = head -> next;
        size++;
    }
    
    freeList(&tok);
    
    if (buff == NULL){
        printf("Malloc error");
        exit(1);
    }
    
    return buff;
    
}


void insertToList(List* list, int data) {
    
    if (list -> head == NULL && list -> tail == NULL) {
        list -> head = newLLNode(data);
        list -> tail = list -> head;
        return;
    }
    
    list -> tail -> next = newLLNode(data);
    list -> tail -> next -> prev = list -> tail;
    list -> tail = list -> tail -> next;
    
}

List* newList(void) {
    
    List* temp = (List*) malloc(sizeof(List));
    
    if (temp == NULL) {
        printf("Malloc error.");
        exit(1);
    }
    
    temp -> head = NULL;
    temp -> tail = NULL;
    
    return temp;
}


LLNode* newLLNode(int data) {
    
    LLNode* temp = (LLNode *) malloc(sizeof(LLNode));
    
    if (temp == NULL) {
        printf("Malloc error.");
        exit(1);
    }
    
    temp -> data = data;
    temp -> next = NULL;
    temp -> prev = NULL;
    
    return temp;
    
}


void insertToTree(Tree* tree, int data) {
    
    if (tree -> root == NULL) {
        tree -> root = newNode(data);
        return;
    }
    
    Node* p = tree -> root;
    Node* q = p;
    
    while ( p != NULL ) {
        q = p;
        
        if (data < p -> data) {
            p = p -> left;
        } else if (data > p -> data) {
            p = p -> right;
        } else {
            return;
        }
        
    }
    
    p = newNode(data);
    
    if (p -> data < q -> data) {
        q -> left = p;
    } else {
        q -> right = p;
    }
    
    p -> parent = q;
    
    
    // Walk up the tree to fix imbalances along ancestral route.
    while (p != NULL) {
        
        updateBals(p);
        
        if ( p -> balanceFactor == 2 ) {
            
            // Left imbalance
            Node* child = p -> left;
            
            if (child -> balanceFactor == 1) {
                // both positive (single rotation)
                // right rotate on root
                rightRotate(tree, p);
            } else {
                // different (double rotation)
                leftRotate(tree, child);
                rightRotate(tree, p);
            }
            
            
        } else if (p -> balanceFactor == -2) {
            
            // Right imbalance
            Node* child = p -> right;
            
            if (child -> balanceFactor == -1) {
                // both negative (single rotation)
                // left rotate on root
                leftRotate(tree, p);
            } else {
                // different (double rotation)
                rightRotate(tree, child);
                leftRotate(tree, p);
            }
            
            
        } else {
            p = p -> parent;
        }
        
    }
    
}


void insertToTreeReg(Tree* tree, int data) {
    
    if (tree -> root == NULL) {
        tree -> root = newNode(data);
        return;
    }
    
    Node* p = tree -> root;
    Node* q = p;
    
    while ( p != NULL ) {
        q = p;
        
        if (data < p -> data) {
            p = p -> left;
        } else if (data > p -> data) {
            p = p -> right;
        } else {
            return;
        }
        
    }
    
    p = newNode(data);
    
    if (p -> data < q -> data) {
        q -> left = p;
    } else {
        q -> right = p;
    }
    
    p -> parent = q;
    
}

void leftRotate(Tree* tree, Node* node) {
    
    Node* oldRoot = node;
    Node* newRoot = node -> right;
    Node* leftSubtree = newRoot -> left;
    Node* temp = oldRoot -> parent;
    
    oldRoot -> right = leftSubtree;
    
    if ( leftSubtree != NULL ) {
        leftSubtree -> parent = oldRoot;
    }
    
    
    newRoot -> left = oldRoot;
    newRoot -> parent = temp;
    oldRoot -> parent = newRoot;
    
    if (node == tree -> root) {
        
        tree -> root = newRoot;
        
    } else {
        
        if (temp -> left == oldRoot) {
            temp -> left = newRoot;
        } else {
            temp -> right = newRoot;
        }
        updateBals(temp);
    }
    
    updateBals(oldRoot);
    updateBals(newRoot);
    
    
}

void rightRotate(Tree* tree, Node* node) {
    
    Node* oldRoot = node;
    Node* newRoot = node -> left;
    Node* rightSubtree = newRoot -> right;
    Node* temp = oldRoot -> parent;
    
    if ( rightSubtree != NULL ) {
        rightSubtree -> parent = oldRoot;
    }
    
    
    oldRoot -> left = rightSubtree;
    newRoot -> right = oldRoot;
    newRoot -> parent = oldRoot -> parent;
    oldRoot -> parent = newRoot;
    
    if (node == tree -> root) {
        
        tree -> root = newRoot;
        
    } else {
        
        if (temp -> left == oldRoot) {
            temp -> left = newRoot;
        } else {
            temp -> right = newRoot;
        }
        updateBals(temp);
        
    }
    
    updateBals(oldRoot);
    updateBals(newRoot);
    
}


Tree* newTree(void) {
    
    Tree* temp = (Tree *) malloc(sizeof(Tree));
    temp -> root = NULL;
    
    if (temp == NULL) {
        printf("Malloc error\n");
        exit(1);
    }
    
    return temp;
}

Node* newNode(int data) {
    
    Node* temp = (Node *) malloc(sizeof(Node));
    
    if (temp == NULL) {
        printf("Malloc error\n");
        exit(1);
    }
    
    temp -> data = data;
    temp -> height = 0;
    temp -> balanceFactor = 0;
    
    temp -> parent = NULL;
    temp -> left = NULL;
    temp -> right = NULL;
    
    return temp;
    
}

void freeList(List** list)
{
    
    LLNode* head = (*list) -> head;
    
    while (head != NULL) {
        LLNode* temp = head -> next;
        free(head);
        head = temp;
    }
    
    (*list) -> head = NULL;
    (*list) -> tail = NULL;
    *list = NULL;
    
}

void freeTree(Tree** tree) {
    
    freeNodes((*tree) -> root);
    (*tree) -> root = NULL;
    *tree = NULL;
}

void freeNodes(Node* root) {
    
    if (root == NULL) {
        return;
    }
    
    freeNodes(root -> left);
    freeNodes(root -> right);
    free(root);
    
}
