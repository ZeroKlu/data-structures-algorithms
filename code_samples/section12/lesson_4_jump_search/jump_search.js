const fs = require("fs");
const path = require("path");

// ======================================================
// Jump Search (index-only version)
// Requires: sorted array
// ======================================================
function jumpSearch(arr, target) {
    const n = arr.length;
    if (n === 0) return -1;

    const step = Math.floor(Math.sqrt(n));
    let prev = 0;
    let curr = step;

    // Jump phase: find block where target could be
    while (curr < n && arr[curr] < target) {
        prev = curr;
        curr += step;
    }

    if (curr >= n) curr = n - 1;

    // Linear search within [prev, curr]
    for (let i = prev; i <= curr; i++) {
        if (arr[i] === target) {
            return i;
        }
    }
    return -1;
}

// ======================================================
// Jump Search with step counting
// steps = number of comparisons performed
// ======================================================
function jumpSearchSteps(arr, target) {
    const n = arr.length;
    let steps = 0;

    if (n === 0) {
        return { index: -1, steps };
    }

    const step = Math.floor(Math.sqrt(n));
    let prev = 0;
    let curr = step;

    // Jump phase
    while (curr < n) {
        steps++; // comparison: arr[curr] < target OR arr[curr] >= target

        if (arr[curr] >= target) break;

        prev = curr;
        curr += step;
    }

    if (curr >= n) curr = n - 1;

    // Linear search phase
    for (let i = prev; i <= curr; i++) {
        steps++; // comparison: arr[i] === target
        if (arr[i] === target) {
            return { index: i, steps };
        }
    }

    return { index: -1, steps };
}

// ======================================================
// Load ordered.txt (sorted integers) relative to this file
// Path: ../data/ordered.txt
// ======================================================
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
        console.error("Error reading ordered.txt:", e.message);
        return [];
    }
}

// ======================================================
// Run tests: first, middle, last, missing
// ======================================================
function runTests(arr) {
    console.log("\n=== Jump Search Tests (ordered data only) ===");
    console.log("Loaded", arr.length, "integers\n");

    // First element
    let r = jumpSearchSteps(arr, arr[0]);
    console.log(`Search first  (${arr[0]}): index=${r.index}, steps=${r.steps}`);

    // Middle element
    const midVal = arr[Math.floor(arr.length / 2)];
    r = jumpSearchSteps(arr, midVal);
    console.log(`Search middle (${midVal}): index=${r.index}, steps=${r.steps}`);

    // Last element
    const lastVal = arr[arr.length - 1];
    r = jumpSearchSteps(arr, lastVal);
    console.log(`Search last   (${lastVal}): index=${r.index}, steps=${r.steps}`);

    // Missing element
    r = jumpSearchSteps(arr, 999_999);
    console.log(`Search missing (999999): index=${r.index}, steps=${r.steps}`);
}

// ======================================================
// MAIN
// ======================================================
const arr = loadOrderedFile();
if (arr.length === 0) {
    console.log("Missing input file — aborting.");
    process.exit(1);
}

runTests(arr);

// Export functions if you want to import in other modules/tests
module.exports = { jumpSearch, jumpSearchSteps };
