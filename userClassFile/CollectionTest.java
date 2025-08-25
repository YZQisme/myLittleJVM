import java.util.*;

class Int implements Comparable<Int>{
    public int x;
    public Int(int x){
        this.x=x;
    }
    public int compareTo(Int o){
        return x-o.x;
    }
    public String toString(){
        return x+"";
    }
}

class N{
    public int w,next,to;
    public N(){}
    public N(int to,int w,int next){
        this.to=to;
        this.w=w;
        this.next=next;
    }
}

class Graph{
    public int[] head=new int[100];
    public int nodeC=0;
    public N[] edge=new N[200];
    public int sp=0;
    Graph(int n){
        nodeC=n;
        for(int i=0;i<=n;++i){
            head[i]=-1;
        }
    }
    public void addEdge(int from,int to,int w){
        edge[sp]=new N(to,w,head[from]);
        head[from]=sp++;
    }
}

class Pair {
    public int x, y;
    public Pair(){}
    @Override
    public int hashCode() {
        return (-x)^y;
    }
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Pair pair = (Pair) o;
        return x == pair.x && y == pair.y;
    }
    public Pair(int x, int y) {
        this.x = x;
        this.y = y;
    }
}

class PN implements Comparable<PN>{
    public int node,dist;
    public PN(int node, int dist) {
        this.node = node;
        this.dist = dist;
    }
    public PN(){}
    @Override
    public int compareTo(PN o) {
        return dist-o.dist;
    }
}

public class CollectionTest {
    public static Map<Pair,Int> edgeMaptoWeight=new HashMap<>();

    public static ArrayList<Int> Dijkstra(Graph g,int start,int end){
        ArrayList<Int> ans=new ArrayList<>();
        PriorityQueue<PN> q=new PriorityQueue<>();
        int[] dist=new int[g.nodeC+1];
        int[] fa=new int[g.nodeC+1];
        for(int i=0;i<=g.nodeC;++i){
            dist[i]=0x7fffffff;
            fa[i]=-1;
        }
        dist[start]=0;
        q.add(new PN(start,0));
        while(!q.isEmpty()){
            PN now=q.poll();
            if(now.node==end){
                break;
            }
            for(int i=g.head[now.node];i!=-1;i=g.edge[i].next){
                if(now.dist+g.edge[i].w<dist[g.edge[i].to]){
                    dist[g.edge[i].to]=now.dist+g.edge[i].w;
                    q.add(new PN(g.edge[i].to,dist[g.edge[i].to]));
                    fa[g.edge[i].to]=now.node;
                }
            }
        }
        for(int i=end;i!=-1;i=fa[i]){
            ans.add(new Int(i));
        }
        return ans;
    }

    public static void main(String[] args) {
        //Scanner sc=new Scanner(System.in);
        //int n=sc.nextInt();
        //int ec=sc.nextInt();
        int n=myIO.readInt();
        int ec=myIO.readInt();
        Graph g=new Graph(n);
        for(int i=0;i<ec;++i){
            //int a=sc.nextInt();
            //int b=sc.nextInt();
            //int c=sc.nextInt();
            int a=myIO.readInt();
            int b=myIO.readInt();
            int c=myIO.readInt();
            g.addEdge(a,b,c);
            edgeMaptoWeight.put(new Pair(a,b),new Int(c));
        }
        //int s=sc.nextInt();
        //int t=sc.nextInt();
        int s=myIO.readInt();
        int t=myIO.readInt();
        ArrayList<Int> ans=Dijkstra(g,s,t);
        for(int i=ans.size()-1;i>=1;--i){
            myIO.println("edge("+
                    ans.get(i)+"->"+ans.get(i-1)+
                    ") weight="+
                    edgeMaptoWeight.get(new Pair(ans.get(i).x,ans.get(i-1).x)).x);
        }
    }
}
/*
6 10
1 2 2
1 6 6
1 5 19
2 6 3
3 2 5
3 4 4
4 3 1
4 5 0
6 3 7
6 4 12
1 5
*/
/*
1->2->6->3->4->5
*/
