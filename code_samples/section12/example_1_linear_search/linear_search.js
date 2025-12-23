const fs = require("fs");       // Provides file system operations (readFileSync)
const path = require("path");   // Provides utilities for handling file paths

// ===============================
// Linear Search (given)
// ===============================

/**
 * Performs a basic linear search on an array.
 *
 * @param {number[]} arr   Array to search
 * @param {number} target Value to locate
 *
 * @returns {number} Index of the target if found, otherwise -1
 */
function linearSearch(arr, target) {
    // Iterate through the array sequentially
    for (let i = 0; i < arr.length; i++) {

        // Check whether the current element matches the target
        if (arr[i] === target) return i;
    }

    // Target was not found in the array
    return -1;
}

// ===============================
// Linear Search with step counting
// ===============================

/**
 * Performs a linear search while counting comparison steps.
 *
 * @param {number[]} arr   Array to search
 * @param {number} target Value to locate
 *
 * @returns {{index: number, steps: number}}
 *          Object containing the index of the target (or -1 if not found)
 *          and the number of comparisons performed
 */
function linearSearchSteps(arr, target) {
    let steps = 0; // Counts how many comparisons are performed

    // Iterate through the array sequentially
    for (let i = 0; i < arr.length; i++) {
        steps++; // Count the comparison arr[i] === target

        // Check if the current element matches the target
        if (arr[i] === target) {
            return { index: i, steps }; // Return index and step count
        }
    }

    // Target was not found after scanning entire array
    return { index: -1, steps };
}

// ===============================
// Load the data file
// ===============================

/**
 * Loads integers from a text file into an array.
 *
 * The path is resolved relative to the current file (__dirname),
 * not the process working directory.
 *
 * @param {string} relativePath Relative path to the input file
 *
 * @returns {number[]} Array of integers, or an empty array on error
 */
function loadFile(relativePath) {
    // Resolve the file path relative to this script's location
    const fullPath = path.join(__dirname, relativePath);

    try {
        // Read entire file contents as UTF-8 text
        const text = fs.readFileSync(fullPath, "utf8");

        // Split on whitespace, remove empty entries, and convert to numbers
        return text
            .split(/\s+/)
            .filter(Boolean)
            .map(Number);

    } catch (err) {
        // Provide diagnostic information if file cannot be read
        console.error("Error reading:", fullPath);
        console.error("Working directory:", process.cwd());
        return []; // Return empty array to signal failure
    }
}

// ===============================
// Run tests on one dataset
// ===============================

/**
 * Executes a standard set of linear search tests on a dataset.
 *
 * @param {string} name Descriptive name of the dataset (used in output)
 * @param {number[]} arr Integer array containing the dataset
 */
function runTestsOnDataset(name, arr) {
    console.log(`=== ${name} ===`);
    console.log(`Loaded ${arr.length} integers.\n`);

    // Test 1: Search for the first element (best-case scenario)
    let r1 = linearSearchSteps(arr, arr[0]);
    console.log(`Search first (${arr[0]}): index=${r1.index}, steps=${r1.steps}`);

    // Test 2: Search for the middle element (average-case scenario)
    let midVal = arr[Math.floor(arr.length / 2)];
    let r2 = linearSearchSteps(arr, midVal);
    console.log(`Search middle (${midVal}): index=${r2.index}, steps=${r2.steps}`);

    // Test 3: Search for the last element (worst-case successful search)
    let lastVal = arr[arr.length - 1];
    let r3 = linearSearchSteps(arr, lastVal);
    console.log(`Search last (${lastVal}): index=${r3.index}, steps=${r3.steps}`);

    // Test 4: Search for a value guaranteed not to exist
    let r4 = linearSearchSteps(arr, 999999);
    console.log(`Search missing (999999): index=${r4.index}, steps=${r4.steps}`);

    console.log();
}

// ===============================
// MAIN
// ===============================

// Data file paths are resolved relative to this script's directory,
// matching the behavior of the Java implementation
const ordered = loadFile("../data/ordered.txt");
const unordered = loadFile("../data/unordered.txt");

// Validate that both datasets were loaded successfully
if (ordered.length === 0 || unordered.length === 0) {
    console.log("Missing input files-see error messages above.");
    process.exit(1); // Exit with non-zero status on failure
}

// Run linear search benchmarks on both datasets
runTestsOnDataset("Ordered Data", ordered);
runTestsOnDataset("Unordered Data", unordered);
