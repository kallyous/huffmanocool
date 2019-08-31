#include <stdio.h>
#include <stdlib.h>
#include "plot_structs.h"

int comp;
int main() {
    ABB * binary_tree = NULL;
    AVL * balanced_tree = NULL;
    FILE *comparision;
    comparision = fopen("Plot.txt","w+");
    int number;
    printf("Adicionando numeros nas arvores...\n")
    for (long int i = 0; i < 30000; ++i) {
        number = rand();
        binary_tree = add(binary_tree,number);
        balanced_tree = add_balanced(balanced_tree,number);
    }
    fprintf(comparision,"VALUE\tABB\tAVL\n");
    printf("Buscando numeros...\n")
    for (long int j = 0; j < 30000; ++j) {
        number = rand();
        search(binary_tree,number);
        fprintf(comparision,"%d\t%d\t",number,comp);
        comp = 0;
        search_balanced(balanced_tree,number);
        fprintf(comparision,"%d\n",comp);
    }
    fclose(comparision);
    printf("Dados escritos com sucesso!");
    return 0;
}
