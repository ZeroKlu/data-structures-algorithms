// Exponential Search with step counting (for sorted arrays)
function exponentialSearch(arr, target) {
    let steps = 0;
    const n = arr.length;
    if (n === 0) return { index: -1, steps };

    // Probe index 0
    steps++;
    if (arr[0] === target) {
        return { index: 0, steps };
    }

    // Find range by repeated doubling
    let bound = 1;
    while (bound < n) {
        steps++;
        if (arr[bound] < target) {
            bound *= 2;
        } else {
            break;
        }
    }

    // Range for binary search
    const left = Math.floor(bound / 2);
    const right = Math.min(bound, n - 1);

    return binarySearchRange(arr, left, right, target, steps);
}


// Binary search in the range [lo, hi], continuing step count
function binarySearchRange(arr, lo, hi, target, steps) {
    while (lo <= hi) {
        const mid = Math.floor(lo + (hi - lo) / 2);

        steps++;
        const val = arr[mid];

        if (val === target) return { index: mid, steps };
        if (val < target) lo = mid + 1;
        else hi = mid - 1;
    }

    return { index: -1, steps };
}


// ---------------------------------------------------------------------------
//  File loading for ordered.txt
// ---------------------------------------------------------------------------

const fs = require("fs");
const path = require("path");

function tryLoadFile(relPath) {
    const full = path.resolve(relPath);
    console.log("Trying:", full);
    try {
        const data = fs.readFileSync(full, "utf8");
        const arr = data
            .trim()
            .split(/\s+/)
            .map(Number);
        console.log("Loaded", arr.length, "integers from", full);
        return arr;
    } catch {
        console.log("  Not found or unreadable.");
        return null;
    }
}

function loadOrderedFile() {
    console.log("Working directory:", process.cwd());

    const candidates = [
        "../data/ordered.txt",
        "../../data/ordered.txt",
        "code_samples/section12/data/ordered.txt"
    ];

    for (const candidate of candidates) {
        const arr = tryLoadFile(candidate);
        if (arr) return arr;
    }

    console.log("Missing input file - aborting.");
    return null;
}


// ---------------------------------------------------------------------------
//  Tests
// ---------------------------------------------------------------------------

function runTests() {
    const arr = loadOrderedFile();
    if (!arr) return;

    const testTargets = [
        -5000,   // first element
        0,       // middle
        5000,    // last element
        6000     // not found
    ];

    for (const target of testTargets) {
        const result = exponentialSearch(arr, target);
        console.log(
            `Target ${target} → index=${result.index}, steps=${result.steps}`
        );
    }
}

runTests();
