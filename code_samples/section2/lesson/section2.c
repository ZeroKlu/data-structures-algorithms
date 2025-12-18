#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Calculate the sum of an array
int sumArray(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

// Reverse an array
void reverseArray(int arr[], int n) {
    int left = 0;
    int right = n - 1;
    while (left < right) {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

// Check if an array has a pair with a given sum
bool hasPairWithSum(int arr[], int n, int target) {
    int left = 0;
    int right = n - 1;
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

// Reverse a string
void reverseString(char* s) {
    int left = 0;
    int right = (int)strlen(s) - 1;
    while (left < right) {
        char temp = s[left];
        s[left] = s[right];
        s[right] = temp;
        left++;
        right--;
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Sum of array: %d\n", sumArray(arr, n));

    reverseArray(arr, n);
    printf("Reversed array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    reverseArray(arr, n);

    printf("Has pair with sum 9: %s\n", hasPairWithSum(arr, n, 9) ? "true" : "false");

    char s[] = "hello world";
    reverseString(s);
    printf("Reversed string: %s\n", s);

    return 0;
}
