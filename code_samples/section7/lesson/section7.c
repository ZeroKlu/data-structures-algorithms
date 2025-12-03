#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ============================================================
 *  String -> int HashTable with chaining
 * ============================================================ */

typedef struct Entry {
    char* key;
    int value;
    struct Entry* next;
} Entry;

#define TABLE_SIZE 101

typedef struct {
    Entry* buckets[TABLE_SIZE];
} HashTable;

static unsigned int hash_str(const char* s) {
    unsigned int h = 2166136261u;
    while (*s) {
        h ^= (unsigned char)*s++;
        h *= 16777619u;
    }
    return h;
}

static char* str_dup(const char* s) {
    size_t len = strlen(s) + 1;
    char* p = (char*)malloc(len);
    if (!p) {
        fprintf(stderr, "Out of memory in str_dup\n");
        exit(EXIT_FAILURE);
    }
    memcpy(p, s, len);
    return p;
}

void ht_init(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
}

void ht_free(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry* curr = ht->buckets[i];
        while (curr) {
            Entry* next = curr->next;
            free(curr->key);
            free(curr);
            curr = next;
        }
        ht->buckets[i] = NULL;
    }
}

void ht_put(HashTable* ht, const char* key, int value) {
    unsigned int h = hash_str(key);
    int idx = (int)(h % TABLE_SIZE);
    Entry* curr = ht->buckets[idx];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            curr->value = value; // update
            return;
        }
        curr = curr->next;
    }
    // not found, insert new at head
    Entry* e = (Entry*)malloc(sizeof(Entry));
    if (!e) {
        fprintf(stderr, "Out of memory in ht_put\n");
        exit(EXIT_FAILURE);
    }
    e->key = str_dup(key);
    e->value = value;
    e->next = ht->buckets[idx];
    ht->buckets[idx] = e;
}

int* ht_get(HashTable* ht, const char* key) {
    unsigned int h = hash_str(key);
    int idx = (int)(h % TABLE_SIZE);
    Entry* curr = ht->buckets[idx];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return &curr->value;
        }
        curr = curr->next;
    }
    return NULL; // not found
}

void count_words(HashTable* ht, const char* words[], int n) {
    for (int i = 0; i < n; i++) {
        const char* w = words[i];
        int* existing = ht_get(ht, w);
        if (existing) {
            (*existing)++;
        } else {
            ht_put(ht, w, 1);
        }
    }
}

/* ============================================================
 *  IntSet (open addressing) for first_duplicate
 * ============================================================ */

typedef struct {
    int* table;
    unsigned char* used;  // 0 = empty, 1 = occupied
    int capacity;
} IntSet;

static int int_hash(int x, int capacity) {
    // simple hash: ensure non-negative then mod
    if (x < 0) x = -x;
    return x % capacity;
}

void set_init(IntSet* s, int capacity) {
    s->capacity = (capacity > 0) ? capacity : 1;
    s->table = (int*)malloc(s->capacity * sizeof(int));
    s->used  = (unsigned char*)malloc(s->capacity * sizeof(unsigned char));
    if (!s->table || !s->used) {
        fprintf(stderr, "Out of memory in set_init\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < s->capacity; i++) {
        s->used[i] = 0;
    }
}

void set_free(IntSet* s) {
    free(s->table);
    free(s->used);
    s->table = NULL;
    s->used = NULL;
    s->capacity = 0;
}

bool set_contains(IntSet* s, int x) {
    int idx = int_hash(x, s->capacity);
    for (int i = 0; i < s->capacity; i++) {
        int probe = (idx + i) % s->capacity;
        if (!s->used[probe]) {
            return false; // empty slot -> x not present
        }
        if (s->table[probe] == x) {
            return true;
        }
    }
    return false;
}

bool set_insert(IntSet* s, int x) {
    int idx = int_hash(x, s->capacity);
    for (int i = 0; i < s->capacity; i++) {
        int probe = (idx + i) % s->capacity;
        if (!s->used[probe]) {
            s->used[probe] = 1;
            s->table[probe] = x;
            return true; // inserted new
        }
        if (s->table[probe] == x) {
            return false; // already present
        }
    }
    // Table is full; for these small tests we won't resize.
    return false;
}

int first_duplicate(const int* arr, int n, IntSet* s) {
    for (int i = 0; i < n; i++) {
        int x = arr[i];
        if (set_contains(s, x)) {
            return x;
        }
        set_insert(s, x);
    }
    return 0; // sentinel indicating no duplicate
}

/* ============================================================
 *  Int -> Int map (open addressing) for two_sum
 * ============================================================ */

typedef struct {
    int* keys;
    int* values;
    unsigned char* used; // 0 = empty, 1 = occupied
    int capacity;
} IntIntMap;

void map_init(IntIntMap* m, int capacity) {
    m->capacity = (capacity > 0) ? capacity : 1;
    m->keys = (int*)malloc(m->capacity * sizeof(int));
    m->values = (int*)malloc(m->capacity * sizeof(int));
    m->used = (unsigned char*)malloc(m->capacity * sizeof(unsigned char));
    if (!m->keys || !m->values || !m->used) {
        fprintf(stderr, "Out of memory in map_init\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < m->capacity; i++) {
        m->used[i] = 0;
    }
}

void map_free(IntIntMap* m) {
    free(m->keys);
    free(m->values);
    free(m->used);
    m->keys = NULL;
    m->values = NULL;
    m->used = NULL;
    m->capacity = 0;
}

bool map_get(IntIntMap* m, int key, int* out_val) {
    int idx = int_hash(key, m->capacity);
    for (int i = 0; i < m->capacity; i++) {
        int probe = (idx + i) % m->capacity;
        if (!m->used[probe]) {
            return false; // empty slot: not found
        }
        if (m->keys[probe] == key) {
            *out_val = m->values[probe];
            return true;
        }
    }
    return false;
}

bool map_put(IntIntMap* m, int key, int value) {
    int idx = int_hash(key, m->capacity);
    for (int i = 0; i < m->capacity; i++) {
        int probe = (idx + i) % m->capacity;
        if (!m->used[probe]) {
            m->used[probe] = 1;
            m->keys[probe] = key;
            m->values[probe] = value;
            return true; // inserted
        }
        if (m->keys[probe] == key) {
            m->values[probe] = value; // update
            return true;
        }
    }
    return false; // table full; no resize for this demo
}

int two_sum(const int* arr, int n, int target, int* out_i, int* out_j,
            IntIntMap* map) {
    // map: value -> index
    for (int i = 0; i < n; i++) {
        int x = arr[i];
        int need = target - x;
        int idx;
        if (map_get(map, need, &idx)) { // need exists
            *out_i = idx;
            *out_j = i;
            return 1;
        }
        map_put(map, x, i);
    }
    return 0;
}

/* ============================================================
 *  Tests
 * ============================================================ */

static void test_hash_chaining_and_word_count(void) {
    printf("=== Test: HashTable chaining & word frequency ===\n");
    HashTable ht;
    ht_init(&ht);

    const char* words[] = {
        "apple", "banana", "apple", "orange", "banana", "apple"
    };
    int n = (int)(sizeof(words) / sizeof(words[0]));

    count_words(&ht, words, n);

    const char* keys_to_check[] = {"apple", "banana", "orange", "pear"};
    int m = (int)(sizeof(keys_to_check) / sizeof(keys_to_check[0]));

    for (int i = 0; i < m; i++) {
        const char* w = keys_to_check[i];
        int* val = ht_get(&ht, w);
        if (val) {
            printf("'%s' -> %d\n", w, *val);
        } else {
            printf("'%s' -> (not found)\n", w);
        }
    }

    printf("Expected counts: apple=3, banana=2, orange=1, pear not found\n\n");

    ht_free(&ht);
}

static void test_first_duplicate(void) {
    printf("=== Test: first_duplicate ===\n");

    int arr1[] = {2, 5, 1, 2, 3, 5, 1};
    int n1 = (int)(sizeof(arr1) / sizeof(arr1[0]));
    IntSet s1;
    set_init(&s1, n1 * 2); // some capacity > n
    int dup1 = first_duplicate(arr1, n1, &s1);
    printf("Array 1: ");
    for (int i = 0; i < n1; i++) printf("%d ", arr1[i]);
    printf("\nFirst duplicate: %d (expected 2)\n\n", dup1);
    set_free(&s1);

    int arr2[] = {1, 2, 3, 4, 5};
    int n2 = (int)(sizeof(arr2) / sizeof(arr2[0]));
    IntSet s2;
    set_init(&s2, n2 * 2);
    int dup2 = first_duplicate(arr2, n2, &s2);
    printf("Array 2: ");
    for (int i = 0; i < n2; i++) printf("%d ", arr2[i]);
    printf("\nFirst duplicate: %d (expected 0 = no duplicate)\n\n", dup2);
    set_free(&s2);
}

static void test_two_sum(void) {
    printf("=== Test: two_sum with IntIntMap ===\n");

    int arr[] = {2, 7, 11, 15};
    int n = (int)(sizeof(arr) / sizeof(arr[0]));
    int target = 9;
    IntIntMap map;
    map_init(&map, n * 2);

    int i, j;
    int found = two_sum(arr, n, target, &i, &j, &map);

    printf("Array: ");
    for (int k = 0; k < n; k++) printf("%d ", arr[k]);
    printf("\nTarget: %d\n", target);
    if (found) {
        printf("two_sum found indices (%d, %d) -> values (%d, %d)\n",
               i, j, arr[i], arr[j]);
        printf("Expected: indices (0,1) or (1,0) with values 2 and 7\n\n");
    } else {
        printf("two_sum did not find any pair (unexpected for this test)\n\n");
    }

    map_free(&map);

    // Negative test: no pair
    printf("--- Negative case (no pair) ---\n");
    int arr2[] = {1, 2, 3};
    int n2 = (int)(sizeof(arr2) / sizeof(arr2[0]));
    int target2 = 100;
    map_init(&map, n2 * 2);
    found = two_sum(arr2, n2, target2, &i, &j, &map);
    printf("Array: ");
    for (int k = 0; k < n2; k++) printf("%d ", arr2[k]);
    printf("\nTarget: %d\n", target2);
    if (!found) {
        printf("two_sum correctly found no pair.\n\n");
    } else {
        printf("two_sum incorrectly found a pair.\n\n");
    }
    map_free(&map);
}

/* ============================================================
 *  main
 * ============================================================ */

int main(void) {
    test_hash_chaining_and_word_count();
    test_first_duplicate();
    test_two_sum();
    return 0;
}
