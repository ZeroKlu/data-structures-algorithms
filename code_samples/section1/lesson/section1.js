// Print each element of an array
function printArrayElements(arr) {
    for (const x of arr) {
        console.log(x);
    }
}

// Print all pairs of elements
function printElementPairs(arr) {
    for (let i = 0; i < arr.length; i++) {
        for (let j = i; j < arr.length; j++) {
            console.log(arr[i], arr[j]);
        }
    }
}

// Print the first and last elements
function printFirstAndLast(arr) {
    if (arr.length > 0) {
        console.log(arr[0], arr[arr.length - 1]);
    }
}

// Print each element of an array twice
function printArrayTwice(arr) {
    for (const x of arr) {
        console.log(x);
    }
    for (const x of arr) {
        console.log(x);
    }
}

// Double each element
function doubleArrayElements(arr) {
    return arr.map(x => x * 2);
}

const arr = [1, 2, 3, 4, 5];

console.log("Ex 1:");
printArrayElements(arr);

console.log("\nEx 2:");
printElementPairs(arr);

console.log("\nEx 3:");
printFirstAndLast(arr);

console.log("\nEx 4:");
printArrayTwice(arr);

console.log("\nEx 5:");
const res = doubleArrayElements(arr);
for (const x of res) {
    console.log(x);
}
