#include <stdio.h>
#include <stdlib.h>

// ============================================================
// Interpolation Search (index-only version)
// Requires: arr sorted in ascending order
// ============================================================
int interpolation_search(const int *arr, int n, int target) {
    if (n <= 0) return -1;

    int lo = 0;
    int hi = n - 1;

    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {
        if (arr[hi] == arr[lo]) {
            // All values in [lo, hi] are the same
            if (arr[lo] == target) return lo;
            else return -1;
        }

        // Estimate position
        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                             (arr[hi] - arr[lo]));

        if (pos < lo || pos > hi) {
            // Shouldn't happen mathematically, but guard anyway
            return -1;
        }

        if (arr[pos] == target) {
            return pos;
        } else if (arr[pos] < target) {
            lo = pos + 1;
        } else {
            hi = pos - 1;
        }
    }

    return -1;
}

// ============================================================
// Interpolation Search with step counting
// steps_out = number of comparisons against arr[pos]/arr[lo]
// ============================================================
int interpolation_search_steps(const int *arr, int n, int target, int *steps_out) {
    int steps = 0;
    if (n <= 0) {
        *steps_out = 0;
        return -1;
    }

    int lo = 0;
    int hi = n - 1;

    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {
        // If all elements in range are equal
        steps++; // compare arr[hi] == arr[lo]
        if (arr[hi] == arr[lo]) {
            steps++; // compare arr[lo] == target
            if (arr[lo] == target) {
                *steps_out = steps;
                return lo;
            } else {
                *steps_out = steps;
                return -1;
            }
        }

        int pos = lo + (int)((double)(hi - lo) * (target - arr[lo]) /
                             (arr[hi] - arr[lo]));

        if (pos < lo || pos > hi) {
            *steps_out = steps;
            return -1;
        }

        steps++; // compare arr[pos] == target
        if (arr[pos] == target) {
            *steps_out = steps;
            return pos;
        }

        steps++; // compare arr[pos] < target
        if (arr[pos] < target) {
            lo = pos + 1;
        } else {
            hi = pos - 1;
        }
    }

    *steps_out = steps;
    return -1;
}

// ============================================================
// Load ordered.txt into a dynamic array
// ============================================================
int* load_file(const char* path, int* out_n) {
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Error reading: %s\n", path);
        *out_n = 0;
        return NULL;
    }

    int capacity = 4096;
    int *arr = (int*)malloc(sizeof(int) * capacity);
    int count = 0;
    int x;

    while (fscanf(f, "%d", &x) == 1) {
        if (count == capacity) {
            capacity *= 2;
            int *tmp = (int*)realloc(arr, sizeof(int) * capacity);
            if (!tmp) {
                free(arr);
                fclose(f);
                *out_n = 0;
                return NULL;
            }
            arr = tmp;
        }
        arr[count++] = x;
    }

    fclose(f);
    *out_n = count;
    return arr;
}

// ============================================================
// Run tests: first, middle, last, missing
// ============================================================
void run_tests(const int *arr, int n) {
    printf("=== Interpolation Search Tests (ordered data only) ===\n");
    printf("Loaded %d integers\n\n", n);

    int steps, idx;

    // First element
    idx = interpolation_search_steps(arr, n, arr[0], &steps);
    printf("Search first  (%d): index=%d, steps=%d\n", arr[0], idx, steps);

    // Middle element
    int midVal = arr[n / 2];
    idx = interpolation_search_steps(arr, n, midVal, &steps);
    printf("Search middle (%d): index=%d, steps=%d\n", midVal, idx, steps);

    // Last element
    idx = interpolation_search_steps(arr, n, arr[n - 1], &steps);
    printf("Search last   (%d): index=%d, steps=%d\n", arr[n - 1], idx, steps);

    // Missing element
    idx = interpolation_search_steps(arr, n, 999999, &steps);
    printf("Search missing (999999): index=%d, steps=%d\n", idx, steps);

    printf("\n");
}

// ============================================================
// MAIN
// ============================================================
int main(void) {
    int n = 0;

    // Adjust path as needed relative to where you build/run this executable.
    // If you run from code_samples/section12/lesson_3_interpolation_search:
    //   "..\\data\\ordered.txt" goes to code_samples/section12/data/ordered.txt
    const char *path = "..\\data\\ordered.txt";

    int *arr = load_file(path, &n);
    if (!arr || n == 0) {
        printf("Missing input file-aborting.\n");
        return 1;
    }

    run_tests(arr, n);

    free(arr);
    return 0;
}
