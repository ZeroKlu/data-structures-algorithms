// ===== Tests =====

static void Print(string label, IEnumerable<int> seq) =>
    Console.WriteLine(label + string.Join(" ", seq));

Console.WriteLine("=== Testing MinHeap ===");

var h = new MinHeap();
int[] values = [5, 3, 8, 1, 7, 2];

Console.Write("Inserting values: ");
foreach (var v in values) {
    Console.Write(v + " ");
    h.Insert(v);
    Console.WriteLine($"\n  After insert {v} -> peek (min): {h.Peek()}");
}
Console.WriteLine();

Console.Write("Extracting in sorted order: ");
while (true) {
    var x = h.Pop();
    if (x == null) break;
    Console.Write(x + " ");
}
Console.WriteLine("\n");

Console.WriteLine("Testing peek/pop on empty heap:");
Console.WriteLine("peek(): " + h.Peek());
Console.WriteLine("pop(): " + h.Pop());
Console.WriteLine();

Console.WriteLine("Reinserting values: 10, 4, 6");
h.Insert(10);
h.Insert(4);
h.Insert(6);

Console.WriteLine($"peek(): {h.Peek()}");

Console.Write("Final pop sequence: ");
while (true) {
    var x = h.Pop();
    if (x == null) break;
    Console.Write(x + " ");
}
Console.WriteLine();

class MinHeap {
    private readonly List<int> data = new List<int>();

    private void Swap(int i, int j) {
        (data[j], data[i]) = (data[i], data[j]);
    }

    private void BubbleUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (data[i] >= data[p]) break;
            Swap(i, p);
            i = p;
        }
    }

    private void HeapifyDown(int i) {
        int n = data.Count;
        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            if (left < n && data[left] < data[smallest]) smallest = left;
            if (right < n && data[right] < data[smallest]) smallest = right;
            if (smallest == i) break;
            Swap(i, smallest);
            i = smallest;
        }
    }

    public void Insert(int x) {
        data.Add(x);
        BubbleUp(data.Count - 1);
    }

    public int? Peek() {
        return data.Count == 0 ? (int?)null : data[0];
    }

    public int? Pop() {
        if (data.Count == 0) return null;
        if (data.Count == 1) {
            int v = data[0];
            data.RemoveAt(0);
            return v;
        }
        int root = data[0];
        data[0] = data[^1];
        data.RemoveAt(data.Count - 1);
        HeapifyDown(0);
        return root;
    }
}
