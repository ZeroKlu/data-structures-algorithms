#include <stdio.h>
#include <stdlib.h>

/* -------------------------------------------------------
   Binary Search helper (counts steps)
   Searches arr[left..right] inclusive.
   ------------------------------------------------------- */
int binary_search_steps(const int* arr, int left, int right, int target, int* steps) {
    while (left <= right) {
        (*steps)++;
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

/* -------------------------------------------------------
   Exponential Search (counts steps)
   ------------------------------------------------------- */
int exponential_search(const int* arr, int n, int target, int* steps) {
    *steps = 0;

    if (n == 0) return -1;

    (*steps)++; // check arr[0]
    if (arr[0] == target) return 0;

    // Find range for binary search
    int bound = 1;

    while (bound < n && arr[bound] < target) {
        (*steps)++;   // each range expansion
        bound *= 2;
    }

    int left = bound / 2;
    int right = (bound < n) ? bound : n - 1;

    // Binary search within [left, right]
    return binary_search_steps(arr, left, right, target, steps);
}

/* -------------------------------------------------------
   Load integer array from file
   ------------------------------------------------------- */
int load_file(const char* path, int** out_arr) {
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Error reading: %s\n", path);
        return -1;
    }

    int capacity = 11000;
    int* arr = malloc(capacity * sizeof(int));
    int count = 0;

    while (fscanf(f, "%d", &arr[count]) == 1) {
        count++;
    }

    fclose(f);
    *out_arr = arr;
    return count;
}

/* -------------------------------------------------------
   MAIN TESTS
   ------------------------------------------------------- */
int main(void) {
    const char* ordered_path = "..\\data\\ordered.txt";
    int* arr = NULL;

    printf("Attempting to read: %s\n", ordered_path);
    int n = load_file(ordered_path, &arr);

    if (n <= 0) {
        printf("Missing input file - aborting.\n");
        return 1;
    }

    printf("Loaded %d integers.\n\n", n);

    int steps, index;

    int tests[] = { -5000, 0, 5000, 1234, -2500, 9999 };
    int tcount = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < tcount; i++) {
        int target = tests[i];
        index = exponential_search(arr, n, target, &steps);

        printf("Target %6d → index %6d, steps %6d\n", target, index, steps);
    }

    free(arr);
    return 0;
}
