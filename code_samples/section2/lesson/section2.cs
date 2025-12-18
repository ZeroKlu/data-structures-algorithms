// Calculate the sum of the elements in an array
static int SumArray(int[] arr) {
    var sum = 0;
    foreach (var x in arr) {
        sum += x;
    }
    return sum;
}

// Reverse the elements in an array
static void ReverseArray(int[] arr) {
    int left = 0;
    int right = arr.Length - 1;
    while (left < right) {
        // Old way requires a temp variable
        // int temp = arr[left];
        // arr[left] = arr[right];
        // arr[right] = temp;

        // In C# 10 and later, we can swap values using a tuple
        (arr[right], arr[left]) = (arr[left], arr[right]);

        left++;
        right--;
    }
}

// Check if an array has a pair with a given sum
static bool HasPairWithSum(int[] arr, int target) {
    int left = 0;
    int right = arr.Length - 1;
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
static string ReverseString(string s) {
    var chars = s.ToCharArray();
    Array.Reverse(chars);
    return new string(chars);
}

int[] arr = { 1, 2, 3, 4, 5 };
Console.WriteLine($"Sum of array: {SumArray(arr)}");

ReverseArray(arr);
Console.WriteLine($"Reversed array: {string.Join(", ", arr)}");
ReverseArray(arr);

Console.WriteLine($"Has pair with sum 9: {HasPairWithSum(arr, 9)}");

Console.WriteLine($"Reversed string: {ReverseString("hello")}");
