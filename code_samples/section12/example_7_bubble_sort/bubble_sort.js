function bubbleSort(arr) {
    let n = arr.length;
    let swapped = true;
    while (swapped) {
        swapped = false;
        for (let i = 1; i < n; i++) {
            if (arr[i - 1] > arr[i]) {
                [arr[i - 1], arr[i]] = [arr[i], arr[i - 1]];
                swapped = true;
            }
        }
        n--;
    }
}

// -------------------------------------------------------------
// Bubble Sort with step counting
// -------------------------------------------------------------
function bubbleSortSteps(arr, stats) {
    let n = arr.length;
    let swapped = true;

    while (swapped) {
        swapped = false;
        for (let i = 1; i < n; i++) {
            stats.comparisons++;
            if (arr[i - 1] > arr[i]) {
                [arr[i - 1], arr[i]] = [arr[i], arr[i - 1]];
                stats.swaps++;
                swapped = true;
            }
        }
        n--; // last element is now in place
    }
}

// -------------------------------------------------------------
// File loading with correct project-root detection
// -------------------------------------------------------------
import fs from "fs";
import path from "path";
import url from "url";

function findProjectRoot() {
    // __dirname for ES modules
    const __dirname = path.dirname(url.fileURLToPath(import.meta.url));
    let cur = path.resolve(__dirname);

    console.log("Current directory:", cur);

    while (cur !== path.parse(cur).root) {
        if (fs.existsSync(path.join(cur, "code_samples"))) {
            console.log("Detected project root:", cur);
            return cur;
        }
        cur = path.dirname(cur);
    }

    // fallback
    return path.resolve(__dirname);
}

function loadFile(filename) {
    try {
        const projectRoot = findProjectRoot();
        const full = path.join(
            projectRoot,
            "code_samples",
            "section12",
            "data",
            filename
        );

        console.log("Attempting to read:", full);

        const text = fs.readFileSync(full, "utf8");
        return text
            .trim()
            .split(/\s+/)
            .map(Number);

    } catch (err) {
        console.error("Error reading:", filename);
        console.error(err.message);
        return null;
    }
}

// -------------------------------------------------------------
// Compare arrays for mismatches
// -------------------------------------------------------------
function compareArrays(a, b) {
    let mismatches = 0;
    for (let i = 0; i < a.length; i++) {
        if (a[i] !== b[i]) {
            if (mismatches < 10) {
                console.log(
                    `Mismatch at index ${i}: got ${a[i]}, expected ${b[i]}`
                );
            }
            mismatches++;
        }
    }
    return mismatches;
}

// -------------------------------------------------------------
// Main test driver
// -------------------------------------------------------------
function main() {
    const unordered = loadFile("unordered.txt");
    const expected = loadFile("ordered.txt");

    if (!unordered || !expected) {
        console.log("Missing input file — aborting.");
        return;
    }

    if (unordered.length !== expected.length) {
        console.log("File size mismatch — aborting.");
        console.log("unordered length:", unordered.length);
        console.log("expected  length:", expected.length);
        return;
    }

    const stats = { comparisons: 0, swaps: 0 };

    console.log("\n--- Bubble Sort Step Count ---");
    bubbleSortSteps(unordered, stats);

    console.log("Comparisons:", stats.comparisons);
    console.log("Swaps:", stats.swaps);

    console.log("\nComparing result with ordered.txt...");
    const mismatches = compareArrays(unordered, expected);

    if (mismatches === 0) {
        console.log("SUCCESS — output matches!");
    } else {
        console.log("FAIL — mismatches found:", mismatches);
    }
}

main();
