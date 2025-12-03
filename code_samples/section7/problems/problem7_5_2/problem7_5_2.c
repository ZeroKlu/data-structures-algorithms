#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* ============================================================
   Simple integer hash set (open addressing)
   ============================================================ */

typedef struct {
    int key;
    int used;  // 0=empty, 1=occupied
} IntEntry;

typedef struct {
    IntEntry *table;
    int capacity;
} IntSet;

static unsigned int int_hash(int x) {
    return (unsigned int)x * 2654435761u;
}

static void intset_init(IntSet *s, int capacity) {
    if (capacity < 8) capacity = 8;
    s->capacity = capacity;
    s->table = (IntEntry*)calloc(capacity, sizeof(IntEntry));
    if (!s->table) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
}

static void intset_free(IntSet *s) {
    free(s->table);
    s->table = NULL;
    s->capacity = 0;
}

static int intset_contains(const IntSet *s, int key) {
    unsigned int idx = int_hash(key) % s->capacity;
    for (int i = 0; i < s->capacity; i++) {
        int pos = (idx + i) % s->capacity;
        if (!s->table[pos].used) return 0;       // empty => not found
        if (s->table[pos].key == key) return 1;  // found
    }
    return 0;
}

static void intset_add(IntSet *s, int key) {
    unsigned int idx = int_hash(key) % s->capacity;
    for (int i = 0; i < s->capacity; i++) {
        int pos = (idx + i) % s->capacity;
        if (!s->table[pos].used ||
            s->table[pos].key == key) {
            s->table[pos].used = 1;
            s->table[pos].key = key;
            return;
        }
    }
    fprintf(stderr, "HashSet full (no resizing implemented)\n");
    exit(EXIT_FAILURE);
}

/* ============================================================
   64-bit pair hash set for (a,b) unique pairs
   ============================================================ */

typedef struct {
    uint64_t key;
    int used;
} PairEntry;

typedef struct {
    PairEntry *table;
    int capacity;
} PairSet;

static unsigned long long pair_hash(uint64_t k) {
    // splitmix64 (good quality hash)
    k += 0x9e3779b97f4a7c15ULL;
    k = (k ^ (k >> 30)) * 0xbf58476d1ce4e5b9ULL;
    k = (k ^ (k >> 27)) * 0x94d049bb133111ebULL;
    return k ^ (k >> 31);
}

static void pairset_init(PairSet *ps, int capacity) {
    if (capacity < 8) capacity = 8;
    ps->capacity = capacity;
    ps->table = (PairEntry*)calloc(capacity, sizeof(PairEntry));
    if (!ps->table) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
}

static void pairset_free(PairSet *ps) {
    free(ps->table);
    ps->table = NULL;
    ps->capacity = 0;
}

static int pairset_contains(const PairSet *ps, uint64_t key) {
    unsigned long long h = pair_hash(key) % ps->capacity;
    for (int i = 0; i < ps->capacity; i++) {
        int pos = (h + i) % ps->capacity;
        if (!ps->table[pos].used) return 0;
        if (ps->table[pos].key == key) return 1;
    }
    return 0;
}

static void pairset_add(PairSet *ps, uint64_t key) {
    unsigned long long h = pair_hash(key) % ps->capacity;
    for (int i = 0; i < ps->capacity; i++) {
        int pos = (h + i) % ps->capacity;
        if (!ps->table[pos].used || ps->table[pos].key == key) {
            ps->table[pos].used = 1;
            ps->table[pos].key = key;
            return;
        }
    }
    fprintf(stderr, "PairSet full (no resizing implemented)\n");
    exit(EXIT_FAILURE);
}

static int pairset_count(const PairSet *ps) {
    int cnt = 0;
    for (int i = 0; i < ps->capacity; i++) {
        if (ps->table[i].used) cnt++;
    }
    return cnt;
}

/* ============================================================
   Unique Two-Sum Pair Count
   ============================================================ */

int countTwoSumUniquePairs(const int *nums, int n, int target) {
    IntSet seen;
    PairSet pairs;

    intset_init(&seen, n * 2 + 1);
    pairset_init(&pairs, n * 2 + 1);

    for (int i = 0; i < n; i++) {
        int x = nums[i];
        int y = target - x;

        if (intset_contains(&seen, y)) {
            int a = (x < y ? x : y);
            int b = (x < y ? y : x);

            uint64_t key = ((uint64_t)(uint32_t)a << 32) | (uint32_t)b;
            pairset_add(&pairs, key);
        }

        intset_add(&seen, x);
    }

    int result = pairset_count(&pairs);

    intset_free(&seen);
    pairset_free(&pairs);

    return result;
}

/* ============================================================
   Testing
   ============================================================ */

static void print_array(const int *a, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", a[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]");
}

int main(void) {
    printf("=== Test: countTwoSumUniquePairs ===\n\n");

    {
        int nums[] = {1, 1, 2, 2, 3, 3};
        int n = sizeof(nums)/sizeof(nums[0]);
        int target = 4;
        printf("Input: "); print_array(nums, n); 
        printf(", target = %d\n", target);
        int r = countTwoSumUniquePairs(nums, n, target);
        printf("Unique pairs: %d (expected 2: {1,3}, {2,2})\n\n", r);
    }

    {
        int nums[] = {2, 7, 11, 15};
        int n = sizeof(nums)/sizeof(nums[0]);
        int target = 9;
        printf("Input: "); print_array(nums, n);
        printf(", target = %d\n", target);
        int r = countTwoSumUniquePairs(nums, n, target);
        printf("Unique pairs: %d (expected 1: {2,7})\n\n", r);
    }

    {
        int nums[] = {3, 3, 3};
        int n = sizeof(nums)/sizeof(nums[0]);
        int target = 6;
        printf("Input: "); print_array(nums, n);
        printf(", target = %d\n", target);
        int r = countTwoSumUniquePairs(nums, n, target);
        printf("Unique pairs: %d (expected 1: {3,3})\n\n", r);
    }

    {
        int nums[] = {1, 2, 3, 4};
        int n = sizeof(nums)/sizeof(nums[0]);
        int target = 10;
        printf("Input: "); print_array(nums, n);
        printf(", target = %d\n", target);
        int r = countTwoSumUniquePairs(nums, n, target);
        printf("Unique pairs: %d (expected 0)\n\n", r);
    }

    return 0;
}
