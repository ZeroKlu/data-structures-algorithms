/**
 * Demonstration of hash-based data structures in JavaScript:
 *  - Map for key/value associations (like a hash table)
 *  - Set for fast membership checks
 *
 * These examples mirror common interview / algorithm patterns:
 *  - Frequency counting
 *  - Detecting duplicates
 *  - Solving the Two Sum problem
 *
 * Notes on Map vs plain objects:
 *  - Map preserves insertion order and allows any value type as a key.
 *  - Map does not have prototype keys, so no accidental collisions like "__proto__".
 */

// ===========================================================
// Core Demonstrations
// ===========================================================

/**
 * demoHash()
 *
 * Purpose:
 *  - Shows basic usage of JavaScript's Map for counting values.
 *
 * Key idea:
 *  - map.get(key) returns undefined if the key does not exist.
 *  - Using (map.get(key) || 0) treats missing entries as 0.
 *
 * Side effects:
 *  - Mutates a local Map only; produces no visible output.
 */
function demoHash() {
    // Create an empty Map: key -> count
    const freq = new Map();

    // Increment "apple":
    //  - freq.get("apple") is undefined initially
    //  - undefined || 0 => 0
    //  - 0 + 1 => 1
    freq.set("apple", (freq.get("apple") || 0) + 1);

    // Increment "banana" by 2 using the same pattern
    freq.set("banana", (freq.get("banana") || 0) + 2);

    // Safe read with default fallback
    const n = freq.get("apple") || 0;

    // n === 1 here; value is unused, just for demonstration
}

/**
 * countWords(words)
 *
 * Purpose:
 *  - Builds a frequency map of words.
 *
 * @param {string[]} words - Array of words/tokens to count.
 * @returns {Map<string, number>} Map from word -> frequency.
 *
 * Complexity:
 *  - Time:  O(n) average
 *  - Space: O(k), where k is the number of distinct words
 */
function countWords(words) {
    // Frequency table
    const freq = new Map();

    for (const w of words) {
        // Read existing count (or 0 if missing) and increment
        freq.set(w, (freq.get(w) || 0) + 1);
    }

    return freq;
}

/**
 * firstDuplicate(arr)
 *
 * Purpose:
 *  - Returns the first value that appears more than once when scanning left-to-right.
 *
 * Example:
 *  - [2, 5, 1, 2, 3, 5, 1] -> 2
 *
 * @param {number[]} arr - Array of numbers to scan.
 * @returns {number|null} The first duplicate, or null if none exists.
 *
 * Complexity:
 *  - Time:  O(n) average
 *  - Space: O(n) worst case
 */
function firstDuplicate(arr) {
    // Set of values we've already seen
    const seen = new Set();

    for (const x of arr) {
        // If x is already in the set, we've found the first duplicate
        if (seen.has(x)) {
            return x;
        }
        // Otherwise record that we've seen x
        seen.add(x);
    }

    // No duplicates found
    return null;
}

/**
 * twoSum(nums, target)
 *
 * Purpose:
 *  - Finds indices (i, j) such that nums[i] + nums[j] == target.
 *
 * @param {number[]} nums - Input array of numbers.
 * @param {number} target - Desired sum.
 * @returns {number[]} A pair of indices [i, j], or [-1, -1] if no solution exists.
 *
 * Complexity:
 *  - Time:  O(n) average
 *  - Space: O(n)
 *
 * Approach:
 *  - Use a Map from value -> index.
 *  - For each element nums[i], compute the needed complement.
 *  - If the complement exists in the map, return the stored index and i.
 */
function twoSum(nums, target) {
    // Map: value -> index where it was last seen
    const indexOf = new Map(); // value -> index

    for (let i = 0; i < nums.length; i++) {
        const need = target - nums[i];

        // Check if we've already seen the needed value
        if (indexOf.has(need)) {
            return [indexOf.get(need), i];
        }

        // Store the current value and its index
        indexOf.set(nums[i], i);
    }

    // No valid pair found
    return [-1, -1];
}

// ===========================================================
// Helper Functions for Testing
// ===========================================================

/**
 * printMap(label, map, keys)
 *
 * Purpose:
 *  - Prints selected keys from a Map in a readable format.
 *
 * @param {string} label - Heading printed before the entries.
 * @param {Map} map - Map to inspect.
 * @param {string[]} keys - Keys to print.
 */
function printMap(label, map, keys) {
    console.log(label);

    for (const k of keys) {
        if (map.has(k)) {
            console.log(`  '${k}' -> ${map.get(k)}`);
        } else {
            console.log(`  '${k}' -> (not found)`);
        }
    }
}

/**
 * printArray(label, arr)
 *
 * Purpose:
 *  - Prints an array in JSON-style formatting.
 *
 * @param {string} label - Prefix before the array.
 * @param {any[]} arr - Array to print.
 */
function printArray(label, arr) {
    console.log(label + JSON.stringify(arr));
}

// ===========================================================
// Tests
// ===========================================================

console.log("=== Test: demoHash ===");
demoHash();
console.log("(demoHash ran - no visible output)\n");

// ------------------------------------------------------------

console.log("=== Test: countWords (word frequency) ===");

// Sample input: apple x3, banana x2, orange x1
const words = ["apple", "banana", "apple", "orange", "banana", "apple"];

// Run function under test
const freq = countWords(words);

// Print selected keys, including a missing one ("pear")
printMap("Frequencies:", freq, ["apple", "banana", "orange", "pear"]);
console.log("Expected: apple=3, banana=2, orange=1, pear not found\n");

// ------------------------------------------------------------

console.log("=== Test: firstDuplicate ===");

// Case 1: duplicates exist
const arr1 = [2, 5, 1, 2, 3, 5, 1];
printArray("Array 1: ", arr1);
console.log("First duplicate:", firstDuplicate(arr1), "(expected 2)\n");

// Case 2: no duplicates
const arr2 = [1, 2, 3, 4, 5];
printArray("Array 2: ", arr2);
console.log("First duplicate:", firstDuplicate(arr2), "(expected null)\n");

// ------------------------------------------------------------

console.log("=== Test: twoSum (hash map) ===");

// Positive case: 2 + 7 = 9
const nums = [2, 7, 11, 15];
const target = 9;

printArray("Array: ", nums);
console.log("Target:", target);

const pair1 = twoSum(nums, target);
console.log("twoSum result indices:", JSON.stringify(pair1));
console.log("Expected indices [0,1] or [1,0] (values 2 and 7)\n");

// Negative test
console.log("--- Negative case (no pair) ---");

const nums2 = [1, 2, 3];
const target2 = 100;

printArray("Array: ", nums2);
console.log("Target:", target2);

const pair2 = twoSum(nums2, target2);
console.log("twoSum result indices:", JSON.stringify(pair2));
console.log("Expected: [-1, -1]\n");
