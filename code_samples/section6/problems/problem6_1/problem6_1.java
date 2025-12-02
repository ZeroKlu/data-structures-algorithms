package code_samples.section6.problems.problem6_1;

class MinHeap {
    private int[] data;
    private int size;

    public MinHeap(int capacity) {
        data = new int[capacity];
        size = 0;
    }

    private void ensureCapacity() {
        if (size < data.length) return;
        int[] nd = new int[data.length * 2];
        System.arraycopy(data, 0, nd, 0, data.length);
        data = nd;
    }

    private void swap(int i, int j) {
        int tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;
    }

    private void bubbleUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (data[i] >= data[p]) break;
            swap(i, p);
            i = p;
        }
    }

    private void heapifyDown(int i) {
        int n = size;
        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            if (left < n && data[left] < data[smallest]) smallest = left;
            if (right < n && data[right] < data[smallest]) smallest = right;
            if (smallest == i) break;
            swap(i, smallest);
            i = smallest;
        }
    }

    public void insert(int x) {
        ensureCapacity();
        data[size] = x;
        bubbleUp(size);
        size++;
    }

    public Integer peek() {
        return size == 0 ? null : data[0];
    }

    public Integer pop() {
        if (size == 0) return null;
        int root = data[0];
        size--;
        if (size > 0) {
            data[0] = data[size];
            heapifyDown(0);
        }
        return root;
    }
}

public class problem6_1 {

    public static void main(String[] args) {
        System.out.println("=== Testing MinHeap ===");

        MinHeap h = new MinHeap(4); // small to test ensureCapacity
        int[] values = {5, 3, 8, 1, 7, 2};

        System.out.print("Inserting values: ");
        for (int v : values) {
            System.out.print(v + " ");
            h.insert(v);
            System.out.println("\n  After insert " + v + " -> peek (min): " + h.peek());
        }
        System.out.println();

        System.out.println("Extracting values in sorted order:");
        while (true) {
            Integer x = h.pop();
            if (x == null) break;
            System.out.print(x + " ");
        }
        System.out.println("\n");

        // Test peek/pop when heap is empty
        System.out.println("Testing peek/pop on empty heap:");
        System.out.println("peek(): " + h.peek());
        System.out.println("pop(): " + h.pop());
        System.out.println();

        // Reinsertion test
        System.out.println("Reinserting values: 10, 4, 6");
        h.insert(10);
        h.insert(4);
        h.insert(6);

        System.out.println("peek(): " + h.peek());

        System.out.print("Final pop sequence: ");
        while (true) {
            Integer x = h.pop();
            if (x == null) break;
            System.out.print(x + " ");
        }
        System.out.println();
    }
}
