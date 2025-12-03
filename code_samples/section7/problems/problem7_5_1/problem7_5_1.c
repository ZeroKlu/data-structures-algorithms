#include <stdio.h>
#include <stdlib.h>

/* ============================================================
   Simple int->int hash map using open addressing (linear probe)
   ============================================================ */

typedef struct {
    int key;
    int value;   // index in nums[]
    int used;    // 0 = empty, 1 = occupied
} Entry;

typedef struct {
    Entry *table;
    int capacity;
} IntMap;

static unsigned int int_hash(int x) {
    return (unsigned int)x * 2654435761u; // Knuth multiplicative hash
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

static int map_get(const IntMap *m, int key, int *out_value) {
    unsigned int idx = int_hash(key) % m->capacity;
    for (int i = 0; i < m->capacity; i++) {
        int pos = (idx + i) % m->capacity;
        if (!m->table[pos].used)
            return 0; // key NOT found
        if (m->table[pos].key == key) {
            *out_value = m->table[pos].value;
            return 1; // found
        }
    }
    return 0;
}

static void map_put(IntMap *m, int key, int value) {
    unsigned int idx = int_hash(key) % m->capacity;
    for (int i = 0; i < m->capacity; i++) {
        int pos = (idx + i) % m->capacity;
        if (!m->table[pos].used || m->table[pos].key == key) {
            m->table[pos].used = 1;
            m->table[pos].key = key;
            m->table[pos].value = value;
            return;
        }
    }
    fprintf(stderr, "Hash map full (no resizing implemented)\n");
    exit(EXIT_FAILURE);
}

/* ============================================================
   Two-Sum Implementation
   ============================================================ */

int twoSum(const int *nums, int n, int target, int *out_i, int *out_j) {
    IntMap map;
    map_init(&map, n * 2 + 1);

    for (int i = 0; i < n; i++) {
        int need = target - nums[i];

        int idx;
        if (map_get(&map, need, &idx)) {
            // Found a pair
            *out_i = idx;
            *out_j = i;
            map_free(&map);
            return 1; // success
        }

        // Otherwise record nums[i] → i
        map_put(&map, nums[i], i);
    }

    map_free(&map);
    return 0; // no solution
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
    printf("=== Test: twoSum ===\n\n");

    {
        int nums[] = {2, 7, 11, 15};
        int n = sizeof(nums)/sizeof(nums[0]);
        int target = 9;
        int i, j;

        printf("Input: ");
        print_array(nums, n);
        printf(", target = %d\n", target);

        if (twoSum(nums, n, target, &i, &j)) {
            printf("Found pair: (%d, %d) => %d + %d = %d\n\n",
                   i, j, nums[i], nums[j], target);
        } else {
            printf("No pair found.\n\n");
        }
    }

    {
        int nums[] = {3, 2, 4};
        int n = sizeof(nums)/sizeof(nums[0]);
        int target = 6;
        int i, j;

        printf("Input: ");
        print_array(nums, n);
        printf(", target = %d\n", target);

        if (twoSum(nums, n, target, &i, &j)) {
            printf("Found pair: (%d, %d) => %d + %d = %d\n\n",
                   i, j, nums[i], nums[j], target);
        } else {
            printf("No pair found.\n\n");
        }
    }

    {
        int nums[] = {1, 2, 3, 4};
        int n = sizeof(nums)/sizeof(nums[0]);
        int target = 10;
        int i, j;

        printf("Input: ");
        print_array(nums, n);
        printf(", target = %d\n", target);

        if (twoSum(nums, n, target, &i, &j)) {
            printf("ERROR: Should not find a pair\n\n");
        } else {
            printf("Correct: No valid pair found.\n\n");
        }
    }

    return 0;
}
