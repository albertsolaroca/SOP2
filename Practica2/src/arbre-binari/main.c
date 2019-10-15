/**
 *
 * Main file 
 * 
 * This file is an example that uses the red-black-tree.c functions.
 *
 * Lluis Garrido, July 2019.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>  

#include "red-black-tree.h"

#define MAXVALUE 10
#define MAXCHAR 100


/**
 *
 *  Main function. This function is an example that shows
 *  how the binary tree works.
 *
 */



void add_word_to_tree(char* paraula, rb_tree* tree){
    node_data *n_data;

   
    /* Search if the key is in the tree */
    n_data = find_node(tree, paraula); 
    

    if (n_data != NULL) {

        /* If the key is in the tree increment 'num' */
        n_data->num_times++;
        free(paraula);
    } else {

        /* If the key is not in the tree, allocate memory for the data
        * and insert in the tree */

        n_data = malloc(sizeof(node_data));
        
        /* This is the key by which the node is indexed in the tree */
        n_data->key = paraula;
        
        /* This is additional information that is stored in the tree */
        n_data->num_times = 1;

        /* We insert the node in the tree */
        insert_node(tree, n_data);
    }
    

}




void process_line(char *line, rb_tree* tree)
{
    int i, j, is_word, len_line;
    char paraula[MAXCHAR];
    node_data *n_data;
    //char* tilde = "'";
    i = 0;

    len_line = strlen(line);

    /* Search for the beginning of a candidate word */

    while ((i < len_line) && (isspace(line[i]) || ispunct(line[i]))) i++; 

    /* This is the main loop that extracts all the words */

    while (i < len_line)
    {
        j = 0;
        is_word = 1;

        /* Extract the candidate word including digits if they are present */

        do {

            if (isalpha(line[i]) || line[i] == '\'')
                paraula[j] = line[i];
            else
                is_word = 0;

            j++; i++;

            /* Check if we arrive to an end of word: space or punctuation character */

        } while ((i < len_line) && (!isspace(line[i])) && (!ispunct(line[i]) || line[i] == '\''));
        

        /* If word insert in list */

        if (is_word) {

            /* Put a '\0' (end-of-word) at the end of the string*/
            paraula[j] = 0;

            n_data = find_node(tree, paraula);

            if (n_data) { 
                printf("La paraula %s apareix %d cops a l'arbre.\n", paraula, n_data->num_times);
                n_data->num_times++;
            }
        }

        /* Search for the beginning of a candidate word */

    while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i]) && strcmp(&line[i], "'")!=0))) i++;

    } /* while (i < len_line) */
}

void process_file(rb_tree* tree){
    FILE *fp;
    char* filename;
    char line[MAXCHAR];
    char* linia;
    int i;
    
    filename = "../base_dades/etext00/00ws110.txt";
    
    fp = fopen(filename, "r");
    if (!fp) {
        printf("Could not open file\n");
        exit(1);
    }
    
    while (fgets(line, MAXCHAR, fp)){
        linia = malloc((strlen(line)+1)*sizeof(char));
        for(i=0;i<strlen(line);i++)
            linia[i] = line[i];
        process_line(linia,tree);
    }
    
}

void extract_words(rb_tree* tree){
    FILE *fp;
    char line[MAXCHAR];
    char *palabra;
    int i = 0;
    
    fp = fopen("../diccionari/words", "r");
    if (!fp) {
        printf("Could not open file\n");
        exit(1);
    }
    
    while (fgets(line, MAXCHAR, fp)){
        palabra = malloc((strlen(line)+1)*sizeof(char));
        for(i=0;i<strlen(line);i++)
            palabra[i] = line[i];
        palabra[strlen(line)-1] = 0;
        add_word_to_tree(palabra,tree);
    }
    fclose(fp);
}

int main(int argc, char **argv)
{
  rb_tree *tree;

  printf("Test with red-black-tree\n");

  /* Allocate memory for tree */
  tree = (rb_tree *) malloc(sizeof(rb_tree));

  /* Initialize the tree */
  init_tree(tree);

  extract_words(tree);
  
  
  process_file(tree);
  
    
  /*char* a = "maraca";
  node_data *n_data;
  n_data = find_node(tree, a);

  if (n_data) { 
    printf("La paraula %s apareix %d cops a l'arbre.\n","maraca", n_data->num_times);
    ct += n_data->num_times;
  }

  printf("Nombre total que vegades que s'ha accedit a l'arbre: %d\n", ct);*/
  
  
  
  
  /* Delete the tree */
  delete_tree(tree);
  free(tree);

  return 0;
}

