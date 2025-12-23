const fs = require("fs");       // Provides file system operations (readFileSync)
const path = require("path");   // Provides utilities for resolving file paths

// ======================================================
// Jump Search (index-only version)
// Requires: sorted array
// ======================================================

/**
 * Performs jump search on a sorted array of numbers.
 *
 * Jump search works in two phases:
 *  1) Jump phase: move forward in blocks of size sqrt(n) to find
 *     a range where the target could exist.
 *  2) Linear search phase: scan linearly within that range.
 *
 * @param {number[]} arr   Sorted array to search
 * @param {number} target Value to locate
 *
 * @returns {number} Index of the target if found; otherwise -1
 */
function jumpSearch(arr, target) {
    const n = arr.length;        // Number of elements in the array
    if (n === 0) return -1;      // Empty array => not found

    // Determine jump (block) size
    const step = Math.floor(Math.sqrt(n));

    let prev = 0;                // Start index of the current block
    let curr = step;             // End index of the current block

    // --------------------------------------------------
    // Jump phase: find block where target could be
    // --------------------------------------------------
    // Continue jumping while:
    //  - curr is within array bounds
    //  - the value at arr[curr] is still less than the target
    while (curr < n && arr[curr] < target) {
        prev = curr;             // Advance block start
        curr += step;            // Jump to next block end
    }

    // Clamp curr to the last valid index if we overshoot
    if (curr >= n) curr = n - 1;

    // --------------------------------------------------
    // Linear search within [prev, curr]
    // --------------------------------------------------
    for (let i = prev; i <= curr; i++) {
        if (arr[i] === target) {
            return i;            // Target found
        }
    }

    // Target not found
    return -1;
}

// ======================================================
// Jump Search with step counting
// steps = number of comparisons performed
// ======================================================

/**
 * Performs jump search while counting the number of comparisons.
 *
 * Comparisons are counted in both phases:
 *  - Jump phase comparisons against arr[curr]
 *  - Linear scan comparisons against arr[i]
 *
 * @param {number[]} arr   Sorted array to search
 * @param {number} target Value to locate
 *
 * @returns {{index: number, steps: number}}
 *          Object containing:
 *           - index: position of the target, or -1 if not found
 *           - steps: number of comparisons performed
 */
function jumpSearchSteps(arr, target) {
    const n = arr.length;   // Number of elements
    let steps = 0;          // Counts total number of comparisons

    // Handle empty array case
    if (n === 0) {
        return { index: -1, steps };
    }

    // Determine jump (block) size
    const step = Math.floor(Math.sqrt(n));

    let prev = 0;           // Start index of the current block
    let curr = step;        // End index of the current block

    // --------------------------------------------------
    // Jump phase
    // --------------------------------------------------
    while (curr < n) {
        // Count comparison involving arr[curr] and target
        steps++; // comparison: arr[curr] < target OR arr[curr] >= target

        // If current block end is >= target, stop jumping
        if (arr[curr] >= target) break;

        // Otherwise, jump to the next block
        prev = curr;
        curr += step;
    }

    // Clamp curr to the last valid index if we overshoot
    if (curr >= n) curr = n - 1;

    // --------------------------------------------------
    // Linear search phase within [prev, curr]
    // --------------------------------------------------
    for (let i = prev; i <= curr; i++) {
        steps++; // comparison: arr[i] === target

        if (arr[i] === target) {
            return { index: i, steps }; // Target found
        }
    }

    // Target not found
    return { index: -1, steps };
}

// ======================================================
// Load ordered.txt (sorted integers) relative to this file
// Path: ../data/ordered.txt
// ======================================================

/**
 * Loads the ordered integer dataset from disk.
 *
 * The file path is resolved relative to this script's directory
 * (__dirname), not the process working directory.
 *
 * @returns {number[]} Array of integers read from ordered.txt,
 *                     or an empty array on failure
 */
function loadOrderedFile() {
    // Build platform-independent path to ordered.txt
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
        // Provide diagnostic output if file cannot be read
        console.error("Error reading ordered.txt:", e.message);
        return [];
    }
}

// ======================================================
// Run tests: first, middle, last, missing
// ======================================================

/**
 * Executes a standardized set of jump search tests on ordered data.
 *
 * @param {number[]} arr Sorted array of integers to test
 */
function runTests(arr) {
    console.log("\n=== Jump Search Tests (ordered data only) ===");
    console.log("Loaded", arr.length, "integers\n");

    // Test 1: Search for the first element
    let r = jumpSearchSteps(arr, arr[0]);
    console.log(`Search first  (${arr[0]}): index=${r.index}, steps=${r.steps}`);

    // Test 2: Search for the middle element
    const midVal = arr[Math.floor(arr.length / 2)];
    r = jumpSearchSteps(arr, midVal);
    console.log(`Search middle (${midVal}): index=${r.index}, steps=${r.steps}`);

    // Test 3: Search for the last element
    const lastVal = arr[arr.length - 1];
    r = jumpSearchSteps(arr, lastVal);
    console.log(`Search last   (${lastVal}): index=${r.index}, steps=${r.steps}`);

    // Test 4: Search for a value guaranteed not to exist
    r = jumpSearchSteps(arr, 999_999);
    console.log(`Search missing (999999): index=${r.index}, steps=${r.steps}`);
}

// ======================================================
// MAIN
// ======================================================

// Load ordered dataset from disk
const arr = loadOrderedFile();

// Validate successful file load
if (arr.length === 0) {
    console.log("Missing input file - aborting.");
    process.exit(1); // Exit with non-zero status on failure
}

// Run jump search benchmarks
runTests(arr);

// Export functions if you want to import them in other modules or tests
module.exports = { jumpSearch, jumpSearchSteps };
