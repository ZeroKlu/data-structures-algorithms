#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int sum_array(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

void reverse_array(int arr[], int n) {
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

bool has_pair_with_sum(int arr[], int n, int target) {
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

void reverse_string(char* s) {
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

    printf("Sum of array: %d\n", sum_array(arr, n));

    reverse_array(arr, n);
    printf("Reversed array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    reverse_array(arr, n);

    printf("Has pair with sum 9: %s\n", has_pair_with_sum(arr, n, 9) ? "true" : "false");

    char s[] = "hello world";
    reverse_string(s);
    printf("Reversed string: %s\n", s);

    return 0;
}
