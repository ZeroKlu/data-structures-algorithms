// function mergeSort(arr) {
//     if (arr.length <= 1) return arr.slice();
//     const mid = Math.floor(arr.length / 2);
//     const left = mergeSort(arr.slice(0, mid));
//     const right = mergeSort(arr.slice(mid));
//     return merge(left, right);
// }

// function merge(left, right) {
//     const result = [];
//     let i = 0, j = 0;
//     while (i < left.length && j < right.length) {
//         if (left[i] <= right[j]) {
//             result.push(left[i++]);
//         } else {
//             result.push(right[j++]);
//         }
//     }
//     return result.concat(left.slice(i)).concat(right.slice(j));
// }

// Example 8 - Merge Sort with Step Counting
import fs from "fs";
import path from "path";
import url from "url";

// Step counters
let comparisons = 0;
let writes = 0;

/** Merge two sorted arrays */
function merge(left, right) {
    const result = [];
    let i = 0, j = 0;

    while (i < left.length && j < right.length) {
        comparisons++;   // count comparison left[i] <= right[j]

        if (left[i] <= right[j]) {
            result.push(left[i++]);
        } else {
            result.push(right[j++]);
        }
        writes++; // wrote a value into result[]
    }

    // remaining elements in left
    while (i < left.length) {
        result.push(left[i++]);
        writes++;
    }

    // remaining elements in right
    while (j < right.length) {
        result.push(right[j++]);
        writes++;
    }

    return result;
}

/** Recursive merge sort */
function mergeSort(arr) {
    if (arr.length <= 1) return arr.slice();

    const mid = Math.floor(arr.length / 2);
    const left = mergeSort(arr.slice(0, mid));
    const right = mergeSort(arr.slice(mid));

    return merge(left, right);
}

// ---------------------------------------------------------------------------
// File Loading Utility (same logic as bubble/selection sort JS examples)
// ---------------------------------------------------------------------------

/**
 * Robust file loader:
 *  1) Try ../data/<file>
 *  2) Try project-root/data/<file>
 */
function loadFile(name) {
    const __dirname = path.dirname(url.fileURLToPath(import.meta.url));

    // 1: relative: ..\data\name
    const rel = path.resolve(__dirname, "..", "data", name);
    console.log("Attempting to read:", rel);
    if (fs.existsSync(rel)) {
        return fs.readFileSync(rel, "utf8")
            .trim()
            .split(/\s+/)
            .map(Number);
    }

    // 2: explicit project path (your known layout)
    const explicit = path.resolve(
        process.env.USERPROFILE,
        "Desktop",
        "data-structures-algorithms",
        "code_samples",
        "section12",
        "data",
        name
    );
    console.log("Relative path not found, trying:", explicit);

    if (fs.existsSync(explicit)) {
        return fs.readFileSync(explicit, "utf8")
            .trim()
            .split(/\s+/)
            .map(Number);
    }

    throw new Error(`Missing file '${name}' in:\n  ${rel}\n  ${explicit}`);
}

// ---------------------------------------------------------------------------
// Main Execution
// ---------------------------------------------------------------------------

function main() {
    try {
        const unordered = loadFile("unordered.txt");
        const expected = loadFile("ordered.txt");

        console.log("Unordered length:", unordered.length);
        console.log("Expected length:", expected.length);

        comparisons = 0;
        writes = 0;

        const sorted = mergeSort(unordered);

        const correct = JSON.stringify(sorted) === JSON.stringify(expected);
        console.log("Sorted correctly?", correct);

        console.log("Comparisons:", comparisons);
        console.log("Writes:", writes);

        // Show a sample
        console.log("First 10 sorted elements:", sorted.slice(0, 10).join(" "));
    }
    catch (err) {
        console.error("Error reading input files:");
        console.error(err.message);
        console.log("Missing input file — aborting.");
    }
}

main();

