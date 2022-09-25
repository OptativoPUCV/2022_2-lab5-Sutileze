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
  TreeMap *new  = (TreeMap *)malloc(sizeof(TreeMap));
  new->root = NULL;
  new->lower_than = lower_than;
  return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  if (tree->root  == NULL){
    tree->root = createTreeNode(key ,value);
  }else {
    tree->current = tree->root;
    while(tree->current != NULL){
      if(is_equal(tree,tree->current->pair->key,key)){
        return;//return sin nada porque s usa para romper la funcion y que como ya está no se hace nada
      }
      if(tree->lower_than(key,tree->current->pair->key)){
        if(tree->current->left == NULL){
          tree->current->left = createTreeNode(key ,value);// si recorre el mapa y se topa con un null , agrega ahi el nuevo nodo
          tree->current->left->parent = tree->current;
          tree->current = tree->current->left;
          return;
        }
        tree->current = tree->current->left;
      }
      else{
        if(tree->current->right == NULL){
          tree->current->right =createTreeNode(key ,value);
          tree->current->right->parent = tree->current;
          tree->current = tree->current->right;
          return;
        }
        tree->current = tree->current->right;
      }
    }

    
  }

  
}

TreeNode * minimum(TreeNode * x){
  if(x==NULL)return NULL;
  while(x->left !=NULL){
    x = x ->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  TreeNode *parentNode = node->parent;
  if (node->right == NULL && node->left == NULL) {
    if (tree->lower_than(parentNode->pair->key, node->pair->key) == 1) {
      parentNode->right = NULL; 
    }else if (tree->lower_than(node->pair->key, parentNode->pair->key) == 1){
      parentNode->left = NULL; 
    }
  }else if ((node->left != NULL && node->right == NULL) || (node->left == NULL && node->right != NULL)) {
    if (node->left == NULL) {
      if (tree->lower_than(parentNode->pair->key, node->pair->key) == 1) {
        parentNode->right = node->right; 
      }else if (tree->lower_than(node->pair->key, parentNode->pair->key) == 1) {
        parentNode->left = node->right; 
      }
      node->right->parent = parentNode; 
    }
    if(node->right == NULL) {
      if (tree->lower_than(parentNode->pair->key, node->pair->key) == 1) {
        parentNode->right = node->left; 
      }else if (tree->lower_than(node->pair->key, parentNode->pair->key) == 1) {
        parentNode->left = node->left;
      }
      node->left->parent = parentNode;
    }
    free(node);
  } 
  else {
    TreeNode *min = minimum(node->right);
    node->pair->key = min->pair->key;
    node->pair->value = min->pair->value;
    removeNode(tree, min);
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
  tree->current = tree->root;
  while (tree->current != NULL){
    if(is_equal(tree,tree->current->pair->key,key)){
      return tree->current->pair;//comparo la raiz con la llave}
    }
    if(tree->lower_than(tree->current->pair->key,key)){
      tree->current = tree->current->right;//va comparando si es menor
    }else{
      tree->current = tree->current->left;
  }
  }
  
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  Pair *aux = searchTreeMap(tree,key);
  if(aux == NULL){
    if(tree->lower_than(tree->current->pair->key,key)){
      return tree->current->pair;
    }
    aux = nextTreeMap(tree);
  }
  return aux;
}

Pair * firstTreeMap(TreeMap * tree){
    tree->current = minimum(tree->root);
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if(tree->current == NULL)return NULL; // no hay siguiente
  if(tree->current->right != NULL){
    tree->current = minimum(tree->current->right);
    return tree->current->pair;
  }else{
    if(tree->current->parent == tree->root ){
      tree->current = NULL;
      return NULL;
    }
  }
  if(tree->lower_than(tree->current->pair->key, tree->current->parent->pair->key) == 1){
    tree->current = tree->current->parent;
  }else{
    if(tree->current->parent->parent == NULL){
      tree->current = NULL;
      return NULL;
    }
    tree->current = tree->current->parent->parent;
  }
  return tree->current->pair;
}
