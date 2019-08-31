//
// Created by jpham on 24/08/2019.
//

#ifndef UNTITLED1_PLOT_STRUCTS_H
#define UNTITLED1_PLOT_STRUCTS_H
typedef struct ABB{

    long int item;

    struct ABB *left;

    struct ABB *right;

}ABB;

ABB* create_binary_tree(long int item, ABB *left,ABB *right)

{

    ABB *new_binary_tree = (ABB*)malloc(sizeof(ABB));

    new_binary_tree->item = item;

    new_binary_tree->left = right;

    new_binary_tree->right = left;

    return new_binary_tree;

}

extern int comp = 0;

typedef struct AVL{

    long int item;

    long int h;

    struct AVL *left;

    struct AVL *right;

}AVL;




AVL* create_balanced_tree(long int item, AVL *left,AVL *right)

{

    AVL *new_binary_tree = (AVL*)malloc(sizeof(AVL));

    new_binary_tree->item = item;

    new_binary_tree->left = right;

    new_binary_tree->h = 0;

    new_binary_tree->right = left;

    return new_binary_tree;

}



ABB* add(ABB *bt, long int item)

{

    if (bt == NULL)

    {

        bt = create_binary_tree(item,NULL,NULL);

    }

    else if(bt->item > item)

    {

        bt->left = add(bt->left,item);

    }

    else

    {

        bt->right = add(bt->right,item);

    }

    return bt;

}

void search(ABB *bt,long int item)

{

    if ((bt == NULL) || (bt->item == item))

    {
        comp++;
        return;

    }

    else if(bt->item > item)

    {
        comp++;
        search(bt->left,item);

    }

    else

    {
        comp++;
        search(bt->right,item);

    }

}

void search_balanced(AVL *bt,long int item)

{

    if ((bt == NULL) || (bt->item == item))

    {
        comp++;
        return;

    }

    else if(bt->item > item)

    {
        comp++;
        search_balanced(bt->left,item);

    }

    else

    {
        comp++;
        search_balanced(bt->right,item);

    }

}

long int max(long int a, long int b){

    return (a > b) ? a : b;

}

long int height(AVL *bt){

    if(bt == NULL){

        return -1;

    }

    else{

        return 1 + max(height(bt->left),height(bt->right));

    }

}




long int balance_factor(AVL *bt)

{

    if (bt == NULL) {

        return 0;

    }

    else if ((bt->left != NULL) && (bt->right != NULL)) {

        return (bt->left->h - bt->right->h);

    }

    else if ((bt->left != NULL) && (bt->right == NULL)) {

        return (1 + bt->left->h);

    }

    else {

        return (-bt->right->h - 1);

    }

}



AVL* rotate_left(AVL *bt)

{

    AVL *subtree_root = NULL;

    if (bt != NULL && bt->right != NULL) {

        subtree_root = bt->right;

        bt->right = subtree_root->left;

        subtree_root->left = bt;

    }

    subtree_root->h = height(subtree_root);

    bt->h = height(bt);

    return subtree_root;

}



AVL* rotate_right(AVL *bt)

{

    AVL *subtree_root = NULL;

    if (bt != NULL && bt->left != NULL) {

        subtree_root = bt->left;

        bt->left = subtree_root->right;

        subtree_root->right = bt;

    }

    subtree_root->h = height(subtree_root);

    bt->h = height(bt);

    return subtree_root;

}



AVL* add_balanced(AVL *bt, long int item)

{

    if (bt == NULL) {

        return create_balanced_tree(item, NULL, NULL);

    } else if (bt->item > item) {

        bt->left = add_balanced(bt->left, item);

    } else {

        bt->right = add_balanced(bt->right, item);

    }

    bt->h = height(bt);

    AVL *child;

    if (balance_factor(bt) == 2 || balance_factor(bt) == -2) {

        if (balance_factor(bt) == 2) {

            child = bt->left;

            if (balance_factor(child) == -1) {

                bt->left = rotate_left(child);

            }

            bt = rotate_right(bt);

        } else if (balance_factor(bt) == -2) {

            child = bt->right;

            if (balance_factor(child) == 1) {

                bt->right = rotate_right(child);

            }

            bt = rotate_left(bt);

        }

    }

    return bt;

}


#endif //UNTITLED1_PLOT_STRUCTS_H
