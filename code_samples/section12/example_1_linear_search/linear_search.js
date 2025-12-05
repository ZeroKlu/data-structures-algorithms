const fs = require("fs");
const path = require("path");

// ===============================
// Linear Search (given)
// ===============================
function linearSearch(arr, target) {
    for (let i = 0; i < arr.length; i++) {
        if (arr[i] === target) return i;
    }
    return -1;
}

// ===============================
// Linear Search with step counting
// ===============================
function linearSearchSteps(arr, target) {
    let steps = 0;
    for (let i = 0; i < arr.length; i++) {
        steps++;
        if (arr[i] === target) {
            return { index: i, steps };
        }
    }
    return { index: -1, steps };
}

// ===============================
// Load the data file
// ===============================
function loadFile(relativePath) {
    const fullPath = path.join(__dirname, relativePath);
    try {
        const text = fs.readFileSync(fullPath, "utf8");
        return text
            .split(/\s+/)
            .filter(Boolean)
            .map(Number);
    } catch (err) {
        console.error("Error reading:", fullPath);
        console.error("Working directory:", process.cwd());
        return [];
    }
}

// ===============================
// Run tests on one dataset
// ===============================
function runTestsOnDataset(name, arr) {
    console.log(`=== ${name} ===`);
    console.log(`Loaded ${arr.length} integers.\n`);

    // First element
    let r1 = linearSearchSteps(arr, arr[0]);
    console.log(`Search first (${arr[0]}): index=${r1.index}, steps=${r1.steps}`);

    // Middle element
    let midVal = arr[Math.floor(arr.length / 2)];
    let r2 = linearSearchSteps(arr, midVal);
    console.log(`Search middle (${midVal}): index=${r2.index}, steps=${r2.steps}`);

    // Last element
    let lastVal = arr[arr.length - 1];
    let r3 = linearSearchSteps(arr, lastVal);
    console.log(`Search last (${lastVal}): index=${r3.index}, steps=${r3.steps}`);

    // Missing element
    let r4 = linearSearchSteps(arr, 999999);
    console.log(`Search missing (999999): index=${r4.index}, steps=${r4.steps}`);

    console.log();
}

// ===============================
// MAIN
// ===============================

// Paths are relative to the current file just like Java version
const ordered = loadFile("../data/ordered.txt");
const unordered = loadFile("../data/unordered.txt");

if (ordered.length === 0 || unordered.length === 0) {
    console.log("Missing input files-see error messages above.");
    process.exit(1);
}

runTestsOnDataset("Ordered Data", ordered);
runTestsOnDataset("Unordered Data", unordered);
