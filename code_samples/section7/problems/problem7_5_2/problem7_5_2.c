#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* ============================================================
   Simple integer hash set (open addressing)
   ============================================================ */

/**
 * IntEntry
 *
 * One slot in an integer hash set.
 *
 * Fields:
 *   key  - stored integer value
 *   used - occupancy flag:
 *           0 = empty slot (never used)
 *           1 = occupied slot (contains a valid key)
 *
 * Notes:
 *   - This structure supports only "insert" and "contains".
 *   - No delete operation is provided. If deletion were needed,
 *     you'd want a tombstone state to preserve probe chains.
 */
typedef struct {
    int key;
    int used;  // 0=empty, 1=occupied
} IntEntry;

/**
 * IntSet
 *
 * A minimal hash set for integers using:
 *   - open addressing
 *   - linear probing
 *
 * Fields:
 *   table    - dynamically allocated array of IntEntry
 *   capacity - number of slots in the table
 *
 * Limitations:
 *   - No resizing: if the table gets too full, insertion fails and exits.
 *   - No delete: only add + contains.
 */
typedef struct {
    IntEntry *table;
    int capacity;
} IntSet;

/**
 * int_hash
 *
 * Purpose:
 *   Hash function for integers using multiplicative hashing.
 *
 * Notes:
 *   - Uses Knuth's constant. Decent distribution for many patterns.
 *   - Caller reduces modulo capacity.
 */
static unsigned int int_hash(int x) {
    return (unsigned int)x * 2654435761u;
}

/**
 * intset_init
 *
 * Purpose:
 *   Allocates and initializes a hash set.
 *
 * Parameters:
 *   s        - set to initialize
 *   capacity - requested capacity (forced to at least 8)
 *
 * Memory:
 *   - Uses calloc so all used flags start at 0 (empty).
 *
 * Error handling:
 *   Exits on allocation failure.
 */
static void intset_init(IntSet *s, int capacity) {
    if (capacity < 8) capacity = 8;
    s->capacity = capacity;

    // calloc zero-initializes: used==0 means empty slots
    s->table = (IntEntry*)calloc(capacity, sizeof(IntEntry));
    if (!s->table) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * intset_free
 *
 * Purpose:
 *   Frees memory owned by the set and clears fields.
 */
static void intset_free(IntSet *s) {
    free(s->table);
    s->table = NULL;
    s->capacity = 0;
}

/**
 * intset_contains
 *
 * Purpose:
 *   Tests whether a key exists in the set.
 *
 * Parameters:
 *   s   - set to query
 *   key - integer to check
 *
 * Returns:
 *   1 if present, 0 if not present.
 *
 * Probing logic:
 *   - Start at hash(key) % capacity and linearly probe.
 *   - If we encounter an unused slot, the key is not present and we can stop.
 */
static int intset_contains(const IntSet *s, int key) {
    unsigned int idx = int_hash(key) % s->capacity;

    for (int i = 0; i < s->capacity; i++) {
        int pos = (idx + i) % s->capacity;

        if (!s->table[pos].used) return 0;       // empty => not found
        if (s->table[pos].key == key) return 1;  // found
    }

    // Full scan fallback (rare unless table is very full)
    return 0;
}

/**
 * intset_add
 *
 * Purpose:
 *   Inserts a key into the set (idempotent: adding an existing key is fine).
 *
 * Parameters:
 *   s   - set to modify
 *   key - key to insert
 *
 * Behavior:
 *   - If an empty slot is found, insert there.
 *   - If the key already exists, do nothing and return.
 *
 * Error handling:
 *   Exits if the set is full (no resizing implemented).
 */
static void intset_add(IntSet *s, int key) {
    unsigned int idx = int_hash(key) % s->capacity;

    for (int i = 0; i < s->capacity; i++) {
        int pos = (idx + i) % s->capacity;

        // Insert into empty slot OR treat same key as "already present"
        if (!s->table[pos].used || s->table[pos].key == key) {
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

/**
 * PairEntry
 *
 * One slot in a hash set storing a 64-bit key.
 *
 * Here, the 64-bit key represents an ordered pair (a, b),
 * packed as:
 *   high 32 bits: (uint32_t)a
 *   low  32 bits: (uint32_t)b
 *
 * We store pairs in sorted order (a <= b) so that:
 *   (1,3) and (3,1) map to the same representation.
 */
typedef struct {
    uint64_t key;
    int used;
} PairEntry;

/**
 * PairSet
 *
 * Hash set for 64-bit keys using open addressing + linear probing.
 *
 * Fields:
 *   table    - array of PairEntry slots
 *   capacity - number of slots
 *
 * Limitations:
 *   - No resizing and no deletion.
 */
typedef struct {
    PairEntry *table;
    int capacity;
} PairSet;

/**
 * pair_hash
 *
 * Purpose:
 *   Hashes a 64-bit key using a splitmix64-style mixing function.
 *
 * Why we need this:
 *   - Our pair keys may have patterns (especially when many pairs share
 *     similar values). A strong mixer reduces clustering and improves
 *     performance for open addressing.
 *
 * Returns:
 *   Mixed 64-bit value (returned as unsigned long long).
 */
static unsigned long long pair_hash(uint64_t k) {
    // splitmix64 (good quality hash)
    k += 0x9e3779b97f4a7c15ULL;
    k = (k ^ (k >> 30)) * 0xbf58476d1ce4e5b9ULL;
    k = (k ^ (k >> 27)) * 0x94d049bb133111ebULL;
    return k ^ (k >> 31);
}

/**
 * pairset_init
 *
 * Purpose:
 *   Allocates and initializes a PairSet.
 */
static void pairset_init(PairSet *ps, int capacity) {
    if (capacity < 8) capacity = 8;
    ps->capacity = capacity;

    // calloc ensures used==0 for empty slots
    ps->table = (PairEntry*)calloc(capacity, sizeof(PairEntry));
    if (!ps->table) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * pairset_free
 *
 * Purpose:
 *   Frees memory owned by the pair set.
 */
static void pairset_free(PairSet *ps) {
    free(ps->table);
    ps->table = NULL;
    ps->capacity = 0;
}

/**
 * pairset_contains
 *
 * Purpose:
 *   Checks whether a packed pair key exists in the set.
 *
 * Returns:
 *   1 if present, 0 otherwise.
 */
static int pairset_contains(const PairSet *ps, uint64_t key) {
    unsigned long long h = pair_hash(key) % ps->capacity;

    for (int i = 0; i < ps->capacity; i++) {
        int pos = (h + i) % ps->capacity;

        if (!ps->table[pos].used) return 0;      // empty => not found
        if (ps->table[pos].key == key) return 1; // found
    }
    return 0;
}

/**
 * pairset_add
 *
 * Purpose:
 *   Inserts a packed pair key into the set (idempotent).
 *
 * Behavior:
 *   - Inserts into an empty slot, or does nothing if already present.
 */
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

/**
 * pairset_count
 *
 * Purpose:
 *   Counts how many unique keys are stored (i.e., how many pairs were inserted).
 *
 * Notes:
 *   - This is O(capacity). Fine for tests/demos.
 *   - In a production implementation you'd track size incrementally.
 */
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

/**
 * countTwoSumUniquePairs
 *
 * Purpose:
 *   Counts the number of *unique value pairs* {a, b} such that:
 *       a + b == target
 *
 *   Uniqueness is by VALUE, not by index.
 *
 * Example:
 *   nums = [1, 1, 2, 2, 3, 3], target = 4
 *   Valid value pairs are:
 *     {1,3} and {2,2}
 *   Even though there are multiple index combinations, the answer is 2.
 *
 * Parameters:
 *   nums   - input array
 *   n      - number of elements
 *   target - desired sum
 *
 * Returns:
 *   Count of distinct pairs (by value) that sum to target.
 *
 * Approach:
 *   - Maintain a set `seen` of values we've already processed.
 *   - For each element x:
 *       y = target - x
 *       If y is in `seen`, we have found a candidate pair.
 *       Normalize the pair as (a=min(x,y), b=max(x,y)) to enforce order.
 *       Pack (a,b) into a 64-bit key and insert into `pairs` set.
 *   - At end, return the number of unique keys stored in `pairs`.
 *
 * Why we need two sets:
 *   - `seen` tells us whether we've previously encountered the complement.
 *   - `pairs` ensures we count each VALUE pair only once even if it occurs
 *     many times (due to duplicates in nums).
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *       Each element does O(1) average-time set operations.
 *   - Space: O(n)
 *       `seen` can store up to n values; `pairs` up to O(n) pairs in worst case.
 *
 * Notes about packing pairs:
 *   - We cast to uint32_t so negative ints are represented consistently in the
 *     low/high 32-bit halves (two's complement bit patterns).
 *   - Because we always store (min,max), pair order is irrelevant.
 */
int countTwoSumUniquePairs(const int *nums, int n, int target) {
    IntSet seen;   // set of numbers we've seen so far
    PairSet pairs; // set of unique normalized pairs (a,b) with a <= b

    // Size heuristics: 2n+1 keeps load factor moderate for simple open addressing
    intset_init(&seen, n * 2 + 1);
    pairset_init(&pairs, n * 2 + 1);

    for (int i = 0; i < n; i++) {
        int x = nums[i];
        int y = target - x;

        // If we've already seen the complement, we have a candidate pair
        if (intset_contains(&seen, y)) {
            // Normalize pair order so (x,y) and (y,x) become the same pair
            int a = (x < y ? x : y);
            int b = (x < y ? y : x);

            // Pack (a,b) into a single 64-bit key: [a in high 32 bits | b in low 32 bits]
            uint64_t key = ((uint64_t)(uint32_t)a << 32) | (uint32_t)b;

            // Insert into the pair set; duplicates are ignored
            pairset_add(&pairs, key);
        }

        // Record that we've seen x (duplicates are fine)
        intset_add(&seen, x);
    }

    // Number of unique pairs inserted
    int result = pairset_count(&pairs);

    // Clean up memory
    intset_free(&seen);
    pairset_free(&pairs);

    return result;
}

/* ============================================================
   Testing
   ============================================================ */

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
 * main
 *
 * Console-based tests for countTwoSumUniquePairs().
 * Demonstrates:
 *   - duplicates creating only a small number of unique value pairs
 *   - normal two-sum case
 *   - repeated single value case (e.g. 3+3)
 *   - no-solution case
 */
int main(void) {
    printf("=== Test: countTwoSumUniquePairs ===\n\n");

    {
        // Unique value pairs summing to 4 are {1,3} and {2,2}
        int nums[] = {1, 1, 2, 2, 3, 3};
        int n = sizeof(nums)/sizeof(nums[0]);
        int target = 4;

        printf("Input: "); print_array(nums, n);
        printf(", target = %d\n", target);

        int r = countTwoSumUniquePairs(nums, n, target);
        printf("Unique pairs: %d (expected 2: {1,3}, {2,2})\n\n", r);
    }

    {
        // Only {2,7} sums to 9
        int nums[] = {2, 7, 11, 15};
        int n = sizeof(nums)/sizeof(nums[0]);
        int target = 9;

        printf("Input: "); print_array(nums, n);
        printf(", target = %d\n", target);

        int r = countTwoSumUniquePairs(nums, n, target);
        printf("Unique pairs: %d (expected 1: {2,7})\n\n", r);
    }

    {
        // Many pairs by index exist, but only one unique value pair {3,3}
        int nums[] = {3, 3, 3};
        int n = sizeof(nums)/sizeof(nums[0]);
        int target = 6;

        printf("Input: "); print_array(nums, n);
        printf(", target = %d\n", target);

        int r = countTwoSumUniquePairs(nums, n, target);
        printf("Unique pairs: %d (expected 1: {3,3})\n\n", r);
    }

    {
        // No two values sum to 10
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
