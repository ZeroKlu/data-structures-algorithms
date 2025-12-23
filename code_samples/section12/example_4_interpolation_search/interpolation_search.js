const fs = require("fs");       // Provides file system operations (readFileSync)
const path = require("path");   // Provides utilities for resolving file paths

// ============================================================
// Interpolation Search (index-only)
// Requires sorted data
// ============================================================

/**
 * Performs interpolation search on a sorted array of numbers.
 *
 * Interpolation search estimates where the target is likely to be
 * based on its value relative to the values at the current bounds.
 * This is most effective when the data is uniformly distributed.
 *
 * @param {number[]} arr   Sorted array to search
 * @param {number} target Value to locate
 *
 * @returns {number} Index of the target if found; otherwise -1
 */
function interpolationSearch(arr, target) {
    let n = arr.length;          // Number of elements in the array
    if (n === 0) return -1;      // Empty array => not found

    let lo = 0;                 // Lower bound of search range (inclusive)
    let hi = n - 1;             // Upper bound of search range (inclusive)

    // Continue searching while:
    //  - bounds are valid, and
    //  - target lies within the value range [arr[lo], arr[hi]]
    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {

        // ----------------------------------------------------
        // Special case: all values in [lo, hi] are identical
        // ----------------------------------------------------
        if (arr[hi] === arr[lo]) {
            // Only possible match is at index lo
            return arr[lo] === target ? lo : -1;
        }

        // ----------------------------------------------------
        // Estimate the likely position using interpolation:
        //
        // pos = lo + (hi - lo) * (target - arr[lo]) / (arr[hi] - arr[lo])
        //
        // Math.floor is used to convert to an integer index.
        // ----------------------------------------------------
        const pos = lo + Math.floor(
            ((hi - lo) * (target - arr[lo])) / (arr[hi] - arr[lo])
        );

        // Guard against out-of-range estimates
        if (pos < lo || pos > hi) return -1;

        // Compare against the estimated position
        if (arr[pos] === target) return pos;
        else if (arr[pos] < target) lo = pos + 1; // Search right subrange
        else hi = pos - 1;                         // Search left subrange
    }

    // Target not found
    return -1;
}

// ============================================================
// Interpolation Search WITH step counting
// ============================================================

/**
 * Performs interpolation search while counting selected comparisons.
 *
 * The step counter includes:
 *  - Checking if arr[hi] === arr[lo]
 *  - Checking if arr[lo] === target (uniform-range case)
 *  - Checking if arr[pos] === target
 *  - Checking if arr[pos] < target (to choose direction)
 *
 * Loop boundary checks are not counted—only the explicitly
 * labeled comparisons are included.
 *
 * @param {number[]} arr   Sorted array to search
 * @param {number} target Value to locate
 *
 * @returns {{index: number, steps: number}}
 *          Object containing:
 *           - index: position of the target, or -1 if not found
 *           - steps: number of comparisons performed
 */
function interpolationSearchSteps(arr, target) {
    let n = arr.length;     // Number of elements
    let steps = 0;          // Counts number of comparisons

    // Handle empty array case
    if (n === 0) return { index: -1, steps };

    let lo = 0;             // Lower bound of search range
    let hi = n - 1;         // Upper bound of search range

    // Continue searching while target remains within value bounds
    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {

        steps++; // Compare arr[hi] === arr[lo]
        if (arr[hi] === arr[lo]) {
            steps++; // Compare arr[lo] === target
            return { index: arr[lo] === target ? lo : -1, steps };
        }

        // Estimate likely position
        const pos = lo + Math.floor(
            ((hi - lo) * (target - arr[lo])) / (arr[hi] - arr[lo])
        );

        // Guard against invalid estimates
        if (pos < lo || pos > hi) return { index: -1, steps };

        steps++; // Compare arr[pos] === target
        if (arr[pos] === target) return { index: pos, steps };

        steps++; // Compare arr[pos] < target
        if (arr[pos] < target) lo = pos + 1;
        else hi = pos - 1;
    }

    // Target not found
    return { index: -1, steps };
}

// ============================================================
// Load ordered.txt
// ============================================================

/**
 * Loads the ordered integer dataset from disk.
 *
 * The file path is resolved relative to this script's directory
 * (__dirname), not the process working directory.
 *
 * Expected path:
 *   ../data/ordered.txt
 *
 * @returns {number[]} Array of integers read from ordered.txt,
 *                     or an empty array on failure
 */
function loadOrderedFile() {
    const filePath = path.resolve(__dirname, "..", "data", "ordered.txt");
    console.log("Attempting to read:", filePath);

    try {
        // Read entire file contents as UTF-8 text
        const text = fs.readFileSync(filePath, "utf8");

        // Split on whitespace, remove empty entries, and convert to numbers
        return text
            .split(/\s+/)
            .filter(Boolean)
            .map(Number);
    } catch (e) {
        // Provide diagnostic output if file loading fails
        console.log("Error reading ordered.txt:");
        console.log(e.message);
        console.log("Working directory:", process.cwd());
        return [];
    }
}

// ============================================================
// Test Harness
// ============================================================

/**
 * Executes a standardized set of interpolation search tests
 * on sorted data.
 *
 * @param {number[]} arr Sorted array of integers to test
 */
function runTests(arr) {
    console.log("\n=== Interpolation Search Tests (sorted data only) ===");
    console.log("Loaded", arr.length, "integers\n");

    // Test 1: Search for the first element
    let r = interpolationSearchSteps(arr, arr[0]);
    console.log(`Search first  (${arr[0]}): index=${r.index}, steps=${r.steps}`);

    // Test 2: Search for the middle element
    const mid = arr[Math.floor(arr.length / 2)];
    r = interpolationSearchSteps(arr, mid);
    console.log(`Search middle (${mid}): index=${r.index}, steps=${r.steps}`);

    // Test 3: Search for the last element
    const last = arr[arr.length - 1];
    r = interpolationSearchSteps(arr, last);
    console.log(`Search last   (${last}): index=${r.index}, steps=${r.steps}`);

    // Test 4: Search for a value guaranteed not to exist
    r = interpolationSearchSteps(arr, 999999);
    console.log(`Search missing (999999): index=${r.index}, steps=${r.steps}`);
}

// ============================================================
// MAIN
// ============================================================

// Load ordered dataset from disk
const arr = loadOrderedFile();

// Validate successful file load
if (arr.length === 0) {
    console.log("Missing input file - aborting.");
    process.exit(1); // Exit with non-zero status on failure
}

// Run interpolation search benchmarks
runTests(arr);

// Export functions for unit tests or reuse in other modules
module.exports = {
    interpolationSearch,
    interpolationSearchSteps
};
