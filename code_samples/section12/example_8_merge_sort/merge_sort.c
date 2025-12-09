#include <stdio.h>
#include <stdlib.h>

/* ----------------------------------------------------------
   Step counter for illustrating time complexity
   ---------------------------------------------------------- */
typedef struct {
    long long comparisons;
    long long writes;
} Stats;

// static void merge(int* arr, int* tmp, int left, int mid, int right) {
//     int i = left, j = mid, k = left;
//     while (i < mid && j < right) {
//         if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
//         else                  tmp[k++] = arr[j++];
//     }
//     while (i < mid)  tmp[k++] = arr[i++];
//     while (j < right) tmp[k++] = arr[j++];
//     for (int p = left; p < right; p++) {
//         arr[p] = tmp[p];
//     }
// }

// static void merge_sort_rec(int* arr, int* tmp, int left, int right) {
//     if (right - left <= 1) return;
//     int mid = left + (right - left) / 2;
//     merge_sort_rec(arr, tmp, left, mid);
//     merge_sort_rec(arr, tmp, mid, right);
//     merge(arr, tmp, left, mid, right);
// }

// void merge_sort(int* arr, int n) {
//     int* tmp = (int*)malloc(sizeof(int) * n);
//     merge_sort_rec(arr, tmp, 0, n);
//     free(tmp);
// }

/* ----------------------------------------------------------
   Merge sort implementation with step counting
   ---------------------------------------------------------- */

static void merge(int* arr, int* tmp, int left, int mid, int right, Stats* stats) {
    int i = left, j = mid, k = left;

    while (i < mid && j < right) {
        stats->comparisons++;              // compare arr[i] vs arr[j]
        if (arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];
        } else {
            tmp[k++] = arr[j++];
        }
        stats->writes++;                   // wrote into tmp[k-1]
    }

    while (i < mid) {
        tmp[k++] = arr[i++];
        stats->writes++;
    }
    while (j < right) {
        tmp[k++] = arr[j++];
        stats->writes++;
    }

    for (int p = left; p < right; p++) {
        arr[p] = tmp[p];
        stats->writes++;
    }
}

static void merge_sort_rec(int* arr, int* tmp, int left, int right, Stats* stats) {
    if (right - left <= 1) return;
    int mid = left + (right - left) / 2;
    merge_sort_rec(arr, tmp, left, mid, stats);
    merge_sort_rec(arr, tmp, mid, right, stats);
    merge(arr, tmp, left, mid, right, stats);
}

void merge_sort(int* arr, int n, Stats* stats) {
    int* tmp = (int*)malloc(sizeof(int) * n);
    if (!tmp) {
        fprintf(stderr, "Failed to allocate temp buffer for merge sort\n");
        exit(1);
    }
    merge_sort_rec(arr, tmp, 0, n, stats);
    free(tmp);
}

/* ----------------------------------------------------------
   Utility: load integers from a file into a dynamic array
   ---------------------------------------------------------- */

int* load_file(const char* path, int* out_n) {
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Error reading: %s\n", path);
        return NULL;
    }

    int capacity = 1024;
    int* arr = (int*)malloc(capacity * sizeof(int));
    if (!arr) {
        fclose(f);
        fprintf(stderr, "Memory allocation failure\n");
        return NULL;
    }

    int n = 0;
    int x;
    while (fscanf(f, "%d", &x) == 1) {
        if (n >= capacity) {
            capacity *= 2;
            int* new_arr = (int*)realloc(arr, capacity * sizeof(int));
            if (!new_arr) {
                fclose(f);
                free(arr);
                fprintf(stderr, "Memory reallocation failure\n");
                return NULL;
            }
            arr = new_arr;
        }
        arr[n++] = x;
    }

    fclose(f);
    *out_n = n;
    return arr;
}

/* ----------------------------------------------------------
   Compare two arrays and count mismatches
   ---------------------------------------------------------- */

int compare_arrays(const int* a, const int* b, int n) {
    int mismatches = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            if (mismatches < 10) {
                printf("Mismatch at index %d: got %d, expected %d\n",
                       i, a[i], b[i]);
            }
            mismatches++;
        }
    }
    return mismatches;
}

/* ----------------------------------------------------------
   Main: load unordered.txt, sort with merge_sort, compare to
         ordered.txt, and print step counts
   ---------------------------------------------------------- */

int main(void) {
    const char* unordered_path = "..\\data\\unordered.txt";
    const char* ordered_path   = "..\\data\\ordered.txt";

    int n_unordered = 0;
    int* unordered = load_file(unordered_path, &n_unordered);
    if (!unordered) {
        fprintf(stderr, "Missing unordered input file - aborting.\n");
        return 1;
    }

    int n_ordered = 0;
    int* ordered = load_file(ordered_path, &n_ordered);
    if (!ordered) {
        fprintf(stderr, "Missing ordered input file - aborting.\n");
        free(unordered);
        return 1;
    }

    if (n_unordered != n_ordered) {
        fprintf(stderr, "File lengths differ: unordered=%d, ordered=%d\n",
                n_unordered, n_ordered);
        free(unordered);
        free(ordered);
        return 1;
    }

    Stats stats = {0, 0};

    printf("Sorting %d elements with merge sort...\n", n_unordered);
    merge_sort(unordered, n_unordered, &stats);

    printf("\n--- Merge Sort Step Counts ---\n");
    printf("Comparisons: %lld\n", stats.comparisons);
    printf("Writes:      %lld\n", stats.writes);

    printf("\nChecking sorted output against ordered.txt...\n");
    int mismatches = compare_arrays(unordered, ordered, n_unordered);

    if (mismatches == 0) {
        printf("SUCCESS — output matches expected sorted list!\n");
    } else {
        printf("FAIL — mismatches found: %d\n", mismatches);
    }

    free(unordered);
    free(ordered);
    return 0;
}
