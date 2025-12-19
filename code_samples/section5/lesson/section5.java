package code_samples.section5.lesson;

import java.util.ArrayList;
import java.util.List;

public class section5 {

    // ==========================
    // BINARY TREE TRAVERSALS
    // ==========================

    // Preorder traversal of a binary tree
    // Order: Root -> Left -> Right
    public static void preorder(TreeNode root) {
        // Base case: if the current node is null, stop recursion
        if (root == null) return;

        // Visit the current node
        System.out.println(root.value);

        // Recursively traverse the left subtree
        preorder(root.left);

        // Recursively traverse the right subtree
        preorder(root.right);
    }

    // Level-order traversal (Breadth-First Search) of a binary tree
    public static void levelOrder(TreeNode root) {
        // If the tree is empty, nothing to traverse
        if (root == null) return;

        // Queue used to process nodes level by level
        java.util.Queue<TreeNode> queue = new java.util.ArrayDeque<>();

        // Start traversal from the root
        queue.add(root);

        // Continue until all nodes have been processed
        while (!queue.isEmpty()) {
            // Remove the next node from the queue
            TreeNode node = queue.remove();

            // Visit the node
            System.out.println(node.value);

            // Add left child to the queue if it exists
            if (node.left != null) queue.add(node.left);

            // Add right child to the queue if it exists
            if (node.right != null) queue.add(node.right);
        }
    }

    // ==========================
    // GRAPH DEPTH-FIRST SEARCH
    // ==========================

    // Helper method for recursive DFS traversal
    public static void dfsUtil(
            int node,
            java.util.List<java.util.List<Integer>> graph,
            boolean[] visited) {

        // Mark the current node as visited
        visited[node] = true;

        // Visit the node
        System.out.println(node);

        // Explore all adjacent nodes
        for (int neighbor : graph.get(node)) {
            // Recurse only if the neighbor has not been visited
            if (!visited[neighbor]) {
                dfsUtil(neighbor, graph, visited);
            }
        }
    }

    // Entry point for DFS traversal
    public static void dfs(int start, java.util.List<java.util.List<Integer>> graph) {
        // Number of nodes in the graph
        int n = graph.size();

        // Track visited nodes to avoid cycles
        boolean[] visited = new boolean[n];

        // Start DFS from the given node
        dfsUtil(start, graph, visited);
    }

    // ==========================
    // SHORTEST PATHS (BFS)
    // ==========================

    // Computes shortest distances from a start node in an unweighted graph
    // Uses Breadth-First Search (BFS)
    public static java.util.List<Integer> shortestDistances(
            int start,
            java.util.List<java.util.List<Integer>> graph) {

        int n = graph.size();

        // Distance list initialized to -1 (unreachable)
        java.util.List<Integer> dist = new java.util.ArrayList<>(n);
        for (int i = 0; i < n; i++) {
            dist.add(-1);
        }

        // Queue used for BFS traversal
        java.util.Queue<Integer> queue = new java.util.ArrayDeque<>();

        // Distance to the start node is zero
        dist.set(start, 0);
        queue.add(start);

        // Standard BFS loop
        while (!queue.isEmpty()) {
            int node = queue.remove();

            // Visit all neighbors of the current node
            for (int neighbor : graph.get(node)) {
                // If neighbor has not been visited yet
                if (dist.get(neighbor) == -1) {
                    // Update distance and enqueue neighbor
                    dist.set(neighbor, dist.get(node) + 1);
                    queue.add(neighbor);
                }
            }
        }

        // Return shortest distances from the start node
        return dist;
    }

    // ==========================
    // TESTING / DEMO
    // ==========================

    public static void main(String[] args) {

        System.out.println("==== TEST BINARY TREE (preorder + levelOrder) ====");

        // Build this binary tree:
        //
        //        1
        //       / \
        //      2   3
        //     / \   \
        //    4   5   6
        //
        TreeNode n4 = new TreeNode(4);
        TreeNode n5 = new TreeNode(5);
        TreeNode n6 = new TreeNode(6);
        TreeNode n2 = new TreeNode(2);
        TreeNode n3 = new TreeNode(3);
        TreeNode root = new TreeNode(1);

        // Connect child nodes
        n2.left = n4;
        n2.right = n5;
        n3.right = n6;
        root.left = n2;
        root.right = n3;

        System.out.println("Preorder traversal (expect 1 2 4 5 3 6):");
        section5.preorder(root);

        System.out.println("\nLevel-order traversal (expect 1 2 3 4 5 6):");
        section5.levelOrder(root);

        System.out.println("\n==== TEST GRAPH DFS ====");

        // Undirected graph:
        //
        // 0 -- 1
        // |  /
        // 2
        // |
        // 3
        //
        // Adjacency list representation
        List<List<Integer>> graph = new ArrayList<>();
        graph.add(List.of(1, 2));      // neighbors of 0
        graph.add(List.of(0, 2));      // neighbors of 1
        graph.add(List.of(0, 1, 3));   // neighbors of 2
        graph.add(List.of(2));         // neighbors of 3

        System.out.println("DFS starting at node 0 (expect 0 1 2 3):");
        section5.dfs(0, graph);

        System.out.println("\n==== TEST shortestDistances (BFS) ====");

        // Compute shortest distances from node 0
        List<Integer> dist = section5.shortestDistances(0, graph);

        System.out.println("Shortest distances from node 0:");
        for (int i = 0; i < dist.size(); i++) {
            System.out.println("dist to " + i + " = " + dist.get(i));
        }

        // Expected:
        // dist[0] = 0
        // dist[1] = 1
        // dist[2] = 1
        // dist[3] = 2

        System.out.println("\n==== ALL TESTS COMPLETE ====");
    }
}
