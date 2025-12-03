#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

void demo_hash() {
    std::unordered_map<std::string, int> freq;
    freq["apple"]++;
    freq["banana"] += 2;
    int n = freq["apple"]; // if missing, default constructed 0 then incremented

    std::cout << "[demo_hash] apple count: " << freq["apple"]
              << ", banana count: " << freq["banana"]
              << " (n variable: " << n << ")\n";
}

std::unordered_map<std::string, int>
countWords(const std::vector<std::string>& words) {
    std::unordered_map<std::string, int> freq;
    for (const auto& w : words) {
        freq[w]++;
    }
    return freq;
}

int firstDuplicate(const std::vector<int>& arr) {
    std::unordered_set<int> seen;
    for (int x : arr) {
        if (seen.count(x)) {
            return x;
        }
        seen.insert(x);
    }
    return 0; // or some sentinel
}

std::pair<int, int> twoSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> indexOf; // value -> index
    for (int i = 0; i < (int)nums.size(); i++) {
        int need = target - nums[i];
        auto it = indexOf.find(need);
        if (it != indexOf.end()) {
            return {it->second, i};
        }
        indexOf[nums[i]] = i;
    }
    return {-1, -1};
}

// ================== Test helpers ==================

template <typename T>
void printVector(const std::string& label, const std::vector<T>& v) {
    std::cout << label;
    for (const auto& x : v) std::cout << x << " ";
    std::cout << "\n";
}

void test_countWords() {
    std::cout << "=== Test: countWords (word frequency) ===\n";
    std::vector<std::string> words = {
        "apple", "banana", "apple", "orange", "banana", "apple"
    };

    auto freq = countWords(words);

    std::vector<std::string> keys = {"apple", "banana", "orange", "pear"};
    for (const auto& k : keys) {
        auto it = freq.find(k);
        if (it != freq.end()) {
            std::cout << "'" << k << "' -> " << it->second << "\n";
        } else {
            std::cout << "'" << k << "' -> (not found)\n";
        }
    }
    std::cout << "Expected: apple=3, banana=2, orange=1, pear not found\n\n";
}

void test_firstDuplicate() {
    std::cout << "=== Test: firstDuplicate ===\n";

    std::vector<int> arr1 = {2, 5, 1, 2, 3, 5, 1};
    printVector("Array 1: ", arr1);
    int dup1 = firstDuplicate(arr1);
    std::cout << "First duplicate: " << dup1 << " (expected 2)\n\n";

    std::vector<int> arr2 = {1, 2, 3, 4, 5};
    printVector("Array 2: ", arr2);
    int dup2 = firstDuplicate(arr2);
    std::cout << "First duplicate: " << dup2
              << " (expected 0 = no duplicate)\n\n";
}

void test_twoSum() {
    std::cout << "=== Test: twoSum (hash map) ===\n";

    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    printVector("Array: ", nums);
    std::cout << "Target: " << target << "\n";
    auto pair1 = twoSum(nums, target);
    if (pair1.first != -1) {
        std::cout << "twoSum found indices (" << pair1.first << ", "
                  << pair1.second << ") -> values (" << nums[pair1.first]
                  << ", " << nums[pair1.second] << ")\n";
        std::cout << "Expected: indices (0,1) or (1,0) with values 2 and 7\n\n";
    } else {
        std::cout << "twoSum did not find a pair (unexpected here)\n\n";
    }

    // Negative test: no pair
    std::cout << "--- Negative case (no pair) ---\n";
    std::vector<int> nums2 = {1, 2, 3};
    int target2 = 100;
    printVector("Array: ", nums2);
    std::cout << "Target: " << target2 << "\n";
    auto pair2 = twoSum(nums2, target2);
    if (pair2.first == -1) {
        std::cout << "twoSum correctly found no pair.\n\n";
    } else {
        std::cout << "twoSum incorrectly found a pair.\n\n";
    }
}

int main() {
    std::cout << "=== demo_hash ===\n";
    demo_hash();
    std::cout << "\n";

    test_countWords();
    test_firstDuplicate();
    test_twoSum();

    return 0;
}
