#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===============================
 * Utilities
 * =============================== */

static char *str_dup(const char *s) {
    size_t len = strlen(s) + 1;
    char *p = (char *)malloc(len);
    if (!p) {
        fprintf(stderr, "Out of memory in str_dup\n");
        exit(EXIT_FAILURE);
    }
    memcpy(p, s, len);
    return p;
}

/* FNV-1a string hash (same as earlier examples) */
static unsigned int hash_str(const char *s) {
    unsigned int h = 2166136261u;
    while (*s) {
        h ^= (unsigned char)*s++;
        h *= 16777619u;
    }
    return h;
}

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

typedef struct AnagramGroup {
    char *key;             // sorted version of the anagram word
    char **items;          // original strings that match this key
    int size;
    int capacity;
    struct AnagramGroup *next;
} AnagramGroup;

#define ANAGRAM_BUCKETS 101

typedef struct {
    AnagramGroup *buckets[ANAGRAM_BUCKETS];
} AnagramMap;

static void map_init(AnagramMap *m) {
    for (int i = 0; i < ANAGRAM_BUCKETS; i++) {
        m->buckets[i] = NULL;
    }
}

static void group_add_item(AnagramGroup *g, char *s) {
    if (g->size >= g->capacity) {
        int newCap = (g->capacity == 0) ? 4 : g->capacity * 2;
        char **nd = (char **)realloc(g->items, newCap * sizeof(char *));
        if (!nd) {
            fprintf(stderr, "Out of memory in group_add_item\n");
            exit(EXIT_FAILURE);
        }
        g->items = nd;
        g->capacity = newCap;
    }
    g->items[g->size++] = s;
}

static AnagramGroup *map_find_group(AnagramMap *m, const char *key, unsigned int h) {
    int idx = (int)(h % ANAGRAM_BUCKETS);
    AnagramGroup *curr = m->buckets[idx];
    while (curr) {
        if (strcmp(curr->key, key) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

static AnagramGroup *map_add_group(AnagramMap *m, const char *key, unsigned int h) {
    int idx = (int)(h % ANAGRAM_BUCKETS);
    AnagramGroup *g = (AnagramGroup *)malloc(sizeof(AnagramGroup));
    if (!g) {
        fprintf(stderr, "Out of memory in map_add_group\n");
        exit(EXIT_FAILURE);
    }
    g->key = str_dup(key);
    g->items = NULL;
    g->size = 0;
    g->capacity = 0;
    g->next = m->buckets[idx];
    m->buckets[idx] = g;
    return g;
}

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

// Fills the map with groups of anagrams based on strs[0..strsSize-1].
void groupAnagrams(char **strs, int strsSize, AnagramMap *map) {
    map_init(map);

    for (int i = 0; i < strsSize; i++) {
        char *s = strs[i];
        int len = (int)strlen(s);

        // Make a sorted copy of s to use as the key
        char *key = (char *)malloc((size_t)len + 1);
        if (!key) {
            fprintf(stderr, "Out of memory in groupAnagrams\n");
            exit(EXIT_FAILURE);
        }
        strcpy(key, s);
        qsort(key, (size_t)len, sizeof(char), char_cmp);

        unsigned int h = hash_str(key);
        AnagramGroup *g = map_find_group(map, key, h);
        if (!g) {
            g = map_add_group(map, key, h);
        }
        // We no longer need this temporary key; group has its own copy
        free(key);

        // Add the original string pointer to the group
        group_add_item(g, s);
    }
}

/* ===============================
 * Testing
 * =============================== */

static void print_groups(const AnagramMap *map) {
    printf("Anagram groups:\n");
    for (int i = 0; i < ANAGRAM_BUCKETS; i++) {
        const AnagramGroup *g = map->buckets[i];
        while (g) {
            printf("  Key \"%s\": [", g->key);
            for (int j = 0; j < g->size; j++) {
                printf("%s", g->items[j]);
                if (j + 1 < g->size) printf(", ");
            }
            printf("]\n");
            g = g->next;
        }
    }
}

int main(void) {
    char *words[] = { "eat", "tea", "tan", "ate", "nat", "bat" };
    int n = (int)(sizeof(words) / sizeof(words[0]));

    AnagramMap map;
    groupAnagrams(words, n, &map);

    printf("=== Test: groupAnagrams ===\n");
    printf("Input words: ");
    for (int i = 0; i < n; i++) {
        printf("%s", words[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("\n\n");

    print_groups(&map);
    printf("\n(Each group should contain anagrams, e.g. [eat, tea, ate], [tan, nat], [bat])\n");

    map_free(&map);
    return 0;
}
