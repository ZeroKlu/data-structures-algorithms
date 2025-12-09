// ------------------------------------------------------------
// Example 10 - Quick Sort (JavaScript)
// ------------------------------------------------------------

import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";

// Track steps
let comparisons = 0;
let writes = 0;

// ------------------------------------------------------------
// Quick Sort Implementation
// ------------------------------------------------------------
function quickSort(arr) {
    const copy = arr.slice(); // avoid mutating input
    quickSortRec(copy, 0, copy.length - 1);
    return copy;
}

function quickSortRec(arr, left, right) {
    if (left >= right) return;

    const p = partition(arr, left, right);
    quickSortRec(arr, left, p - 1);
    quickSortRec(arr, p + 1, right);
}

function partition(arr, left, right) {
    let pivot = arr[right];
    let i = left;

    for (let j = left; j < right; j++) {
        comparisons++;
        if (arr[j] < pivot) {
            [arr[i], arr[j]] = [arr[j], arr[i]];
            writes += 2;
            i++;
        }
    }

    // Final swap
    [arr[i], arr[right]] = [arr[right], arr[i]];
    writes += 2;

    return i;
}

// ------------------------------------------------------------
// File Loader with Robust Path Searching
// ------------------------------------------------------------

function loadFile(filename) {
    const __filename = fileURLToPath(import.meta.url);
    const __dirname = path.dirname(__filename);

    const searchPaths = [
        path.join(__dirname, filename),
        path.join(__dirname, "data", filename),
        path.join(__dirname, "..", "data", filename),
        path.join(__dirname, "..", "section12", "data", filename),
        path.join(__dirname, "..", "..", "section12", "data", filename),

        // Also support running from project root
        path.join(process.cwd(), "code_samples", "section12", "data", filename),
        path.join(process.cwd(), "data", filename)
    ];

    const tried = [];

    for (const p of searchPaths) {
        tried.push(p);
        if (fs.existsSync(p)) {
            console.log("Loaded:", p);
            const text = fs.readFileSync(p, "utf-8");
            return text.trim().split(/\s+/).map(Number);
        }
    }

    console.error("Error reading:", filename);
    console.log("Working directory:", process.cwd());
    console.log("Search paths attempted:");
    for (const t of tried) console.log("  " + t);
    console.error("Missing input file — aborting.");
    process.exit(1);
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
function main() {
    const unordered = loadFile("unordered.txt");
    const expected = loadFile("ordered.txt");

    comparisons = 0;
    writes = 0;

    const result = quickSort(unordered);

    const correct =
        result.length === expected.length &&
        result.every((v, i) => v === expected[i]);

    console.log("\nQuick Sort (JavaScript)");
    console.log("-----------------------");
    console.log("Elements:     ", unordered.length);
    console.log("Comparisons:  ", comparisons);
    console.log("Writes:       ", writes);
    console.log("Correct?      ", correct ? "YES ✔" : "NO ✘");

    if (!correct) {
        console.log("First few mismatches:");
        for (let i = 0; i < 20 && i < unordered.length; i++) {
            if (result[i] !== expected[i]) {
                console.log(`  index ${i}: got ${result[i]}, expected ${expected[i]}`);
                break;
            }
        }
    }
}

main();
