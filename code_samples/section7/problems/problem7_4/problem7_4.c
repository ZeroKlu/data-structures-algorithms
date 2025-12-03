#include <stdio.h>
#include <stdlib.h>

/* ============================================
   Simple int->int hash map (open addressing)
   ============================================ */

typedef struct {
    int key;
    int value;
    int used;   // 0 = empty, 1 = occupied
} Entry;

typedef struct {
    Entry *table;
    int capacity;
} IntMap;

static unsigned int int_hash(int x) {
    // simple, but OK for testing
    return (unsigned int)x * 2654435761u;
}

static void map_init(IntMap *m, int capacity) {
    if (capacity < 8) capacity = 8;
    m->capacity = capacity;
    m->table = (Entry*)calloc(capacity, sizeof(Entry));
    if (!m->table) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
}

static void map_free(IntMap *m) {
    free(m->table);
    m->table = NULL;
    m->capacity = 0;
}

static int map_get_or_default(IntMap *m, int key, int def) {
    unsigned idx = int_hash(key) % m->capacity;
    for (int i = 0; i < m->capacity; i++) {
        int pos = (idx + i) % m->capacity;
        if (!m->table[pos].used) {
            return def;  // empty cell → not found
        }
        if (m->table[pos].used && m->table[pos].key == key) {
            return m->table[pos].value;
        }
    }
    return def; // not found
}

static void map_put(IntMap *m, int key, int value) {
    unsigned idx = int_hash(key) % m->capacity;
    for (int i = 0; i < m->capacity; i++) {
        int pos = (idx + i) % m->capacity;
        if (!m->table[pos].used) {
            m->table[pos].used = 1;
            m->table[pos].key = key;
            m->table[pos].value = value;
            return;
        }
        if (m->table[pos].key == key) {
            m->table[pos].value = value;
            return;
        }
    }
    fprintf(stderr, "Hash table full — no resize implemented.\n");
    exit(EXIT_FAILURE);
}

/* ============================================
   subarraySum implementation
   ============================================ */

int subarraySum(int *nums, int numsSize, int k) {
    IntMap freq;
    map_init(&freq, numsSize * 2 + 1);

    map_put(&freq, 0, 1); // prefix sum 0 appears once before we start

    int prefix = 0;
    int count = 0;

    for (int i = 0; i < numsSize; i++) {
        prefix += nums[i];

        int need = prefix - k;
        count += map_get_or_default(&freq, need, 0);

        int oldcount = map_get_or_default(&freq, prefix, 0);
        map_put(&freq, prefix, oldcount + 1);
    }

    map_free(&freq);
    return count;
}

/* ============================================
   Testing
   ============================================ */

static void print_array(int *a, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", a[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]");
}

int main(void) {
    printf("=== Test: subarraySum ===\n\n");

    {
        int nums[] = {1, 1, 1};
        int n = sizeof(nums)/sizeof(nums[0]);
        int k = 2;
        printf("Input: ");
        print_array(nums, n);
        printf(", k = %d\n", k);
        int r = subarraySum(nums, n, k);
        printf("Result: %d (expected 2)\n\n", r);
    }

    {
        int nums[] = {1, 2, 3};
        int n = sizeof(nums)/sizeof(nums[0]);
        int k = 3;
        printf("Input: ");
        print_array(nums, n);
        printf(", k = %d\n", k);
        int r = subarraySum(nums, n, k);
        printf("Result: %d (expected 2: [1,2], [3])\n\n", r);
    }

    {
        int nums[] = {3, 4, 7, 2, -3, 1, 4, 2};
        int n = sizeof(nums)/sizeof(nums[0]);
        int k = 7;
        printf("Input: ");
        print_array(nums, n);
        printf(", k = %d\n", k);
        int r = subarraySum(nums, n, k);
        printf("Result: %d (expected 4)\n\n", r);
    }

    {
        int *nums = NULL;
        int n = 0;
        int k = 0;
        printf("Input: []\n");
        int r = subarraySum(nums, n, k);
        printf("Result: %d (expected 0)\n\n", r);
    }

    return 0;
}
