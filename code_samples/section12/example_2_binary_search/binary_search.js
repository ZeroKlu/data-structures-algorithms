const fs = require("fs");
const path = require("path");

// ------------------------------------------------------
// Original binary search
// ------------------------------------------------------
function binarySearch(arr, target) {
    let lo = 0, hi = arr.length - 1;
    while (lo <= hi) {
        const mid = Math.floor(lo + (hi - lo) / 2);
        if (arr[mid] === target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

// ------------------------------------------------------
// Step-counting version
// ------------------------------------------------------
function binarySearchSteps(arr, target) {
    let lo = 0, hi = arr.length - 1;
    let steps = 0;

    while (lo <= hi) {
        steps++;
        const mid = Math.floor(lo + (hi - lo) / 2);

        if (arr[mid] === target) return { index: mid, steps };
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }

    return { index: -1, steps };
}

// ------------------------------------------------------
// Load ordered.txt RELATIVE TO THIS SCRIPT'S FOLDER
// ------------------------------------------------------
function loadOrderedFile() {
    const filePath = path.resolve(__dirname, "..", "data", "ordered.txt");
    console.log("Attempting to read:", filePath);

    try {
        const text = fs.readFileSync(filePath, "utf8");
        return text.split(/\s+/).filter(Boolean).map(Number);
    } catch (e) {
        console.error("Error reading ordered.txt:", e.message);
        return [];
    }
}

// ------------------------------------------------------
// Test runner
// ------------------------------------------------------
function runTests(arr) {
    console.log("\n=== Binary Search Tests ===");
    console.log("Loaded", arr.length, "integers\n");

    // first
    let result = binarySearchSteps(arr, arr[0]);
    console.log(`Search first (${arr[0]}): index=${result.index}, steps=${result.steps}`);

    // middle
    const midVal = arr[Math.floor(arr.length / 2)];
    result = binarySearchSteps(arr, midVal);
    console.log(`Search middle (${midVal}): index=${result.index}, steps=${result.steps}`);

    // last
    result = binarySearchSteps(arr, arr[arr.length - 1]);
    console.log(`Search last (${arr[arr.length - 1]}): index=${result.index}, steps=${result.steps}`);

    // missing
    result = binarySearchSteps(arr, 999999);
    console.log(`Search missing (999999): index=${result.index}, steps=${result.steps}`);
}

// ------------------------------------------------------
// MAIN
// ------------------------------------------------------
const arr = loadOrderedFile();
if (arr.length === 0) {
    console.log("Missing input file - aborting.");
    process.exit(1);
}

runTests(arr);
