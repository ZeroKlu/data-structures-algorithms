// Calculate the sum of the elements in an array
function sumArray(arr) {
    let sum = 0;
    for (const x of arr) {
        sum += x;
    }
    return sum;
}

// Reverse the elements in an array
function reverseArray(arr) {
    let left = 0;
    let right = arr.length - 1;
    while (left < right) {
        const temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

// Check if there is a pair of elements in an array that sum to a given value
function hasPairWithSum(arr, target) {
    let left = 0;
    let right = arr.length - 1;
    while (left < right) {
        const sum = arr[left] + arr[right];
        if (sum === target) {
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
function reverseString(s) {
    return s.split("").reverse().join("");
}

const arr = [1, 2, 3, 4, 5];

console.log("Sum of array:", sumArray(arr));

reverseArray(arr);
console.log("Reversed array:", arr);
reverseArray(arr);

console.log("Has pair with sum 9:", hasPairWithSum(arr, 9));

console.log("Reversed string:", reverseString("hello"));
