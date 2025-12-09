#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------
// GLOBAL COUNTERS
// ------------------------------------------------------------
static long long comparisons = 0;
static long long writes = 0;

// ------------------------------------------------------------
// Quick Sort (Lomuto partition)
// ------------------------------------------------------------
static int partition(int *arr, int left, int right) {
    int pivot = arr[right];
    int i = left;

    for (int j = left; j < right; j++) {
        comparisons++; // compare arr[j] <= pivot
        if (arr[j] <= pivot) {
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            writes += 3;
            i++;
        }
    }

    int tmp = arr[i];
    arr[i] = arr[right];
    arr[right] = tmp;
    writes += 3;

    return i;
}

static void quick_sort_rec(int *arr, int left, int right) {
    if (left < right) {
        int p = partition(arr, left, right);
        quick_sort_rec(arr, left, p - 1);
        quick_sort_rec(arr, p + 1, right);
    }
}

void quick_sort(int *arr, int n) {
    comparisons = 0;
    writes = 0;
    if (n > 0) {
        quick_sort_rec(arr, 0, n - 1);
    }
}

// ------------------------------------------------------------
// FILE LOADER  (FIXED)
// ------------------------------------------------------------
// Tries a few reasonable relative paths for the given filename.
// Example calls:
//   load_file("unordered.txt", &n);
//   load_file("ordered.txt", &n);
static int *load_file(const char *filename, int *out_size) {
    const char *prefixes[] = {
        "",             // unordered.txt, ordered.txt in CWD
        "data/",        // ./data/unordered.txt
        "../data/",     // ../data/unordered.txt   (your usual layout)
        "../../data/",  // ../../data/unordered.txt (if run deeper)
        NULL
    };

    FILE *f = NULL;
    char fullpath[512];

    for (int i = 0; prefixes[i] != NULL; i++) {
        snprintf(fullpath, sizeof(fullpath), "%s%s", prefixes[i], filename);
        f = fopen(fullpath, "r");
        if (f) {
            printf("Loaded: %s\n", fullpath);
            break;
        }
    }

    if (!f) {
        printf("Error reading: %s\n", filename);
        printf("Search paths attempted:\n");
        for (int i = 0; prefixes[i] != NULL; i++) {
            snprintf(fullpath, sizeof(fullpath), "%s%s", prefixes[i], filename);
            printf("  %s\n", fullpath);
        }
        printf("Missing input file — aborting.\n");
        exit(1);
    }

    int capacity = 1024;
    int *arr = malloc(capacity * sizeof(int));
    if (!arr) {
        perror("malloc");
        exit(1);
    }

    int size = 0;
    while (fscanf(f, "%d", &arr[size]) == 1) {
        size++;
        if (size >= capacity) {
            capacity *= 2;
            int *tmp = realloc(arr, capacity * sizeof(int));
            if (!tmp) {
                perror("realloc");
                free(arr);
                exit(1);
            }
            arr = tmp;
        }
    }
    fclose(f);

    *out_size = size;
    return arr;
}

// ------------------------------------------------------------
// MAIN TEST
// ------------------------------------------------------------
int main(void) {
    int n1, n2;
    int *unordered = load_file("unordered.txt", &n1);
    int *expected  = load_file("ordered.txt", &n2);

    if (n1 != n2) {
        printf("Mismatch in file lengths (%d vs %d) — aborting.\n", n1, n2);
        free(unordered);
        free(expected);
        exit(1);
    }

    int *arr = malloc(n1 * sizeof(int));
    if (!arr) {
        perror("malloc");
        free(unordered);
        free(expected);
        exit(1);
    }
    memcpy(arr, unordered, n1 * sizeof(int));

    quick_sort(arr, n1);

    // Validate
    int ok = 1;
    for (int i = 0; i < n1; i++) {
        if (arr[i] != expected[i]) {
            ok = 0;
            printf("Mismatch at index %d: got %d, expected %d\n",
                   i, arr[i], expected[i]);
            break;
        }
    }

    printf("\nQuick Sort (C)\n");
    printf("--------------\n");
    printf("Elements:     %d\n", n1);
    printf("Comparisons:  %lld\n", comparisons);
    printf("Writes:       %lld\n", writes);
    printf("Correct?      %s\n", ok ? "YES ✔" : "NO ✘");

    if (ok) {
        printf("\nFirst 10 sorted values:\n");
        for (int i = 0; i < 10 && i < n1; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    free(unordered);
    free(expected);
    free(arr);
    return 0;
}
