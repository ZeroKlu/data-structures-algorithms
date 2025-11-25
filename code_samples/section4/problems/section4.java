package code_samples.section4.problems;

import java.util.Deque;
import java.util.List;
import java.util.Queue;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;

public class section4 {
    public static boolean isValid(String s) {
        Deque<Character> stack = new ArrayDeque<>();
        for (char c : s.toCharArray()) {
            if (c == '(' || c == '[' || c == '{') {
                stack.push(c);
            } else {
                if (stack.isEmpty()) return false;
                char open = stack.pop();
                if ((c == ')' && open != '(') ||
                    (c == ']' && open != '[') ||
                    (c == '}' && open != '{')) {
                    return false;
                }
            }
        }
        return stack.isEmpty();
    }

    int[] nextGreaterElements(int[] nums) {
        int n = nums.length;
        int[] ans = new int[n];
        java.util.Arrays.fill(ans, -1);
        Deque<Integer> stack = new ArrayDeque<>(); // stores values

        for (int i = n - 1; i >= 0; i--) {
            int x = nums[i];
            while (!stack.isEmpty() && stack.peek() <= x) {
                stack.pop();
            }
            ans[i] = stack.isEmpty() ? -1 : stack.peek();
            stack.push(x);
        }
        return ans;
    }

    int[] dailyTemperatures(int[] temps) {
        int n = temps.length;
        int[] ans = new int[n];
        Deque<Integer> stack = new ArrayDeque<>(); // indices

        for (int i = 0; i < n; i++) {
            while (!stack.isEmpty() && temps[i] > temps[stack.peek()]) {
                int idx = stack.pop();
                ans[idx] = i - idx;
            }
            stack.push(i);
        }
        return ans;
    }

    List<List<Integer>> levelOrder(TreeNode root) {
        List<List<Integer>> result = new ArrayList<>();
        if (root == null) return result;

        Queue<TreeNode> q = new ArrayDeque<>();
        q.add(root);

        while (!q.isEmpty()) {
            int size = q.size();
            List<Integer> level = new ArrayList<>(size);
            for (int i = 0; i < size; i++) {
                TreeNode node = q.remove();
                level.add(node.val);
                if (node.left != null) q.add(node.left);
                if (node.right != null) q.add(node.right);
            }
            result.add(level);
        }
        return result;
    }

    int[] maxSlidingWindow(int[] nums, int k) {
        int n = nums.length;
        if (n == 0 || k == 0) return new int[0];

        int[] res = new int[n - k + 1];
        Deque<Integer> dq = new ArrayDeque<>(); // indices
        int idx = 0;

        for (int i = 0; i < n; i++) {
            while (!dq.isEmpty() && dq.peekFirst() <= i - k) {
                dq.removeFirst();
            }
            while (!dq.isEmpty() && nums[dq.peekLast()] <= nums[i]) {
                dq.removeLast();
            }
            dq.addLast(i);

            if (i >= k - 1) {
                res[idx++] = nums[dq.peekFirst()];
            }
        }
        return res;
    }
    
    public static void main(String[] args) {
        section4 helper = new section4();

        System.out.println("==== TEST isValid ====");
        String[] parenTests = {
            "()",
            "([])",
            "{[()]}",
            "([)]",
            "((())",
            "abc(def[ghi]{jkl})",
            "",
            "{[}]"
        };

        for (String s : parenTests) {
            System.out.println(
                "Test \"" + s + "\": " +
                (section4.isValid(s) ? "valid" : "invalid")
            );
        }

        System.out.println("\n==== TEST MyQueue ====");
        MyQueue q = new MyQueue();
        System.out.println("Enqueue: 1, 2, 3");
        q.enqueue(1);
        q.enqueue(2);
        q.enqueue(3);

        System.out.println("Front: " + q.front());
        System.out.println("Dequeue: " + q.dequeue());
        System.out.println("Dequeue: " + q.dequeue());
        System.out.println("Empty? " + (q.empty() ? "true" : "false"));
        System.out.println("Dequeue: " + q.dequeue());
        System.out.println("Empty? " + (q.empty() ? "true" : "false"));

        System.out.println("\n==== TEST nextGreaterElements ====");
        int[] nums = {2, 1, 2, 4, 3};
        int[] nge = helper.nextGreaterElements(nums);
        System.out.println("nums: " + Arrays.toString(nums));
        System.out.println("NGE : " + Arrays.toString(nge));

        System.out.println("\n==== TEST dailyTemperatures ====");
        int[] temps = {73, 74, 75, 71, 69, 72, 76, 73};
        int[] days = helper.dailyTemperatures(temps);
        System.out.println("temps: " + Arrays.toString(temps));
        System.out.println("wait : " + Arrays.toString(days));

        System.out.println("\n==== TEST levelOrder ====");

        // Build this tree:
        //
        //        1
        //       / \
        //      2   3
        //     /   / \
        //    4   5   6
        //
        TreeNode node4 = new TreeNode(4);
        TreeNode node5 = new TreeNode(5);
        TreeNode node6 = new TreeNode(6);
        TreeNode node2 = new TreeNode(2);
        TreeNode node3 = new TreeNode(3);
        TreeNode root  = new TreeNode(1);

        node2.left = node4;
        node3.left = node5;
        node3.right = node6;
        root.left = node2;
        root.right = node3;

        List<List<Integer>> levels = helper.levelOrder(root);
        for (int i = 0; i < levels.size(); i++) {
            System.out.println("level " + i + ": " + levels.get(i));
        }

        System.out.println("\n==== TEST maxSlidingWindow ====");
        int[] arr = {1, 3, -1, -3, 5, 3, 6, 7};
        int k = 3;
        int[] windowMax = helper.maxSlidingWindow(arr, k);
        System.out.println("nums: " + Arrays.toString(arr));
        System.out.println("max window size " + k + ": " + Arrays.toString(windowMax));

        System.out.println("\n==== ALL TESTS COMPLETE ====");
    }
}
