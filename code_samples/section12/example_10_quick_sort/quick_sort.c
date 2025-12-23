/*
    quick_sort.c
    ------------
    Quick Sort (C) using Lomuto partitioning, with step counting and file-based tests.

    PURPOSE
    -------
    This program:
      1) loads integers from unordered.txt
      2) sorts them using Quick Sort (Lomuto partition scheme)
      3) compares the result to ordered.txt (a known-correct sorted list)
      4) prints step counts (comparisons + writes) to illustrate algorithm cost

    INPUT FILES
    -----------
    - unordered.txt : integers in arbitrary order
    - ordered.txt   : the same integers sorted ascending

    Integers can be separated by any whitespace.

    STEP COUNTING MODEL
    -------------------
    We count two things:
      - comparisons:
          every time we evaluate (arr[j] <= pivot) during partition
      - writes:
          every time we write into the array due to a swap.
          A swap of two elements is counted as 3 writes here:
              tmp = arr[i]   (read + local write; we still count as part of swap cost)
              arr[i] = arr[j]
              arr[j] = tmp
          This matches the style you’ve been using in other C examples (counting
          “writes” as assignments involved in moving data).

    NOTE:
      - Counting models vary. Some count “array writes only” and would NOT count
        tmp assignment. Here we keep your existing convention (writes += 3).

    COMPLEXITY (Big-O)
    ------------------
    Average case:  O(n log n)
    Worst case:    O(n^2)   (e.g., already sorted input with poor pivot choices)
    Best case:     O(n log n)

    This implementation always uses the last element as the pivot (arr[right]),
    which can cause worst-case behavior on already sorted or reverse-sorted data.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------
// GLOBAL COUNTERS
// ------------------------------------------------------------
// These are reset inside quick_sort() so each run starts at 0.
static long long comparisons = 0;  // how many value comparisons we perform
static long long writes      = 0;  // how many data-movement writes (swap cost model)

// ------------------------------------------------------------
// Quick Sort (Lomuto partition)
// ------------------------------------------------------------
/*
    partition()
    -----------
    Lomuto partition scheme:
      - Choose pivot = arr[right] (last element)
      - Maintain index i such that:
            arr[left..i-1]   <= pivot
            arr[i..j-1]      >  pivot  (in-progress)
      - Scan j from left to right-1:
            if arr[j] <= pivot:
                swap arr[i] and arr[j]
                i++
      - Finally swap pivot into its final location at i:
            swap arr[i] and arr[right]
      - Return i (pivot’s final index)

    STEP COUNTING
    -------------
    - comparisons++ for each (arr[j] <= pivot) check.
    - writes += 3 for each swap (our swap-cost model).
*/
static int partition(int *arr, int left, int right) {
    // Pivot selection: last element in the current range
    int pivot = arr[right];

    // i tracks the boundary between <= pivot and > pivot regions
    int i = left;

    // j scans across the range [left, right-1]
    for (int j = left; j < right; j++) {
        comparisons++; // compare arr[j] <= pivot

        if (arr[j] <= pivot) {
            // Swap arr[i] and arr[j] so arr[i] becomes part of the <= pivot region
            int tmp = arr[i];
            arr[i]  = arr[j];
            arr[j]  = tmp;

            // Count the "writes" involved in swapping values
            writes += 3;

            i++;
        }
    }

    // Place pivot in its final position by swapping arr[i] with arr[right]
    int tmp = arr[i];
    arr[i]  = arr[right];
    arr[right] = tmp;
    writes += 3;

    return i; // pivot final index
}

/*
    quick_sort_rec()
    ----------------
    Recursive Quick Sort:
      - Partition the array segment [left..right]
      - Recursively sort left partition [left..p-1]
      - Recursively sort right partition [p+1..right]

    Base case:
      - If left >= right, segment has 0 or 1 element => already sorted.
*/
static void quick_sort_rec(int *arr, int left, int right) {
    if (left < right) {
        int p = partition(arr, left, right);

        // Sort elements less than or equal to pivot
        quick_sort_rec(arr, left, p - 1);

        // Sort elements greater than pivot
        quick_sort_rec(arr, p + 1, right);
    }
}

/*
    quick_sort()
    ------------
    Public API:
      - resets global counters
      - sorts arr[0..n-1] if n > 0
*/
void quick_sort(int *arr, int n) {
    comparisons = 0;
    writes = 0;

    if (n > 0) {
        quick_sort_rec(arr, 0, n - 1);
    }
}

// ------------------------------------------------------------
// FILE LOADER
// ------------------------------------------------------------
/*
    load_file()
    -----------
    Loads integers from a whitespace-separated text file into a dynamically
    allocated int array.

    WHY MULTIPLE PATHS?
    -------------------
    Students run executables from different working directories. To reduce
    “file not found” frustration, we try several likely relative locations:

      1) <CWD>/<filename>
      2) <CWD>/data/<filename>
      3) <CWD>/../data/<filename>      (your typical lesson layout)
      4) <CWD>/../../data/<filename>   (if you run from a deeper folder)

    On success:
      - returns malloc'd int* array
      - writes number of loaded ints to *out_size
    On failure:
      - prints attempted paths and exits
*/
static int *load_file(const char *filename, int *out_size) {
    const char *prefixes[] = {
        "",             // filename in current working directory
        "data/",        // ./data/filename
        "../data/",     // ../data/filename  (common layout)
        "../../data/",  // ../../data/filename
        NULL
    };

    FILE *f = NULL;
    char fullpath[512];

    // Try each prefix until we successfully open the file
    for (int i = 0; prefixes[i] != NULL; i++) {
        snprintf(fullpath, sizeof(fullpath), "%s%s", prefixes[i], filename);
        f = fopen(fullpath, "r");
        if (f) {
            printf("Loaded: %s\n", fullpath);
            break;
        }
    }

    // If no attempt worked, print diagnostics and abort
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

    // Dynamic array growth: start with a modest capacity and grow as needed
    int capacity = 1024;
    int *arr = (int *)malloc(capacity * sizeof(int));
    if (!arr) {
        perror("malloc");
        exit(1);
    }

    int size = 0;

    // Read ints until fscanf fails (EOF or non-integer token)
    while (fscanf(f, "%d", &arr[size]) == 1) {
        size++;

        // Grow the buffer when we run out of room
        if (size >= capacity) {
            capacity *= 2;

            int *tmp = (int *)realloc(arr, capacity * sizeof(int));
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
/*
    main()
    ------
    1) Load unordered.txt and ordered.txt
    2) Copy unordered into a working array
    3) Sort with quick_sort()
    4) Verify output matches ordered.txt
    5) Print step counts + a sample of sorted values
*/
int main(void) {
    int n1 = 0, n2 = 0;

    // Load input files
    int *unordered = load_file("unordered.txt", &n1);
    int *expected  = load_file("ordered.txt", &n2);

    // Both files must contain the same number of integers
    if (n1 != n2) {
        printf("Mismatch in file lengths (%d vs %d) — aborting.\n", n1, n2);
        free(unordered);
        free(expected);
        exit(1);
    }

    // Create a working copy of unordered so we don't modify the original buffer
    int *arr = (int *)malloc(n1 * sizeof(int));
    if (!arr) {
        perror("malloc");
        free(unordered);
        free(expected);
        exit(1);
    }
    memcpy(arr, unordered, n1 * sizeof(int));

    // Sort + collect step counts
    quick_sort(arr, n1);

    // Validate result by comparing against expected sorted list
    int ok = 1;
    for (int i = 0; i < n1; i++) {
        if (arr[i] != expected[i]) {
            ok = 0;
            printf("Mismatch at index %d: got %d, expected %d\n",
                   i, arr[i], expected[i]);
            break;
        }
    }

    // Report results
    printf("\nQuick Sort (C)\n");
    printf("--------------\n");
    printf("Elements:     %d\n", n1);
    printf("Comparisons:  %lld\n", comparisons);
    printf("Writes:       %lld\n", writes);
    printf("Correct?      %s\n", ok ? "YES ✔" : "NO ✘");

    // Show a small sample if everything is correct
    if (ok) {
        printf("\nFirst 10 sorted values:\n");
        for (int i = 0; i < 10 && i < n1; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    // Cleanup
    free(unordered);
    free(expected);
    free(arr);
    return 0;
}
