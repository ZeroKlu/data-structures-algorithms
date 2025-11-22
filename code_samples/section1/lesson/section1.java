public class section1 {
    public static void ex1(int[] arr) {
        for (int x : arr) {
            System.out.println(x);
        }
    }

    public static void ex2(int[] arr) {
        for (int i = 0; i < arr.length; i++) {
            for (int j = i; j < arr.length; j++) {
                System.out.println(arr[i] + ", " + arr[j]);
            }
        }
    }

    public static void ex3(int[] arr) {
        if (arr.length > 0) {
            System.out.println(arr[0] + " " + arr[arr.length - 1]);
        }
    }

    public static void ex4(int[] arr) {
        for (int x : arr) {
            System.out.println(x);
        }
        for (int x : arr) {
            System.out.println(x);
        }
    }

    public static int[] ex5(int[] arr) {
        int[] res = new int[arr.length];
        for (int i = 0; i < arr.length; i++) {
            res[i] = arr[i] * 2;
        }
        return res;
    }

    public static void main(String[] args) {
        int[] arr = { 1, 2, 3, 4, 5 };

        System.out.println("Ex 1:");
        ex1(arr);

        System.out.println("\nEx 2:");
        ex2(arr);

        System.out.println("\nEx 3:");
        ex3(arr);

        System.out.println("\nEx 4:");
        ex4(arr);

        System.out.println("\nEx 5:");
        int[] res = ex5(arr);
        for (int x : res) {
            System.out.println(x);
        }
    }
}
