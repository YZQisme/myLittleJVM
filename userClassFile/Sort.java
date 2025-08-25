public class Sort {
    //快速排序
    public static int[] arr=null;
    public static void fastSort(int left, int right){
        if(left>=right){
            return;
        }
        int pivot=arr[left];
        int i=left,temp;
        for(int j=left+1;j<=right;++j){
            if(arr[j]<=pivot){
                ++i;
                temp=arr[i];
                arr[i]=arr[j];
                arr[j]=temp;
            }
        }
        temp=arr[i];
        arr[i]=arr[left];
        arr[left]=temp;
        fastSort(left, i-1);
        fastSort(i+1, right);
    }
    public static void main(String[] args) {
        int n=myIO.readInt();
        arr=new int[n];
        for(int i=0;i<n;++i){
            arr[i]=myIO.readInt();
        }
        fastSort(0, n-1);
        for(int i=0;i<n;++i){
            myIO.print(arr[i]+" ");
        }
        myIO.println("");
    }
}
