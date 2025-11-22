function ex1(arr) {
    for (const x of arr) {
        console.log(x);
    }
}

function ex2(arr) {
    for (let i = 0; i < arr.length; i++) {
        for (let j = i; j < arr.length; j++) {
            console.log(arr[i], arr[j]);
        }
    }
}

function ex3(arr) {
    if (arr.length > 0) {
        console.log(arr[0], arr[arr.length - 1]);
    }
}

function ex4(arr) {
    for (const x of arr) {
        console.log(x);
    }
    for (const x of arr) {
        console.log(x);
    }
}

function ex5(arr) {
    return arr.map(x => x * 2);
}

const arr = [1, 2, 3, 4, 5];

console.log("Ex 1:");
ex1(arr);

console.log("\nEx 2:");
ex2(arr);

console.log("\nEx 3:");
ex3(arr);

console.log("\nEx 4:");
ex4(arr);

console.log("\nEx 5:");
const res = ex5(arr);
for (const x of res) {
    console.log(x);
}
