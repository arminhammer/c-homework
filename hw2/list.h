#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

struct ListNode;

typedef struct ListNode {
    struct ListNode *next;
    void *value;
} ListNode;

typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;

List *listCreate();
void listDestroy(List *list);
void listClear(List *list);
void listClearDestroy(List *list);

#define listCount(L) ((L)->count)
#define listFirst(L) ((L)->first != NULL ? (L)->first->value : NULL)
#define listLast(L) ((L)->last != NULL ? (L)->last->value : NULL)

void listPush(List *list, void *value);

char *listShift(List *list);

char *listRemove(List *list, ListNode *node);
