/**
 *
 * Practica 3 
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  

#include "red-black-tree.h"

#define MAXCHAR      100
#define MAGIC_NUMBER 0x01234567

/**
 * 
 *  Menu
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
        n_data->num_times = 0;

        /* We insert the node in the tree */
        insert_node(tree, n_data);
        tree->num_nodes++;
    }
    

}




void process_line(char *line, rb_tree* tree)
{
    int i, j, is_word, len_line;
    char paraula[MAXCHAR];
    node_data *n_data;
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
                n_data->num_times++;
            }
        }

        /* Search for the beginning of a candidate word */

    while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i]) && strcmp(&line[i], "'")!=0))) i++;

    } /* while (i < len_line) */
}

void process_file(char* filename, rb_tree* tree){
    FILE *fp;
    char line[MAXCHAR];
    char* linia;
    int i;

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

void process_data_base(char* database, rb_tree* tree){

    int i = 0,j;
    char num_files[MAXCHAR];
    char line[MAXCHAR];
    char* file;
    char* database_directory = "../base_dades/";
    char* complete_name;
    FILE* fp;

    fp = fopen(database,"r");
    if (!fp) {
      printf("Could not open file\n");
      exit(1);
    }

    //Number of files to read
    if(atoi(fgets(num_files,MAXCHAR,fp)) == 1){
        printf("File found!");
    }

    while(fgets(line,MAXCHAR,fp) && i < atoi(num_files)){
        file = malloc((strlen(line) + 1) * sizeof(char));
        complete_name = malloc((strlen(line)+1)*sizeof(char) + strlen(database_directory)*sizeof(char));
        strcpy(complete_name, database_directory);
        for(j=0;j<strlen(line);j++)
            file[j] = line[j];
        file[strlen(line)-1] = 0;
        strcat(complete_name,file);
        process_file(complete_name,tree);
        i++;
    }
}

void process_dictionary(rb_tree* tree, char* dictionary){
    FILE *fp;
    char line[MAXCHAR];
    char *palabra;
    int i = 0;
    
    fp = fopen(dictionary, "r");
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














int menu() 
{
    char str[5];
    int opcio;

    printf("\n\nMenu\n\n");
    printf(" 1 - Creacio de l'arbre\n");
    printf(" 2 - Emmagatzemar arbre a disc\n");
    printf(" 3 - Llegir arbre de disc\n");
    printf(" 4 - Consultar informacio de l'arbre\n");
    printf(" 5 - Sortir\n\n");
    printf("   Escull opcio: ");

    fgets(str, 5, stdin);
    opcio = atoi(str); 

    return opcio;
}

/**
 * 
 *  Main procedure
 *
 */




int main(int argc, char **argv)
{
    char str1[MAXCHAR], str2[MAXCHAR];
    int opcio;

    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");
    
    rb_tree* tree;    
    tree = (rb_tree *) malloc(sizeof(rb_tree));
    init_tree(tree);
    FILE *fp;
    
    do {
        opcio = menu();
        printf("\n\n");

       /* Feu servir aquest codi com a pantilla */

        switch (opcio) {
            case 1: //DONE
                printf("Fitxer de diccionari de paraules: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;

                printf("Fitxer de base de dades: ");
                fgets(str2, MAXCHAR, stdin);
                str2[strlen(str2)-1]=0;
                
                if (tree->root != NIL){
                    delete_tree(tree);
                    free(tree);
                    tree = (rb_tree *) malloc(sizeof(rb_tree));    
                    init_tree(tree);
                }
                
                process_dictionary(tree, str1);
                process_data_base(str2, tree);
                
                break;

            case 2:
                printf("Nom de fitxer en que es desara l'arbre: ");
                fgets(str1, MAXCHAR, stdin);

                /* Falta codi */
                if (tree->root == NIL){
                    printf("No existeix cap arbre");
                } else{
                    fp = fopen(str1, "w");
                    save_tree(tree,fp);
                    fclose(fp);
                }
                break;

            case 3:
                printf("Nom del fitxer que conté l'arbre: ");
                fgets(str1, MAXCHAR, stdin);
                if (tree->root != NIL){
                    delete_tree(tree);
                    free(tree);
                    tree = (rb_tree *) malloc(sizeof(rb_tree));    
                    init_tree(tree);
                }
                fp = fopen(str1, "r");
                if (!fp) {
                    printf("Could not open file\n");
                }else{
                    load_tree(tree,fp);
                    fclose(fp);
                }
                
                break;

            case 4://DONE.
                printf("Paraula a buscar o prem enter per saber la paraula que apareix mes vegades: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;
                if(tree->root != NIL){
                    if(str1[0] == 0){
                        node* max = find_most_occurrences(tree);
                        
                        printf ("The word with most appearances is %s with %d appearances.", max->data->key, max->data->num_times ); 
                    } else{
                        node_data * word_info = find_node(tree,str1);
                        printf("The word %s appears %d times.", word_info->key,word_info->num_times);
                    }
                } else{
                    printf("S'ha de crear un arbre abans de fer comprovacions");
                }
                
                break;

            case 5: //DONE

                /* Delete the tree */
                delete_tree(tree);
                free(tree);
                break;

            default:
                printf("Opcio no valida\n");

        } /* switch */
    }
    while (opcio != 5);

    return 0;
}

