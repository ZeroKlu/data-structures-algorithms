package code_samples.section2.lesson;

public class section2 {
    public static int sumArray(int[] arr) {
        int sum = 0;
        for (int x : arr) {
            sum += x;
        }
        return sum;
    }

    public static void reverseArray(int[] arr) {
        int left = 0;
        int right = arr.length - 1;
        while (left < right) {
            int temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
            left++;
            right--;
        }
    }

    public static boolean hasPairWithSum(int[] arr, int target) {
        int left = 0;
        int right = arr.length - 1;
        while (left < right) {
            int sum = arr[left] + arr[right];
            if (sum == target) {
                return true;
            } else if (sum < target) {
                left++;
            } else {
                right--;
            }
        }
        return false;
    }

    public static String reverseString(String s) {
        StringBuilder sb = new StringBuilder(s);
        sb.reverse();
        return sb.toString();
    }

    public static void main(String[] args) {
        int[] arr = { 1, 2, 3, 4, 5 };
        
        System.out.println("Sum of array: " + sumArray(arr));
        
        reverseArray(arr);
        System.out.print("Reversed array: ");
        for (int x : arr) {
            System.out.print(x + " ");
        }
        System.out.println();
        reverseArray(arr);
        
        System.out.println("Has pair with sum 9: " + (hasPairWithSum(arr, 9) ? "true" : "false"));
        
        String s = "hello";
        System.out.println("Reversed string: " + reverseString(s));
    }
}
