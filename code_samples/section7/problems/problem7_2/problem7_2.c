#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===============================
 * Utilities
 * =============================== */

/**
 * str_dup
 *
 * Purpose:
 *   Duplicates a NUL-terminated string by allocating a new buffer and copying
 *   the contents, including the terminator.
 *
 * Parameters:
 *   s - Input C string to duplicate (must be non-NULL).
 *
 * Returns:
 *   Newly allocated string owned by the caller (must be freed with free()).
 *
 * Error handling:
 *   Exits the program on allocation failure.
 *
 * Notes:
 *   - Equivalent to POSIX strdup(), but implemented here for portability and
 *     to allow custom error handling.
 */
static char *str_dup(const char *s) {
    // +1 for the NUL terminator
    size_t len = strlen(s) + 1;

    // Allocate buffer
    char *p = (char *)malloc(len);
    if (!p) {
        fprintf(stderr, "Out of memory in str_dup\n");
        exit(EXIT_FAILURE);
    }

    // Copy the string including '\0'
    memcpy(p, s, len);
    return p;
}

/**
 * hash_str
 *
 * Purpose:
 *   Computes an FNV-1a hash for a C string.
 *
 * Parameters:
 *   s - NUL-terminated string (must be non-NULL).
 *
 * Returns:
 *   32-bit unsigned hash value.
 *
 * Notes:
 *   - FNV-1a is fast and simple, commonly used for hashing strings.
 *   - Hash quality is usually acceptable for small demos; production hash
 *     table implementations may incorporate additional protections.
 */
static unsigned int hash_str(const char *s) {
    // Standard 32-bit FNV-1a offset basis
    unsigned int h = 2166136261u;

    // For each byte:
    //   - XOR into the hash
    //   - Multiply by FNV prime
    while (*s) {
        h ^= (unsigned char)*s++;
        h *= 16777619u;
    }
    return h;
}

/**
 * char_cmp
 *
 * Purpose:
 *   Comparator for qsort() to sort bytes/characters ascending.
 *
 * Parameters:
 *   a, b - pointers to elements being compared (each points to a char).
 *
 * Returns:
 *   - negative if *a < *b
 *   - positive if *a > *b
 *   - 0 if equal
 *
 * Notes:
 *   - We cast to unsigned char to avoid implementation-defined behavior
 *     when char is signed and values are >= 128.
 */
static int char_cmp(const void *a, const void *b) {
    unsigned char ca = *(const unsigned char *)a;
    unsigned char cb = *(const unsigned char *)b;
    if (ca < cb) return -1;
    if (ca > cb) return 1;
    return 0;
}

/* ===============================
 * Anagram grouping hash map
 * =============================== */

/**
 * AnagramGroup
 *
 * Represents one "bucketed group" of words that are anagrams of each other.
 *
 * Fields:
 *   key      - the canonical key for the group (sorted letters of a word)
 *   items    - dynamic array of pointers to original strings in this group
 *   size     - number of strings currently stored
 *   capacity - allocated size of items[]
 *   next     - linked list pointer for separate chaining in the hash map
 *
 * Ownership / memory model:
 *   - key is allocated (str_dup) and owned by the group.
 *   - items[] array is allocated and owned by the group, BUT the strings
 *     stored in items[] are NOT duplicated in this program. They point to
 *     original input strings (e.g., from the caller’s array).
 */
typedef struct AnagramGroup {
    char *key;             // sorted version of the anagram word
    char **items;          // original strings that match this key
    int size;
    int capacity;
    struct AnagramGroup *next;
} AnagramGroup;

/**
 * Hash map sizing:
 *   - 101 buckets is a small prime number, helpful for simple modulo hashing.
 *   - For larger inputs, you'd want more buckets (or resizing) to reduce collisions.
 */
#define ANAGRAM_BUCKETS 101

/**
 * AnagramMap
 *
 * Simple hash map implementation using:
 *   - fixed bucket array
 *   - separate chaining via singly linked lists (AnagramGroup.next)
 */
typedef struct {
    AnagramGroup *buckets[ANAGRAM_BUCKETS];
} AnagramMap;

/**
 * map_init
 *
 * Initializes all buckets to NULL.
 *
 * Important:
 *   Must be called before using the map.
 */
static void map_init(AnagramMap *m) {
    for (int i = 0; i < ANAGRAM_BUCKETS; i++) {
        m->buckets[i] = NULL;
    }
}

/**
 * group_add_item
 *
 * Adds a string pointer to the group's dynamic items array.
 *
 * Parameters:
 *   g - group to append to
 *   s - pointer to a string (not copied)
 *
 * Behavior:
 *   - Grows capacity geometrically (x2) to keep amortized O(1) append.
 *   - Stores the pointer s as-is; caller must ensure s remains valid
 *     until after map_free().
 */
static void group_add_item(AnagramGroup *g, char *s) {
    // Ensure enough capacity in items[]; grow if needed.
    if (g->size >= g->capacity) {
        int newCap = (g->capacity == 0) ? 4 : g->capacity * 2;

        // Reallocate items array to the new capacity
        char **nd = (char **)realloc(g->items, newCap * sizeof(char *));
        if (!nd) {
            fprintf(stderr, "Out of memory in group_add_item\n");
            exit(EXIT_FAILURE);
        }

        g->items = nd;
        g->capacity = newCap;
    }

    // Append the string pointer
    g->items[g->size++] = s;
}

/**
 * map_find_group
 *
 * Looks up an anagram group by key.
 *
 * Parameters:
 *   m   - map to search
 *   key - sorted string key to match
 *   h   - precomputed hash of key (to avoid hashing twice)
 *
 * Returns:
 *   Pointer to existing group if found, otherwise NULL.
 *
 * Notes:
 *   - Uses separate chaining: traverse the linked list in the selected bucket.
 */
static AnagramGroup *map_find_group(AnagramMap *m, const char *key, unsigned int h) {
    int idx = (int)(h % ANAGRAM_BUCKETS);
    AnagramGroup *curr = m->buckets[idx];

    while (curr) {
        if (strcmp(curr->key, key) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

/**
 * map_add_group
 *
 * Creates a new group for the given key and inserts it into the map.
 *
 * Parameters:
 *   m   - map to modify
 *   key - sorted string key (the function duplicates it)
 *   h   - precomputed hash (used to choose bucket)
 *
 * Returns:
 *   Pointer to the newly created group.
 *
 * Insertion strategy:
 *   - Insert at head of the bucket's linked list (O(1)).
 */
static AnagramGroup *map_add_group(AnagramMap *m, const char *key, unsigned int h) {
    int idx = (int)(h % ANAGRAM_BUCKETS);

    // Allocate group node
    AnagramGroup *g = (AnagramGroup *)malloc(sizeof(AnagramGroup));
    if (!g) {
        fprintf(stderr, "Out of memory in map_add_group\n");
        exit(EXIT_FAILURE);
    }

    // Duplicate key so the group owns its key string
    g->key = str_dup(key);

    // Start with empty dynamic array of items
    g->items = NULL;
    g->size = 0;
    g->capacity = 0;

    // Insert at bucket head (separate chaining)
    g->next = m->buckets[idx];
    m->buckets[idx] = g;

    return g;
}

/**
 * map_free
 *
 * Frees all memory owned by the map:
 *   - Each group node
 *   - Each group's key string
 *   - Each group's items array
 *
 * Important:
 *   - This does NOT free the strings in items[], because those are pointers
 *     to caller-owned memory (e.g., the input words array).
 *   - If you changed the program to duplicate input strings, you would also
 *     need to free each items[j] here.
 */
static void map_free(AnagramMap *m) {
    for (int i = 0; i < ANAGRAM_BUCKETS; i++) {
        AnagramGroup *curr = m->buckets[i];

        while (curr) {
            AnagramGroup *next = curr->next;

            free(curr->key);
            free(curr->items);  // we did not allocate copies of the original strings here
            free(curr);

            curr = next;
        }

        m->buckets[i] = NULL;
    }
}

/* ===============================
 * groupAnagrams implementation
 * =============================== */

/**
 * groupAnagrams
 *
 * Purpose:
 *   Groups strings that are anagrams of each other.
 *
 * Definition of anagram group key:
 *   - Sort the letters of each word; anagrams share the same sorted form.
 *     Example: "eat", "tea", "ate" -> key "aet"
 *
 * Parameters:
 *   strs     - array of C-string pointers
 *   strsSize - number of strings in strs
 *   map      - output map to be filled with groups (initialized inside)
 *
 * Side effects:
 *   - Initializes and populates the provided AnagramMap.
 *
 * Complexity:
 *   Let n = strsSize, and let L be average string length:
 *     - For each string, we sort its characters: O(L log L)
 *     - Total time is approximately O(n * L log L) plus hashmap overhead.
 *     - Space is O(number_of_groups + total_items) for the stored pointers and keys.
 *
 * Assumptions / limitations:
 *   - Strings are treated as byte arrays; sorting is byte-wise.
 *   - Caller must ensure that input strings remain valid until after map_free().
 */
void groupAnagrams(char **strs, int strsSize, AnagramMap *map) {
    // Always start from a clean map state
    map_init(map);

    // Process each string and place it into its anagram group
    for (int i = 0; i < strsSize; i++) {
        char *s = strs[i];
        int len = (int)strlen(s);

        // -----------------------------
        // Build the canonical key string
        // -----------------------------
        // Allocate a temporary copy of the string so we can sort it in-place.
        // (We must not sort the original input string unless that's allowed.)
        char *key = (char *)malloc((size_t)len + 1);
        if (!key) {
            fprintf(stderr, "Out of memory in groupAnagrams\n");
            exit(EXIT_FAILURE);
        }

        // Copy s into key (including NUL terminator)
        strcpy(key, s);

        // Sort the characters to produce the anagram key
        qsort(key, (size_t)len, sizeof(char), char_cmp);

        // Hash the sorted key to pick a bucket
        unsigned int h = hash_str(key);

        // Look for an existing group with this key
        AnagramGroup *g = map_find_group(map, key, h);
        if (!g) {
            // If no group exists, create it (map_add_group duplicates key)
            g = map_add_group(map, key, h);
        }

        // Temporary key is no longer needed because group has its own copy
        free(key);

        // -----------------------------
        // Add original string to group
        // -----------------------------
        // Store the pointer to the original string; no string duplication here.
        group_add_item(g, s);
    }
}

/* ===============================
 * Testing
 * =============================== */

/**
 * print_groups
 *
 * Prints all anagram groups currently stored in the map.
 *
 * Output format:
 *   Key "aet": [eat, tea, ate]
 *   Key "abt": [bat]
 *   ...
 *
 * Notes:
 *   - Map bucket order is arbitrary, and so is the order of groups within buckets.
 *   - The order of items in each group reflects the input order.
 */
static void print_groups(const AnagramMap *map) {
    printf("Anagram groups:\n");

    // Walk each bucket and print every chained group
    for (int i = 0; i < ANAGRAM_BUCKETS; i++) {
        const AnagramGroup *g = map->buckets[i];

        while (g) {
            printf("  Key \"%s\": [", g->key);

            // Print group's items array
            for (int j = 0; j < g->size; j++) {
                printf("%s", g->items[j]);
                if (j + 1 < g->size) printf(", ");
            }

            printf("]\n");
            g = g->next;
        }
    }
}

/**
 * main
 *
 * Builds a small set of words, groups them into anagrams, prints the groups,
 * and then frees the map.
 */
int main(void) {
    // Input words. These are string literals (static storage duration),
    // so their pointers remain valid for the lifetime of the program.
    char *words[] = { "eat", "tea", "tan", "ate", "nat", "bat" };
    int n = (int)(sizeof(words) / sizeof(words[0]));

    // Output map for groups
    AnagramMap map;
    groupAnagrams(words, n, &map);

    printf("=== Test: groupAnagrams ===\n");

    // Print the input list
    printf("Input words: ");
    for (int i = 0; i < n; i++) {
        printf("%s", words[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("\n\n");

    // Print grouped results
    print_groups(&map);

    printf("\n(Each group should contain anagrams, e.g. [eat, tea, ate], [tan, nat], [bat])\n");

    // Free memory owned by the map
    map_free(&map);
    return 0;
}
