#include "include/sort.h"
#include "include/listbuilder.h"


void bubble_sort(ListNode * arr[], int length)
{
    int i, j;
    ListNode * temp;
    for (i=0; i < length-1; i++) {
        for (j=0; j < length-i-1; j++) {
            if (arr[j]->count > arr[j+1]->count) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
