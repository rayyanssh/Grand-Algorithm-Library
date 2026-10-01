#include <stdio.h>
#include <stdlib.h>

typedef struct T {
    int data;
    struct T* left;
    struct T* right;
} T;

void inorder(T* tree, int h) {
    T* stack[h]; int top = -1;
    while (top != -1 || tree != NULL) {
        while (tree != NULL) {
            stack[++top] = tree;
            tree = tree->left;
        }
        tree = stack[top--];
        printf("%d ", tree->data);
        tree = tree->right;
    }
}

void postorder(T* tree, int h) {
    T* stack[h]; int top = -1; T* prev = NULL;
    while (top != -1 || tree != NULL) {
        while (tree != NULL) {
            stack[++top] = tree;
            tree = tree->left;
        }
        tree = stack[top];
        if (tree->right && prev != tree->right) tree = tree->right;
        else {
            printf("%d", tree->data);
            top--;
            prev = tree;
            tree = NULL;      
        }
    }
}