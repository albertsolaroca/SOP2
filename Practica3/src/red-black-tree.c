/**
 *
 * Red-black tree implementation.
 *
 * Binary search trees work best when they are balanced or the path length from
 * root to any leaf is within some bounds. The red-black tree algorithm is a
 * method for balancing trees. The name derives from the fact that each node is
 * colored red or black, and the color of the node is instrumental in
 * determining the balance of the tree. During insert and delete operations,
 * nodes may be rotated to maintain tree balance. Both average and worst-case
 * search time is O(lg n).
 *
 * This implementation is original from John Morris, University of Auckland, at
 * the following link
 *
 * http://www.cs.auckland.ac.nz/~jmor159/PLDS210/niemann/s_rbt.htm
 *
 * and has been adapted here by Lluis Garrido, June 2018.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "red-black-tree.h"
#define MAXCHAR      100
#define MAGIC_NUMBER 0x01234567


/**
 *
 * Free data element. The user should adapt this function to their needs.  This
 * function is called internally by deletenode and is used to free the dynamic
 * memory that is stored inside node_data. The memory is allocated by the user
 * code, just before the node is inserted in the tree.
 *
 */

void free_node_data(node_data *data)
{
    free(data->key);
    free(data);
}

/**
 *
 * Compares if key1 is less than key2. Should return 1 (true) if condition
 * is satisfied, 0 (false) otherwise.
 *
 */

int compare_key1_less_than_key2(char* key1, char* key2)
{
    int rc;

    rc = 0;

    if (strcasecmp(key1,key2) < 0)
        rc = 1;

    return rc;
}

/**
 *
 * Compares if key1 is equal to key2. Should return 1 (true) if condition
 * is satisfied, 0 (false) otherwise.
 *
 */

int compare_key1_equal_to_key2(char* key1, char* key2)
{
    int result = 0;

    if (strcasecmp(key1, key2) == 0){
        result = 1;
    }
    //printf("Key1: %s , Key2: %s, result: %d\n",key1,key2,result);
    return result;
}

/**
 *
 * The red-black tree uses the sentilnel code as a leave node.
 *
 */

node sentinel = { NULL, NULL, NULL, NULL, BLACK};

/**
 *
 * Initialize the tree.
 *
 */

void init_tree(rb_tree *tree)
{
    tree->root = NIL;
    tree->num_nodes = 0;
}

/**
 *
 *  Find node containing the specified key. Returns the node.
 *  Returns NULL if not found.
 *
 */

node_data *find_node(rb_tree *tree, char* key) {

    node *current = tree->root;
    while(current != NIL){
        //printf("Entrem al while, current, key %s %s\n", current->data->key,key);
        if(compare_key1_equal_to_key2(key, current->data->key)){
            //printf("Number of times: %d", current->data->num_times);
            return (current->data);
        } else{
            current = compare_key1_less_than_key2(key, current->data->key) ?
                current->left : current->right;
        }
    }
    return NULL;
}

//TODO CHANGE CURRENT TO x
node* find_most_occurrences_recursive(node* x){
    node* current = x;
    node* max_right = NIL;
    node* max_left = NIL;
    if(current->right != NIL)
        max_right = find_most_occurrences_recursive(x->right);
    if(current->left != NIL)
        max_left = find_most_occurrences_recursive(x->left);
    if(max_right != NIL && max_right->data->num_times > current->data->num_times)
        current = max_right;
    if(max_left != NIL && max_left->data->num_times > current->data->num_times)
        current = max_left;
    return current;
}

node* find_most_occurrences(rb_tree* tree){
    if(tree->root != NIL){
        return find_most_occurrences_recursive(tree->root);
    } else{
        return NIL;
    }
}

void save_tree_recursive(node* node, FILE* fp){
    int len, num_times;
    if(node->right != NIL){
        len = strlen(node->right->data->key);
        num_times = node->right->data->num_times;
        fwrite(&len, sizeof(int), 1, fp);
        fwrite(node->right->data->key, sizeof(char),len,fp);
        fwrite(&num_times,sizeof(int),1,fp);
        save_tree_recursive(node->right,fp);
    }if(node->left != NIL){
        len = strlen(node->left->data->key);
        num_times = node->left->data->num_times;
        fwrite(&len, sizeof(int), 1, fp);
        fwrite(node->left->data->key, sizeof(char),len,fp);
        fwrite(&num_times,sizeof(int),1,fp);
        save_tree_recursive(node->left,fp);
    }
}

void save_tree(rb_tree* tree, FILE* fp){
    int magicNumber = MAGIC_NUMBER;
    int nodeNumber = tree->num_nodes;
    fwrite(&magicNumber,sizeof(int),1, fp);
    fwrite(&nodeNumber,sizeof(int),1, fp);
    if(tree->root != NIL){
        save_tree_recursive(tree->root,fp);
    }
}

void load_tree(rb_tree* tree, FILE* fp){
    int magicNumber;
    int nodeNumber;
    int i;
    int comodin;
    int len, num_times;
    char word[MAXCHAR];
    char* paraula;
    node_data *n_data;
    int it;
    comodin = fread(&magicNumber, sizeof(int), 1, fp);
    if(magicNumber == MAGIC_NUMBER){
        comodin = fread(&nodeNumber,sizeof(int),1, fp);
        for(i=0; i < nodeNumber - 1; i++){
            comodin = fread(&len, sizeof(int),1, fp);
            if (comodin != 0){
                printf("Longitud:%d\n", len);
            }
            comodin = fread(word, sizeof(char),len,fp);
            n_data = malloc(sizeof(node_data));

            paraula = malloc((len+1)* sizeof(char));
            for (it = 0; it <= len; it++){
                paraula[it] = word[it];
            }
            paraula[len] = '\0';
            printf("Word:%s \n", paraula);

            /* This is the key by which the node is indexed in the tree */
            n_data->key = paraula;

            comodin = fread(&num_times,sizeof(int),1,fp);
            if (comodin != 0){
                printf("Occurrences:%d \n", num_times);
            }
            /* This is additional information that is stored in the tree */
            n_data->num_times = num_times;

            /* We insert the node in the tree */
            insert_node(tree, n_data);
            tree->num_nodes++; 
            //free(n_data);
        }
    }
}

/**
 *
 * NO FA FALTA MODIFICAR EL CODI QUE HI HA SOTA PER FER LES PRACTIQUES.
 *
 */

/**
 *
 *  Function used to delete a tree. Do not call directly.
 *
 */

void delete_tree_recursive(node *x)
{
  //  printf("%d %s\n", x->data->num_times,x->data->key);
    if (x->right != NIL)
        delete_tree_recursive(x->right);

    if (x->left != NIL)
        delete_tree_recursive(x->left);

    free_node_data(x->data);
    free(x);
}


/**
 *
 *  Delete a tree. All the nodes and all the data pointed to by
 *  the tree is deleted.
 *
 */

void delete_tree(rb_tree *tree)
{
    if (tree->root != NIL)
        delete_tree_recursive(tree->root);
}

/**
 *
 *  Rotate node x to left. Should not be called directly by the user. This
 *  function is used internally by other functions.
 *
 */

void rotate_left(rb_tree *tree, node *x) {

    node *y = x->right;

    /* establish x->right link */
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;

    /* establish y->parent link */
    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    } else {
        tree->root = y;
    }

    /* link x and y */
    y->left = x;
    if (x != NIL) x->parent = y;
}

/**
 *
 *  Rotate node x to right. Should not be called directly by the user. This
 *  function is used internally by other functions.
 *
 */

void rotate_right(rb_tree *tree, node *x) {
    node *y = x->left;

    /* establish x->left link */
    x->left = y->right;
    if (y->right != NIL) y->right->parent = x;

    /* establish y->parent link */
    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
    } else {
        tree->root = y;
    }

    /* link x and y */
    y->right = x;
    if (x != NIL) x->parent = y;
}

/**
 *
 * Maintain Red-Black tree balance  after inserting node x. Should not be
 * called directly by the user. This function is used internally by other
 * functions.
 *
 */

void insert_fixup(rb_tree *tree, node *x) {
    /* check Red-Black properties */
    while (x != tree->root && x->parent->color == RED) {
        /* we have a violation */
        if (x->parent == x->parent->parent->left) {
            node *y = x->parent->parent->right;
            if (y->color == RED) {

                /* uncle is RED */
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {

                /* uncle is BLACK */
                if (x == x->parent->right) {
                    /* make x a left child */
                    x = x->parent;
                    rotate_left(tree,x);
                }

                /* recolor and rotate */
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotate_right(tree, x->parent->parent);
            }
        } else {

            /* mirror image of above code */
            node *y = x->parent->parent->left;
            if (y->color == RED) {

                /* uncle is RED */
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {

                /* uncle is BLACK */
                if (x == x->parent->left) {
                    x = x->parent;
                    rotate_right(tree, x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotate_left(tree,x->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

/**
 *
 * Allocate node for data and insert in tree. This function does not perform a
 * copy of data when inserting it in the tree, it rather creates a node and
 * makes this node point to the data. Thus, the contents of data should not be
 * overwritten after calling this function.
 *
 */

void insert_node(rb_tree *tree, node_data *data) {
    node *current, *parent, *x;

    /* Find where node belongs */
    current = tree->root;
    parent = 0;
    while (current != NIL) {
        if (compare_key1_equal_to_key2(data->key, current->data->key)) {
            printf("insert_node: trying to insert but primary key is already in tree.\n");
            exit(1);
        }
        parent = current;
        current = compare_key1_less_than_key2(data->key, current->data->key) ?
            current->left : current->right;
    }

    /* setup new node */
    if ((x = malloc (sizeof(*x))) == 0) {
        printf("insufficient memory (insert_node)\n");
        exit(1);
    }

    /* Note that the data is not copied. Just the pointer
       is assigned. This means that the pointer to the
       data should not be overwritten after calling this
       function. */

    x->data = data;

    /* Copy remaining data */
    x->parent = parent;
    x->left = NIL;
    x->right = NIL;
    x->color = RED;

    /* Insert node in tree */
    if(parent) {
        if(compare_key1_less_than_key2(data->key, parent->data->key))
            parent->left = x;
        else
            parent->right = x;
    } else {
        tree->root = x;
    }

    insert_fixup(tree, x);
}
