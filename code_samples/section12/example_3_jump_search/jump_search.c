#include <stdio.h>   // Provides standard input/output functions
#include <stdlib.h>  // Provides dynamic memory allocation (malloc, realloc, free)
#include <math.h>    // Provides mathematical functions such as sqrt

// ============================================================
// Jump Search (returns index only)
// ============================================================

/**
 * Performs a jump search on a sorted integer array.
 *
 * Jump search works by jumping ahead fixed block sizes
 * (typically sqrt(n)) to find a range that may contain
 * the target, followed by a linear scan within that range.
 *
 * @param arr     Pointer to the sorted array
 * @param n       Number of elements in the array
 * @param target  Value to search for
 *
 * @return Index of the target if found; otherwise -1
 */
int jump_search(const int *arr, int n, int target) {
    // Handle empty array case
    if (n <= 0) return -1;

    // Determine jump size (block size)
    int step = (int)sqrt(n);

    int prev = 0;        // Start index of the current block
    int curr = step;    // End index of the current block

    // Jump phase: skip ahead in blocks until target may be in range
    // Stop if we reach a value >= target or exceed array bounds
    while (curr < n && arr[curr] < target) {
        prev = curr;
        curr += step;
    }

    // Clamp curr to the last valid index if it exceeds array bounds
    if (curr >= n) curr = n - 1;

    // Linear search phase within the identified block
    for (int i = prev; i <= curr; i++) {
        if (arr[i] == target) {
            return i;  // Target found
        }
    }

    // Target not found
    return -1;
}

// ============================================================
// Jump Search WITH step counting
// steps = number of comparisons
// ============================================================

/**
 * Performs jump search while counting the number of comparisons.
 *
 * The step count includes:
 *   - Comparisons during the jump phase
 *   - Comparisons during the linear scan phase
 *
 * @param arr        Pointer to the sorted array
 * @param n          Number of elements in the array
 * @param target     Value to search for
 * @param steps_out  Output parameter to store comparison count
 *
 * @return Index of the target if found; otherwise -1
 */
int jump_search_steps(const int *arr, int n, int target, int *steps_out) {
    int steps = 0;  // Counts total number of comparisons

    // Handle empty array case
    if (n <= 0) {
        *steps_out = steps;
        return -1;
    }

    // Determine jump size
    int step = (int)sqrt(n);

    int prev = 0;        // Start of the current block
    int curr = step;    // End of the current block

    // --------------------------------------------------------
    // Jump phase: move block-by-block
    // --------------------------------------------------------
    while (curr < n) {
        steps++; // Count comparison arr[curr] < target

        // If current block end is >= target, stop jumping
        if (arr[curr] >= target)
            break;

        prev = curr;
        curr += step;
    }

    // Clamp curr to last valid index
    if (curr >= n) curr = n - 1;

    // --------------------------------------------------------
    // Linear search phase within identified block
    // --------------------------------------------------------
    for (int i = prev; i <= curr; i++) {
        steps++; // Count comparison arr[i] == target

        if (arr[i] == target) {
            *steps_out = steps;
            return i;  // Target found
        }
    }

    // Target not found
    *steps_out = steps;
    return -1;
}

// ============================================================
// Load file into array
// ============================================================

/**
 * Loads integers from a text file into a dynamically allocated array.
 *
 * @param path   Path to the input file
 * @param out_n  Output parameter to store number of integers read
 *
 * @return Pointer to allocated array, or NULL on failure
 */
int* load_file(const char* path, int* out_n) {
    // Attempt to open the file for reading
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Error reading: %s\n", path);
        *out_n = 0;
        return NULL;
    }

    int capacity = 4096;                   // Initial array capacity
    int *arr = malloc(sizeof(int) * capacity);
    int count = 0;                         // Number of values read
    int x;                                 // Temporary input variable

    // Read integers until EOF or invalid input
    while (fscanf(f, "%d", &x) == 1) {
        // Expand array if capacity is reached
        if (count == capacity) {
            capacity *= 2;
            arr = realloc(arr, sizeof(int) * capacity);
        }
        arr[count++] = x;
    }

    // Close file after reading
    fclose(f);

    // Output number of integers read
    *out_n = count;
    return arr;
}

// ============================================================
// Test runner
// ============================================================

/**
 * Executes a standardized set of jump search tests on a dataset.
 *
 * @param arr Pointer to the sorted array
 * @param n   Number of elements in the array
 */
void run_tests(int *arr, int n) {
    printf("=== Jump Search Tests ===\n");
    printf("Loaded %d integers\n\n", n);

    int idx;     // Search result index
    int steps;   // Number of comparisons performed

    // Test 1: Search for the first element
    idx = jump_search_steps(arr, n, arr[0], &steps);
    printf("Search first (%d): index=%d, steps=%d\n", arr[0], idx, steps);

    // Test 2: Search for the middle element
    int mid = arr[n / 2];
    idx = jump_search_steps(arr, n, mid, &steps);
    printf("Search middle (%d): index=%d, steps=%d\n", mid, idx, steps);

    // Test 3: Search for the last element
    idx = jump_search_steps(arr, n, arr[n - 1], &steps);
    printf("Search last (%d): index=%d, steps=%d\n", arr[n - 1], idx, steps);

    // Test 4: Search for a value guaranteed not to exist
    idx = jump_search_steps(arr, n, 999999, &steps);
    printf("Search missing (999999): index=%d, steps=%d\n", idx, steps);

    printf("\n");
}

// ============================================================
// MAIN
// ============================================================

/**
 * Program entry point.
 *
 * Loads an ordered dataset, runs jump search tests,
 * and frees allocated memory before exiting.
 */
int main(void) {
    int n;  // Number of elements loaded

    // Load ordered dataset from disk
    int *arr = load_file("..\\data\\ordered.txt", &n);

    // Validate successful file load
    if (!arr || n == 0) {
        printf("Missing input file-aborting.\n");
        return 1;
    }

    // Run jump search benchmarks
    run_tests(arr, n);

    // Free dynamically allocated memory
    free(arr);
    return 0;
}
