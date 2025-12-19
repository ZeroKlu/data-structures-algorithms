package code_samples.section5.lesson;

// Represents a single node in a binary tree
public class TreeNode {

    // Value stored in the node
    int value;

    // Reference to the left child node (null if none)
    TreeNode left;

    // Reference to the right child node (null if none)
    TreeNode right;

    // Constructor to create a node with a given value
    // Left and right children are initialized to null by default
    TreeNode(int value) {
        this.value = value;
    }
}
