#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

int sumArray(const std::vector<int>& arr) {
    int sum = 0;
    for (int x : arr) {
        sum += x;
    }
    return sum;
}

void reverseArray(std::vector<int>& arr) {
    int left = 0;
    int right = (int)arr.size() - 1;
    while (left < right) {
        std::swap(arr[left], arr[right]);
        left++;
        right--;
    }
}

bool hasPairWithSum(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = (int)arr.size() - 1;
    while (left < right) {
        int sum = arr[left] + arr[right];
        if (sum == target) {
            return true;
        } else if (sum < target) {
            left++;
        } else {
            right--;
        }
    }
    return false;
}

std::string reverseString(const std::string& s) {
    std::string result = s;
    int left = 0;
    int right = (int)result.size() - 1;
    while (left < right) {
        std::swap(result[left], result[right]);
        left++;
        right--;
    }
    return result;
}

int main() {
    vector<int> arr = {1, 2, 3, 4, 5};
    cout << "Sum of array: " << sumArray(arr) << endl;

    reverseArray(arr);
    cout << "Reversed array: ";
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;
    reverseArray(arr);

    cout << "Has pair with sum 9: " << (hasPairWithSum(arr, 9) ? "true" : "false") << endl;

    string s = "hello";
    cout << "Reversed string: " << reverseString(s) << endl;

    return 0;    
}
