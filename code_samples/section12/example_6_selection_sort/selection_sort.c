#include <stdio.h>   // Standard I/O (printf, fopen, fscanf)
#include <stdlib.h>  // Memory allocation (malloc, free)
#include <string.h>  // String utilities (not heavily used here)

// ==============================
// File Loading Helper
// ==============================

/**
 * Loads a whitespace-separated list of integers from a file
 * into a dynamically allocated array.
 *
 * Implementation strategy:
 *   1) First pass: count how many integers are in the file.
 *   2) Allocate exactly enough memory for that many integers.
 *   3) Second pass: read the integers into the array.
 *
 * @param path   Path to the input file
 * @param out_n  Output parameter that receives the number of integers loaded
 *
 * @return Pointer to the allocated integer array on success,
 *         or NULL on failure.
 *
 * Note:
 *   - The caller is responsible for freeing the returned array.
 */
static int* load_file(const char* path, int* out_n) {
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Error reading: %s\n", path);
        return NULL;
    }

    // --------------------------------------------------
    // First pass: count integers
    // --------------------------------------------------
    int count = 0, x;
    while (fscanf(f, "%d", &x) == 1) count++;

    // Rewind file pointer to beginning for second pass
    rewind(f);

    // Allocate exactly enough memory for all integers
    int* arr = malloc(count * sizeof(int));
    if (!arr) {
        fclose(f);
        return NULL;
    }

    // --------------------------------------------------
    // Second pass: read integers into array
    // --------------------------------------------------
    for (int i = 0; i < count; i++) {
        fscanf(f, "%d", &arr[i]);
    }

    fclose(f);
    *out_n = count;
    return arr;
}

// ==============================
// Selection Sort with Step Count
// ==============================

/**
 * Structure used to collect statistics about the sorting process.
 *
 * comparisons:
 *   Number of element-to-element comparisons performed.
 *
 * swaps:
 *   Number of swaps actually executed.
 */
typedef struct {
    long long comparisons;
    long long swaps;
} SortStats;

/**
 * Performs selection sort on an integer array while collecting
 * step-count statistics.
 *
 * Selection sort algorithm:
 *   - Repeatedly select the minimum element from the unsorted portion
 *     of the array.
 *   - Swap it into its correct position.
 *
 * Step counting:
 *   - comparisons: incremented every time arr[j] < arr[min_idx] is evaluated
 *   - swaps: incremented only when a swap actually occurs
 *
 * @param arr    Array of integers to sort (in place)
 * @param n      Number of elements in the array
 * @param stats  Pointer to a SortStats structure to record counts
 */
void selection_sort(int* arr, int n, SortStats* stats) {
    // Initialize statistics counters
    stats->comparisons = 0;
    stats->swaps = 0;

    // Outer loop selects position i to place the minimum element
    for (int i = 0; i < n; i++) {
        int min_idx = i;

        // Inner loop finds the index of the smallest element
        // in the unsorted portion [i+1, n-1]
        for (int j = i + 1; j < n; j++) {
            stats->comparisons++;  // counting comparison
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }

        // Swap the minimum element into position i (if needed)
        if (min_idx != i) {
            int tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
            stats->swaps++;  // counting swap
        }
    }
}

// ==============================
// Main Test Driver
// ==============================

/**
 * Program entry point.
 *
 * Workflow:
 *   1) Load unordered.txt (unsorted input).
 *   2) Load ordered.txt (correct sorted reference).
 *   3) Run selection sort on the unordered data.
 *   4) Report comparison and swap counts.
 *   5) Verify the sorted result matches ordered.txt exactly.
 */
int main(void) {
    const char* ordered_path   = "..\\data\\ordered.txt";
    const char* unordered_path = "..\\data\\unordered.txt";

    int n_unordered = 0, n_ordered = 0;

    // --------------------------------------------------
    // Load unordered input data
    // --------------------------------------------------
    printf("Attempting to read: %s\n", unordered_path);
    int* unordered = load_file(unordered_path, &n_unordered);
    if (!unordered) {
        printf("Missing unordered input file - aborting.\n");
        return 1;
    }

    // --------------------------------------------------
    // Load ordered reference data
    // --------------------------------------------------
    printf("Attempting to read: %s\n", ordered_path);
    int* ordered = load_file(ordered_path, &n_ordered);
    if (!ordered) {
        printf("Missing ordered input file - aborting.\n");
        free(unordered);
        return 1;
    }

    // Ensure both files contain the same number of elements
    if (n_unordered != n_ordered) {
        printf("File lengths differ! unordered=%d ordered=%d\n",
               n_unordered, n_ordered);
        free(unordered);
        free(ordered);
        return 1;
    }

    // --------------------------------------------------
    // Run selection sort and report statistics
    // --------------------------------------------------
    printf("\n--- Selection Sort (Step Counting) ---\n");
    SortStats stats;
    selection_sort(unordered, n_unordered, &stats);

    printf("Comparisons: %lld\n", stats.comparisons);
    printf("Swaps:       %lld\n", stats.swaps);

    // --------------------------------------------------
    // Verify sorted output against ordered.txt
    // --------------------------------------------------
    printf("\nComparing sorted result with ordered.txt...\n");
    int mismatches = 0;

    for (int i = 0; i < n_unordered; i++) {
        if (unordered[i] != ordered[i]) {
            // Print only the first few mismatches to avoid flooding output
            if (mismatches < 10) {
                printf("Mismatch at index %d: got %d expected %d\n",
                       i, unordered[i], ordered[i]);
            }
            mismatches++;
        }
    }

    // Report verification result
    if (mismatches == 0) {
        printf("SUCCESS: Sorted output exactly matches ordered.txt.\n");
    } else {
        printf("FAIL: %d mismatches detected (first 10 shown).\n", mismatches);
    }

    // Clean up allocated memory
    free(unordered);
    free(ordered);
    return 0;
}
