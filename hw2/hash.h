#include <stdint.h>
#include "list.h"

#define DEFAULT_NUMBER_OF_BUCKETS 5

typedef int (*Hashmap_compare)(char *a, char *b);
typedef uint32_t (*Hashmap_hash)(char *key);

typedef struct Hashmap {
    List *buckets[DEFAULT_NUMBER_OF_BUCKETS];
    Hashmap_compare compare;
    Hashmap_hash hash;
} Hashmap;

typedef struct HashmapNode {
    char *key;
    void *data;
    uint32_t hash;
} HashmapNode;

typedef int (*Hashmap_traverse_cb)(HashmapNode *node);

Hashmap *hashmapCreate(Hashmap_compare compare, Hashmap_hash);

void hashmapDestroy(Hashmap *map);

int hashmapSet(Hashmap *map, char *key, void *data);

void *hashmapGet(Hashmap *map, char *key);

void *hashmapDelete(Hashmap *map, char *key);

List *hashmapGetKeys(Hashmap *map);
