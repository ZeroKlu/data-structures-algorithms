function groupAnagrams(strs) {
    const map = new Map();
    for (const s of strs) {
        const key = s.split("").sort().join("");
        if (!map.has(key)) {
            map.set(key, []);
        }
        map.get(key).push(s);
    }
    return Array.from(map.values());
}

// ============================
// Tests (top-level)
// ============================

const input = ["eat", "tea", "tan", "ate", "nat", "bat"];

console.log("=== Test: groupAnagrams ===\n");
console.log("Input:", input, "\n");

const groups = groupAnagrams(input);

console.log("Anagram Groups:");
for (const g of groups) {
    console.log("  [" + g.join(", ") + "]");
}

console.log("\nExpected groups (order may vary):");
console.log("  [eat, tea, ate]");
console.log("  [tan, nat]");
console.log("  [bat]");
