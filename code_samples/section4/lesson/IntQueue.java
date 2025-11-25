package code_samples.section4.lesson;

public class IntQueue {
    private java.util.ArrayDeque<Integer> data = new java.util.ArrayDeque<>();

    public void enqueue(int value) {
        data.addLast(value);
    }

    public int dequeue() {
        if (data.isEmpty()) {
            throw new IllegalStateException("queue underflow");
        }
        return data.removeFirst();
    }

    public boolean isEmpty() {
        return data.isEmpty();
    }
}
