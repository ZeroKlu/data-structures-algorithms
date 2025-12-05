const fs = require("fs");
const path = require("path");

// ============================================================
// Interpolation Search (index-only)
// Requires sorted data
// ============================================================
function interpolationSearch(arr, target) {
    let n = arr.length;
    if (n === 0) return -1;

    let lo = 0;
    let hi = n - 1;

    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {
        if (arr[hi] === arr[lo]) {
            return arr[lo] === target ? lo : -1;
        }

        const pos = lo + Math.floor(
            ((hi - lo) * (target - arr[lo])) / (arr[hi] - arr[lo])
        );

        if (pos < lo || pos > hi) return -1;

        if (arr[pos] === target) return pos;
        else if (arr[pos] < target) lo = pos + 1;
        else hi = pos - 1;
    }

    return -1;
}

// ============================================================
// Interpolation Search WITH step counting
// ============================================================
function interpolationSearchSteps(arr, target) {
    let n = arr.length;
    let steps = 0;
    if (n === 0) return { index: -1, steps };

    let lo = 0;
    let hi = n - 1;

    while (lo <= hi && target >= arr[lo] && target <= arr[hi]) {
        steps++; // compare arr[hi] == arr[lo]
        if (arr[hi] === arr[lo]) {
            steps++; // arr[lo] == target
            return { index: arr[lo] === target ? lo : -1, steps };
        }

        const pos = lo + Math.floor(
            ((hi - lo) * (target - arr[lo])) / (arr[hi] - arr[lo])
        );

        if (pos < lo || pos > hi) return { index: -1, steps };

        steps++; // arr[pos] == target
        if (arr[pos] === target) return { index: pos, steps };

        steps++; // arr[pos] < target
        if (arr[pos] < target) lo = pos + 1;
        else hi = pos - 1;
    }

    return { index: -1, steps };
}

// ============================================================
// Load ordered.txt
// ============================================================
function loadOrderedFile() {
    const filePath = path.resolve(__dirname, "..", "data", "ordered.txt");
    console.log("Attempting to read:", filePath);

    try {
        const text = fs.readFileSync(filePath, "utf8");
        return text
            .split(/\s+/)
            .filter(Boolean)
            .map(Number);
    } catch (e) {
        console.log("Error reading ordered.txt:");
        console.log(e.message);
        console.log("Working directory:", process.cwd());
        return [];
    }
}

// ============================================================
// Test Harness
// ============================================================
function runTests(arr) {
    console.log("\n=== Interpolation Search Tests (sorted data only) ===");
    console.log("Loaded", arr.length, "integers\n");

    // First
    let r = interpolationSearchSteps(arr, arr[0]);
    console.log(`Search first  (${arr[0]}): index=${r.index}, steps=${r.steps}`);

    // Middle
    const mid = arr[Math.floor(arr.length / 2)];
    r = interpolationSearchSteps(arr, mid);
    console.log(`Search middle (${mid}): index=${r.index}, steps=${r.steps}`);

    // Last
    const last = arr[arr.length - 1];
    r = interpolationSearchSteps(arr, last);
    console.log(`Search last   (${last}): index=${r.index}, steps=${r.steps}`);

    // Missing
    r = interpolationSearchSteps(arr, 999999);
    console.log(`Search missing (999999): index=${r.index}, steps=${r.steps}`);
}

// ============================================================
// MAIN
// ============================================================
const arr = loadOrderedFile();
if (arr.length === 0) {
    console.log("Missing input file — aborting.");
    process.exit(1);
}

runTests(arr);

// For unit tests if needed
module.exports = {
    interpolationSearch,
    interpolationSearchSteps
};
