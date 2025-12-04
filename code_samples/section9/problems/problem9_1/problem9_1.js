function lengthOfLIS(nums) {
    const tails = [];
    for (const x of nums) {
        let lo = 0, hi = tails.length;
        while (lo < hi) {
            const mid = (lo + hi) >> 1;
            if (tails[mid] < x) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        tails[lo] = x;
    }
    return tails.length;
}

// ===========================
// Tests
// ===========================

function test(name, arr, expected) {
    const result = lengthOfLIS(arr);
    console.log(`${name}`);
    console.log(`Input: [${arr.join(",")}]`);
    console.log(`lengthOfLIS = ${result} (expected ${expected})\n`);
}

console.log("=== Testing lengthOfLIS ===\n");

test("Test 1: Increasing sequence", [1, 2, 3, 4, 5], 5);

test("Test 2: Mixed sequence", [10, 9, 2, 5, 3, 7, 101, 18], 4);

test("Test 3: All identical", [7, 7, 7, 7, 7], 1);

test("Test 4: Empty array", [], 0);

test("Test 5: Decreasing sequence", [9, 7, 5, 3, 1], 1);

test("Test 6: Random pattern", [4, 10, 4, 3, 8, 9], 3);
