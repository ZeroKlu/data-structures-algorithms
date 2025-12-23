#include <stdio.h>   // Provides standard input/output functions (printf, fopen, fscanf)
#include <stdlib.h>  // Provides dynamic memory management (malloc, realloc, free)

// ==============================================
// Binary search (given)
// ==============================================

/**
 * Performs a standard binary search on a sorted integer array.
 *
 * @param arr     Pointer to the sorted array
 * @param n       Number of elements in the array
 * @param target  Value to search for
 *
 * @return Index of the target if found; otherwise -1
 */
int binary_search(const int* arr, int n, int target) {
    int lo = 0;        // Lower bound of the search range (inclusive)
    int hi = n - 1;    // Upper bound of the search range (inclusive)

    // Continue searching while the range is valid
    while (lo <= hi) {
        // Compute midpoint safely to avoid integer overflow
        int mid = lo + (hi - lo) / 2;

        // Check if the midpoint element matches the target
        if (arr[mid] == target) {
            return mid;  // Target found
        }
        // If target is larger, discard left half
        else if (arr[mid] < target) {
            lo = mid + 1;
        }
        // If target is smaller, discard right half
        else {
            hi = mid - 1;
        }
    }

    // Target was not found in the array
    return -1;
}

// Same search *with step counting*

/**
 * Performs a binary search while counting comparison steps.
 *
 * @param arr        Pointer to the sorted array
 * @param n          Number of elements in the array
 * @param target     Value to search for
 * @param steps_out  Output parameter to record the number of iterations performed
 *
 * @return Index of the target if found; otherwise -1
 */
int binary_search_steps(const int* arr, int n, int target, int* steps_out) {
    int steps = 0;     // Counts how many loop iterations occur
    int lo = 0;        // Lower bound of search range
    int hi = n - 1;    // Upper bound of search range

    // Continue searching while the range is valid
    while (lo <= hi) {
        steps++;  // Count this iteration / comparison step

        // Compute midpoint of current search range
        int mid = lo + (hi - lo) / 2;

        // Check for a match at the midpoint
        if (arr[mid] == target) {
            *steps_out = steps;  // Store total steps taken
            return mid;          // Target found
        }
        // Target lies in the right half
        else if (arr[mid] < target) {
            lo = mid + 1;
        }
        // Target lies in the left half
        else {
            hi = mid - 1;
        }
    }

    // Target was not found after exhausting search range
    *steps_out = steps;
    return -1;
}

// ==============================================
// Load integer file into array
// ==============================================

/**
 * Loads integers from a text file into a dynamically allocated array.
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
        // Handle file open failure
        printf("Error: cannot open %s\n", path);
        *out_n = 0;
        return NULL;
    }

    int capacity = 4096;  // Initial array capacity
    int* arr = malloc(sizeof(int) * capacity);  // Allocate memory
    int count = 0;        // Number of integers read
    int x;                // Temporary variable for fscanf input

    // Read integers until EOF or invalid input
    while (fscanf(f, "%d", &x) == 1) {
        // Grow array if capacity is reached
        if (count == capacity) {
            capacity *= 2;  // Double capacity for amortized efficiency
            arr = realloc(arr, sizeof(int) * capacity);
        }

        // Store the read integer
        arr[count++] = x;
    }

    // Close the file after reading completes
    fclose(f);

    // Output the number of integers read
    *out_n = count;

    // Return pointer to populated array
    return arr;
}

// ==============================================
// Run tests
// ==============================================

/**
 * Runs a set of binary search tests on a sorted dataset.
 *
 * @param arr  Pointer to the sorted array
 * @param n    Number of elements in the array
 */
void run_binary_tests(const int* arr, int n) {
    printf("=== Binary Search Tests on ordered.txt ===\n");
    printf("Loaded %d integers\n\n", n);

    int index;  // Stores index returned by search
    int steps;  // Stores number of steps taken

    // Test 1: Search for the first element (best-case scenario)
    index = binary_search_steps(arr, n, arr[0], &steps);
    printf("Search first (%d):  index=%d, steps=%d\n",
           arr[0], index, steps);

    // Test 2: Search for the middle element
    int midVal = arr[n / 2];
    index = binary_search_steps(arr, n, midVal, &steps);
    printf("Search middle (%d): index=%d, steps=%d\n",
           midVal, index, steps);

    // Test 3: Search for the last element (worst-case successful search)
    index = binary_search_steps(arr, n, arr[n - 1], &steps);
    printf("Search last (%d):   index=%d, steps=%d\n",
           arr[n - 1], index, steps);

    // Test 4: Search for a value guaranteed not to exist
    index = binary_search_steps(arr, n, 999999, &steps);
    printf("Search missing (999999): index=%d, steps=%d\n",
           index, steps);

    printf("\n");
}

// ==============================================
// MAIN
// ==============================================

/**
 * Program entry point.
 *
 * Loads an ordered dataset from disk, runs binary search tests,
 * and releases allocated memory before exiting.
 */
int main(void) {
    int n;  // Number of elements in the dataset

    // Load ordered dataset from file
    int* ordered = load_file("..\\data\\ordered.txt", &n);

    // Validate successful file load
    if (!ordered || n == 0) {
        printf("Failed to load ordered.txt\n");
        return 1;  // Exit with error code
    }

    // Run binary search performance tests
    run_binary_tests(ordered, n);

    // Free dynamically allocated memory
    free(ordered);

    return 0;  // Successful program termination
}
