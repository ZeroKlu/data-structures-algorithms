#include <stdio.h>   // Standard I/O (printf, fopen, fscanf)
#include <stdlib.h>  // Memory management (malloc, free)

/* -------------------------------------------------------
   Binary Search helper (counts steps)
   Searches arr[left..right] inclusive.
   ------------------------------------------------------- */

/**
 * Performs a binary search on a sorted integer array within a subrange
 * [left, right], inclusive, while counting "steps".
 *
 * Step counting:
 *   - Each loop iteration increments *steps once.
 *   - This roughly corresponds to "one probe" (one midpoint evaluation)
 *     per iteration.
 *
 * Important note:
 *   - This helper does NOT reset *steps; it increments the value it receives.
 *   - In this program, exponential_search() initializes *steps to 0 first,
 *     then this helper adds on the binary-search iterations.
 *
 * @param arr    Pointer to the sorted array
 * @param left   Left boundary index (inclusive)
 * @param right  Right boundary index (inclusive)
 * @param target Value to locate
 * @param steps  Pointer to an integer counter to be incremented per iteration
 *
 * @return Index of the target if found; otherwise -1
 */
int binary_search_steps(const int* arr, int left, int right, int target, int* steps) {
    // Standard binary search loop over an inclusive range
    while (left <= right) {
        (*steps)++; // Count this iteration / midpoint probe

        // Compute midpoint safely (avoids overflow in left+right)
        int mid = left + (right - left) / 2;

        // Compare midpoint against the target
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;  // Target must be to the right
        else right = mid - 1;                         // Target must be to the left
    }

    // Target not found in this subrange
    return -1;
}

/* -------------------------------------------------------
   Exponential Search (counts steps)
   ------------------------------------------------------- */

/**
 * Performs exponential search on a sorted integer array while counting "steps".
 *
 * Exponential search works in two phases:
 *   1) Exponential range expansion:
 *      Start with bound=1 and repeatedly double it (1,2,4,8,...) until
 *      arr[bound] >= target or bound goes out of range.
 *   2) Binary search within the identified range:
 *      Once a candidate range [bound/2, min(bound, n-1)] is found,
 *      perform binary search there.
 *
 * Step counting:
 *   - Counts the initial check of arr[0].
 *   - Counts each successful range-expansion comparison (arr[bound] < target).
 *   - Then counts each binary-search iteration performed by binary_search_steps().
 *
 * @param arr    Pointer to the sorted array
 * @param n      Number of elements in the array
 * @param target Value to locate
 * @param steps  Output parameter: receives total step count for this search
 *
 * @return Index of the target if found; otherwise -1
 */
int exponential_search(const int* arr, int n, int target, int* steps) {
    *steps = 0; // Reset step counter for this search

    // Handle empty array case
    if (n == 0) return -1;

    // ---------------------------------------------------
    // Quick check of the first element
    // ---------------------------------------------------
    (*steps)++; // check arr[0]
    if (arr[0] == target) return 0;

    // ---------------------------------------------------
    // Phase 1: Find a range for binary search by doubling bound
    // ---------------------------------------------------
    int bound = 1;

    // Expand the bound while:
    //  - bound is in range, and
    //  - arr[bound] is still less than target (meaning target must be further right)
    while (bound < n && arr[bound] < target) {
        (*steps)++;   // count each range expansion step (probe at arr[bound])
        bound *= 2;   // double the bound
    }

    // After the loop, target (if present) must lie within:
    //   left  = bound/2
    //   right = min(bound, n-1)
    int left = bound / 2;
    int right = (bound < n) ? bound : n - 1;

    // ---------------------------------------------------
    // Phase 2: Binary search within [left, right]
    // ---------------------------------------------------
    // Note: binary_search_steps() will continue incrementing *steps
    return binary_search_steps(arr, left, right, target, steps);
}

/* -------------------------------------------------------
   Load integer array from file
   ------------------------------------------------------- */

/**
 * Loads integers from a text file into a dynamically allocated array.
 *
 * Implementation notes / assumptions:
 *   - The function allocates a fixed initial capacity of 11000 integers.
 *   - It does not currently check for exceeding that capacity while reading.
 *     (So the file should contain <= 11000 integers to avoid out-of-bounds writes.)
 *   - Returns the number of integers read, or -1 on file open failure.
 *
 * @param path     File path to read from
 * @param out_arr  Output parameter to receive the allocated array pointer
 *
 * @return Count of integers read (>=0), or -1 on error opening file
 */
int load_file(const char* path, int** out_arr) {
    // Attempt to open file for reading
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Error reading: %s\n", path);
        return -1;
    }

    int capacity = 11000;                  // Fixed capacity assumption
    int* arr = malloc(capacity * sizeof(int));
    int count = 0;

    // Read integers directly into the allocated array
    // (Stops when fscanf fails or hits EOF)
    while (fscanf(f, "%d", &arr[count]) == 1) {
        count++;
    }

    // Close file after reading
    fclose(f);

    // Return results to caller
    *out_arr = arr;
    return count;
}

/* -------------------------------------------------------
   MAIN TESTS
   ------------------------------------------------------- */

/**
 * Program entry point.
 *
 * Loads ordered data from disk, then runs exponential search on several
 * target values and prints the resulting index and step count.
 */
int main(void) {
    // Path to ordered.txt relative to where this executable is run
    const char* ordered_path = "..\\data\\ordered.txt";
    int* arr = NULL;

    // Diagnostic output to confirm file path being used
    printf("Attempting to read: %s\n", ordered_path);

    // Load dataset from disk
    int n = load_file(ordered_path, &arr);

    // Validate successful file load
    if (n <= 0) {
        printf("Missing input file - aborting.\n");
        return 1;
    }

    printf("Loaded %d integers.\n\n", n);

    int steps, index; // steps = probe count; index = search result

    // Test targets: includes values expected in range and one missing (9999)
    // Note: These assume the dataset resembles the earlier examples (-5000..5000).
    int tests[] = { -5000, 0, 5000, 1234, -2500, 9999 };
    int tcount = sizeof(tests) / sizeof(tests[0]);

    // Run exponential search for each target and print results
    for (int i = 0; i < tcount; i++) {
        int target = tests[i];
        index = exponential_search(arr, n, target, &steps);

        printf("Target %6d → index %6d, steps %6d\n", target, index, steps);
    }

    // Free dynamically allocated array before exiting
    free(arr);
    return 0;
}
