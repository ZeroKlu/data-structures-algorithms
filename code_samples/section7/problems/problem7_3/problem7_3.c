#include <stdio.h>
#include <stdlib.h>

/* ===============================
 * Simple IntSet using open addressing
 * =============================== */

typedef struct {
    int *keys;
    unsigned char *used;  // 0 = empty, 1 = occupied
    int capacity;
} IntSet;

static int int_hash(int x, int capacity) {
    // cast to unsigned to avoid issues with negatives
    unsigned int ux = (unsigned int)x;
    return (int)(ux % (unsigned int)capacity);
}

static void set_init(IntSet *s, int capacity) {
    if (capacity <= 0) capacity = 1;
    s->capacity = capacity;
    s->keys = (int *)malloc(sizeof(int) * capacity);
    s->used = (unsigned char *)malloc(sizeof(unsigned char) * capacity);
    if (!s->keys || !s->used) {
        fprintf(stderr, "Out of memory in set_init\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < capacity; i++) {
        s->used[i] = 0;
    }
}

static void set_free(IntSet *s) {
    free(s->keys);
    free(s->used);
    s->keys = NULL;
    s->used = NULL;
    s->capacity = 0;
}

static int set_contains(const IntSet *s, int x) {
    int idx = int_hash(x, s->capacity);
    for (int i = 0; i < s->capacity; i++) {
        int probe = (idx + i) % s->capacity;
        if (!s->used[probe]) {
            // empty slot means key is not present
            return 0;
        }
        if (s->keys[probe] == x) {
            return 1;
        }
    }
    return 0;
}

static void set_add(IntSet *s, int x) {
    int idx = int_hash(x, s->capacity);
    for (int i = 0; i < s->capacity; i++) {
        int probe = (idx + i) % s->capacity;
        if (!s->used[probe]) {
            // insert new
            s->used[probe] = 1;
            s->keys[probe] = x;
            return;
        }
        if (s->keys[probe] == x) {
            // already present, do nothing
            return;
        }
    }
    // For simplicity, we do not resize here. For small test cases, it's fine.
    // In production code, you'd want to resize when the table gets too full.
}

/* ===============================
 * longestConsecutive implementation
 * =============================== */

int longestConsecutive(const int *nums, int numsSize) {
    if (numsSize == 0) return 0;

    // Use a set to hold all numbers.
    IntSet set;
    // Simple heuristic: capacity = 2*n + 1 to keep load factor <= 0.5
    set_init(&set, numsSize * 2 + 1);

    for (int i = 0; i < numsSize; i++) {
        set_add(&set, nums[i]);
    }

    int best = 0;

    // Iterate over the underlying hash table to find starts of sequences
    for (int i = 0; i < set.capacity; i++) {
        if (!set.used[i]) continue;
        int x = set.keys[i];

        // Only start counting if x-1 is not in the set
        if (!set_contains(&set, x - 1)) {
            int cur = x;
            int length = 1;
            while (set_contains(&set, cur + 1)) {
                cur++;
                length++;
            }
            if (length > best) {
                best = length;
            }
        }
    }

    set_free(&set);
    return best;
}

/* ===============================
 * Testing
 * =============================== */

static void print_array(const int *a, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", a[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]");
}

static void test_longestConsecutive(void) {
    printf("=== Test: longestConsecutive ===\n\n");

    {
        int nums[] = {100, 4, 200, 1, 3, 2};
        int n = (int)(sizeof(nums) / sizeof(nums[0]));
        int res = longestConsecutive(nums, n);
        printf("Input: ");
        print_array(nums, n);
        printf("\nLongest consecutive length: %d (expected 4: 1,2,3,4)\n\n", res);
    }

    {
        int nums[] = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
        int n = (int)(sizeof(nums) / sizeof(nums[0]));
        int res = longestConsecutive(nums, n);
        printf("Input: ");
        print_array(nums, n);
        printf("\nLongest consecutive length: %d (expected 9: 0..8)\n\n", res);
    }

    {
        int nums[] = {1, 2, 0, 1};
        int n = (int)(sizeof(nums) / sizeof(nums[0]));
        int res = longestConsecutive(nums, n);
        printf("Input: ");
        print_array(nums, n);
        printf("\nLongest consecutive length: %d (expected 3: 0,1,2)\n\n", res);
    }

    {
        int *nums = NULL;
        int n = 0;
        int res = longestConsecutive(nums, n);
        printf("Input: []\n");
        printf("Longest consecutive length: %d (expected 0)\n\n", res);
    }

    {
        int nums[] = {-2, -1, 0, 2, 3, 4, 5};
        int n = (int)(sizeof(nums) / sizeof(nums[0]));
        int res = longestConsecutive(nums, n);
        printf("Input: ");
        print_array(nums, n);
        printf("\nLongest consecutive length: %d (expected 4: 2,3,4,5 or 3: -2,-1,0 depending on chain; here longest is 4)\n\n", res);
    }
}

int main(void) {
    test_longestConsecutive();
    return 0;
}
