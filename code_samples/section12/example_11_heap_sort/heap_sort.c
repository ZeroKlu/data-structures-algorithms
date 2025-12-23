/* ============================================================================
   Example 11 - Heap Sort (C) with step counting (comparisons & writes)

   PURPOSE
   -------
   Demonstrate Heap Sort on integer data while counting steps to illustrate
   time complexity.

   INPUT FILES
   -----------
   This program expects TWO text files containing the SAME integers:
     - unordered.txt : unsorted integers
     - ordered.txt   : the same integers sorted in ascending order

   Integers may be separated by any whitespace (spaces, tabs, newlines).

   PROGRAM FLOW
   ------------
   1) Load unordered.txt and ordered.txt using a robust "try many paths" loader
   2) Heap-sort the unordered data IN PLACE (mutates the array)
   3) Track:
        - g_comparisons : how many element-to-element comparisons we perform
        - g_writes      : how many array writes we perform (swap counted as 3)
   4) Verify the sorted result matches ordered.txt exactly
   5) Print summary stats and PASS/FAIL

   STEP COUNTING MODEL
   -------------------
   comparisons:
     - We count a comparison each time we compare an array value to another
       array value inside heapify():
         arr[left]  > arr[largest]
         arr[right] > arr[largest]

     - We do NOT count index bounds checks like (left < n) as comparisons,
       because those are not comparisons between *data values*.

   writes:
     - We treat a swap of two array elements as 3 writes (temp + two stores),
       matching the same convention used in your other C/C++ examples.

   HEAP SORT NOTES
   --------------
   - We build a MAX-HEAP (largest element at the root)
   - Then repeatedly swap the root (max) with the end of the array and
     heapify the remaining prefix

   Complexity:
   - Build heap:     O(n)
   - Extract n times: O(n log n)
   - Total:          O(n log n) time, O(1) extra space (in-place)

   ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------------------------------------------------------------
   GLOBAL COUNTERS (illustrate complexity)
   ---------------------------------------------------------------------------- */
static long long g_comparisons = 0;  /* counts value-to-value comparisons */
static long long g_writes      = 0;  /* counts array writes (swaps counted as 3) */


/* ----------------------------------------------------------------------------
   Utility: robust file loader
   ----------------------------------------------------------------------------
   load_file("unordered.txt", &n) attempts several likely relative paths:

     1) "unordered.txt"                (current working directory)
     2) "data/unordered.txt"           (./data)
     3) "../data/unordered.txt"        (sibling data folder)
     4) "../../data/unordered.txt"     (if running from a deeper folder)
     5) "../section12/data/unordered.txt"
     6) "../../section12/data/unordered.txt"

   On success:
     - prints the exact path used
     - returns a dynamically-allocated int array
     - writes the number of ints into *out_n

   On failure:
     - prints diagnostics and exits(1)
   ---------------------------------------------------------------------------- */
static int *load_file(const char *filename, int *out_n) {
    /* Candidate paths (we store some in stack buffers) */
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

    /* Try each candidate until one opens successfully */
    for (int i = 0; i < 6; i++) {
        f = fopen(candidates[i], "r");
        if (f != NULL) {
            used_path = candidates[i];
            break;
        }
    }

    /* If none opened, print diagnostics and abort */
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

    /* Read all ints into a dynamically-growing buffer */
    int capacity = 1024;  /* initial capacity */
    int size = 0;
    int *arr = (int *)malloc((size_t)capacity * sizeof(int));
    if (!arr) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        exit(1);
    }

    int x;
    while (fscanf(f, "%d", &x) == 1) {
        /* Grow buffer if needed */
        if (size >= capacity) {
            capacity *= 2;
            int *tmp = (int *)realloc(arr, (size_t)capacity * sizeof(int));
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


/* ----------------------------------------------------------------------------
   Heap sort implementation with step counting
   ---------------------------------------------------------------------------- */

/* Swap two ints by pointer.
   Step-count model:
     - We treat swap as 3 writes (tmp, then write a, then write b).
     - NOTE: tmp is not an array element, but we keep the same convention
       used in your earlier C quick-sort examples (swap = 3 writes).
 */
static void swap_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
    g_writes += 3;
}

/* heapify(arr, n, i)
   -------------------
   Ensure the subtree rooted at index i obeys the MAX-HEAP property:
     arr[i] >= arr[left] and arr[i] >= arr[right]

   Parameters:
     arr : array representing heap
     n   : heap size (we only consider indices 0..n-1)
     i   : root index of the subtree to fix

   How it works:
     - Find the largest among i, left child, right child
     - If a child is larger than arr[i], swap and recurse

   Step counting:
     - For each child that exists, we do one value comparison:
         arr[left]  > arr[largest]
         arr[right] > arr[largest]
 */
static void heapify(int *arr, int n, int i) {
    int largest = i;          /* assume root is largest */
    int left  = 2 * i + 1;    /* left child index */
    int right = 2 * i + 2;    /* right child index */

    /* Compare left child to current largest */
    if (left < n) {
        g_comparisons++;      /* compare arr[left] vs arr[largest] */
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }

    /* Compare right child to current largest */
    if (right < n) {
        g_comparisons++;      /* compare arr[right] vs arr[largest] */
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }

    /* If the largest is not the root, swap and continue fixing downward */
    if (largest != i) {
        swap_int(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

/* heap_sort(arr, n)
   -----------------
   In-place Heap Sort using a MAX-HEAP.

   Steps:
     1) Build a max-heap from the array:
          - Start from the last parent node (n/2 - 1) down to 0
          - heapify each node
     2) Repeatedly extract the max element:
          - swap arr[0] (max) with arr[end]
          - reduce heap size to end
          - heapify root again to restore heap property

   Step counters:
     - Reset at start so each run reports clean counts.
 */
void heap_sort(int *arr, int n) {
    g_comparisons = 0;
    g_writes = 0;

    if (n <= 1) {
        return; /* already sorted */
    }

    /* 1) Build max-heap */
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    /* 2) Extract max one by one */
    for (int end = n - 1; end > 0; end--) {
        /* Move current max (root) to the end of the array */
        swap_int(&arr[0], &arr[end]);

        /* Heapify the reduced heap [0 .. end-1] */
        heapify(arr, end, 0);
    }
}


/* ----------------------------------------------------------------------------
   Test harness
   ----------------------------------------------------------------------------
   - Load unordered + expected ordered arrays
   - Heap sort unordered in place
   - Verify element-by-element equality
   - Print summary statistics
   ---------------------------------------------------------------------------- */
int main(void) {
    int n_unordered = 0;
    int n_ordered   = 0;

    /* Load data sets */
    int *unordered = load_file("unordered.txt", &n_unordered);
    int *expected  = load_file("ordered.txt", &n_ordered);

    /* Sanity check: must contain the same number of integers */
    if (n_unordered != n_ordered) {
        fprintf(stderr, "Size mismatch: unordered=%d, ordered=%d\n",
                n_unordered, n_ordered);
        free(unordered);
        free(expected);
        return 1;
    }

    /* Sort the unordered data in place */
    heap_sort(unordered, n_unordered);

    /* Verify sorted output against ordered.txt */
    int ok = 1;
    for (int i = 0; i < n_unordered; i++) {
        if (unordered[i] != expected[i]) {
            printf("Mismatch at index %d: got %d, expected %d\n",
                   i, unordered[i], expected[i]);
            ok = 0;
            break;
        }
    }

    /* Print results */
    printf("\nHeap Sort (C)\n");
    printf("-------------\n");
    printf("Elements:     %d\n", n_unordered);
    printf("Comparisons:  %lld\n", g_comparisons);
    printf("Writes:       %lld\n", g_writes);
    printf("Correct?      %s\n", ok ? "YES ✓" : "NO ✘");

    /* Clean up heap allocations */
    free(unordered);
    free(expected);

    return ok ? 0 : 1;
}
