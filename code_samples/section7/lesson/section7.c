#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
    This file bundles THREE small hash-based utilities + tests:

      1) HashTable (string -> int) with separate chaining (linked lists)
         - Used to count word frequencies.

      2) IntSet (set of ints) with open addressing + linear probing
         - Used to find the first duplicate in an array.

      3) IntIntMap (int -> int) with open addressing + linear probing
         - Used to solve the classic "two_sum" problem.

    These are educational/demo implementations:
      - No resizing/growth (except the chained table naturally grows via nodes).
      - No deletion support (so no tombstones needed for probing tables).
      - Capacity choices in tests are sized to avoid filling the tables.
*/

/* ============================================================
 *  String -> int HashTable with chaining
 * ============================================================ */

/*
    Entry
    -----
    Node stored in a bucket's linked list for separate chaining.

    Each Entry owns:
      - key: heap-allocated duplicate of the key string (malloc'd in str_dup)
      - value: associated int
      - next: pointer to next Entry in the chain
*/
typedef struct Entry {
    char* key;
    int value;
    struct Entry* next;
} Entry;

/*
    TABLE_SIZE
    ----------
    Fixed number of buckets. Using a prime can help distribution for modulo hashing.
*/
#define TABLE_SIZE 101

/*
    HashTable
    ---------
    Array of bucket heads. Each bucket is a singly linked list of Entry nodes.
*/
typedef struct {
    Entry* buckets[TABLE_SIZE];
} HashTable;

/*
    hash_str
    --------
    Hash function for strings (FNV-1a-like).

    Steps:
      - Start with an offset basis (2166136261u).
      - For each byte in the string:
          * XOR the byte into the hash
          * multiply by a prime (16777619u)

    Return:
      - unsigned integer hash value (later reduced to a bucket index by modulo).
*/
static unsigned int hash_str(const char* s) {
    unsigned int h = 2166136261u;
    while (*s) {
        h ^= (unsigned char)*s++; // XOR in next byte, then advance pointer
        h *= 16777619u;           // multiply by prime
    }
    return h;
}

/*
    str_dup
    -------
    Portable replacement for strdup() (which is POSIX, not standard C).

    Allocates a new buffer large enough for the string (including '\0')
    and copies it.

    Returns:
      - Newly allocated C string. Caller owns it and must free() it.

    Behavior on OOM:
      - Prints error and exits.
*/
static char* str_dup(const char* s) {
    size_t len = strlen(s) + 1;        // include null terminator
    char* p = (char*)malloc(len);
    if (!p) {
        fprintf(stderr, "Out of memory in str_dup\n");
        exit(EXIT_FAILURE);
    }
    memcpy(p, s, len);
    return p;
}

/*
    ht_init
    -------
    Initializes all buckets to NULL (empty chains).
*/
void ht_init(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
}

/*
    ht_free
    -------
    Frees ALL entries (and their keys) in the hash table.

    Important:
      - This frees memory allocated by ht_put/str_dup.
      - After this, ht is empty and safe to re-use (buckets reset to NULL).
*/
void ht_free(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry* curr = ht->buckets[i];
        while (curr) {
            Entry* next = curr->next; // save next before freeing
            free(curr->key);          // free duplicated key string
            free(curr);               // free entry node
            curr = next;
        }
        ht->buckets[i] = NULL;
    }
}

/*
    ht_put
    ------
    Inserts or updates a (key -> value) mapping.

    If key exists:
      - Update its value in place and return.

    If key does NOT exist:
      - Allocate a new Entry
      - Duplicate the key string
      - Insert at the head of the bucket chain for O(1) insertion.
*/
void ht_put(HashTable* ht, const char* key, int value) {
    unsigned int h = hash_str(key);
    int idx = (int)(h % TABLE_SIZE);

    // Search for existing key in chain
    Entry* curr = ht->buckets[idx];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            curr->value = value; // update
            return;
        }
        curr = curr->next;
    }

    // Not found: insert new entry at head
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

/*
    ht_get
    ------
    Looks up a key and returns a pointer to its stored value.

    Why return int*?
      - Allows callers to modify the value in place (e.g., increment a counter).

    Returns:
      - &entry->value if found
      - NULL if not found

    Warning:
      - Returned pointer becomes invalid if the table is freed.
*/
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

/*
    count_words
    -----------
    Uses the hash table as a frequency map for an array of word strings.

    For each word:
      - If already present: increment stored count.
      - Otherwise: insert with count=1.
*/
void count_words(HashTable* ht, const char* words[], int n) {
    for (int i = 0; i < n; i++) {
        const char* w = words[i];
        int* existing = ht_get(ht, w);

        if (existing) {
            (*existing)++;          // in-place increment
        } else {
            ht_put(ht, w, 1);       // first occurrence
        }
    }
}

/* ============================================================
 *  IntSet (open addressing) for first_duplicate
 * ============================================================ */

/*
    IntSet
    ------
    Integer set implemented with open addressing and linear probing.

    Storage:
      - table[] holds inserted integers
      - used[] indicates whether a slot is occupied
          0 = empty
          1 = occupied

    No deletion:
      - Because there is no remove operation, encountering used==0 during
        lookup means the key is not present (safe early-exit).
*/
typedef struct {
    int* table;
    unsigned char* used;  // 0 = empty, 1 = occupied
    int capacity;
} IntSet;

/*
    int_hash
    --------
    Hashes an integer into [0, capacity).

    Current approach:
      - make x non-negative (x < 0 => x = -x)
      - return x % capacity

    Caveat:
      - If x == INT_MIN, negation overflows (undefined behavior in C).
        A safer alternative is to cast to unsigned and hash that.
*/
static int int_hash(int x, int capacity) {
    // simple hash: ensure non-negative then mod
    if (x < 0) x = -x;
    return x % capacity;
}

/*
    set_init
    --------
    Allocates the backing arrays for the set and clears used[] to 0.

    capacity:
      - forced to at least 1 to avoid division/mod by zero.
*/
void set_init(IntSet* s, int capacity) {
    s->capacity = (capacity > 0) ? capacity : 1;

    s->table = (int*)malloc(s->capacity * sizeof(int));
    s->used  = (unsigned char*)malloc(s->capacity * sizeof(unsigned char));

    if (!s->table || !s->used) {
        fprintf(stderr, "Out of memory in set_init\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < s->capacity; i++) {
        s->used[i] = 0; // mark all slots empty
    }
}

/*
    set_free
    --------
    Frees the set's allocated arrays and clears fields.
*/
void set_free(IntSet* s) {
    free(s->table);
    free(s->used);
    s->table = NULL;
    s->used = NULL;
    s->capacity = 0;
}

/*
    set_contains
    ------------
    Returns true if x is present in the set.

    Probing:
      - Start at hash(x)
      - Probe sequentially (linear probing) until:
          a) find x -> return true
          b) hit an empty slot -> return false (since no deletions)
          c) probe full table -> return false
*/
bool set_contains(IntSet* s, int x) {
    int idx = int_hash(x, s->capacity);

    for (int i = 0; i < s->capacity; i++) {
        int probe = (idx + i) % s->capacity;

        if (!s->used[probe]) {
            // Empty slot means x was never inserted along this probe path
            return false;
        }
        if (s->table[probe] == x) {
            return true;
        }
    }

    // If the table is completely full and we didn't find x:
    return false;
}

/*
    set_insert
    ----------
    Inserts x if not already present.

    Returns:
      - true if newly inserted
      - false if already present OR table is full (no resize here)

    Notes:
      - In a production hash set, you'd typically resize when load factor is high.
*/
bool set_insert(IntSet* s, int x) {
    int idx = int_hash(x, s->capacity);

    for (int i = 0; i < s->capacity; i++) {
        int probe = (idx + i) % s->capacity;

        if (!s->used[probe]) {
            // Found an empty slot: insert here
            s->used[probe] = 1;
            s->table[probe] = x;
            return true;
        }
        if (s->table[probe] == x) {
            // Duplicate key: no insertion
            return false;
        }
    }

    // Table is full; for these small tests we won't resize.
    return false;
}

/*
    first_duplicate
    ---------------
    Scans array left-to-right and returns the first value whose second
    occurrence is encountered earliest.

    Parameters:
      - arr: input array
      - n: length
      - s: IntSet used to track "seen" values (must be initialized)

    Returns:
      - duplicate value if found
      - 0 sentinel if no duplicate

    Sentinel caveat:
      - 0 is ambiguous if 0 can appear in the array. A more robust API might
        return a bool and write the duplicate through an out parameter.
*/
int first_duplicate(const int* arr, int n, IntSet* s) {
    for (int i = 0; i < n; i++) {
        int x = arr[i];

        // If x is already in the set, we found the first duplicate
        if (set_contains(s, x)) {
            return x;
        }

        // Otherwise remember x
        set_insert(s, x);
    }

    return 0; // sentinel indicating no duplicate
}

/* ============================================================
 *  Int -> Int map (open addressing) for two_sum
 * ============================================================ */

/*
    IntIntMap
    ---------
    Hash map from int -> int using open addressing + linear probing.

    Here it’s used to map:
      value -> index

    This supports the two_sum algorithm in O(n) average time.
*/
typedef struct {
    int* keys;
    int* values;
    unsigned char* used; // 0 = empty, 1 = occupied
    int capacity;
} IntIntMap;

/*
    map_init
    --------
    Allocates internal arrays and marks all slots as empty.
*/
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

/*
    map_free
    --------
    Releases allocated memory and clears fields.
*/
void map_free(IntIntMap* m) {
    free(m->keys);
    free(m->values);
    free(m->used);
    m->keys = NULL;
    m->values = NULL;
    m->used = NULL;
    m->capacity = 0;
}

/*
    map_get
    -------
    Looks up key and stores its mapped value in out_val if found.

    Returns:
      - true if found
      - false otherwise

    Early-exit:
      - If we find an unused slot, the key is not in the map (no deletions).
*/
bool map_get(IntIntMap* m, int key, int* out_val) {
    int idx = int_hash(key, m->capacity);

    for (int i = 0; i < m->capacity; i++) {
        int probe = (idx + i) % m->capacity;

        if (!m->used[probe]) {
            return false; // empty slot => not found
        }
        if (m->keys[probe] == key) {
            *out_val = m->values[probe];
            return true;
        }
    }

    return false;
}

/*
    map_put
    -------
    Inserts or updates key -> value.

    Returns:
      - true if inserted/updated
      - false if the table is full (no resize in this demo)
*/
bool map_put(IntIntMap* m, int key, int value) {
    int idx = int_hash(key, m->capacity);

    for (int i = 0; i < m->capacity; i++) {
        int probe = (idx + i) % m->capacity;

        if (!m->used[probe]) {
            // Insert new mapping
            m->used[probe] = 1;
            m->keys[probe] = key;
            m->values[probe] = value;
            return true;
        }
        if (m->keys[probe] == key) {
            // Update existing mapping
            m->values[probe] = value;
            return true;
        }
    }

    // Table full; no resize for this demo
    return false;
}

/*
    two_sum
    -------
    Finds indices i and j such that arr[i] + arr[j] == target.

    Parameters:
      - arr, n: input array and length
      - target: desired sum
      - out_i, out_j: outputs for found indices
      - map: IntIntMap for value->index lookups (must be initialized)

    Algorithm:
      - For each element x at index i:
          need = target - x
          if need is already in map:
              return the stored index for need, and i
          else:
              store x -> i in map

    Returns:
      - 1 if a pair is found
      - 0 otherwise
*/
int two_sum(const int* arr, int n, int target, int* out_i, int* out_j,
            IntIntMap* map) {
    // map: value -> index
    for (int i = 0; i < n; i++) {
        int x = arr[i];
        int need = target - x;

        int idx;
        if (map_get(map, need, &idx)) { // complement found
            *out_i = idx;
            *out_j = i;
            return 1;
        }

        // Store current value with its index for future matches
        map_put(map, x, i);
    }

    return 0;
}

/* ============================================================
 *  Tests
 * ============================================================ */

/*
    test_hash_chaining_and_word_count
    --------------------------------
    Verifies:
      - Counting occurrences of repeated words.
      - Looking up present and missing keys.
      - Proper cleanup via ht_free.
*/
static void test_hash_chaining_and_word_count(void) {
    printf("=== Test: HashTable chaining & word frequency ===\n");

    HashTable ht;
    ht_init(&ht);

    // Input words with repeats
    const char* words[] = {
        "apple", "banana", "apple", "orange", "banana", "apple"
    };
    int n = (int)(sizeof(words) / sizeof(words[0]));

    // Count frequencies
    count_words(&ht, words, n);

    // Validate counts by querying known keys
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

/*
    test_first_duplicate
    --------------------
    Verifies:
      - Finding the first duplicate in an array with repeats.
      - Returning sentinel 0 when there are no duplicates.
*/
static void test_first_duplicate(void) {
    printf("=== Test: first_duplicate ===\n");

    // Case 1: duplicates exist
    int arr1[] = {2, 5, 1, 2, 3, 5, 1};
    int n1 = (int)(sizeof(arr1) / sizeof(arr1[0]));

    IntSet s1;
    set_init(&s1, n1 * 2); // capacity chosen to reduce collisions in demo

    int dup1 = first_duplicate(arr1, n1, &s1);

    printf("Array 1: ");
    for (int i = 0; i < n1; i++) printf("%d ", arr1[i]);
    printf("\nFirst duplicate: %d (expected 2)\n\n", dup1);

    set_free(&s1);

    // Case 2: no duplicates
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

/*
    test_two_sum
    ------------
    Verifies:
      - Finding a valid pair in the positive case.
      - Correctly reporting no pair in the negative case.
*/
static void test_two_sum(void) {
    printf("=== Test: two_sum with IntIntMap ===\n");

    // Positive case
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

    // Negative case: no pair
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

/*
    main
    ----
    Runs all test suites.
*/
int main(void) {
    test_hash_chaining_and_word_count();
    test_first_duplicate();
    test_two_sum();
    return 0;
}
