// ----------------------------------------
// Example 11 - Heap Sort with Step Counting (JavaScript / Node.js)
// ----------------------------------------
//
// PURPOSE
// -------
// Demonstrate Heap Sort on an array of integers while counting steps, so you can
// *measure* the algorithm’s work and connect it to Big-O complexity.
//
// INPUT FILES
// -----------
// This script expects TWO text files containing the SAME integers:
//   - unordered.txt : the integers in unsorted order
//   - ordered.txt   : the same integers sorted ascending
//
// Integers can be separated by any whitespace (spaces, newlines, tabs).
//
// WHAT WE COUNT
// -------------
// stats.comparisons
//   - Counts value comparisons during heapify, specifically:
//       arr[left]  > arr[largest]
//       arr[right] > arr[largest]
//   - We do NOT count loop condition checks or index bounds checks.
//   - This keeps the count focused on “data comparisons” used to order values.
//
// stats.writes
//   - Counts array writes due to swapping.
//   - We treat a swap as 3 writes (temp, arr[a], arr[b]) to match the
//     convention used in your C/C++/Java/C# examples.
//
// HEAP SORT OVERVIEW (MAX-HEAP)
// ----------------------------
// Heap Sort works in two main phases:
//
// 1) Build a MAX-HEAP
//    - Arrange the array so the largest value is at index 0,
//      and every parent is >= its children.
//
// 2) Repeatedly extract the max
//    - Swap index 0 (max) with the last element of the heap,
//      shrink the heap by one, and heapify index 0 again.
//
// Complexity:
//   - Time:  O(n log n)
//   - Space: O(1) extra (in-place), aside from file loading.
//
// NOTE ABOUT MUTATION
// -------------------
// heapSort() sorts the array in place. In main() we sort a copy of `unordered`
// to preserve the original input array.
//
// ----------------------------------------
// Heap Sort with Step Counting
// ----------------------------------------

function heapSort(arr, stats) {
    const n = arr.length;

    // ----------------------------------------
    // heapify(heapSize, i)
    // ----------------------------------------
    // Restores the MAX-HEAP property for the subtree rooted at index i,
    // assuming the subtrees below i are already valid heaps.
    //
    // Parameters:
    //   heapSize : only indices [0 .. heapSize-1] are considered in the heap
    //   i        : the root index of the subtree to fix
    //
    // Algorithm:
    //   - Find the largest among: parent (i), left child, right child
    //   - If a child is larger than parent, swap and recurse downward
    function heapify(heapSize, i) {
        let largest = i;              // assume parent is largest
        const left = 2 * i + 1;       // left child index
        const right = 2 * i + 2;      // right child index

        // Compare left child (if it exists) with current largest
        if (left < heapSize) {
            stats.comparisons++; // counting arr[left] > arr[largest]
            if (arr[left] > arr[largest]) largest = left;
        }

        // Compare right child (if it exists) with current largest
        if (right < heapSize) {
            stats.comparisons++; // counting arr[right] > arr[largest]
            if (arr[right] > arr[largest]) largest = right;
        }

        // If the parent is not the largest, swap and continue heapifying
        if (largest !== i) {
            // Swap arr[i] and arr[largest]
            const temp = arr[i];
            arr[i] = arr[largest];
            arr[largest] = temp;

            // Count swap as 3 writes for consistency with your other languages
            stats.writes += 3;

            // After swapping, the affected child subtree may violate heap rules
            heapify(heapSize, largest);
        }
    }

    // ----------------------------------------
    // Phase 1: Build the max heap
    // ----------------------------------------
    // The last index that can have children is floor(n/2) - 1.
    // We heapify from that index down to 0.
    for (let i = Math.floor(n / 2) - 1; i >= 0; i--) {
        heapify(n, i);
    }

    // ----------------------------------------
    // Phase 2: Extract max repeatedly
    // ----------------------------------------
    // Each iteration:
    //   - Swap max at index 0 with arr[end]
    //   - Reduce heap size to `end`
    //   - Heapify root again
    for (let end = n - 1; end > 0; end--) {
        // Swap root (max) with last element in current heap
        const temp = arr[0];
        arr[0] = arr[end];
        arr[end] = temp;
        stats.writes += 3; // count this swap as 3 writes

        // Restore heap property on the reduced heap [0 .. end-1]
        heapify(end, 0);
    }
}

// ----------------------------------------
// File loader with robust path search
// ----------------------------------------
//
// We try multiple likely locations because Node’s working directory can vary
// depending on how the script is launched (VS Code, terminal, lesson folder, etc).
//
// IMPORTANT:
// - __dirname is the folder containing THIS script file (ES module style).
// - The search list includes common sibling/parent "data" locations.
//
// If no path works, we print diagnostics and exit with failure code.

import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";

// Resolve __dirname for ES modules
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

function loadFile(filename) {
    const attempts = [
        // Same folder as the script
        path.join(__dirname, filename),

        // ./data relative to script
        path.join(__dirname, "data", filename),

        // ../data and ../../data relative to script (common layouts)
        path.join(__dirname, "..", "data", filename),
        path.join(__dirname, "..", "..", "data", filename),

        // Possible repo structures for section12
        path.join(__dirname, "..", "section12", "data", filename),
        path.join(__dirname, "..", "..", "section12", "data", filename),

        // (These two are duplicates of earlier entries in your original list,
        // but kept here to preserve your intent; they’re harmless.)
        path.join(__dirname, "..", "data", filename),
        path.join(__dirname, "..", "..", "data", filename),
    ];

    for (const p of attempts) {
        try {
            if (fs.existsSync(p)) {
                console.log("Loaded:", p);

                // Read file as text, split on ANY whitespace, parse as numbers
                const content = fs.readFileSync(p, "utf8").trim();
                return content
                    .split(/\s+/)
                    .filter((x) => x.length > 0)
                    .map(Number);
            }
        } catch (err) {
            // If something goes wrong (permissions, etc.), try the next path.
        }
    }

    // If we reach here, no path worked: print helpful debugging output.
    console.log("Error reading:", filename);
    console.log("Working directory:", process.cwd());
    console.log("Search paths attempted:");
    for (const p of attempts) console.log("  " + p);

    console.log("Missing input file — aborting.");
    process.exit(1);
}

// ----------------------------------------
// Main test harness
// ----------------------------------------
//
// 1) Load unordered + expected arrays from files.
// 2) Sort a COPY of unordered using heap sort.
// 3) Compare element-by-element to expected.
// 4) Print stats and correctness result.

function main() {
    const unordered = loadFile("unordered.txt");
    const expected = loadFile("ordered.txt");

    // Basic sanity check: both files should contain the same number of ints.
    if (unordered.length !== expected.length) {
        console.log("Length mismatch between unordered and ordered files.");
        console.log("unordered length:", unordered.length);
        console.log("expected  length:", expected.length);
        process.exit(1);
    }

    // Sort a copy so we don't mutate the original unordered input
    const arr = [...unordered];

    // Step counters (reset for this run)
    const stats = { comparisons: 0, writes: 0 };

    // Run heap sort (in-place)
    heapSort(arr, stats);

    // Validate: compare sorted result to expected output
    let ok = true;
    let badIndex = -1;

    for (let i = 0; i < arr.length; i++) {
        if (arr[i] !== expected[i]) {
            ok = false;
            badIndex = i;
            break;
        }
    }

    // Print report
    console.log("\nHeap Sort (JavaScript)");
    console.log("----------------------");
    console.log("Elements:     ", arr.length);
    console.log("Comparisons:  ", stats.comparisons);
    console.log("Writes:       ", stats.writes);
    console.log("Correct?      ", ok ? "YES ✓" : "NO ✗");

    // If incorrect, print the first mismatch to help debugging
    if (!ok) {
        console.log(
            `First mismatch at index ${badIndex}: got ${arr[badIndex]}, expected ${expected[badIndex]}`
        );
    } else {
        // Optional: show a small sample of sorted output
        console.log("\nFirst 10 sorted values:", arr.slice(0, 10).join(" "));
    }
}

main();
