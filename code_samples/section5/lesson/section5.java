package code_samples.section5.lesson;

import java.util.ArrayList;
import java.util.List;

public class section5 {
    public static void preorder(TreeNode root) {
        if (root == null) return;
        System.out.println(root.value);
        preorder(root.left);
        preorder(root.right);
    }
    
    public static void levelOrder(TreeNode root) {
        if (root == null) return;
        java.util.Queue<TreeNode> queue = new java.util.ArrayDeque<>();
        queue.add(root);

        while (!queue.isEmpty()) {
            TreeNode node = queue.remove();
            System.out.println(node.value);
            if (node.left != null) queue.add(node.left);
            if (node.right != null) queue.add(node.right);
        }
    }

    public static void dfsUtil(int node, java.util.List<java.util.List<Integer>> graph,
             boolean[] visited) {
        visited[node] = true;
        System.out.println(node);
        for (int neighbor : graph.get(node)) {
            if (!visited[neighbor]) {
                dfsUtil(neighbor, graph, visited);
            }
        }
    }

    public static void dfs(int start, java.util.List<java.util.List<Integer>> graph) {
        int n = graph.size();
        boolean[] visited = new boolean[n];
        dfsUtil(start, graph, visited);
    }

    public static java.util.List<Integer> shortestDistances(int start,
            java.util.List<java.util.List<Integer>> graph) {
        int n = graph.size();
        java.util.List<Integer> dist = new java.util.ArrayList<>(n);
        for (int i = 0; i < n; i++) {
            dist.add(-1);
        }

        java.util.Queue<Integer> queue = new java.util.ArrayDeque<>();
        dist.set(start, 0);
        queue.add(start);

        while (!queue.isEmpty()) {
            int node = queue.remove();
            for (int neighbor : graph.get(node)) {
                if (dist.get(neighbor) == -1) {
                    dist.set(neighbor, dist.get(node) + 1);
                    queue.add(neighbor);
                }
            }
        }
        return dist;
    }
    
    public static void main(String[] args) {

        System.out.println("==== TEST BINARY TREE (preorder + levelOrder) ====");

        // Build this tree:
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

        // Graph:
        // 0 -- 1
        // |  /
        // 2
        // |
        // 3
        //
        // Adjacency list (undirected)
        List<List<Integer>> graph = new ArrayList<>();
        graph.add(List.of(1, 2));      // neighbors of 0
        graph.add(List.of(0, 2));      // neighbors of 1
        graph.add(List.of(0, 1, 3));   // neighbors of 2
        graph.add(List.of(2));         // neighbors of 3

        System.out.println("DFS starting at node 0 (expect 0 1 2 3):");
        section5.dfs(0, graph);

        System.out.println("\n==== TEST shortestDistances (BFS) ====");

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
