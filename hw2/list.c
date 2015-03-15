#include "list.h"

List *listCreate()
{
    return calloc(1, sizeof(List));
}

void listDestroy(List *list)
{
    if(list->count > 0) {
        ListNode *current = list->first;

        if(current == NULL) {
            printf("current is NULL");
        }
        if(current->next == NULL) {
            free(current->value);
            free(current);
        } else {
            while(current->next != NULL) {
                ListNode *nextNode = current->next;

                free(current->value);
                free(current);
                current = nextNode;
            }
            free(current->value);
            free(current);
        }
    }

    free(list);
}

void listPush(List *list, void *value)
{
    ListNode *node = calloc(1, sizeof(ListNode));

    node->value = value;

    if(list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }

    list->count++;

}

char *listShift(List *list)
{
    ListNode *node = list->first;
    return node != NULL ? listRemove(list, node) : NULL;
}


char *listRemove(List *list, ListNode *node)
{
    void *result = NULL;

    if (!(list->first && list->last)) {
        printf("List is empty.");
    }
    if(!node) {
        printf("node can't be NULL");
    }
    if(node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if(node == list->first) {
        list->first = node->next;
        if(list->first == NULL) {
            printf("Invalid list, somehow got a first that is NULL.");
        }
    } else if (node == list->last) {
        if(list->last == NULL) {
            printf("Invalid list, somehow got a next that is NULL.");
        }
        list->last->next = NULL;
    } 
    list->count--;
    result = node->value;
    free(node);

    return result;
}

/*
int main()
{
    List *list = NULL;
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";
    
    list = listCreate();

    listPush(list, test1);
    printf("Pushing val: %s\n", test1);
    
    listPush(list, test2);
    printf("Pushing val: %s\n", test2);
    
    listPush(list, test3);
    printf("Pushing val: %s\n", test3);
    //listDestroy(list);
    
    
    int i;
    int listCount = list->count;
    for(i = 0; i < listCount; i++) {
        char *val = listShift(list);
        if(val == NULL) {
            printf("val is null");
        }
        else {
            printf("Shift val: %s\n", val);
        }
    }
    
    listDestroy(list);
    

    return 0;
}
*/
