package code_samples.section6.lesson;

class MinHeap {
    private java.util.ArrayList<Integer> data = new java.util.ArrayList<>();

    public void insert(int value) {
        data.add(value);
        int i = data.size() - 1;
        // heapify up
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data.get(parent) <= data.get(i)) break;
            int tmp = data.get(parent);
            data.set(parent, data.get(i));
            data.set(i, tmp);
            i = parent;
        }
    }
}

class MinHeapWithExtract {
    private java.util.ArrayList<Integer> data = new java.util.ArrayList<>();

    public void insert(int value) {
        data.add(value);
        int i = data.size() - 1;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data.get(parent) <= data.get(i)) break;
            int tmp = data.get(parent);
            data.set(parent, data.get(i));
            data.set(i, tmp);
            i = parent;
        }
    }

    public int extractMin() {
        if (data.isEmpty()) {
            throw new IllegalStateException("heap underflow");
        }
        int minVal = data.get(0);
        int last = data.remove(data.size() - 1);
        if (data.isEmpty()) {
            return minVal;
        }
        data.set(0, last);

        int n = data.size();
        int i = 0;
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;
            if (left < n && data.get(left) < data.get(smallest)) {
                smallest = left;
            }
            if (right < n && data.get(right) < data.get(smallest)) {
                smallest = right;
            }
            if (smallest == i) break;
            int tmp = data.get(i);
            data.set(i, data.get(smallest));
            data.set(smallest, tmp);
            i = smallest;
        }
        return minVal;
    }
}

public class section6 {
    void heapifyDownArray(int[] a, int n, int i) {
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;
            if (left < n && a[left] < a[smallest]) smallest = left;
            if (right < n && a[right] < a[smallest]) smallest = right;
            if (smallest == i) break;
            int tmp = a[i];
            a[i] = a[smallest];
            a[smallest] = tmp;
            i = smallest;
        }
    }

    void buildMinHeap(int[] a) {
        int n = a.length;
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapifyDownArray(a, n, i);
        }
    }

    void heapsortDescMinHeap(int[] a) {
        buildMinHeap(a);
        for (int end = a.length - 1; end > 0; end--) {
            int tmp = a[0];
            a[0] = a[end];
            a[end] = tmp;
            heapifyDownArray(a, end, 0);
        }
    }

    // ---------- TESTS BELOW (same style as C / C++) ----------
    public static void main(String[] args) {
        // --- Test MinHeap.insert (no extract) ---
        System.out.println("=== Testing MinHeap.insert ===");
        MinHeap h1 = new MinHeap();
        int[] valuesToInsert = { 5, 3, 8, 1, 7, 2 };

        System.out.print("Inserting into MinHeap (no extract available): ");
        for (int v : valuesToInsert) {
            System.out.print(v + " ");
            h1.insert(v);  // just exercising insert
        }
        System.out.println();
        System.out.println();

        // --- Test MinHeapWithExtract.insert / extractMin ---
        System.out.println("=== Testing MinHeapWithExtract insert / extractMin ===");
        MinHeapWithExtract h2 = new MinHeapWithExtract();

        System.out.print("Inserting: ");
        for (int v : valuesToInsert) {
            System.out.print(v + " ");
            h2.insert(v);
        }
        System.out.println();

        System.out.print("Extracting in sorted order: ");
        while (true) {
            try {
                int minVal = h2.extractMin();
                System.out.print(minVal + " ");
            } catch (IllegalStateException e) {
                // heap underflow => done
                break;
            }
        }
        System.out.println();
        System.out.println();

        // --- Test buildMinHeap (uses heapifyDownArray) ---
        System.out.println("=== Testing buildMinHeap ===");
        section6 s = new section6();

        int[] arr1 = { 9, 4, 7, 1, 0, 3, 2 };
        System.out.print("Original array: ");
        printArray(arr1);

        s.buildMinHeap(arr1);
        System.out.print("After buildMinHeap (min-heap array): ");
        printArray(arr1);
        System.out.println();

        // --- Test heapsortDescMinHeap (also uses buildMinHeap & heapifyDownArray) ---
        System.out.println("=== Testing heapsortDescMinHeap (descending sort) ===");
        int[] arr2 = { 12, 3, 17, 8, 34, 25, 1 };

        System.out.print("Original array: ");
        printArray(arr2);

        s.heapsortDescMinHeap(arr2);
        System.out.print("After heapsortDescMinHeap (descending): ");
        printArray(arr2);
    }

    private static void printArray(int[] a) {
        for (int v : a) {
            System.out.print(v + " ");
        }
        System.out.println();
    }
}
