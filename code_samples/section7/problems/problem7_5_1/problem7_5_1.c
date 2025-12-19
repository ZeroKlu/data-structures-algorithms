#include <stdio.h>
#include <stdlib.h>

/* ============================================================
   Simple int->int hash map using open addressing (linear probe)
   ============================================================ */

/**
 * Entry
 *
 * Represents one slot in the hash table.
 *
 * Fields:
 *   key   - the integer key stored in this slot (in this problem: a value from nums[])
 *   value - the integer value associated with the key (in this problem: an index in nums[])
 *   used  - occupancy flag:
 *            0 = empty slot (never used)
 *            1 = occupied slot (contains a valid key/value pair)
 *
 * Notes:
 *   - This map does NOT support deletion. If you add deletion later,
 *     you will need a "tombstone" state; otherwise probing chains break.
 */
typedef struct {
    int key;
    int value;   // index in nums[]
    int used;    // 0 = empty, 1 = occupied
} Entry;

/**
 * IntMap
 *
 * Minimal hash map (int -> int) backed by:
 *   - a fixed-size table of Entry slots
 *   - open addressing with linear probing
 *
 * Fields:
 *   table    - dynamically allocated array of Entry
 *   capacity - number of slots in table
 *
 * Limitations:
 *   - No resizing: if the table fills up, operations can fail and the program exits.
 *   - Only insert/update + lookup are supported.
 */
typedef struct {
    Entry *table;
    int capacity;
} IntMap;

/**
 * int_hash
 *
 * Purpose:
 *   Computes a hash for an integer key using Knuth's multiplicative hashing.
 *
 * Parameters:
 *   x - integer key
 *
 * Returns:
 *   Unsigned hash value; caller reduces it modulo capacity.
 *
 * Notes:
 *   - Multiplicative hashing spreads patterns reasonably well in many cases.
 *   - Still a simple hash; good enough for demo/testing.
 */
static unsigned int int_hash(int x) {
    return (unsigned int)x * 2654435761u; // Knuth multiplicative hash
}

/**
 * map_init
 *
 * Purpose:
 *   Initializes a hash map with at least 8 slots.
 *
 * Parameters:
 *   m        - map to initialize
 *   capacity - requested capacity (forced to >= 8)
 *
 * Memory:
 *   - Allocates table with calloc so entries start as all-zero (used=0).
 *
 * Error handling:
 *   Exits the program on allocation failure.
 */
static void map_init(IntMap *m, int capacity) {
    if (capacity < 8) capacity = 8;
    m->capacity = capacity;

    // calloc zero-initializes the table: used==0 means empty slot.
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
 * map_get
 *
 * Purpose:
 *   Looks up a key in the map.
 *
 * Parameters:
 *   m         - map to query
 *   key       - key to search for
 *   out_value - output pointer where the value will be stored if found
 *
 * Returns:
 *   1 if found (out_value populated), 0 if not found.
 *
 * Probing logic:
 *   - Start at hash(key) % capacity.
 *   - Linear probe: idx, idx+1, idx+2, ... (wrapping around).
 *   - If an empty slot is found, the key is not present and we can stop early.
 *
 * Important invariant:
 *   Because we never delete entries, encountering an empty slot guarantees the
 *   key was never inserted in this probe chain.
 */
static int map_get(const IntMap *m, int key, int *out_value) {
    unsigned int idx = int_hash(key) % m->capacity;

    for (int i = 0; i < m->capacity; i++) {
        int pos = (idx + i) % m->capacity;

        if (!m->table[pos].used)
            return 0; // key NOT found (empty slot ends the probe chain)

        if (m->table[pos].key == key) {
            *out_value = m->table[pos].value;
            return 1; // found
        }
    }

    // Full scan fallback (rare unless table is very full)
    return 0;
}

/**
 * map_put
 *
 * Purpose:
 *   Inserts or updates a key/value pair in the map.
 *
 * Parameters:
 *   m     - map to modify
 *   key   - key to insert/update
 *   value - value to store
 *
 * Behavior:
 *   - Probes until it finds:
 *       a) an empty slot → insert new entry
 *       b) the same key  → overwrite value (update)
 *
 * Error handling:
 *   - If the table is full (no empty slot found after capacity probes),
 *     prints an error and exits, because no resizing is implemented.
 */
static void map_put(IntMap *m, int key, int value) {
    unsigned int idx = int_hash(key) % m->capacity;

    for (int i = 0; i < m->capacity; i++) {
        int pos = (idx + i) % m->capacity;

        // Insert into empty slot OR update existing key in place.
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

/**
 * twoSum
 *
 * Purpose:
 *   Finds two distinct indices i and j such that:
 *       nums[i] + nums[j] == target
 *
 * Parameters:
 *   nums   - input array of integers
 *   n      - length of nums
 *   target - desired sum
 *   out_i  - output: first index of the pair (only valid if function returns 1)
 *   out_j  - output: second index of the pair (only valid if function returns 1)
 *
 * Returns:
 *   1 if a pair is found (out_i/out_j are set), 0 if no solution exists.
 *
 * Approach (single pass with hash map):
 *   - As we scan nums from left to right:
 *       need = target - nums[i]
 *       If need is already in the map, we have found a previous number that
 *       complements nums[i] to reach target.
 *       Otherwise, record nums[i] -> i in the map and continue.
 *
 * Complexity (expected / average):
 *   - Time:  O(n)
 *       Each element does O(1) average hash operations.
 *   - Space: O(n)
 *       Map may store up to n entries.
 *
 * Notes / limitations:
 *   - This map stores only one index per value (the most recently inserted index).
 *     That is sufficient for typical Two-Sum requirements, but if you needed
 *     *all pairs* or special duplicate handling, the map would need changes.
 */
int twoSum(const int *nums, int n, int target, int *out_i, int *out_j) {
    // Hash map: value -> index
    IntMap map;

    // Capacity heuristic to reduce collisions (keeps load factor low for small n)
    map_init(&map, n * 2 + 1);

    for (int i = 0; i < n; i++) {
        // Compute the complement needed to reach target
        int need = target - nums[i];

        int idx;
        if (map_get(&map, need, &idx)) {
            // Found a previous value such that nums[idx] + nums[i] == target
            *out_i = idx;
            *out_j = i;

            // Clean up allocated map memory before returning
            map_free(&map);
            return 1; // success
        }

        // Otherwise record current value -> current index for future matches
        map_put(&map, nums[i], i);
    }

    // No solution found
    map_free(&map);
    return 0;
}

/* ============================================================
   Testing
   ============================================================ */

/**
 * print_array
 *
 * Utility helper that prints an integer array in [a, b, c] format.
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
 * Console-based tests for twoSum():
 *   - standard case with a solution
 *   - another case with a solution
 *   - negative case with no solution
 */
int main(void) {
    printf("=== Test: twoSum ===\n\n");

    {
        // Expected: indices (0,1) => 2 + 7 = 9
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
        // Expected: indices (1,2) => 2 + 4 = 6
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
        // No pair in [1,2,3,4] sums to 10
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
