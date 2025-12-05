#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==============================
// File Loading Helper
// ==============================

static int* load_file(const char* path, int* out_n) {
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Error reading: %s\n", path);
        return NULL;
    }

    int count = 0, x;
    while (fscanf(f, "%d", &x) == 1) count++;

    rewind(f);

    int* arr = malloc(count * sizeof(int));
    if (!arr) {
        fclose(f);
        return NULL;
    }

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

typedef struct {
    long long comparisons;
    long long swaps;
} SortStats;

void selection_sort(int* arr, int n, SortStats* stats) {
    stats->comparisons = 0;
    stats->swaps = 0;

    for (int i = 0; i < n; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            stats->comparisons++;  // counting comparison
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
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

int main(void) {
    const char* ordered_path   = "..\\data\\ordered.txt";
    const char* unordered_path = "..\\data\\unordered.txt";

    int n_unordered = 0, n_ordered = 0;

    printf("Attempting to read: %s\n", unordered_path);
    int* unordered = load_file(unordered_path, &n_unordered);
    if (!unordered) {
        printf("Missing unordered input file - aborting.\n");
        return 1;
    }

    printf("Attempting to read: %s\n", ordered_path);
    int* ordered = load_file(ordered_path, &n_ordered);
    if (!ordered) {
        printf("Missing ordered input file - aborting.\n");
        free(unordered);
        return 1;
    }

    if (n_unordered != n_ordered) {
        printf("File lengths differ! unordered=%d ordered=%d\n",
               n_unordered, n_ordered);
        free(unordered);
        free(ordered);
        return 1;
    }

    printf("\n--- Selection Sort (Step Counting) ---\n");
    SortStats stats;
    selection_sort(unordered, n_unordered, &stats);

    printf("Comparisons: %lld\n", stats.comparisons);
    printf("Swaps:       %lld\n", stats.swaps);

    printf("\nComparing sorted result with ordered.txt...\n");
    int mismatches = 0;
    for (int i = 0; i < n_unordered; i++) {
        if (unordered[i] != ordered[i]) {
            if (mismatches < 10) {
                printf("Mismatch at index %d: got %d expected %d\n",
                       i, unordered[i], ordered[i]);
            }
            mismatches++;
        }
    }

    if (mismatches == 0) {
        printf("SUCCESS: Sorted output exactly matches ordered.txt.\n");
    } else {
        printf("FAIL: %d mismatches detected (first 10 shown).\n", mismatches);
    }

    free(unordered);
    free(ordered);
    return 0;
}
