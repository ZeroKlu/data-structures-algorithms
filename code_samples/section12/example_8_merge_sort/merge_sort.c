#include <stdio.h>
#include <stdlib.h>

/* ----------------------------------------------------------
   Step counter for illustrating time complexity
   ---------------------------------------------------------- */
/*
    Stats holds counters that let you *measure* work done by the algorithm,
    rather than only reasoning about Big-O.

    comparisons:
        - Counts key comparisons between elements during the merge step
          (specifically, each time we compare arr[i] to arr[j]).

    writes:
        - Counts assignment-style writes into arrays used by merge sort.
        - In this implementation, we count:
            1) writes into the temporary buffer `tmp[]` during merging
            2) writes back into the original array `arr[]` when copying the
               merged result back.
        - This helps illustrate that merge sort does additional memory writes
          (and uses extra memory) compared to in-place sorts.
*/
typedef struct {
    long long comparisons;
    long long writes;
} Stats;

/*
    The block of commented-out code below appears to be a prior/alternate
    merge sort implementation without stats tracking.

    Keeping it commented can be useful for teaching:
      - shows the "plain" algorithm
      - then contrasts with the instrumented version below

    If left in production code, consider removing to reduce clutter,
    but for lesson material it's fine as a reference.
*/
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
//
// static void merge_sort_rec(int* arr, int* tmp, int left, int right) {
//     if (right - left <= 1) return;
//     int mid = left + (right - left) / 2;
//     merge_sort_rec(arr, tmp, left, mid);
//     merge_sort_rec(arr, tmp, mid, right);
//     merge(arr, tmp, left, mid, right);
// }
//
// void merge_sort(int* arr, int n) {
//     int* tmp = (int*)malloc(sizeof(int) * n);
//     merge_sort_rec(arr, tmp, 0, n);
//     free(tmp);
// }

/* ----------------------------------------------------------
   Merge sort implementation with step counting
   ---------------------------------------------------------- */

/*
    merge()
    -------
    Merges two adjacent sorted ranges into one sorted range.

    Inputs:
        arr   : the main array containing two sorted halves
        tmp   : temporary buffer used to build the merged result
        left  : start index (inclusive) of the left half
        mid   : start index (inclusive) of the right half
        right : end index (exclusive) of the right half
        stats : pointer to Stats struct to accumulate counts

    Range convention:
        - left half  is arr[left .. mid-1]
        - right half is arr[mid  .. right-1]
        - result is written into tmp[left .. right-1]
          then copied back into arr[left .. right-1]

    Step counting:
        - comparisons increments once per "arr[i] vs arr[j]" decision
        - writes increments for every assignment into tmp or back into arr
*/
static void merge(int* arr, int* tmp, int left, int mid, int right, Stats* stats) {
    int i = left, j = mid, k = left;

    /*
        Main merge loop:
          - Walk i through left half and j through right half
          - Repeatedly take the smaller element and write into tmp[k]
    */
    while (i < mid && j < right) {
        stats->comparisons++;              // compare arr[i] vs arr[j]

        if (arr[i] <= arr[j]) {
            // Take next element from left half
            tmp[k++] = arr[i++];
        } else {
            // Take next element from right half
            tmp[k++] = arr[j++];
        }

        // One element written into tmp
        stats->writes++;                   // wrote into tmp[k-1]
    }

    /*
        Copy any remaining left-half elements.
        (If right half ran out first.)
    */
    while (i < mid) {
        tmp[k++] = arr[i++];
        stats->writes++;                   // write into tmp
    }

    /*
        Copy any remaining right-half elements.
        (If left half ran out first.)
    */
    while (j < right) {
        tmp[k++] = arr[j++];
        stats->writes++;                   // write into tmp
    }

    /*
        Copy merged result back from tmp to arr for this range.

        Note: This implementation counts these writes too, since they are
        real assignments and a major part of merge sort's cost profile.
    */
    for (int p = left; p < right; p++) {
        arr[p] = tmp[p];
        stats->writes++;                   // write back into arr
    }
}

/*
    merge_sort_rec()
    --------------
    Recursive merge sort on a half-open interval [left, right).

    Base case:
        - If the range has 0 or 1 element (right - left <= 1), it's already sorted.

    Recursive case:
        - Split the range into two halves:
            [left, mid) and [mid, right)
        - Sort each half recursively
        - Merge the two sorted halves

    Stats pointer:
        - Passed through all recursive calls so counts accumulate globally.
*/
static void merge_sort_rec(int* arr, int* tmp, int left, int right, Stats* stats) {
    // Base case: range size 0 or 1 is sorted
    if (right - left <= 1) return;

    // Midpoint chosen to avoid overflow (though overflow is unlikely with int indices here)
    int mid = left + (right - left) / 2;

    // Sort left half
    merge_sort_rec(arr, tmp, left, mid, stats);

    // Sort right half
    merge_sort_rec(arr, tmp, mid, right, stats);

    // Merge sorted halves
    merge(arr, tmp, left, mid, right, stats);
}

/*
    merge_sort()
    -----------
    Public wrapper that:
        - allocates the temp buffer once (size n)
        - calls the recursive routine
        - frees the temp buffer

    Important:
        - stats is NOT reset here; caller should initialize stats before calling.
          (In main(), you do Stats stats = {0, 0}; which is correct.)
*/
void merge_sort(int* arr, int n, Stats* stats) {
    // Temporary buffer used for all merges (avoids repeated allocation per merge)
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

/*
    load_file()
    ----------
    Reads whitespace-delimited integers from a text file and returns a
    dynamically allocated array.

    Inputs:
        path   : filesystem path to the data file
        out_n  : output parameter to receive the number of integers read

    Returns:
        - pointer to heap-allocated int array on success
        - NULL on failure

    Memory strategy:
        - Starts with a capacity of 1024
        - Doubles capacity as needed (amortized linear growth)

    Caller responsibility:
        - Must free() the returned array when done.
*/
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

    // Read integers until EOF or parse failure
    while (fscanf(f, "%d", &x) == 1) {
        // If full, grow the buffer
        if (n >= capacity) {
            capacity *= 2;

            int* new_arr = (int*)realloc(arr, capacity * sizeof(int));
            if (!new_arr) {
                // realloc failure: must free original buffer to avoid leak
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

/*
    compare_arrays()
    ---------------
    Compares arrays `a` and `b` of length n element-by-element.

    Prints up to the first 10 mismatches to avoid massive output.

    Returns:
        - total mismatch count
*/
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

/*
    main()
    ------
    Test harness for merge sort.

    Workflow:
      1) Load unordered.txt (input)
      2) Load ordered.txt (expected sorted output)
      3) Validate both files loaded and have same length
      4) Run merge_sort() on unordered data with step counting
      5) Print stats and verify output matches expected

    Expected file layout (relative paths in this program):
      ..\data\unordered.txt
      ..\data\ordered.txt
*/
int main(void) {
    const char* unordered_path = "..\\data\\unordered.txt";
    const char* ordered_path   = "..\\data\\ordered.txt";

    // Load unsorted input
    int n_unordered = 0;
    int* unordered = load_file(unordered_path, &n_unordered);
    if (!unordered) {
        fprintf(stderr, "Missing unordered input file - aborting.\n");
        return 1;
    }

    // Load expected sorted output
    int n_ordered = 0;
    int* ordered = load_file(ordered_path, &n_ordered);
    if (!ordered) {
        fprintf(stderr, "Missing ordered input file - aborting.\n");
        free(unordered);
        return 1;
    }

    // Sanity check: arrays must be same length to compare element-by-element
    if (n_unordered != n_ordered) {
        fprintf(stderr, "File lengths differ: unordered=%d, ordered=%d\n",
                n_unordered, n_ordered);
        free(unordered);
        free(ordered);
        return 1;
    }

    // Initialize counters to zero before sorting
    Stats stats = {0, 0};

    printf("Sorting %d elements with merge sort...\n", n_unordered);

    // Sort unordered[] in place and accumulate stats
    merge_sort(unordered, n_unordered, &stats);

    // Print measured work
    printf("\n--- Merge Sort Step Counts ---\n");
    printf("Comparisons: %lld\n", stats.comparisons);
    printf("Writes:      %lld\n", stats.writes);

    // Validate correctness vs. known-good sorted output
    printf("\nChecking sorted output against ordered.txt...\n");
    int mismatches = compare_arrays(unordered, ordered, n_unordered);

    if (mismatches == 0) {
        printf("SUCCESS — output matches expected sorted list!\n");
    } else {
        printf("FAIL — mismatches found: %d\n", mismatches);
    }

    // Cleanup heap allocations
    free(unordered);
    free(ordered);

    return 0;
}
