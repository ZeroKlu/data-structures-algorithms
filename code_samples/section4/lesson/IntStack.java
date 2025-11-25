package code_samples.section4.lesson;

public class IntStack {
    private java.util.ArrayList<Integer> data = new java.util.ArrayList<>();

    public void push(int value) {
        data.add(value);
    }

    public int pop() {
        if (data.isEmpty()) {
            throw new IllegalStateException("stack underflow");
        }
        return data.remove(data.size() - 1);
    }

    public boolean isEmpty() {
        return data.isEmpty();
    }    
}
