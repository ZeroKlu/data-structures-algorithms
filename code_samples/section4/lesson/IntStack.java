package code_samples.section4.lesson;

// Defines a stack of integers
public class IntStack {
    // The stack
    private java.util.ArrayList<Integer> data = new java.util.ArrayList<>();

    // Add value to the stack
    public void push(int value) {
        data.add(value);
    }

    // Remove value from the stack
    public int pop() {
        if (data.isEmpty()) {
            throw new IllegalStateException("stack underflow");
        }
        return data.remove(data.size() - 1);
    }

    // Check if the stack is empty
    public boolean isEmpty() {
        return data.isEmpty();
    }    
}
