// insertion_sort.c
// Insertion sort with step counting and file-based tests.
//
// Expects two files containing the SAME integers:
//   unordered.txt  - unsorted
//   ordered.txt    - sorted ascending
//
// Each file should contain integers separated by whitespace.
//
// Compile (Windows, gcc example):
//   gcc -std=c17 -O2 insertion_sort.c -o insertion_sort
//
// Run:
//   insertion_sort

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
    #include <direct.h>
    #define GETCWD _getcwd
#else
    #include <unistd.h>
    #define GETCWD getcwd
#endif

// Global counters to illustrate complexity
static long long g_comparisons = 0;
static long long g_writes      = 0;

// ---------------- Insertion Sort ----------------

void insertion_sort(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        // We'll count comparisons between arr[j] and key,
        // plus writes when we shift elements / insert key.
        while (j >= 0) {
            g_comparisons++;           // compare arr[j] vs key
            if (arr[j] > key) {
                arr[j + 1] = arr[j];   // shift
                g_writes++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
        g_writes++;
    }
}

// ---------------- File Loading ----------------

// Try to open the file from several likely locations.
// Returns FILE* or NULL; also copies the chosen path into out_path.
static FILE *open_with_fallbacks(const char *name, char *out_path, size_t out_path_sz) {
    FILE *f = NULL;

    // 1) Current directory
    snprintf(out_path, out_path_sz, "%s", name);
    f = fopen(out_path, "r");
    if (f) return f;

    // 2) ../data/name (sibling data folder)
    snprintf(out_path, out_path_sz, "../data/%s", name);
    f = fopen(out_path, "r");
    if (f) return f;

    // 3) ../../data/name (if you're one level deeper)
    snprintf(out_path, out_path_sz, "../../data/%s", name);
    f = fopen(out_path, "r");
    if (f) return f;

    // 4) <HOME or USERPROFILE>/Desktop/data/name
    const char *home = getenv("USERPROFILE");
    if (!home) home = getenv("HOME");
    if (home) {
        snprintf(out_path, out_path_sz, "%s/Desktop/data/%s", home, name);
        f = fopen(out_path, "r");
        if (f) return f;
    }

    // If we reach here, everything failed; out_path holds last attempt.
    return NULL;
}

// Load all ints from a file into a dynamically allocated array.
// Returns 1 on success, 0 on failure.
static int load_file(const char *name, int **out_arr, int *out_n) {
    char path[1024];
    FILE *f = open_with_fallbacks(name, path, sizeof(path));
    if (!f) {
        fprintf(stderr, "Error reading: %s\n", path);
        char cwd[1024];
        if (GETCWD(cwd, sizeof(cwd))) {
            fprintf(stderr, "Working directory: %s\n", cwd);
        }
        fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));
        return 0;
    }

    // Read ints dynamically
    int capacity = 1024;
    int *arr = (int *)malloc(capacity * sizeof(int));
    if (!arr) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        return 0;
    }

    int n = 0;
    int x;
    while (fscanf(f, "%d", &x) == 1) {
        if (n >= capacity) {
            capacity *= 2;
            int *tmp = (int *)realloc(arr, capacity * sizeof(int));
            if (!tmp) {
                fprintf(stderr, "Realloc failed\n");
                free(arr);
                fclose(f);
                return 0;
            }
            arr = tmp;
        }
        arr[n++] = x;
    }
    fclose(f);

    *out_arr = arr;
    *out_n = n;
    return 1;
}

// ---------------- Main Test Harness ----------------

int main(void) {
    int *unordered = NULL;
    int *expected  = NULL;
    int n_unordered = 0;
    int n_expected  = 0;

    if (!load_file("unordered.txt", &unordered, &n_unordered)) {
        fprintf(stderr, "Missing unordered input file — aborting.\n");
        return 1;
    }
    if (!load_file("ordered.txt", &expected, &n_expected)) {
        fprintf(stderr, "Missing ordered input file — aborting.\n");
        free(unordered);
        return 1;
    }

    if (n_unordered != n_expected) {
        fprintf(stderr, "Size mismatch: unordered has %d numbers, ordered has %d.\n",
                n_unordered, n_expected);
        free(unordered);
        free(expected);
        return 1;
    }

    // Copy unordered array for sorting
    int *arr = (int *)malloc(n_unordered * sizeof(int));
    if (!arr) {
        fprintf(stderr, "Memory allocation failed\n");
        free(unordered);
        free(expected);
        return 1;
    }
    for (int i = 0; i < n_unordered; i++) {
        arr[i] = unordered[i];
    }

    // Reset counters
    g_comparisons = 0;
    g_writes = 0;

    // Run insertion sort
    insertion_sort(arr, n_unordered);

    // Verify result against expected
    int mismatch_index = -1;
    for (int i = 0; i < n_unordered; i++) {
        if (arr[i] != expected[i]) {
            mismatch_index = i;
            break;
        }
    }

    printf("Insertion Sort Test\n");
    printf("--------------------\n");
    printf("Number of elements: %d\n", n_unordered);
    printf("Comparisons:        %lld\n", g_comparisons);
    printf("Writes (shifts+key): %lld\n", g_writes);

    if (mismatch_index == -1) {
        printf("Result:             PASS (sorted output matches ordered.txt)\n");
    } else {
        printf("Result:             FAIL (first mismatch at index %d: got %d, expected %d)\n",
               mismatch_index, arr[mismatch_index], expected[mismatch_index]);
    }

    free(unordered);
    free(expected);
    free(arr);
    return 0;
}
