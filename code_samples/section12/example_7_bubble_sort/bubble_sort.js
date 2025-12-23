// =============================================================
// Bubble Sort (baseline / no step counting)
// =============================================================
//
// Classic bubble sort:
//   - Repeatedly passes through the array
//   - Swaps adjacent elements if they are out of order
//   - Stops early if a full pass makes no swaps
//
// Optimization used:
//   - After each pass, the largest element in the current unsorted range
//     is guaranteed to be at the end of that range, so we decrement `n`.
//
function bubbleSort(arr) {
    let n = arr.length;
    let swapped = true;

    // Keep making passes until we complete a pass with no swaps
    while (swapped) {
        swapped = false;

        // Compare adjacent pairs within the current range [0..n-1]
        for (let i = 1; i < n; i++) {
            if (arr[i - 1] > arr[i]) {
                // Destructuring swap of adjacent elements
                [arr[i - 1], arr[i]] = [arr[i], arr[i - 1]];
                swapped = true;
            }
        }

        // After one pass, the element at index n-1 is in its final position
        n--;
    }
}

// -------------------------------------------------------------
// Bubble Sort with step counting
// -------------------------------------------------------------
//
// Same algorithm as bubbleSort(), but tracks work performed using `stats`.
//
// Step definitions here:
//   - stats.comparisons: number of adjacent comparisons performed
//       (the check: arr[i - 1] > arr[i])
//   - stats.swaps: number of swaps actually executed
//
// Notes:
//   - Sorts `arr` in place.
//   - Uses the same early exit optimization (stop when no swaps occur).
//   - Shrinks the effective pass length with n-- each pass.
//
function bubbleSortSteps(arr, stats) {
    let n = arr.length;
    let swapped = true;

    while (swapped) {
        swapped = false;

        for (let i = 1; i < n; i++) {
            // Count every adjacent comparison performed
            stats.comparisons++;

            // Swap if adjacent elements are out of order
            if (arr[i - 1] > arr[i]) {
                [arr[i - 1], arr[i]] = [arr[i], arr[i - 1]];
                stats.swaps++;     // Count actual swap
                swapped = true;    // Another pass needed
            }
        }

        // Largest element in the current range has "bubbled" to index n-1
        n--; // last element is now in place
    }
}

// -------------------------------------------------------------
// File loading with correct project-root detection
// -------------------------------------------------------------
//
// This script is written as an ES module, so:
//   - CommonJS __dirname is not available directly.
//   - We reconstruct the module directory using import.meta.url.
//
// Because the script may be executed from different working directories,
// we *do not* rely on process.cwd() for file paths. Instead, we attempt
// to locate the repository/project root by walking up the directory tree
// until we find a folder containing "code_samples".
//
import fs from "fs";
import path from "path";
import url from "url";

// -------------------------------------------------------------
// Find project root (directory that contains "code_samples")
// -------------------------------------------------------------
//
// Strategy:
//   1) Start from the folder containing THIS script (module directory).
//   2) Walk upward toward filesystem root.
//   3) Return the first directory that contains a "code_samples" folder.
//
// Returns:
//   - Detected project root path
//   - Fallback: the directory of this script if not found
//
function findProjectRoot() {
    // __dirname equivalent for ES modules
    const __dirname = path.dirname(url.fileURLToPath(import.meta.url));
    let cur = path.resolve(__dirname);

    console.log("Current directory:", cur);

    // Walk upward until we hit the filesystem root
    while (cur !== path.parse(cur).root) {
        // Heuristic: repo root contains "code_samples"
        if (fs.existsSync(path.join(cur, "code_samples"))) {
            console.log("Detected project root:", cur);
            return cur;
        }
        cur = path.dirname(cur);
    }

    // fallback: if we never found a "code_samples" directory
    return path.resolve(__dirname);
}

// -------------------------------------------------------------
// Load a data file from code_samples/section12/data/<filename>
// -------------------------------------------------------------
//
// Builds full path using the detected project root:
//
//   <root>/code_samples/section12/data/<filename>
//
// File format:
//   - integers separated by whitespace
//
// Returns:
//   - number[] on success
//   - null on failure (missing file, unreadable file, etc.)
//
function loadFile(filename) {
    try {
        const projectRoot = findProjectRoot();

        // Construct full path to the requested data file
        const full = path.join(
            projectRoot,
            "code_samples",
            "section12",
            "data",
            filename
        );

        // Diagnostic output confirms the exact path used
        console.log("Attempting to read:", full);

        // Read entire file as text, split on whitespace, parse as numbers
        const text = fs.readFileSync(full, "utf8");
        return text
            .trim()
            .split(/\s+/)
            .map(Number);

    } catch (err) {
        // Handles missing files, permission errors, etc.
        console.error("Error reading:", filename);
        console.error(err.message);
        return null;
    }
}

// -------------------------------------------------------------
// Compare arrays for mismatches
// -------------------------------------------------------------
//
// Compares two arrays element-by-element and returns mismatch count.
// Prints only the first 10 mismatches to keep output readable.
//
// Assumption:
//   - Caller has already ensured lengths match.
//   - This function still loops over `a.length`, so unequal lengths could
//     cause undefined behavior for b[i] (not an issue here because main()
//     checks lengths before calling).
//
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
//
// Workflow:
//   1) Load unordered.txt (input to be sorted)
//   2) Load ordered.txt   (expected correct sorted output)
//   3) Run bubble sort with step counting
//   4) Print comparisons + swaps
//   5) Validate output by comparing to ordered reference
//
function main() {
    const unordered = loadFile("unordered.txt");
    const expected = loadFile("ordered.txt");

    // Abort if either file did not load
    if (!unordered || !expected) {
        console.log("Missing input file — aborting.");
        return;
    }

    // Abort if array sizes differ (cannot do a 1:1 comparison)
    if (unordered.length !== expected.length) {
        console.log("File size mismatch — aborting.");
        console.log("unordered length:", unordered.length);
        console.log("expected  length:", expected.length);
        return;
    }

    // Stats object used by bubbleSortSteps()
    const stats = { comparisons: 0, swaps: 0 };

    console.log("\n--- Bubble Sort Step Count ---");

    // Sort IN PLACE and accumulate step counts
    bubbleSortSteps(unordered, stats);

    // Print step counts
    console.log("Comparisons:", stats.comparisons);
    console.log("Swaps:", stats.swaps);

    // Verify correctness against ordered.txt
    console.log("\nComparing result with ordered.txt...");
    const mismatches = compareArrays(unordered, expected);

    if (mismatches === 0) {
        console.log("SUCCESS — output matches!");
    } else {
        console.log("FAIL — mismatches found:", mismatches);
    }
}

// Run the test harness
main();
