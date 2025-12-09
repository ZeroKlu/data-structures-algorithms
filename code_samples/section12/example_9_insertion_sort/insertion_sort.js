// insertion_sort.js
// Node.js script for insertion sort with step counting and file tests

import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";

// Resolve directory of this script
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// ------------------------------------------------------------
// Insertion Sort with stats
// ------------------------------------------------------------
function insertionSort(arr) {
    let comparisons = 0;
    let writes = 0;

    for (let i = 1; i < arr.length; i++) {
        let key = arr[i];
        let j = i - 1;

        while (j >= 0) {
            comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                writes++;
                j--;
            } else {
                break;
            }
        }

        arr[j + 1] = key;
        writes++;
    }

    return { comparisons, writes };
}

// ------------------------------------------------------------
// File loader with correct fallback paths
// ------------------------------------------------------------
function loadFile(name) {
    const attempts = [
        path.join(__dirname, name),
        path.join(__dirname, "data", name),
        path.join(__dirname, "..", "data", name),
        path.join(__dirname, "..", "..", "data", name),

        // Your known actual folder:
        path.join(
            process.env.USERPROFILE || process.env.HOME,
            "Desktop",
            "data-structures-algorithms",
            "code_samples",
            "section12",
            "data",
            name
        )
    ];

    for (const p of attempts) {
        try {
            const text = fs.readFileSync(p, "utf8");
            console.log("Loaded:", p);
            return text
                .trim()
                .split(/\s+/)
                .map(Number);
        } catch (e) {
            // try next
        }
    }

    console.error("Error: Could not load", name);
    console.error("Search paths were:");
    for (const p of attempts) console.error("  " + p);

    process.exit(1);
}

// ------------------------------------------------------------
// Test harness
// ------------------------------------------------------------
function main() {
    const unordered = loadFile("unordered.txt");
    const expected  = loadFile("ordered.txt");

    const arr = [...unordered];
    const stats = insertionSort(arr);

    // Verify correctness
    let mismatch = -1;
    for (let i = 0; i < arr.length; i++) {
        if (arr[i] !== expected[i]) {
            mismatch = i;
            break;
        }
    }

    console.log("\nInsertion Sort (JavaScript)");
    console.log("---------------------------");
    console.log("Elements:     ", arr.length);
    console.log("Comparisons:  ", stats.comparisons);
    console.log("Writes:       ", stats.writes);

    if (mismatch === -1) {
        console.log("Result:       PASS ✔");
    } else {
        console.log("Result:       FAIL ✘");
        console.log(
            `Mismatch at index ${mismatch}: got ${arr[mismatch]}, expected ${expected[mismatch]}`
        );
    }
}

main();
