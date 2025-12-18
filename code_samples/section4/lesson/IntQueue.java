package code_samples.section4.lesson;

// Defines a queue of integers
public class IntQueue {
    // The queue
    private java.util.ArrayDeque<Integer> data = new java.util.ArrayDeque<>();

    // Add value to the queue
    public void enqueue(int value) {
        data.addLast(value);
    }

    // Remove value from the queue
    public int dequeue() {
        if (data.isEmpty()) {
            throw new IllegalStateException("queue underflow");
        }
        return data.removeFirst();
    }

    // Check if the queue is empty
    public boolean isEmpty() {
        return data.isEmpty();
    }
}
