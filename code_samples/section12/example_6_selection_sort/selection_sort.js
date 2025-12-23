import fs from "fs";                   // File system access
import path from "path";               // Path utilities
import { fileURLToPath } from "url";   // Convert module URL to file path

// ------------------------------------------------------------
// Resolve __dirname for ES modules
// ------------------------------------------------------------
//
// In ES modules, __dirname is not defined by default.
// This pattern reconstructs it using the module's URL.
//
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// ================================
// Selection Sort with step counting
// ================================
//
// Implements classic selection sort:
//
//   For each index i:
//     - Find the smallest element in arr[i .. n-1]
//     - Swap it into position i
//
// Step counting:
//   - stats.comparisons counts element-to-element comparisons
//   - stats.swaps counts actual swap operations
//
// The array is sorted IN PLACE.
//
function selectionSort(arr, stats) {
    const n = arr.length;

    // Initialize statistics
    stats.comparisons = 0;
    stats.swaps = 0;

    // Outer loop selects the next position to fill
    for (let i = 0; i < n; i++) {
        let minIdx = i;

        // Inner loop finds the smallest element
        // in the unsorted suffix [i+1 .. n-1]
        for (let j = i + 1; j < n; j++) {
            stats.comparisons++; // count each comparison
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }

        // Swap only if a smaller element was found
        if (minIdx !== i) {
            // Destructuring assignment swap
            [arr[i], arr[minIdx]] = [arr[minIdx], arr[i]];
            stats.swaps++; // count actual swap
        }
    }
}

// ================================
// File loader (relative to this .js)
// ================================
//
// Loads a whitespace-delimited list of integers from:
//
//   ../data/<filename>
//
// relative to the location of THIS JavaScript file.
//
// Returns:
//   - Array<number> on success
//   - null on failure
//
function loadFile(filename) {
    try {
        // Data folder is a sibling of this lesson folder
        const full = path.resolve(__dirname, "..", "data", filename);

        // Diagnostic output showing the exact path used
        console.log(`Attempting to read: ${full}`);

        // Read entire file contents
        const text = fs.readFileSync(full, "utf-8");

        // Split on any whitespace and parse integers
        return text
            .trim()
            .split(/\s+/)
            .map((x) => parseInt(x, 10));
    } catch (err) {
        // Handles missing files, permission errors, etc.
        console.log(`Error reading: ${filename}`);
        console.error(err);
        return null;
    }
}

// ================================
// Test Harness
// ================================
//
// Orchestrates:
//   1) Loading unordered.txt
//   2) Loading ordered.txt (reference correct output)
//   3) Running selection sort with step counting
//   4) Verifying sorted output matches ordered.txt
//
(function main() {
    // --------------------------------------------------
    // Load unordered input
    // --------------------------------------------------
    const unordered = loadFile("unordered.txt");
    if (!unordered) {
        console.log("Missing unordered file - aborting.");
        return;
    }

    // --------------------------------------------------
    // Load ordered reference data
    // --------------------------------------------------
    const ordered = loadFile("ordered.txt");
    if (!ordered) {
        console.log("Missing ordered file - aborting.");
        return;
    }

    // Sanity check: both files must have same length
    if (unordered.length !== ordered.length) {
        console.log("File lengths differ!");
        console.log("unordered =", unordered.length);
        console.log("ordered   =", ordered.length);
        return;
    }

    // --------------------------------------------------
    // Run selection sort
    // --------------------------------------------------
    const stats = {};
    console.log("\n--- Selection Sort (Step Counting) ---");
    selectionSort(unordered, stats);

    console.log("Comparisons:", stats.comparisons);
    console.log("Swaps:", stats.swaps);

    // --------------------------------------------------
    // Verify correctness against ordered.txt
    // --------------------------------------------------
    console.log("\nComparing sorted output to ordered.txt...");
    let mismatches = 0;

    for (let i = 0; i < unordered.length; i++) {
        if (unordered[i] !== ordered[i]) {
            // Print only the first few mismatches
            if (mismatches < 10) {
                console.log(
                    `Mismatch at index ${i}: got ${unordered[i]}, expected ${ordered[i]}`
                );
            }
            mismatches++;
        }
    }

    // Final verdict
    if (mismatches === 0) {
        console.log("SUCCESS: Sorted output matches ordered.txt");
    } else {
        console.log(`FAIL: ${mismatches} mismatches found.`);
    }
})();
