package code_samples.section6.problems.problem6_2;

class HeapUtils {
    private static void heapify(int[] arr, int n, int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[left] < arr[smallest]) smallest = left;
        if (right < n && arr[right] < arr[smallest]) smallest = right;
        if (smallest != i) {
            int tmp = arr[i];
            arr[i] = arr[smallest];
            arr[smallest] = tmp;
            heapify(arr, n, smallest);
        }
    }

    public static void buildHeap(int[] arr) {
        int n = arr.length;
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, n, i);
        }
    }
}

public class problem6_2 {

    // Local heapify-down that works on a prefix [0, size)
    private static void heapifyDown(int[] arr, int size, int i) {
        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            if (left < size && arr[left] < arr[smallest]) smallest = left;
            if (right < size && arr[right] < arr[smallest]) smallest = right;
            if (smallest == i) break;
            int tmp = arr[i];
            arr[i] = arr[smallest];
            arr[smallest] = tmp;
            i = smallest;
        }
    }

    // Pop min from an array-based min-heap stored in arr[0..size-1]
    private static int popMin(int[] arr, int[] sizeRef) {
        int size = sizeRef[0];
        if (size == 0) return Integer.MIN_VALUE;

        int root = arr[0];
        size--;
        sizeRef[0] = size;

        if (size > 0) {
            arr[0] = arr[size];
            heapifyDown(arr, size, 0);
        }
        return root;
    }

    private static void printArray(String label, int[] arr) {
        System.out.print(label);
        for (int x : arr) System.out.print(x + " ");
        System.out.println();
    }

    public static void main(String[] args) {
        int[] arr = {9, 4, 7, 1, 0, 3, 2};

        printArray("Original array: ", arr);

        HeapUtils.buildHeap(arr);

        printArray("After buildHeap (min-heap array): ", arr);

        System.out.print("Extracting elements in sorted order: ");
        int[] heapCopy = arr.clone();
        int[] sizeRef = { heapCopy.length };

        while (sizeRef[0] > 0) {
            int v = popMin(heapCopy, sizeRef);
            System.out.print(v + " ");
        }
        System.out.println();
    }
}
