#include <stdio.h>
#include <stdlib.h>

/* ============================================
   Simple int->int hash map (open addressing)
   ============================================ */

/**
 * Entry
 *
 * One slot in the hash table.
 *
 * Fields:
 *   key   - integer key stored in this slot
 *   value - integer value associated with key (used here as a frequency counter)
 *   used  - occupancy flag:
 *            0 = empty slot (never used)
 *            1 = occupied slot (contains a valid key/value)
 *
 * Notes:
 *   - This table does NOT support deletions. If you wanted deletion, you'd need
 *     a third state like "tombstone" to keep probing chains intact.
 */
typedef struct {
    int key;
    int value;
    int used;   // 0 = empty, 1 = occupied
} Entry;

/**
 * IntMap
 *
 * Minimal hash map from int -> int using open addressing with linear probing.
 *
 * Fields:
 *   table    - dynamically allocated array of Entry
 *   capacity - number of slots in table
 *
 * Limitations (important):
 *   - No resizing; if too full, map_put will exit.
 *   - No delete operation (no tombstones).
 *   - Intended for educational / small test cases.
 */
typedef struct {
    Entry *table;
    int capacity;
} IntMap;

/**
 * int_hash
 *
 * Purpose:
 *   Computes a hash for an integer key.
 *
 * Parameters:
 *   x - key to hash
 *
 * Returns:
 *   32-bit unsigned hash value.
 *
 * Notes:
 *   - Uses multiplication by a constant (Knuth's multiplicative hashing).
 *   - This is simple and decent for spreading bits for many integer patterns.
 *   - Still not a full-featured hash; good enough for demo/testing.
 */
static unsigned int int_hash(int x) {
    // simple, but OK for testing
    return (unsigned int)x * 2654435761u;
}

/**
 * map_init
 *
 * Purpose:
 *   Allocates and initializes a hash map.
 *
 * Parameters:
 *   m        - map to initialize
 *   capacity - requested capacity (will be forced to at least 8)
 *
 * Memory:
 *   - Uses calloc to zero-initialize entries (used=0, key=0, value=0).
 *
 * Error handling:
 *   Exits the program on allocation failure.
 */
static void map_init(IntMap *m, int capacity) {
    if (capacity < 8) capacity = 8;
    m->capacity = capacity;

    // calloc initializes all entries to zero: used=0 means "empty"
    m->table = (Entry*)calloc(capacity, sizeof(Entry));
    if (!m->table) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * map_free
 *
 * Purpose:
 *   Releases memory owned by the map and resets fields for safety.
 */
static void map_free(IntMap *m) {
    free(m->table);
    m->table = NULL;
    m->capacity = 0;
}

/**
 * map_get_or_default
 *
 * Purpose:
 *   Looks up a key and returns its value, or returns a provided default if absent.
 *
 * Parameters:
 *   m   - map to query
 *   key - key to search for
 *   def - value to return if key is not found
 *
 * Returns:
 *   value associated with key if present, otherwise def.
 *
 * Probing logic:
 *   - Compute starting index from hash(key) % capacity.
 *   - Probe linearly (idx, idx+1, idx+2, ...) wrapping around.
 *   - If we hit an empty slot (used==0), the key is not present and we can stop early.
 *
 * Why early stop is valid:
 *   - Because we never delete entries, an empty slot means the probe chain
 *     for this key was never extended beyond that point.
 */
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

    // Full-table scan fallback (rare unless table is nearly full)
    return def; // not found
}

/**
 * map_put
 *
 * Purpose:
 *   Inserts or updates a key/value pair.
 *
 * Parameters:
 *   m     - map to modify
 *   key   - key to insert/update
 *   value - value to store
 *
 * Behavior:
 *   - Linear probes until it finds:
 *       a) an empty slot (insert new entry)
 *       b) an existing entry with matching key (overwrite value)
 *
 * Error handling:
 *   - Exits if the table is full, because no resizing is implemented.
 */
static void map_put(IntMap *m, int key, int value) {
    unsigned idx = int_hash(key) % m->capacity;

    for (int i = 0; i < m->capacity; i++) {
        int pos = (idx + i) % m->capacity;

        // Empty slot: insert new entry
        if (!m->table[pos].used) {
            m->table[pos].used = 1;
            m->table[pos].key = key;
            m->table[pos].value = value;
            return;
        }

        // Existing key: update in place
        if (m->table[pos].key == key) {
            m->table[pos].value = value;
            return;
        }
    }

    // If we probed every slot, the table is full.
    fprintf(stderr, "Hash table full - no resize implemented.\n");
    exit(EXIT_FAILURE);
}

/* ============================================
   subarraySum implementation
   ============================================ */

/**
 * subarraySum
 *
 * Purpose:
 *   Counts the number of contiguous subarrays whose sum equals k.
 *
 * Parameters:
 *   nums     - pointer to array of integers (may be NULL if numsSize == 0)
 *   numsSize - number of elements in nums
 *   k        - target sum
 *
 * Returns:
 *   Number of subarrays whose sum is exactly k.
 *
 * Key idea (prefix sums + frequency map):
 *   Let prefix[i] be the sum of nums[0..i].
 *   The sum of subarray (j+1..i) is:
 *       prefix[i] - prefix[j]
 *
 *   We want prefix[i] - prefix[j] == k  =>  prefix[j] == prefix[i] - k
 *
 *   So for each i:
 *     - compute current prefix sum
 *     - add how many times (prefix - k) has appeared before
 *     - record that we've now seen this prefix sum
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *   - Space: O(n) for the prefix frequency map
 *
 * Notes:
 *   - Works with negative numbers (unlike sliding window techniques).
 *   - The custom IntMap is sized to roughly 2n+1 to keep load factor low.
 */
int subarraySum(int *nums, int numsSize, int k) {
    // Frequency map: prefix_sum -> count of how many times we've seen it
    IntMap freq;

    // Capacity heuristic keeps expected collisions/probing small.
    // (Still no resize, so extremely unlucky inputs could fill the table.)
    map_init(&freq, numsSize * 2 + 1);

    // Base case: prefix sum 0 occurs once before reading any elements.
    // This allows subarrays starting at index 0 to be counted correctly.
    map_put(&freq, 0, 1);

    int prefix = 0;  // running prefix sum
    int count = 0;   // total number of qualifying subarrays found

    // Scan through the array once
    for (int i = 0; i < numsSize; i++) {
        prefix += nums[i];

        // If we previously saw prefix sums equal to (prefix - k),
        // then each such occurrence defines a subarray ending at i with sum k.
        int need = prefix - k;
        count += map_get_or_default(&freq, need, 0);

        // Record that we've now seen this prefix sum.
        // We store frequencies because the same prefix sum can occur multiple times.
        int oldcount = map_get_or_default(&freq, prefix, 0);
        map_put(&freq, prefix, oldcount + 1);
    }

    // Clean up allocated hash table memory
    map_free(&freq);

    return count;
}

/* ============================================
   Testing
   ============================================ */

/**
 * print_array
 *
 * Utility helper that prints an integer array as [a, b, c].
 */
static void print_array(int *a, int n) {
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
 * Console-based tests for subarraySum() across:
 *   - small positives
 *   - multiple solutions in one array
 *   - mixture including negatives
 *   - empty input
 */
int main(void) {
    printf("=== Test: subarraySum ===\n\n");

    {
        // Two subarrays sum to 2: [1,1] at indices (0..1) and (1..2)
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
        // Two subarrays sum to 3: [1,2] and [3]
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
        // Classic example with negatives:
        // Expected 4 subarrays sum to 7.
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
        // Empty input should always yield 0, regardless of k.
        int *nums = NULL;
        int n = 0;
        int k = 0;

        printf("Input: []\n");

        int r = subarraySum(nums, n, k);
        printf("Result: %d (expected 0)\n\n", r);
    }

    return 0;
}
