// Exponential Search with step counting (for sorted arrays)

/**
 * Performs exponential search on a sorted array while counting steps.
 *
 * Exponential search works in two phases:
 *   1) Exponential range expansion:
 *      Repeatedly double the index (1, 2, 4, 8, ...) until the target
 *      is less than or equal to arr[bound], or the bound exceeds the array.
 *   2) Binary search:
 *      Perform binary search within the identified range.
 *
 * Step counting:
 *   - Counts probes of arr[0]
 *   - Counts each probe during exponential range expansion
 *   - Continues counting during the binary-search phase
 *
 * @param {number[]} arr   Sorted array to search
 * @param {number} target Value to locate
 *
 * @returns {{index: number, steps: number}}
 *          Object containing the index of the target (or -1)
 *          and the number of steps performed
 */
function exponentialSearch(arr, target) {
    let steps = 0;              // Total step counter
    const n = arr.length;       // Number of elements in the array
    if (n === 0) return { index: -1, steps };

    // ---------------------------------------------------
    // Step 0: Probe the first element directly
    // ---------------------------------------------------
    steps++;                    // Count probe of arr[0]
    if (arr[0] === target) {
        return { index: 0, steps };
    }

    // ---------------------------------------------------
    // Phase 1: Find search range by repeated doubling
    // ---------------------------------------------------
    let bound = 1;

    // Double the bound while:
    //  - bound is within array limits, and
    //  - arr[bound] is still less than the target
    while (bound < n) {
        steps++;                // Count probe at arr[bound]
        if (arr[bound] < target) {
            bound *= 2;         // Exponentially expand the range
        } else {
            break;              // Upper bound found
        }
    }

    // ---------------------------------------------------
    // Phase 2: Binary search within the identified range
    // ---------------------------------------------------
    const left = Math.floor(bound / 2);
    const right = Math.min(bound, n - 1);

    return binarySearchRange(arr, left, right, target, steps);
}


// Binary search in the range [lo, hi], continuing step count

/**
 * Performs binary search within a restricted index range [lo, hi],
 * continuing the step count accumulated during exponential search.
 *
 * @param {number[]} arr   Sorted array to search
 * @param {number} lo     Lower bound index (inclusive)
 * @param {number} hi     Upper bound index (inclusive)
 * @param {number} target Value to locate
 * @param {number} steps  Step count accumulated so far
 *
 * @returns {{index: number, steps: number}}
 *          Object containing search result and updated step count
 */
function binarySearchRange(arr, lo, hi, target, steps) {
    // Standard binary search loop
    while (lo <= hi) {
        // Compute midpoint safely
        const mid = Math.floor(lo + (hi - lo) / 2);

        steps++;                // Count probe of arr[mid]
        const val = arr[mid];

        // Compare midpoint value against target
        if (val === target) return { index: mid, steps };
        if (val < target) lo = mid + 1;  // Search right half
        else hi = mid - 1;               // Search left half
    }

    // Target not found in this range
    return { index: -1, steps };
}


// ---------------------------------------------------------------------------
//  File loading for ordered.txt
// ---------------------------------------------------------------------------

const fs = require("fs");       // File system access
const path = require("path");   // Path resolution utilities

/**
 * Attempts to load a whitespace-delimited integer file
 * from a given relative path.
 *
 * @param {string} relPath Relative file path to try
 *
 * @returns {number[] | null}
 *          Parsed integer array if successful; otherwise null
 */
function tryLoadFile(relPath) {
    const full = path.resolve(relPath);
    console.log("Trying:", full);

    try {
        // Read entire file as UTF-8 text
        const data = fs.readFileSync(full, "utf8");

        // Split on whitespace and convert tokens to numbers
        const arr = data
            .trim()
            .split(/\s+/)
            .map(Number);

        console.log("Loaded", arr.length, "integers from", full);
        return arr;
    } catch {
        // File does not exist or cannot be read
        console.log("  Not found or unreadable.");
        return null;
    }
}

/**
 * Attempts to load ordered.txt from several candidate locations.
 *
 * This allows the script to run correctly when executed from:
 *   - a lesson folder
 *   - a nested Node.js directory
 *   - the repository root
 *
 * @returns {number[] | null}
 */
function loadOrderedFile() {
    console.log("Working directory:", process.cwd());

    // Candidate relative paths to ordered.txt
    const candidates = [
        "../data/ordered.txt",
        "../../data/ordered.txt",
        "code_samples/section12/data/ordered.txt"
    ];

    // Try each candidate path
    for (const candidate of candidates) {
        const arr = tryLoadFile(candidate);
        if (arr) return arr;
    }

    // All attempts failed
    console.log("Missing input file - aborting.");
    return null;
}


// ---------------------------------------------------------------------------
//  Tests
// ---------------------------------------------------------------------------

/**
 * Loads ordered data from disk and runs exponential search
 * on a set of representative target values.
 */
function runTests() {
    const arr = loadOrderedFile();
    if (!arr) return;

    // Representative test targets:
    const testTargets = [
        -5000,   // first element (best case)
        0,       // middle element
        5000,    // last element
        6000     // not found
    ];

    // Run exponential search for each target
    for (const target of testTargets) {
        const result = exponentialSearch(arr, target);
        console.log(
            `Target ${target} → index=${result.index}, steps=${result.steps}`
        );
    }
}

// Execute test harness
runTests();
