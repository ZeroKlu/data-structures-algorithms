#include <stdio.h>   // Provides standard I/O functions (printf, fopen, fscanf)
#include <stdlib.h>  // Provides dynamic memory management (malloc, realloc, free)

// ============================================================
// Interpolation Search (index-only version)
// Requires: arr sorted in ascending order
// ============================================================

/**
 * Performs interpolation search on a sorted integer array.
 *
 * Interpolation search is similar to binary search, but instead of always
 * picking the midpoint, it *estimates* the position of the target based on
 * the target's value relative to the values at the current bounds.
 *
 * This works best when values are reasonably uniformly distributed.
 *
 * @param arr     Pointer to the sorted array
 * @param n       Number of elements in the array
 * @param target  Value to search for
 *
 * @return Index of the target if found; otherwise -1
 */
int interpolation_search(const int *arr, int n, int target) {
    // Handle empty array case
    if (n <= 0) return -1;

    int lo = 0;       // Lower bound of the current search range (inclusive)
    int hi = n - 1;   // Upper bound of the current search range (inclusive)

    // Continue searching while:
    //  - bounds are valid, and
    //  - target falls within the value range [arr[lo], arr[hi]]
    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {

        // Special case: arr[lo] == arr[hi] means all values in [lo, hi] are equal.
        // If that shared value equals target, return lo; otherwise, target can't exist.
        if (arr[hi] == arr[lo]) {
            if (arr[lo] == target) return lo;
            else return -1;
        }

        // --------------------------------------------------------
        // Estimate target position (pos) using linear interpolation:
        // pos = lo + (hi - lo) * (target - arr[lo]) / (arr[hi] - arr[lo])
        //
        // Casting to double helps avoid integer truncation during division,
        // then the result is converted back to int for indexing.
        // --------------------------------------------------------
        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                             (arr[hi] - arr[lo]));

        // Guard: pos should always be within [lo, hi] mathematically, but
        // rounding or extreme distributions can produce out-of-range values.
        if (pos < lo || pos > hi) {
            return -1;
        }

        // Compare against the estimated position
        if (arr[pos] == target) {
            return pos;                 // Target found
        } else if (arr[pos] < target) {
            lo = pos + 1;               // Target is in the right subrange
        } else {
            hi = pos - 1;               // Target is in the left subrange
        }
    }

    // Target not found (either range exhausted or target out of bounds)
    return -1;
}

// ============================================================
// Interpolation Search with step counting
// steps_out = number of comparisons against arr[pos]/arr[lo]
// ============================================================

/**
 * Performs interpolation search while counting comparisons.
 *
 * Step counting here focuses on key comparisons made during the search:
 *  - Checking if arr[hi] == arr[lo] (uniform range check)
 *  - Checking if arr[lo] == target when uniform range
 *  - Checking if arr[pos] == target
 *  - Checking if arr[pos] < target (to decide direction)
 *
 * Note: The while-loop boundary checks (lo<=hi and target range checks) are
 * not explicitly counted here; only the labeled comparison steps are counted.
 *
 * @param arr        Pointer to the sorted array
 * @param n          Number of elements in the array
 * @param target     Value to search for
 * @param steps_out  Output parameter to store comparison count
 *
 * @return Index of the target if found; otherwise -1
 */
int interpolation_search_steps(const int *arr, int n, int target, int *steps_out) {
    int steps = 0;  // Counts selected comparisons performed by the algorithm

    // Handle empty array case
    if (n <= 0) {
        *steps_out = 0;
        return -1;
    }

    int lo = 0;       // Lower bound of search range
    int hi = n - 1;   // Upper bound of search range

    // Continue searching while the target is within the current value bounds
    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {

        // If all elements in the range are equal, handle as a special case
        steps++; // compare arr[hi] == arr[lo]
        if (arr[hi] == arr[lo]) {

            steps++; // compare arr[lo] == target
            if (arr[lo] == target) {
                *steps_out = steps;
                return lo;             // Target found
            } else {
                *steps_out = steps;
                return -1;             // Target not present
            }
        }

        // Estimate likely position of the target
        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                             (arr[hi] - arr[lo]));

        // Guard against out-of-range estimated positions
        if (pos < lo || pos > hi) {
            *steps_out = steps;
            return -1;
        }

        steps++; // compare arr[pos] == target
        if (arr[pos] == target) {
            *steps_out = steps;
            return pos;                // Target found
        }

        steps++; // compare arr[pos] < target
        if (arr[pos] < target) {
            lo = pos + 1;              // Search right subrange
        } else {
            hi = pos - 1;              // Search left subrange
        }
    }

    // Target not found
    *steps_out = steps;
    return -1;
}

// ============================================================
// Load ordered.txt into a dynamic array
// ============================================================

/**
 * Loads integers from a text file into a dynamically allocated array.
 *
 * The array grows as needed using realloc when capacity is reached.
 * This implementation also guards against realloc failure using a
 * temporary pointer to avoid losing the original allocation.
 *
 * @param path   Path to the input file
 * @param out_n  Output parameter to store number of integers read
 *
 * @return Pointer to allocated integer array, or NULL on failure
 */
int* load_file(const char* path, int* out_n) {
    // Attempt to open the file for reading
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Error reading: %s\n", path);
        *out_n = 0;
        return NULL;
    }

    int capacity = 4096;                         // Initial capacity
    int *arr = (int*)malloc(sizeof(int) * capacity);
    int count = 0;                               // Number of integers read
    int x;                                       // Temporary input variable

    // Read integers until EOF or invalid input
    while (fscanf(f, "%d", &x) == 1) {

        // Expand the array if capacity is reached
        if (count == capacity) {
            capacity *= 2;

            // Use a temporary pointer to safely handle realloc failure
            int *tmp = (int*)realloc(arr, sizeof(int) * capacity);
            if (!tmp) {
                free(arr);       // Prevent memory leak
                fclose(f);       // Close file before returning
                *out_n = 0;
                return NULL;
            }
            arr = tmp;           // Update array pointer to resized allocation
        }

        // Store the newly read integer
        arr[count++] = x;
    }

    // Close file after reading completes
    fclose(f);

    // Output total count
    *out_n = count;
    return arr;
}

// ============================================================
// Run tests: first, middle, last, missing
// ============================================================

/**
 * Executes a standardized set of interpolation search tests on ordered data.
 *
 * @param arr Pointer to the sorted array
 * @param n   Number of elements in the array
 */
void run_tests(const int *arr, int n) {
    printf("=== Interpolation Search Tests (ordered data only) ===\n");
    printf("Loaded %d integers\n\n", n);

    int steps; // Number of comparisons recorded by step-counting version
    int idx;   // Search result index

    // Test 1: Search for the first element
    idx = interpolation_search_steps(arr, n, arr[0], &steps);
    printf("Search first  (%d): index=%d, steps=%d\n", arr[0], idx, steps);

    // Test 2: Search for the middle element
    int midVal = arr[n / 2];
    idx = interpolation_search_steps(arr, n, midVal, &steps);
    printf("Search middle (%d): index=%d, steps=%d\n", midVal, idx, steps);

    // Test 3: Search for the last element
    idx = interpolation_search_steps(arr, n, arr[n - 1], &steps);
    printf("Search last   (%d): index=%d, steps=%d\n", arr[n - 1], idx, steps);

    // Test 4: Search for a value guaranteed not to exist
    idx = interpolation_search_steps(arr, n, 999999, &steps);
    printf("Search missing (999999): index=%d, steps=%d\n", idx, steps);

    printf("\n");
}

// ============================================================
// MAIN
// ============================================================

/**
 * Program entry point.
 *
 * Loads an ordered dataset from disk, runs interpolation search tests,
 * and frees allocated memory before exiting.
 */
int main(void) {
    int n = 0;  // Number of elements loaded

    // Adjust path as needed relative to where you build/run this executable.
    // If you run from code_samples/section12/lesson_3_interpolation_search:
    //   "..\\data\\ordered.txt" goes to code_samples/section12/data/ordered.txt
    const char *path = "..\\data\\ordered.txt";

    // Load dataset from disk
    int *arr = load_file(path, &n);

    // Validate successful file load
    if (!arr || n == 0) {
        printf("Missing input file-aborting.\n");
        return 1;
    }

    // Run interpolation search benchmarks
    run_tests(arr, n);

    // Free dynamically allocated memory
    free(arr);
    return 0;
}
