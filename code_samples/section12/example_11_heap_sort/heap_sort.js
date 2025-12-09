// ----------------------------------------
// Heap Sort with Step Counting (JavaScript)
// ----------------------------------------

function heapSort(arr, stats) {
    const n = arr.length;

    // Heapify subtree at index i
    function heapify(n, i) {
        let largest = i;
        const left = 2 * i + 1;
        const right = 2 * i + 2;

        if (left < n) {
            stats.comparisons++;
            if (arr[left] > arr[largest]) largest = left;
        }
        if (right < n) {
            stats.comparisons++;
            if (arr[right] > arr[largest]) largest = right;
        }

        if (largest !== i) {
            // Swap
            const temp = arr[i];
            arr[i] = arr[largest];
            arr[largest] = temp;
            stats.writes += 3;

            heapify(n, largest);
        }
    }

    // Build max heap
    for (let i = Math.floor(n / 2) - 1; i >= 0; i--) {
        heapify(n, i);
    }

    // Extract from heap
    for (let end = n - 1; end > 0; end--) {
        // Swap root with last element
        const temp = arr[0];
        arr[0] = arr[end];
        arr[end] = temp;
        stats.writes += 3;

        heapify(end, 0);
    }
}

// ----------------------------------------
// File loader with robust path search
// ----------------------------------------

import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";

// resolve __dirname for ES modules
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

function loadFile(filename) {
    const attempts = [
        path.join(__dirname, filename),
        path.join(__dirname, "data", filename),
        path.join(__dirname, "..", "data", filename),
        path.join(__dirname, "..", "..", "data", filename),

        // repo-root based locations
        path.join(__dirname, "..", "section12", "data", filename),
        path.join(__dirname, "..", "..", "section12", "data", filename),
        path.join(__dirname, "..", "data", filename),
        path.join(__dirname, "..", "..", "data", filename),
    ];

    for (const p of attempts) {
        if (fs.existsSync(p)) {
            console.log("Loaded:", p);
            const content = fs.readFileSync(p, "utf8").trim();
            return content
                .split(/\s+/)
                .filter(x => x.length > 0)
                .map(Number);
        }
    }

    console.log("Error reading:", filename);
    console.log("Working directory:", process.cwd());
    console.log("Search paths attempted:");
    for (const p of attempts) console.log("  " + p);

    console.log("Missing input file — aborting.");
    process.exit(1);
}

// ----------------------------------------
// Main
// ----------------------------------------

function main() {
    const unordered = loadFile("unordered.txt");
    const expected = loadFile("ordered.txt");

    if (unordered.length !== expected.length) {
        console.log("Length mismatch between unordered and ordered files.");
        process.exit(1);
    }

    const arr = [...unordered];

    const stats = { comparisons: 0, writes: 0 };
    heapSort(arr, stats);

    // Validate
    let ok = true;
    for (let i = 0; i < arr.length; i++) {
        if (arr[i] !== expected[i]) {
            console.log(
                `Mismatch at index ${i}: got ${arr[i]}, expected ${expected[i]}`
            );
            ok = false;
            break;
        }
    }

    console.log("\nHeap Sort (JavaScript)");
    console.log("----------------------");
    console.log("Elements:     ", arr.length);
    console.log("Comparisons:  ", stats.comparisons);
    console.log("Writes:       ", stats.writes);
    console.log("Correct?      ", ok ? "YES ✓" : "NO ✗");
}

main();
