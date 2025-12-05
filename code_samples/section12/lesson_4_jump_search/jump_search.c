#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// ============================================================
// Jump Search (returns index only)
// ============================================================
int jump_search(const int *arr, int n, int target) {
    if (n <= 0) return -1;

    int step = (int)sqrt(n);
    int prev = 0;
    int curr = step;

    // Jump in steps until arr[curr] >= target or out of array
    while (curr < n && arr[curr] < target) {
        prev = curr;
        curr += step;
    }

    // Linear search in the found block
    if (curr >= n) curr = n - 1;

    for (int i = prev; i <= curr; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

// ============================================================
// Jump Search WITH step counting
// steps = number of comparisons
// ============================================================
int jump_search_steps(const int *arr, int n, int target, int *steps_out) {
    int steps = 0;

    if (n <= 0) {
        *steps_out = steps;
        return -1;
    }

    int step = (int)sqrt(n);
    int prev = 0;
    int curr = step;

    // Jump phase
    while (curr < n) {
        steps++; // compare arr[curr] < target

        if (arr[curr] >= target)
            break;

        prev = curr;
        curr += step;
    }

    if (curr >= n) curr = n - 1;

    // Linear search phase
    for (int i = prev; i <= curr; i++) {
        steps++; // compare arr[i] == target
        if (arr[i] == target) {
            *steps_out = steps;
            return i;
        }
    }

    *steps_out = steps;
    return -1;
}

// ============================================================
// Load file into array
// ============================================================
int* load_file(const char* path, int* out_n) {
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Error reading: %s\n", path);
        *out_n = 0;
        return NULL;
    }

    int capacity = 4096;
    int *arr = malloc(sizeof(int) * capacity);
    int count = 0;
    int x;

    while (fscanf(f, "%d", &x) == 1) {
        if (count == capacity) {
            capacity *= 2;
            arr = realloc(arr, sizeof(int) * capacity);
        }
        arr[count++] = x;
    }

    fclose(f);
    *out_n = count;
    return arr;
}

// ============================================================
// Test runner
// ============================================================
void run_tests(int *arr, int n) {
    printf("=== Jump Search Tests ===\n");
    printf("Loaded %d integers\n\n", n);

    int idx, steps;

    // First
    idx = jump_search_steps(arr, n, arr[0], &steps);
    printf("Search first (%d): index=%d, steps=%d\n", arr[0], idx, steps);

    // Middle
    int mid = arr[n/2];
    idx = jump_search_steps(arr, n, mid, &steps);
    printf("Search middle (%d): index=%d, steps=%d\n", mid, idx, steps);

    // Last
    idx = jump_search_steps(arr, n, arr[n-1], &steps);
    printf("Search last (%d): index=%d, steps=%d\n", arr[n-1], idx, steps);

    // Missing
    idx = jump_search_steps(arr, n, 999999, &steps);
    printf("Search missing (999999): index=%d, steps=%d\n", idx, steps);

    printf("\n");
}

// ============================================================
// MAIN
// ============================================================
int main(void) {
    int n;
    int *arr = load_file("..\\data\\ordered.txt", &n);

    if (!arr || n == 0) {
        printf("Missing input file—aborting.\n");
        return 1;
    }

    run_tests(arr, n);

    free(arr);
    return 0;
}
