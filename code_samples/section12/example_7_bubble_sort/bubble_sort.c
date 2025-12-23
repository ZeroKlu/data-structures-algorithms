#include <stdio.h>   // printf, fopen, fscanf
#include <stdlib.h>  // malloc, free
#include <stdbool.h> // bool, true, false
#include <string.h>  // (not used heavily here, but included)

// ------------------------------------------------------------
// Bubble Sort with Step Counting
// ------------------------------------------------------------
/*
    Bubble sort repeatedly passes through the array, swapping adjacent
    out-of-order elements. After each full pass, the largest remaining
    element "bubbles" to the end of the unsorted portion.

    This implementation includes an optimization:
      - If a full pass makes NO swaps, the array is already sorted and
        we can stop early.

    Step counting:
      - *comparisons counts how many adjacent comparisons we perform:
            arr[i-1] > arr[i]
      - *swaps counts how many swaps we actually execute

    Parameters:
      - arr: array to sort (modified in place)
      - n: number of elements
      - comparisons: output counter for comparisons
      - swaps: output counter for swaps
*/
void bubble_sort(int* arr, int n, long long* comparisons, long long* swaps) {
    bool swapped = true;  // controls whether we need another pass

    // Initialize counters
    *comparisons = 0;
    *swaps = 0;

    // Continue making passes while we keep swapping something
    while (swapped) {
        swapped = false;  // assume no swaps this pass unless we perform one

        // One pass compares adjacent pairs from index 1..n-1
        for (int i = 1; i < n; i++) {
            (*comparisons)++;  // Count comparison of one adjacent pair

            // If elements are out of order, swap them
            if (arr[i - 1] > arr[i]) {
                int tmp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = tmp;

                (*swaps)++;    // Count swap
                swapped = true; // Signal that we need another pass
            }
        }

        // After each pass, the last element of the current range is in its
        // final sorted position, so we can reduce the range by 1.
        n--; // last element is in correct position
    }
}

// ------------------------------------------------------------
// Helper: Load integers from a file
// ------------------------------------------------------------
/*
    Loads a whitespace-delimited list of integers from a file.

    Strategy:
      1) Count how many integers are in the file.
      2) Allocate an array of exactly that size.
      3) Rewind and read the integers into the array.

    Parameters:
      - filename: path to the file to read
      - outArr: output pointer that receives the allocated array

    Returns:
      - number of integers read on success
      - -1 on failure

    Caller responsibility:
      - Must free(*outArr) when done.
*/
int load_file(const char* filename, int** outArr) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Error reading: %s\n", filename);
        return -1;
    }

    // -----------------------------
    // First pass: count numbers
    // -----------------------------
    int count = 0, tmp;
    while (fscanf(f, "%d", &tmp) == 1) {
        count++;
    }

    // Return to start of file for second pass
    rewind(f);

    // Allocate enough space for all integers
    int* arr = malloc(count * sizeof(int));

    // Second pass: read values into the allocated array
    for (int i = 0; i < count; i++) {
        fscanf(f, "%d", &arr[i]);
    }

    fclose(f);

    // Provide the allocated array to the caller
    *outArr = arr;
    return count;
}

// ------------------------------------------------------------
// Compare two arrays for equality
// ------------------------------------------------------------
/*
    Compares two arrays element-by-element and counts mismatches.

    Also prints up to the first 10 mismatches for debugging so output
    doesn't become overwhelming.

    Parameters:
      - a: first array
      - b: second array (reference / expected)
      - n: number of elements to compare

    Returns:
      - number of mismatched positions found
*/
int compare_arrays(int* a, int* b, int n) {
    int mismatches = 0;

    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            // Print only a limited number of mismatches
            if (mismatches < 10)
                printf("Mismatch at %d: got %d, expected %d\n", i, a[i], b[i]);
            mismatches++;
        }
    }

    return mismatches;
}

// ------------------------------------------------------------
// Main test driver
// ------------------------------------------------------------
/*
    Test workflow:
      1) Load unordered.txt (unsorted input data)
      2) Load ordered.txt   (reference sorted output)
      3) Run bubble sort on unordered data, counting comparisons & swaps
      4) Compare the sorted result against the reference
      5) Print statistics and success/failure summary

    Notes on paths:
      - Uses relative paths like "../data/unordered.txt"
      - These paths depend on where the program is run from.
*/
int main(void) {
    const char* unordered_path = "../data/unordered.txt";
    const char* ordered_path   = "../data/ordered.txt";

    int *unordered = NULL, *ordered = NULL;

    // -----------------------------
    // Load unordered input
    // -----------------------------
    printf("Attempting to read: %s\n", unordered_path);
    int n1 = load_file(unordered_path, &unordered);
    if (n1 <= 0) {
        printf("Missing unordered file - aborting.\n");
        return 1;
    }

    // -----------------------------
    // Load ordered reference output
    // -----------------------------
    printf("Attempting to read: %s\n", ordered_path);
    int n2 = load_file(ordered_path, &ordered);
    if (n2 <= 0) {
        printf("Missing ordered file - aborting.\n");
        free(unordered);
        return 1;
    }

    // Sanity check: both files should have the same number of values
    if (n1 != n2) {
        printf("File length mismatch - aborting.\n");
        free(unordered);
        free(ordered);
        return 1;
    }

    printf("\n--- Bubble Sort Step Count ---\n");

    // Counters for algorithm work performed
    long long comparisons = 0, swaps = 0;

    // Sort the unordered data in place
    bubble_sort(unordered, n1, &comparisons, &swaps);

    // Report step counts
    printf("Comparisons: %lld\n", comparisons);
    printf("Swaps:       %lld\n", swaps);

    // -----------------------------
    // Verify correctness
    // -----------------------------
    printf("\nComparing result to ordered.txt...\n");

    int mismatches = compare_arrays(unordered, ordered, n1);

    if (mismatches == 0)
        printf("SUCCESS: Output matches ordered.txt\n");
    else
        printf("FAIL: %d mismatches found.\n", mismatches);

    // Clean up allocated memory
    free(unordered);
    free(ordered);

    return 0;
}
