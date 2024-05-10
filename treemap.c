#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * newNode = createTreeNode(key, value);
    if (tree->root == NULL) {
        tree->root = newNode;
        tree->current = newNode;
    } else {
        TreeNode * aux = tree->root;
        while(1) {
            if (tree->lower_than(key, aux->pair->key) == 1) {
                if (aux->left == NULL) {
                    aux->left = newNode;
                    newNode->parent = aux;
                    tree->current = newNode;
                    break;
                } else {
                    aux = aux->left;
                }
            } else if (tree->lower_than(key, aux->pair->key) == -1) {
                if (aux->right == NULL) {
                    aux->right = newNode;
                    newNode->parent = aux;
                    tree->current = newNode;
                    break;
                } else {
                    aux = aux->right;
                }
            } else {
                aux->pair->value = value;
                tree->current = newNode;
                free(newNode);
                break;
            }
        }
    }
}

TreeNode * minimum(TreeNode * node){
    if (node == NULL) return NULL;
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode* aux = tree->root;
    //TreeNode* parent = NULL;

    while (aux != NULL && aux != node) {
        //parent = aux;
        if (tree->lower_than(node->pair->key, aux->pair->key))
            aux = aux->left;
        else
            aux = aux->right;
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * node = tree->root;
    while (node != NULL) {
        if (is_equal(tree, node->pair->key, key)) {
            tree->current = node;
            return node->pair;
        }
        if (tree->lower_than(node->pair->key, key)) {
            node = node->right;
        }
        else node = node->left;    
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * node = minimum(tree->root);
    if (node == NULL) return NULL;
    tree->current = node;
    return node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
