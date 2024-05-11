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
    if (node->left == NULL && node->right == NULL) {
        if (node != tree->root) {
            if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        } else {
            tree->root = NULL;
        }
        free(node);
    } 
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child;
        if (node->left != NULL) {
            child = node->left;
        } else {
            child = node->right;
        }
        if (node != tree->root) {
            if (node->parent->left == node) {
                node->parent->left = child;
                if (child != NULL) {
                    child->parent = node->parent;
                }
            } else {
                node->parent->right = child;
                if (child != NULL) {
                    child->parent = node->parent;
                }
            }
        } else {
            tree->root = child;
            if (child != NULL) {
                child->parent = NULL;
            }
        }
        free(node);
    }
    else {
        TreeNode* successor = minimum(node->right);
        int succKey = *(int*)successor->pair->key;
        void* succValue = successor->pair->value;
        if (successor->parent == node) {
            node->right = successor->right;
            if (successor->right != NULL) {
                successor->right->parent = node;
            }
        } else {
            successor->parent->left = successor->right;
            if (successor->right != NULL) {
                successor->right->parent = successor->parent;
            }
            successor->parent->left = successor->right;
            successor->parent = node->parent;
        }
        free(successor);
        node->pair->key = &succKey;
        node->pair->value = succValue;
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
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * node = tree->root;
    TreeNode * ub_node = NULL;
    while (node != NULL) {
        if (tree->lower_than(node->pair->key, key) == 1) {
            if (ub_node == NULL || tree->lower_than(node->pair->key, ub_node->pair->key) == 1)) {
                ub_node = node;
            }
            node = node->right;
        } else {
            if (node->left == NULL) return ub_node->pair;
            node = node->left;            
        }  
    } 
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
