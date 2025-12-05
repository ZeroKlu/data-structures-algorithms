#include <stdio.h>
#include <stdlib.h>

// ==============================================
// Binary search (given)
// ==============================================
int binary_search(const int* arr, int n, int target) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

// Same search *with step counting*
int binary_search_steps(const int* arr, int n, int target, int* steps_out) {
    int steps = 0;
    int lo = 0, hi = n - 1;

    while (lo <= hi) {
        steps++;
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] == target) {
            *steps_out = steps;
            return mid;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    *steps_out = steps;
    return -1;
}

// ==============================================
// Load integer file into array
// ==============================================
int* load_file(const char* path, int* out_n) {
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Error: cannot open %s\n", path);
        *out_n = 0;
        return NULL;
    }

    int capacity = 4096;
    int* arr = malloc(sizeof(int) * capacity);
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

// ==============================================
// Run tests
// ==============================================
void run_binary_tests(const int* arr, int n) {
    printf("=== Binary Search Tests on ordered.txt ===\n");
    printf("Loaded %d integers\n\n", n);

    int index, steps;

    // 1. first element
    index = binary_search_steps(arr, n, arr[0], &steps);
    printf("Search first (%d):  index=%d, steps=%d\n",
           arr[0], index, steps);

    // 2. middle element
    int midVal = arr[n / 2];
    index = binary_search_steps(arr, n, midVal, &steps);
    printf("Search middle (%d): index=%d, steps=%d\n",
           midVal, index, steps);

    // 3. last element
    index = binary_search_steps(arr, n, arr[n - 1], &steps);
    printf("Search last (%d):   index=%d, steps=%d\n",
           arr[n - 1], index, steps);

    // 4. missing value
    index = binary_search_steps(arr, n, 999999, &steps);
    printf("Search missing (999999): index=%d, steps=%d\n",
           index, steps);

    printf("\n");
}

// ==============================================
// MAIN
// ==============================================
int main(void) {
    int n;
    int* ordered = load_file("..\\data\\ordered.txt", &n);

    if (!ordered || n == 0) {
        printf("Failed to load ordered.txt\n");
        return 1;
    }

    run_binary_tests(ordered, n);

    free(ordered);
    return 0;
}
