import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";

// Resolve __dirname for ES modules
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// ================================
// Selection Sort with step counting
// ================================
function selectionSort(arr, stats) {
    const n = arr.length;
    stats.comparisons = 0;
    stats.swaps = 0;

    for (let i = 0; i < n; i++) {
        let minIdx = i;
        for (let j = i + 1; j < n; j++) {
            stats.comparisons++; // count comparison
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        if (minIdx !== i) {
            [arr[i], arr[minIdx]] = [arr[minIdx], arr[i]];
            stats.swaps++;
        }
    }
}

// ================================
// File loader (relative to this .js)
// ================================
function loadFile(filename) {
    try {
        // data folder is a sibling of this lesson folder
        const full = path.resolve(__dirname, "..", "data", filename);
        console.log(`Attempting to read: ${full}`);

        const text = fs.readFileSync(full, "utf-8");
        return text
            .trim()
            .split(/\s+/)
            .map((x) => parseInt(x, 10));
    } catch (err) {
        console.log(`Error reading: ${filename}`);
        console.error(err);
        return null;
    }
}

// ================================
// Test Harness
// ================================
(function main() {
    const unordered = loadFile("unordered.txt");
    if (!unordered) {
        console.log("Missing unordered file - aborting.");
        return;
    }

    const ordered = loadFile("ordered.txt");
    if (!ordered) {
        console.log("Missing ordered file - aborting.");
        return;
    }

    if (unordered.length !== ordered.length) {
        console.log("File lengths differ!");
        console.log("unordered =", unordered.length);
        console.log("ordered   =", ordered.length);
        return;
    }

    const stats = {};
    console.log("\n--- Selection Sort (Step Counting) ---");
    selectionSort(unordered, stats);

    console.log("Comparisons:", stats.comparisons);
    console.log("Swaps:", stats.swaps);

    console.log("\nComparing sorted output to ordered.txt...");
    let mismatches = 0;

    for (let i = 0; i < unordered.length; i++) {
        if (unordered[i] !== ordered[i]) {
            if (mismatches < 10) {
                console.log(
                    `Mismatch at index ${i}: got ${unordered[i]}, expected ${ordered[i]}`
                );
            }
            mismatches++;
        }
    }

    if (mismatches === 0) {
        console.log("SUCCESS: Sorted output matches ordered.txt");
    } else {
        console.log(`FAIL: ${mismatches} mismatches found.`);
    }
})();
