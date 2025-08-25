public class EulerSieve {
    public static void main(String[] args) {
        myIO.println("欧拉筛，请输入n:");
        int n=myIO.readInt();
        if(n<2){
            myIO.println("n过于小");
        }else {
            int[] arr = new int[n + 1];
            int[] prime = new int[n + 1];
            int sp = 0;
            for (int i = 2; i <= n; ++i) {
                arr[i] = 1;
            }
            for (int i = 2; i <= n; ++i) {
                if (arr[i] == 1) {
                    prime[sp++] = i;
                }
                for (int j = 0; j < sp; ++j) {
                    if (i * prime[j] > n) {
                        break;
                    }
                    arr[i * prime[j]] = 0;
                    if (i % prime[j] == 0) {
                        break;
                    }
                }
            }
            for (int i = 0; i < sp; ++i) {
                myIO.print(prime[i] + " ");
            }
        }
    }
}
