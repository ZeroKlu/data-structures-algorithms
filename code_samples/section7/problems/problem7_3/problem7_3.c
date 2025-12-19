#include <stdio.h>
#include <stdlib.h>

/* ===============================
 * Simple IntSet using open addressing
 * =============================== */

/**
 * IntSet
 *
 * A minimal integer set implemented as a hash table with open addressing
 * (linear probing).
 *
 * Storage:
 *   keys[i] holds an integer key when used[i] == 1.
 *   used[i] indicates whether a slot is occupied:
 *     0 = empty slot
 *     1 = occupied slot
 *
 * Limitations:
 *   - No deletion support (no tombstones), so we only ever insert and query.
 *   - No resizing; if the table becomes full, inserts may fail silently.
 *   - Designed for small / educational examples.
 */
typedef struct {
    int *keys;
    unsigned char *used;  // 0 = empty, 1 = occupied
    int capacity;
} IntSet;

/**
 * int_hash
 *
 * Purpose:
 *   Computes a bucket index for an integer key.
 *
 * Parameters:
 *   x        - integer to hash
 *   capacity - number of slots in the table
 *
 * Returns:
 *   Index in range [0, capacity).
 *
 * Notes:
 *   - Casting to unsigned makes negative values produce a deterministic
 *     non-negative modulo result.
 *   - This is a very simple hash (mod capacity). For educational use it's fine,
 *     but production hash tables typically use stronger mixing for ints.
 */
static int int_hash(int x, int capacity) {
    // cast to unsigned to avoid issues with negatives
    unsigned int ux = (unsigned int)x;
    return (int)(ux % (unsigned int)capacity);
}

/**
 * set_init
 *
 * Purpose:
 *   Allocates and initializes an IntSet with a fixed capacity.
 *
 * Parameters:
 *   s        - set to initialize
 *   capacity - requested capacity; if <= 0, will be forced to at least 1
 *
 * Memory:
 *   Allocates:
 *     - keys[]:  capacity ints
 *     - used[]:  capacity bytes
 *
 * Error handling:
 *   Exits the program if allocation fails.
 */
static void set_init(IntSet *s, int capacity) {
    if (capacity <= 0) capacity = 1;

    s->capacity = capacity;
    s->keys = (int *)malloc(sizeof(int) * capacity);
    s->used = (unsigned char *)malloc(sizeof(unsigned char) * capacity);

    if (!s->keys || !s->used) {
        fprintf(stderr, "Out of memory in set_init\n");
        exit(EXIT_FAILURE);
    }

    // Mark all slots empty
    for (int i = 0; i < capacity; i++) {
        s->used[i] = 0;
    }
}

/**
 * set_free
 *
 * Purpose:
 *   Frees memory owned by the IntSet and resets pointers for safety.
 */
static void set_free(IntSet *s) {
    free(s->keys);
    free(s->used);

    s->keys = NULL;
    s->used = NULL;
    s->capacity = 0;
}

/**
 * set_contains
 *
 * Purpose:
 *   Checks whether x exists in the set.
 *
 * Parameters:
 *   s - set to query
 *   x - value to search for
 *
 * Returns:
 *   1 if x is present, 0 otherwise.
 *
 * How linear probing works here:
 *   - Start at the hashed index.
 *   - Probe sequential slots (wrapping around) until:
 *       a) We find an empty slot -> x is not present
 *       b) We find x -> present
 *       c) We've probed capacity slots -> not present (table full / pathological)
 *
 * Key invariant:
 *   - Because we never delete, an empty slot guarantees the key was never inserted
 *     in that probe chain, so search can stop early.
 */
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

    // If we get here, we examined the whole table (rare with low load factor).
    return 0;
}

/**
 * set_add
 *
 * Purpose:
 *   Inserts x into the set if not already present.
 *
 * Parameters:
 *   s - set to modify
 *   x - value to add
 *
 * Behavior:
 *   - Uses linear probing to find either:
 *       a) an empty slot (insert), or
 *       b) an existing x (do nothing).
 *
 * Limitation:
 *   - If the table is completely full (or probe cycles), the function returns
 *     without inserting and without error. This is acceptable for demo-sized
 *     inputs but not for production usage.
 */
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

/**
 * longestConsecutive
 *
 * Purpose:
 *   Computes the length of the longest run of consecutive integers in an array,
 *   regardless of order. This matches the common "Longest Consecutive Sequence"
 *   problem.
 *
 * Example:
 *   nums = [100, 4, 200, 1, 3, 2] -> longest sequence is [1,2,3,4] => 4
 *
 * Parameters:
 *   nums     - pointer to input array (may be NULL if numsSize == 0)
 *   numsSize - number of elements in nums
 *
 * Returns:
 *   Length of the longest consecutive sequence (0 if input is empty).
 *
 * Approach:
 *   - Insert all numbers into a set for O(1) average membership checks.
 *   - For each number x in the set, only start counting when x-1 is not present.
 *     This ensures we only count sequences starting at their smallest element,
 *     avoiding O(n^2) behavior.
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *       Each value is inserted once, and each sequence is scanned once overall.
 *   - Space: O(n)
 *       For the set.
 *
 * Notes / caveats:
 *   - This depends on the hash set having a reasonable load factor; this demo uses
 *     capacity ~ 2n+1 to keep it <= ~0.5, reducing probe chains.
 *   - This implementation iterates over the set's underlying table slots.
 *     That is fine because we don’t delete and slots are stable.
 */
int longestConsecutive(const int *nums, int numsSize) {
    // Empty input => no consecutive sequence
    if (numsSize == 0) return 0;

    // Use a set to hold all numbers.
    IntSet set;

    // Simple heuristic: capacity = 2*n + 1 to keep load factor <= 0.5
    // This reduces collisions and speeds up probing.
    set_init(&set, numsSize * 2 + 1);

    // Insert all numbers into the set (duplicates automatically ignored)
    for (int i = 0; i < numsSize; i++) {
        set_add(&set, nums[i]);
    }

    int best = 0;

    // Iterate over the underlying hash table to find starts of sequences.
    // We only consider occupied slots.
    for (int i = 0; i < set.capacity; i++) {
        if (!set.used[i]) continue;

        int x = set.keys[i];

        // Only start counting if x-1 is not in the set.
        // This ensures x is the start (minimum) of its consecutive run.
        if (!set_contains(&set, x - 1)) {
            int cur = x;
            int length = 1;

            // Count upward as long as the next integer exists in the set.
            while (set_contains(&set, cur + 1)) {
                cur++;
                length++;
            }

            // Track the maximum sequence length seen so far.
            if (length > best) {
                best = length;
            }
        }
    }

    // Clean up allocated set storage
    set_free(&set);

    return best;
}

/* ===============================
 * Testing
 * =============================== */

/**
 * print_array
 *
 * Utility helper to print an integer array in [a, b, c] format.
 */
static void print_array(const int *a, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", a[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]");
}

/**
 * test_longestConsecutive
 *
 * Runs a few representative test cases:
 *   - mixed order with clear longest chain
 *   - includes duplicates
 *   - includes negative numbers
 *   - empty input
 */
static void test_longestConsecutive(void) {
    printf("=== Test: longestConsecutive ===\n\n");

    {
        // Classic example: longest is 1,2,3,4 => length 4
        int nums[] = {100, 4, 200, 1, 3, 2};
        int n = (int)(sizeof(nums) / sizeof(nums[0]));

        int res = longestConsecutive(nums, n);

        printf("Input: ");
        print_array(nums, n);
        printf("\nLongest consecutive length: %d (expected 4: 1,2,3,4)\n\n", res);
    }

    {
        // Example with duplicates: 0..8 => length 9
        int nums[] = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
        int n = (int)(sizeof(nums) / sizeof(nums[0]));

        int res = longestConsecutive(nums, n);

        printf("Input: ");
        print_array(nums, n);
        printf("\nLongest consecutive length: %d (expected 9: 0..8)\n\n", res);
    }

    {
        // Duplicates again: values {0,1,2} => length 3
        int nums[] = {1, 2, 0, 1};
        int n = (int)(sizeof(nums) / sizeof(nums[0]));

        int res = longestConsecutive(nums, n);

        printf("Input: ");
        print_array(nums, n);
        printf("\nLongest consecutive length: %d (expected 3: 0,1,2)\n\n", res);
    }

    {
        // Empty input: nums pointer can be NULL because numsSize == 0
        int *nums = NULL;
        int n = 0;

        int res = longestConsecutive(nums, n);

        printf("Input: []\n");
        printf("Longest consecutive length: %d (expected 0)\n\n", res);
    }

    {
        // Includes negatives; longest chain is 2,3,4,5 => length 4
        int nums[] = {-2, -1, 0, 2, 3, 4, 5};
        int n = (int)(sizeof(nums) / sizeof(nums[0]));

        int res = longestConsecutive(nums, n);

        printf("Input: ");
        print_array(nums, n);
        printf("\nLongest consecutive length: %d (expected 4: 2,3,4,5 or 3: -2,-1,0 depending on chain; here longest is 4)\n\n", res);
    }
}

/**
 * main
 *
 * Entry point that runs the test suite.
 */
int main(void) {
    test_longestConsecutive();
    return 0;
}
