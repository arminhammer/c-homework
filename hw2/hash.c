#include <stdint.h>
#include <string.h>

#include "hash.h"

static int defaultCompare(char *a, char *b)
{
    return strcmp(a, b);
}

/** 
 * Simple Bob Jenkins's hash algorithm taken from the
 * wikipedia description.
 */
static uint32_t defaultHash(char *a)
{
    size_t len = strlen(a);
    char *key = a;
    uint32_t hash = 0;
    uint32_t i = 0;

    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

Hashmap *hashmapCreate(Hashmap_compare compare, Hashmap_hash hash)
{
    Hashmap *map = calloc(1, sizeof(Hashmap));

    map->compare = compare == NULL ? defaultCompare : compare;
    map->hash = hash == NULL ? defaultHash : hash;

    int i;
    for(i = 0; i < DEFAULT_NUMBER_OF_BUCKETS; i++) {
      map->buckets[i] = listCreate();
    }

    return map;
}


void hashmapDestroy(Hashmap *map)
{

    if(map) {
        if(map->buckets) {
          int i;
          for(i = 0; i < DEFAULT_NUMBER_OF_BUCKETS; i++) {
                List *bucket = map->buckets[i];
                if(bucket) {
                    listDestroy(bucket);
                }
            }
        }
        free(map);
    }
}

static inline HashmapNode *hashmapNodeCreate(int hash, void *key, void *data)
{
    HashmapNode *node = calloc(1, sizeof(HashmapNode));

    node->key = key;
    node->data = data;
    node->hash = hash;

    return node;

}


static inline List *hashmapFindBucket(Hashmap *map, char *key, uint32_t *hash_out)
{
    uint32_t hash = map->hash(key);
    int bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS;
    if(!(bucket_n >= 0)) {
      printf("Invalid bucket found: %d\n", bucket_n);
    }

    *hash_out = hash; // store it for the return so the caller can use it

    List *bucket = map->buckets[bucket_n];


    return bucket;

}


int hashmapSet(Hashmap *map, char *key, void *data)
{
    uint32_t hash = 0;
    List *bucket = hashmapFindBucket(map, key, &hash);
    if(!bucket) {
      printf("Error, can't find bucket.\n");
    }

    HashmapNode *newNode = hashmapNodeCreate(hash, key, data);
    listPush(bucket, newNode);

    return 0;

}

void *hashmapGet(Hashmap *map, char *key)
{
    uint32_t hash = 0;
    List *bucket = hashmapFindBucket(map, key, &hash);
    
    if(!bucket) {
      printf("Could not find bucket with key %s\n", key);
    }

    ListNode *current = bucket->first;
    HashmapNode *currentHMNode = current->value;
    if(strcmp(key, currentHMNode->key) == 0) {
        return currentHMNode->data;  
    }
    while(current->next != NULL) {
      current = current->next;
      HashmapNode *currentHMNode = current->value;
      if(strcmp(key, currentHMNode->key) == 0) {
        return currentHMNode->data;  
      }
    }

    return NULL;

}

List *hashmapGetKeys(Hashmap *map) {
  List *keyList = listCreate();

  int i;
    for(i = 0; i < DEFAULT_NUMBER_OF_BUCKETS; i++) {
      List *current = map->buckets[i];
      if(current->first == NULL) {
      } else {
        ListNode *currentNode = current->first;
        HashmapNode *currentHMNode = currentNode->value;
        listPush(keyList, currentHMNode->key);
        while(currentNode->next != NULL) {
          currentNode = currentNode->next;
          HashmapNode *currentHMNode = currentNode->value;
          listPush(keyList, currentHMNode->key);
        }
      }
    }

    return keyList;

}

/*
int main() {
  Hashmap *map = NULL;
  map = hashmapCreate(NULL, NULL);

  char *key1 = "one";
  char *key2 = "two";
  char *key3 = "three";
  char *key4 = "four";
  char *key5 = "five";
  char *key6 = "six";

  char *value1 = "Value 1";
  char *value2 = "Value 2";
  char *value3 = "Value 3";
  char *value4 = "Value 4";
  char *value5 = "Value 5";
  char *value6 = "Value 6";

  printf("Setting key: %s, value: %s\n", key1, value1);
  hashmapSet(map, key1, value1);
  printf("Setting key: %s, value: %s\n", key2, value2);
  hashmapSet(map, key2, value2);
  printf("Setting key: %s, value: %s\n", key3, value3);
  hashmapSet(map, key3, value3);
  printf("Setting key: %s, value: %s\n", key4, value4);
  hashmapSet(map, key4, value4);
  printf("Setting key: %s, value: %s\n", key2, value2);
  hashmapSet(map, key5, value5);
  printf("Setting key: %s, value: %s\n", key6, value6);
  hashmapSet(map, key6, value6);


  List *keyList1 = hashmapGetKeys(map);
  free(keyList1);

  char *result1 = (char*) hashmapGet(map, key1);
  printf("Printing results for %s : %s\n", key1, result1);
  char *result2 = (char*) hashmapGet(map, key2);
  printf("Printing results for %s : %s\n", key2, result2);
  char *result3 = (char*) hashmapGet(map, key3);
  printf("Printing results for %s : %s\n", key3, result3);
  char *result4 = (char*) hashmapGet(map, key4);
  printf("Printing results for %s : %s\n", key4, result4);
  char *result5 = (char*) hashmapGet(map, key5);
  printf("Printing results for %s : %s\n", key5, result5);
  char *result6 = (char*) hashmapGet(map, key6);
  printf("Printing results for %s : %s\n", key6, result6);
  

  List *keyList2 = hashmapGetKeys(map);
  free(keyList2);

  hashmapDestroy(map);

  return 0;
}
*/
