#include <stdio.h>   // Provides input/output functions like printf, fopen, fscanf
#include <stdlib.h>  // Provides memory management (malloc, realloc, free) and exit codes

/**
 * Performs a linear search on an integer array.
 *
 * @param arr        Pointer to the array to search
 * @param n          Number of elements in the array
 * @param target     Value to search for
 * @param steps_out  Output parameter that records how many comparisons were made
 *
 * @return Index of the target if found, otherwise -1
 */
int linear_search(const int* arr, int n, int target, int* steps_out) {
    int steps = 0;  // Counts how many element comparisons are performed

    // Iterate through each element in the array sequentially
    for (int i = 0; i < n; i++) {
        steps++;                    // Count the comparison arr[i] == target

        // Check if the current element matches the target
        if (arr[i] == target) {
            *steps_out = steps;     // Output total number of steps taken
            return i;               // Return index where target was found
        }
    }

    // If loop completes, target was not found
    *steps_out = steps;             // Total comparisons equals array length
    return -1;                      // Signal failure to find target
}

/**
 * Loads integers from a text file into a dynamically allocated array.
 *
 * @param path   Path to the input file
 * @param out_n  Output parameter to store number of integers read
 *
 * @return Pointer to allocated array containing file data, or NULL on failure
 */
int* load_file(const char* path, int* out_n) {
    // Attempt to open the file in read mode
    FILE* f = fopen(path, "r");
    if (!f) {
        // Handle file open failure
        printf("Error: cannot open %s\n", path);
        *out_n = 0;
        return NULL;
    }

    // Initial capacity for the dynamic array
    int capacity = 2048;

    // Allocate memory for the integer array
    int* arr = malloc(sizeof(int) * capacity);

    int count = 0;  // Number of integers successfully read
    int x;          // Temporary variable for fscanf input

    // Read integers until EOF or invalid input
    while (fscanf(f, "%d", &x) == 1) {

        // Resize array if capacity is reached
        if (count == capacity) {
            capacity *= 2;  // Double the capacity for amortized efficiency
            arr = realloc(arr, sizeof(int) * capacity);
        }

        // Store the read integer in the array
        arr[count++] = x;
    }

    // Close the file after reading is complete
    fclose(f);

    // Output the total number of elements read
    *out_n = count;

    // Return pointer to the populated array
    return arr;
}

/**
 * Runs a series of linear search tests on a given dataset.
 *
 * @param name  Descriptive name of the dataset (for output labeling)
 * @param arr   Pointer to the data array
 * @param n     Number of elements in the array
 */
void run_search_tests(const char* name, const int* arr, int n) {
    printf("=== %s ===\n", name);
    printf("Loaded %d integers.\n\n", n);

    int index;  // Stores search result index
    int steps;  // Stores number of comparisons made

    // Test 1: Search for the first element (best-case scenario)
    index = linear_search(arr, n, arr[0], &steps);
    printf("Search for first element (%d): index=%d, steps=%d\n",
           arr[0], index, steps);

    // Test 2: Search for the middle element (average-case scenario)
    index = linear_search(arr, n, arr[n / 2], &steps);
    printf("Search for middle element (%d): index=%d, steps=%d\n",
           arr[n / 2], index, steps);

    // Test 3: Search for the last element (worst-case successful search)
    index = linear_search(arr, n, arr[n - 1], &steps);
    printf("Search for last element (%d): index=%d, steps=%d\n",
           arr[n - 1], index, steps);

    // Test 4: Search for a value guaranteed not to exist
    index = linear_search(arr, n, 999999, &steps);
    printf("Search for missing element (999999): index=%d, steps=%d\n",
           index, steps);

    printf("\n");
}

/**
 * Program entry point.
 *
 * Loads ordered and unordered datasets, runs linear search tests on each,
 * and frees allocated memory before exiting.
 */
int main(void) {
    int n1, n2;  // Store sizes of the two datasets

    // Load ordered dataset from file
    int* ordered = load_file("..\\data\\ordered.txt", &n1);
    if (!ordered) return 1;  // Exit if file load failed

    // Load unordered dataset from file
    int* unordered = load_file("..\\data\\unordered.txt", &n2);
    if (!unordered) {
        free(ordered);       // Prevent memory leak
        return 1;
    }

    // Run linear search performance tests on both datasets
    run_search_tests("Ordered Data", ordered, n1);
    run_search_tests("Unordered Data", unordered, n2);

    // Free dynamically allocated memory
    free(ordered);
    free(unordered);

    return 0;  // Successful program termination
}
