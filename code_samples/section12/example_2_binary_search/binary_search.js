const fs = require("fs");       // Provides file system operations (readFileSync)
const path = require("path");   // Provides utilities for resolving file paths

// ------------------------------------------------------
// Original binary search
// ------------------------------------------------------

/**
 * Performs a standard binary search on a sorted array.
 *
 * @param {number[]} arr   Sorted array to search
 * @param {number} target Value to locate
 *
 * @returns {number} Index of the target if found; otherwise -1
 */
function binarySearch(arr, target) {
    let lo = 0;                  // Lower bound of search range (inclusive)
    let hi = arr.length - 1;     // Upper bound of search range (inclusive)

    // Continue searching while the range is valid
    while (lo <= hi) {
        // Compute midpoint safely
        const mid = Math.floor(lo + (hi - lo) / 2);

        // Check if midpoint element matches the target
        if (arr[mid] === target) return mid;

        // Decide which half of the array to continue searching
        if (arr[mid] < target) lo = mid + 1;   // Target lies in the right half
        else hi = mid - 1;                      // Target lies in the left half
    }

    // Target was not found in the array
    return -1;
}

// ------------------------------------------------------
// Step-counting version
// ------------------------------------------------------

/**
 * Performs a binary search while counting the number of iterations.
 *
 * @param {number[]} arr   Sorted array to search
 * @param {number} target Value to locate
 *
 * @returns {{index: number, steps: number}}
 *          Object containing the index of the target (or -1 if not found)
 *          and the number of loop iterations performed
 */
function binarySearchSteps(arr, target) {
    let lo = 0;                  // Lower bound of search range
    let hi = arr.length - 1;     // Upper bound of search range
    let steps = 0;               // Counts number of loop iterations

    // Continue searching while the range is valid
    while (lo <= hi) {
        steps++;  // Count this iteration / comparison step

        // Compute midpoint of the current search range
        const mid = Math.floor(lo + (hi - lo) / 2);

        // Check for a match at the midpoint
        if (arr[mid] === target) return { index: mid, steps };

        // Decide which half of the array to search next
        if (arr[mid] < target) lo = mid + 1;   // Target lies in the right half
        else hi = mid - 1;                      // Target lies in the left half
    }

    // Target was not found after exhausting search range
    return { index: -1, steps };
}

// ------------------------------------------------------
// Load ordered.txt RELATIVE TO THIS SCRIPT'S FOLDER
// ------------------------------------------------------

/**
 * Loads the ordered integer dataset from disk.
 *
 * The file path is resolved relative to this script's directory (__dirname),
 * not the process working directory.
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

        // Split on whitespace, filter empty entries, and convert to numbers
        return text.split(/\s+/).filter(Boolean).map(Number);
    } catch (e) {
        // Provide diagnostic output if file cannot be read
        console.error("Error reading ordered.txt:", e.message);
        return []; // Return empty array to signal failure
    }
}

// ------------------------------------------------------
// Test runner
// ------------------------------------------------------

/**
 * Executes a standardized set of binary search tests on a dataset.
 *
 * @param {number[]} arr Sorted array of integers to test
 */
function runTests(arr) {
    console.log("\n=== Binary Search Tests ===");
    console.log("Loaded", arr.length, "integers\n");

    // Test 1: Search for the first element (best-case scenario)
    let result = binarySearchSteps(arr, arr[0]);
    console.log(`Search first (${arr[0]}): index=${result.index}, steps=${result.steps}`);

    // Test 2: Search for the middle element
    const midVal = arr[Math.floor(arr.length / 2)];
    result = binarySearchSteps(arr, midVal);
    console.log(`Search middle (${midVal}): index=${result.index}, steps=${result.steps}`);

    // Test 3: Search for the last element (worst-case successful search)
    result = binarySearchSteps(arr, arr[arr.length - 1]);
    console.log(`Search last (${arr[arr.length - 1]}): index=${result.index}, steps=${result.steps}`);

    // Test 4: Search for a value guaranteed not to exist
    result = binarySearchSteps(arr, 999999);
    console.log(`Search missing (999999): index=${result.index}, steps=${result.steps}`);
}

// ------------------------------------------------------
// MAIN
// ------------------------------------------------------

// Load ordered dataset from disk
const arr = loadOrderedFile();

// Validate successful file load
if (arr.length === 0) {
    console.log("Missing input file - aborting.");
    process.exit(1); // Exit with non-zero status on failure
}

// Run binary search benchmarks
runTests(arr);
