#include <stdio.h>
#include <stdlib.h>

int linear_search(const int* arr, int n, int target, int* steps_out) {
    int steps = 0;
    for (int i = 0; i < n; i++) {
        steps++;                    // count the comparison arr[i] == target
        if (arr[i] == target) {
            *steps_out = steps;
            return i;
        }
    }
    *steps_out = steps;
    return -1;
}

int* load_file(const char* path, int* out_n) {
    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Error: cannot open %s\n", path);
        *out_n = 0;
        return NULL;
    }

    int capacity = 2048;
    int* arr = malloc(sizeof(int) * capacity);
    int count = 0, x;

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

void run_search_tests(const char* name, const int* arr, int n) {
    printf("=== %s ===\n", name);
    printf("Loaded %d integers.\n\n", n);

    int index, steps;

    // Test 1: First element
    index = linear_search(arr, n, arr[0], &steps);
    printf("Search for first element (%d): index=%d, steps=%d\n",
           arr[0], index, steps);

    // Test 2: Middle element
    index = linear_search(arr, n, arr[n/2], &steps);
    printf("Search for middle element (%d): index=%d, steps=%d\n",
           arr[n/2], index, steps);

    // Test 3: Last element
    index = linear_search(arr, n, arr[n-1], &steps);
    printf("Search for last element (%d): index=%d, steps=%d\n",
           arr[n-1], index, steps);

    // Test 4: Not found case
    index = linear_search(arr, n, 999999, &steps);
    printf("Search for missing element (999999): index=%d, steps=%d\n",
           index, steps);

    printf("\n");
}

int main(void) {
    int n1, n2;

    int* ordered   = load_file("..\\data\\ordered.txt", &n1);
    if (!ordered) return 1;

    int* unordered = load_file("..\\data\\unordered.txt", &n2);
    if (!unordered) {
        free(ordered);
        return 1;
    }

    run_search_tests("Ordered Data", ordered, n1);
    run_search_tests("Unordered Data", unordered, n2);

    free(ordered);
    free(unordered);

    return 0;
}
