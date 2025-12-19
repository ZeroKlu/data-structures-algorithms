package code_samples.section4.problems;

import java.util.Deque;
import java.util.ArrayDeque;

// Defines an integer queue
public class MyQueue {
    // Optimize enqueue and dequeue by using two lists
    // Enqueue by adding to `in`
    private Deque<Integer> in = new ArrayDeque<>();
    // Dequeue by moving all values from `in` to `out` then removing from `out`
    private Deque<Integer> out = new ArrayDeque<>();

    // Move values from in to out
    private void moveInToOut() {
        if (out.isEmpty()) {
            while (!in.isEmpty()) {
                out.push(in.pop());
            }
        }
    }

    // Add value to the queue
    public void enqueue(int x) {
        in.push(x);
    }

    // Remove value from the queue
    public int dequeue() {
        moveInToOut();
        return out.pop();
    }

    // Return value at front of queue
    public int front() {
        moveInToOut();
        return out.peek();
    }

    // Check if the queue is empty
    public boolean empty() {
        return in.isEmpty() && out.isEmpty();
    }
}
