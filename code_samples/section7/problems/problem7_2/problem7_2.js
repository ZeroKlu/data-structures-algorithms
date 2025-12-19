/**
 * groupAnagrams
 *
 * Purpose:
 *   Groups strings that are anagrams of each other.
 *
 * Definition:
 *   Two strings are anagrams if they contain the same characters with the same
 *   frequencies, possibly in a different order.
 *
 * Parameters:
 *   strs - Array of input strings.
 *
 * Returns:
 *   An array of groups, where each group is an array of strings that are
 *   anagrams of each other.
 *
 * Approach:
 *   - Use a Map where:
 *       key   = canonical representation of the string
 *       value = array of original strings that match that key
 *
 *   - Canonical key strategy:
 *       Split the string into characters, sort them, and re-join.
 *       All anagrams produce the same sorted string.
 *       Example:
 *         "eat" -> ["e","a","t"] -> ["a","e","t"] -> "aet"
 *         "tea" -> "aet"  (same key → same group)
 *
 * Complexity:
 *   Let:
 *     n = number of strings
 *     k = average string length
 *
 *   - Time:  O(n · k log k)
 *       Sorting each string dominates the cost.
 *   - Space: O(n · k)
 *       For storing keys and grouped strings.
 *
 * Notes:
 *   - Map preserves insertion order of keys, but the order of groups
 *     still depends on the first time each anagram key is encountered.
 *   - Group order and item order are allowed to vary for this problem.
 */
function groupAnagrams(strs) {
    // Map: sorted-string key -> array of original strings
    const map = new Map();

    // Process each string in the input array
    for (const s of strs) {
        // Create the canonical anagram key by sorting characters
        const key = s.split("").sort().join("");

        // Initialize a new group if this key hasn't been seen before
        if (!map.has(key)) {
            map.set(key, []);
        }

        // Add the original string to its anagram group
        map.get(key).push(s);
    }

    // Convert the map's values (arrays of strings) into a plain array
    // to match the expected return type.
    return Array.from(map.values());
}

// ============================
// Tests (top-level)
// ============================
//
// Simple console-based test harness demonstrating usage.
//

const input = ["eat", "tea", "tan", "ate", "nat", "bat"];

console.log("=== Test: groupAnagrams ===\n");
console.log("Input:", input, "\n");

// Run the function under test
const groups = groupAnagrams(input);

// Print each anagram group
console.log("Anagram Groups:");
for (const g of groups) {
    console.log("  [" + g.join(", ") + "]");
}

// Expected output guidance (ordering may differ)
console.log("\nExpected groups (order may vary):");
console.log("  [eat, tea, ate]");
console.log("  [tan, nat]");
console.log("  [bat]");
