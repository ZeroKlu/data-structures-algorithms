function heapify(arr, n, i) {
    let smallest = i;
    const left = 2 * i + 1;
    const right = 2 * i + 2;

    if (left < n && arr[left] < arr[smallest]) smallest = left;
    if (right < n && arr[right] < arr[smallest]) smallest = right;

    if (smallest !== i) {
        [arr[i], arr[smallest]] = [arr[smallest], arr[i]];
        heapify(arr, n, smallest);
    }
}

function buildHeap(arr) {
    const n = arr.length;
    for (let i = Math.floor(n / 2) - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

// ===== Tests =====

// Print helper
function printArray(label, arr) {
    console.log(label + arr.join(" "));
}

// Correct heap-pop using prefix size
function popMin(arr, sizeObj) {
    let size = sizeObj.size;
    if (size === 0) return null;

    const root = arr[0];
    size--;
    sizeObj.size = size;

    if (size > 0) {
        arr[0] = arr[size];
        heapify(arr, size, 0);
    }

    return root;
}

// ===== Test run =====

let arr = [9, 4, 7, 1, 0, 3, 2];

printArray("Original array: ", arr);

buildHeap(arr);
printArray("After buildHeap (min-heap array): ", arr);

// Extract values to test correctness
console.log("Extracting elements in sorted order:");
let heapCopy = arr.slice();     // clone
let sizeObj = { size: heapCopy.length };

let out = [];
while (sizeObj.size > 0) {
    out.push(popMin(heapCopy, sizeObj));
}

printArray("", out);
