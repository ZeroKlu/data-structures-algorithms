#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ------------------------------------------------------------
// Bubble Sort with Step Counting
// ------------------------------------------------------------
void bubble_sort(int* arr, int n, long long* comparisons, long long* swaps) {
    bool swapped = true;

    *comparisons = 0;
    *swaps = 0;

    while (swapped) {
        swapped = false;
        for (int i = 1; i < n; i++) {
            (*comparisons)++;  // Count comparison
            if (arr[i - 1] > arr[i]) {
                int tmp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = tmp;

                (*swaps)++;  // Count swap
                swapped = true;
            }
        }
        n--; // last element is in correct position
    }
}

// ------------------------------------------------------------
// Helper: Load integers from a file
// ------------------------------------------------------------
int load_file(const char* filename, int** outArr) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Error reading: %s\n", filename);
        return -1;
    }

    // Count numbers
    int count = 0, tmp;
    while (fscanf(f, "%d", &tmp) == 1) {
        count++;
    }

    rewind(f);

    int* arr = malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) {
        fscanf(f, "%d", &arr[i]);
    }

    fclose(f);
    *outArr = arr;
    return count;
}

// ------------------------------------------------------------
// Compare two arrays for equality
// ------------------------------------------------------------
int compare_arrays(int* a, int* b, int n) {
    int mismatches = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
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
int main(void) {
    const char* unordered_path = "../data/unordered.txt";
    const char* ordered_path   = "../data/ordered.txt";

    int *unordered = NULL, *ordered = NULL;

    printf("Attempting to read: %s\n", unordered_path);
    int n1 = load_file(unordered_path, &unordered);
    if (n1 <= 0) {
        printf("Missing unordered file - aborting.\n");
        return 1;
    }

    printf("Attempting to read: %s\n", ordered_path);
    int n2 = load_file(ordered_path, &ordered);
    if (n2 <= 0) {
        printf("Missing ordered file - aborting.\n");
        free(unordered);
        return 1;
    }

    if (n1 != n2) {
        printf("File length mismatch - aborting.\n");
        free(unordered);
        free(ordered);
        return 1;
    }

    printf("\n--- Bubble Sort Step Count ---\n");

    long long comparisons = 0, swaps = 0;

    bubble_sort(unordered, n1, &comparisons, &swaps);

    printf("Comparisons: %lld\n", comparisons);
    printf("Swaps:       %lld\n", swaps);

    printf("\nComparing result to ordered.txt...\n");

    int mismatches = compare_arrays(unordered, ordered, n1);

    if (mismatches == 0)
        printf("SUCCESS: Output matches ordered.txt\n");
    else
        printf("FAIL: %d mismatches found.\n", mismatches);

    free(unordered);
    free(ordered);

    return 0;
}
