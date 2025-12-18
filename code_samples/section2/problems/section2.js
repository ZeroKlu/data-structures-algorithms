// Reverse an array O(n)
function reverseArray(arr) {
    let i = 0;
    let j = arr.length - 1;
    while (i < j) {
        const tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        i++;
        j--;
    }
}

// Check for pair of elements that sum to target - Brute force O(n^2)
function twoSumBruteforce(nums, target) {
    const n = nums.length;
    for (let i = 0; i < n; i++) {
        for (let j = i + 1; j < n; j++) {
            if (nums[i] + nums[j] === target) {
                return true;
            }
        }
    }
    return false;
}

// Check for pair of elements that sum to target - Hash-based O(n) expected
function twoSumHash(nums, target) {
    const seen = new Set();
    for (const x of nums) {
        const need = target - x;
        if (seen.has(need)) {
            return true;
        }
        seen.add(x);
    }
    return false;
}

// Move zeroes to the end - O(n)
function moveZeroes(nums) {
    let write = 0;
    for (let read = 0; read < nums.length; read++) {
        if (nums[read] !== 0) {
            nums[write] = nums[read];
            write++;
        }
    }
    while (write < nums.length) {
        nums[write] = 0;
        write++;
    }
}

// Check if two strings are anagrams - O(n)
function isAnagram(s, t) {
    if (s.length !== t.length) return false;
    const freq = new Array(26).fill(0);
    for (let i = 0; i < s.length; i++) {
        freq[s.charCodeAt(i) - 97]++;
        freq[t.charCodeAt(i) - 97]--;
    }
    for (let i = 0; i < 26; i++) {
        if (freq[i] !== 0) return false;
    }
    return true;
}

// Find the length of the longest substring without repeating characters - O(n)
function lengthOfLongestSubstring(s) {
    const lastPos = new Map();
    let best = 0;
    let left = 0;

    for (let right = 0; right < s.length; right++) {
        const ch = s[right];
        if (lastPos.has(ch) && lastPos.get(ch) >= left) {
            left = lastPos.get(ch) + 1;
        }
        lastPos.set(ch, right);
        const windowLen = right - left + 1;
        if (windowLen > best) best = windowLen;
    }
    return best;
}

// Rotate an array O(n)
function rotate(nums, k) {
    const n = nums.length;
    if (n === 0) return;
    k %= n;
    if (k === 0) return;

    reverseRange(nums, 0, n - 1);
    reverseRange(nums, 0, k - 1);
    reverseRange(nums, k, n - 1);
}

// Reverse a range of an array - O(n)
function reverseRange(nums, l, r) {
    while (l < r) {
        const tmp = nums[l];
        nums[l] = nums[r];
        nums[r] = tmp;
        l++;
        r--;
    }
}

const arr = [1, 2, 3, 4, 5];

reverseArray(arr);
console.log("Reversed array: ", arr);

console.log("Has pair with sum 9 (brute force): ", twoSumBruteforce(arr, 9));
console.log("Has pair with sum 9 (hash-based): ", twoSumHash(arr, 9));

const nums = [0, 1, 0, 3, 12];
moveZeroes(nums);
console.log(nums);

const s1 = "listen";
const s2 = "silent";
console.log("Is \"" + s1 + "\" an anagram of \"" + s2 + "\": ", isAnagram(s1, s2));

console.log(
    "Length of longest substring without repeating characters: ",
    lengthOfLongestSubstring("abcabcbb")
);

rotate([1, 2, 3, 4, 5], 2);
console.log("Array rotated 2 positions: ", [4, 5, 1, 2, 3]);
