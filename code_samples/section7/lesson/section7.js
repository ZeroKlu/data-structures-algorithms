function demoHash() {
    const freq = new Map();
    freq.set("apple", (freq.get("apple") || 0) + 1);
    freq.set("banana", (freq.get("banana") || 0) + 2);
    const n = freq.get("apple") || 0;
}

function countWords(words) {
    const freq = new Map();
    for (const w of words) {
        freq.set(w, (freq.get(w) || 0) + 1);
    }
    return freq;
}

function firstDuplicate(arr) {
    const seen = new Set();
    for (const x of arr) {
        if (seen.has(x)) {
            return x;
        }
        seen.add(x);
    }
    return null;
}

function twoSum(nums, target) {
    const indexOf = new Map(); // value -> index
    for (let i = 0; i < nums.length; i++) {
        const need = target - nums[i];
        if (indexOf.has(need)) {
            return [indexOf.get(need), i];
        }
        indexOf.set(nums[i], i);
    }
    return [-1, -1];
}

// ===== Helper Functions for Testing =====

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

function printArray(label, arr) {
    console.log(label + JSON.stringify(arr));
}

// ===== Tests =====

console.log("=== Test: demoHash ===");
demoHash();
console.log("(demoHash ran - no visible output)\n");

// ------------------------------------------------------------

console.log("=== Test: countWords (word frequency) ===");

const words = ["apple", "banana", "apple", "orange", "banana", "apple"];
const freq = countWords(words);

printMap("Frequencies:", freq, ["apple", "banana", "orange", "pear"]);
console.log("Expected: apple=3, banana=2, orange=1, pear not found\n");

// ------------------------------------------------------------

console.log("=== Test: firstDuplicate ===");

const arr1 = [2, 5, 1, 2, 3, 5, 1];
printArray("Array 1: ", arr1);
console.log("First duplicate:", firstDuplicate(arr1), "(expected 2)\n");

const arr2 = [1, 2, 3, 4, 5];
printArray("Array 2: ", arr2);
console.log("First duplicate:", firstDuplicate(arr2), "(expected null)\n");

// ------------------------------------------------------------

console.log("=== Test: twoSum (hash map) ===");

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
