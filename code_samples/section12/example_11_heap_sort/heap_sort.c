/* Example: Heap Sort with step counting (comparisons & writes)
 *
 * Expects two data files containing space-separated integers:
 *   - unordered.txt : unsorted integers
 *   - ordered.txt   : the same integers in sorted order
 *
 * This program:
 *   1. Loads both files.
 *   2. Runs heap sort on the unordered data.
 *   3. Counts comparisons and writes.
 *   4. Verifies the result against ordered.txt.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global counters to illustrate complexity */
static long long g_comparisons = 0;
static long long g_writes      = 0;

/* ---------- Utility: robust file loader ---------- */

static int *load_file(const char *filename, int *out_n) {
    /* Try multiple likely paths, in order */
    const char *candidates[6];
    char buf1[512], buf2[512], buf3[512], buf4[512], buf5[512];

    /* 1) filename as-is (e.g., "unordered.txt") */
    candidates[0] = filename;

    /* 2) "./data/filename" */
    snprintf(buf1, sizeof(buf1), "data/%s", filename);
    candidates[1] = buf1;

    /* 3) "../data/filename" */
    snprintf(buf2, sizeof(buf2), "../data/%s", filename);
    candidates[2] = buf2;

    /* 4) "../../data/filename" */
    snprintf(buf3, sizeof(buf3), "../../data/%s", filename);
    candidates[3] = buf3;

    /* 5) "../section12/data/filename" */
    snprintf(buf4, sizeof(buf4), "../section12/data/%s", filename);
    candidates[4] = buf4;

    /* 6) "../../section12/data/filename" */
    snprintf(buf5, sizeof(buf5), "../../section12/data/%s", filename);
    candidates[5] = buf5;

    FILE *f = NULL;
    const char *used_path = NULL;

    for (int i = 0; i < 6; i++) {
        f = fopen(candidates[i], "r");
        if (f != NULL) {
            used_path = candidates[i];
            break;
        }
    }

    if (!f) {
        fprintf(stderr, "Error reading: %s\n", filename);
        fprintf(stderr, "Search paths attempted:\n");
        for (int i = 0; i < 6; i++) {
            fprintf(stderr, "  %s\n", candidates[i]);
        }
        fprintf(stderr, "Missing input file - aborting.\n");
        exit(1);
    }

    printf("Loaded: %s\n", used_path);

    /* Read all ints (space/newline separated) */
    int capacity = 1024;
    int size = 0;
    int *arr = (int *)malloc(capacity * sizeof(int));
    if (!arr) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        exit(1);
    }

    int x;
    while (fscanf(f, "%d", &x) == 1) {
        if (size >= capacity) {
            capacity *= 2;
            int *tmp = (int *)realloc(arr, capacity * sizeof(int));
            if (!tmp) {
                fprintf(stderr, "Realloc failed\n");
                free(arr);
                fclose(f);
                exit(1);
            }
            arr = tmp;
        }
        arr[size++] = x;
    }

    fclose(f);
    *out_n = size;
    return arr;
}

/* ---------- Heap sort implementation with counting ---------- */

static void swap_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
    /* three writes to the array locations */
    g_writes += 3;
}

/* Heapify subtree rooted at index i, in arr[0..n-1] */
static void heapify(int *arr, int n, int i) {
    int largest = i;
    int left  = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        g_comparisons++;
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }
    if (right < n) {
        g_comparisons++;
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }

    if (largest != i) {
        swap_int(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(int *arr, int n) {
    g_comparisons = 0;
    g_writes = 0;

    /* Build max-heap */
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    /* Extract elements one by one from heap */
    for (int end = n - 1; end > 0; end--) {
        swap_int(&arr[0], &arr[end]);   /* move max to end */
        heapify(arr, end, 0);           /* fix heap on remaining prefix */
    }
}

/* ---------- Test harness ---------- */

int main(void) {
    int n_unordered = 0;
    int n_ordered   = 0;

    int *unordered = load_file("unordered.txt", &n_unordered);
    int *expected  = load_file("ordered.txt", &n_ordered);

    if (n_unordered != n_ordered) {
        fprintf(stderr, "Size mismatch: unordered=%d, ordered=%d\n",
                n_unordered, n_ordered);
        free(unordered);
        free(expected);
        return 1;
    }

    heap_sort(unordered, n_unordered);

    /* Verify */
    int ok = 1;
    for (int i = 0; i < n_unordered; i++) {
        if (unordered[i] != expected[i]) {
            printf("Mismatch at index %d: got %d, expected %d\n",
                   i, unordered[i], expected[i]);
            ok = 0;
            break;
        }
    }

    printf("\nHeap Sort (C)\n");
    printf("-------------\n");
    printf("Elements:     %d\n", n_unordered);
    printf("Comparisons:  %lld\n", g_comparisons);
    printf("Writes:       %lld\n", g_writes);
    printf("Correct?      %s\n", ok ? "YES ✓" : "NO ✘");

    free(unordered);
    free(expected);
    return ok ? 0 : 1;
}
