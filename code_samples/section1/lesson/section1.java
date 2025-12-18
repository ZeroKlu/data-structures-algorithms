public class section1 {
    // Print each element of an array
    public static void printArrayElements(int[] arr) {
        for (int x : arr) {
            System.out.println(x);
        }
    }

    // Print all pairs of elements of an array
    public static void printElementPairs(int[] arr) {
        for (int i = 0; i < arr.length; i++) {
            for (int j = i; j < arr.length; j++) {
                System.out.println(arr[i] + ", " + arr[j]);
            }
        }
    }

    // Print the first and last elements of an array
    public static void printFirstAndLast(int[] arr) {
        if (arr.length > 0) {
            System.out.println(arr[0] + " " + arr[arr.length - 1]);
        }
    }

    // Print each element of an array twice
    public static void printArrayTwice(int[] arr) {
        for (int x : arr) {
            System.out.println(x);
        }
        for (int x : arr) {
            System.out.println(x);
        }
    }

    // Multiply each element of an array by 2
    public static int[] doubleArrayElements(int[] arr) {
        int[] res = new int[arr.length];
        for (int i = 0; i < arr.length; i++) {
            res[i] = arr[i] * 2;
        }
        return res;
    }

    public static void main(String[] args) {
        int[] arr = { 1, 2, 3, 4, 5 };

        System.out.println("Ex 1:");
        printArrayElements(arr);

        System.out.println("\nEx 2:");
        printElementPairs(arr);

        System.out.println("\nEx 3:");
        printFirstAndLast(arr);

        System.out.println("\nEx 4:");
        printArrayTwice(arr);

        System.out.println("\nEx 5:");
        int[] res = doubleArrayElements(arr);
        for (int x : res) {
            System.out.println(x);
        }
    }
}
