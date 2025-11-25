package code_samples.section4.problems;

import java.util.Deque;
import java.util.ArrayDeque;

public class MyQueue {
    private Deque<Integer> in = new ArrayDeque<>();
    private Deque<Integer> out = new ArrayDeque<>();

    private void moveInToOut() {
        if (out.isEmpty()) {
            while (!in.isEmpty()) {
                out.push(in.pop());
            }
        }
    }

    public void enqueue(int x) {
        in.push(x);
    }

    public int dequeue() {
        moveInToOut();
        return out.pop();
    }

    public int front() {
        moveInToOut();
        return out.peek();
    }

    public boolean empty() {
        return in.isEmpty() && out.isEmpty();
    }
}
