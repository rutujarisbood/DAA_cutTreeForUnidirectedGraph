//student id: 1001843943
//name: Rutuja Risbood

// Derived from
//ref :  http://ranger.uta.edu/~weems/NOTES5311/ffLab.c


#include <stdio.h>
#include <stdlib.h>

// Basic Definitions

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define oo 1000000000

// Declarations

int n;  // number of nodes
int residualEdges;  // number of edges in residual network
struct edge {
    int tail, head, capacity, flow, inverse;
};
typedef struct edge edgeType;
edgeType* edgeTab;
int* firstEdge;  // Table indicating first in range of edges with a common tail

int* color;      // Needed for breadth-first search               
int* pred;       // Array to store augmenting path
int* predEdge;   // edgeTab subscript of edge used to reach vertex i in BFS

int* arrayX;
int* arrayY;
int* P;
int* F;
int Xctr = 0, Yctr = 0;
int flag;

//float CPUtime()
//{
//    struct rusage rusage;
//
//    getrusage(RUSAGE_SELF, &rusage);
//    return rusage.ru_utime.tv_sec + rusage.ru_utime.tv_usec / 1000000.0
//        + rusage.ru_stime.tv_sec + rusage.ru_stime.tv_usec / 1000000.0;
//}

int minm(int x, int y)
{
    return x < y ? x : y;  // returns minimum of x and y
}

// Queue for breadth-first search - not circular.

int head, tail;
int* q;

void enqueue(int x)
{
    q[tail] = x;
    tail++;
    color[x] = GRAY;
}

int dequeue()
{
    int x = q[head];
    head++;
    color[x] = BLACK;
    return x;
}

// Breadth-First Search for an augmenting path

int bfs(int start, int target)
// Searches for path from start to target.
// Returns 1 if found, otherwise 0.
{
    int u, v, i;

    for (u = 0; u < n; u++)
        color[u] = WHITE;
    head = tail = 0;  // Since q is not circular, it is reinitialized for each BFS
    enqueue(start);
    pred[start] = -1;
    while (head != tail)
    {
        u = dequeue();
        if (u == target)
            return 1;

        // Search all adjacent white nodes v. If the residual capacity
        // from u to v in the residual network is positive,
        // enqueue v.
        for (i = firstEdge[u]; i < firstEdge[u + 1]; i++)
        {
            v = edgeTab[i].head;
            if (color[v] == WHITE && edgeTab[i].capacity - edgeTab[i].flow > 0)
            {
                enqueue(v);
                pred[v] = u;
                predEdge[v] = i;
            }
        }
    }
    // No augmenting path remains, so a maximum flow and minimum cut
    // have been found.  Black vertices are in the
    // source side (S) of the minimum cut, while white vertices are in the
    // sink side (T).

    return 0;
}

// Ford-Fulkerson Algorithm
void resetXY() {
    for (int i = 0;i < n;i++) {
        arrayX[i] = -1;
        arrayY[i] = -1;
    }
}

int max_flow(int source, int sink)
{
    resetXY();
    int i, j, u;
    int max_flow;
    int APcount = 0;

    color = (int*)malloc(n * sizeof(int));
    pred = (int*)malloc(n * sizeof(int));
    predEdge = (int*)malloc(n * sizeof(int));
    q = (int*)malloc(n * sizeof(int));
    if (!color || !pred || !predEdge || !q)
    {
        printf("malloc failed %d\n", __LINE__);
        exit(0);
    }

    // Initialize empty flow.
    max_flow = 0;
    for (i = 0; i < residualEdges; i++)
        edgeTab[i].flow = 0;

    // While there exists an augmenting path,
    // increment the flow along this path.
    while (bfs(source, sink))
    {
        // Determine the amount by which we can increment the flow.
        int increment = oo;
        APcount++;
        for (u = sink; pred[u] != (-1); u = pred[u])
        {
            i = predEdge[u];
            increment = minm(increment, edgeTab[i].capacity - edgeTab[i].flow);
        }
        // Now increment the flow.
        for (u = sink; pred[u] != (-1); u = pred[u])
        {
            i = edgeTab[predEdge[u]].inverse;
            edgeTab[predEdge[u]].flow += increment;
            edgeTab[i].flow -= increment;  // Reverse in residual
        }
        if (n <= 20)
        {
            // Path trace
            for (u = sink; pred[u] != (-1); u = pred[u]) {}
                //printf("%d<-", u);
            //printf("%d adds %d incremental flow\n", source, increment);
        }
        max_flow += increment;
    }
    //printf("%d augmenting paths\n", APcount);
    // No more augmenting paths, so cut is based on reachability from last BFS.
    if (n <= 20)
    {
        //printf("\nS side of min-cut:\n");
        
        for (u = 0; u < n; u++)
            if (color[u] == BLACK) {
                //printf("%d\n", u);
                arrayX[Xctr] = u;Xctr++;
            }

        //printf("T side of min-cut:\n");
        for (u = 0; u < n; u++)
            if (color[u] == WHITE) {
                //printf("%d\n", u);
                arrayY[Yctr] = u;Yctr++;
            }
    }

    free(color);
    free(pred);
    free(predEdge);
    free(q);

    return max_flow;
}

// Reading the input file and organize adjacency lists for residual network.

int tailThenHead(const void* xin, const void* yin)
// Used in calls to qsort() and bsearch() for read_input_file()
{
    int result;
    edgeType* x, * y;

    x = (edgeType*)xin;
    y = (edgeType*)yin;
    result = x->tail - y->tail;
    if (result != 0)
        return result;
    else
        return x->head - y->head;
}

void dumpEdges(int count)
{
    int i;

    //printf("  i tail head  cap\n");
    for (i = 0; i < count; i++) {}
        //printf("%3d %3d  %3d %5d\n", i, edgeTab[i].tail, edgeTab[i].head,
            //edgeTab[i].capacity);
}

void dumpFinal()
{
    int i;

    //printf("Initialized residual network:\n");
    //printf("Vertex firstEdge\n");
    for (i = 0; i < n; i++)
        //printf(" %3d    %3d\n", i, firstEdge[i]);
    //printf("=================\n");
    //printf(" %3d    %3d\n", n, firstEdge[n]);

    //printf("  i tail head  cap  inv\n");
        for (i = 0; i < residualEdges; i++) {}
        //printf("%3d %3d  %3d %5d  %3d\n", i, edgeTab[i].tail, edgeTab[i].head,
            //edgeTab[i].capacity, edgeTab[i].inverse);
}

void read_input_file()
{
    int tail, head, capacity, i, j;
    int inputEdges;     // Number of edges in input file.
    int workingEdges;   // Number of residual network edges initially 
                        // generated from input file.
    edgeType work;
    edgeType* ptr;
    float startCPU, stopCPU;

    // read number of nodes and edges
    scanf("%d %d", &n, &inputEdges);
    arrayX = (int*)malloc(n * sizeof(int));
    arrayY = (int*)malloc(n * sizeof(int));
    P = (int*)malloc(n * sizeof(int));
    F = (int*)malloc(n * sizeof(int));

    for(int i = 0;i < n;i++) {
        P[i] = 0;
        F[i] = 0;
        //arrayX[i] = (int)0;
        //printf("in for arrayX[i] : %d",arrayX[i]);
    }
    // Table is allocated at worst-case size, since space for inverses is needed.
    edgeTab = (edgeType*)malloc(2 * inputEdges * sizeof(edgeType));
    if (!edgeTab)
    {
        //printf("edgeTab malloc failed %d\n", __LINE__);
        exit(0);
    }
    // read edges, each with a capacity
    workingEdges = 0;
    for (i = 0; i < inputEdges; i++)
    {
        scanf("%d %d %d", &tail, &head, &capacity);
        // Test for illegal edge, including incoming to source and outgoing from
        // sink.
        if (tail < 0 || tail > n - 1 || head < 0 || head >= n || capacity <= 0)
        {
            printf("Invalid input %d %d %d at %d\n", tail, head, capacity, __LINE__);
            exit(0);
        }
        // Save input edge
        edgeTab[workingEdges].tail = tail;
        edgeTab[workingEdges].head = head;
        edgeTab[workingEdges].capacity = capacity;
        workingEdges++;
        // Save inverse of input edge
        edgeTab[workingEdges].tail = head;
        edgeTab[workingEdges].head = tail;
        edgeTab[workingEdges].capacity = capacity;
        workingEdges++;
    }
    if (n <= 20)
    {
        //printf("Input & inverses:\n");
        dumpEdges(workingEdges);
    }

    // Sort edges to make edges with common tail contiguous in edgeTab,
    // along with making parallel edges contiguous.
    // A faster sort is unlikely to speed-up substantially.
    //startCPU = CPUtime();
    qsort(edgeTab, workingEdges, sizeof(edgeType), tailThenHead);
    //stopCPU = CPUtime();
    //printf("qsort CPU %f\n", stopCPU - startCPU);
    if (n <= 20)
    {
        //printf("Sorted edges:\n");
        dumpEdges(workingEdges);
    }

    // Coalesce parallel edges into a single edge by adding their capacities.
    residualEdges = 0;
    for (i = 1; i < workingEdges; i++)
        if (edgeTab[residualEdges].tail == edgeTab[i].tail
            && edgeTab[residualEdges].head == edgeTab[i].head)
            edgeTab[residualEdges].capacity += edgeTab[i].capacity;  // || case
        else
        {
            residualEdges++;
            edgeTab[residualEdges].tail = edgeTab[i].tail;
            edgeTab[residualEdges].head = edgeTab[i].head;
            edgeTab[residualEdges].capacity = edgeTab[i].capacity;
        }
    residualEdges++;
    if (n <= 20)
    {
        //printf("Coalesced edges:\n");
        dumpEdges(residualEdges);
    }

    // Set field in each edgeTab struct to point to inverse
    //startCPU = CPUtime();
    for (i = 0; i < residualEdges; i++)
    {
        work.tail = edgeTab[i].head;
        work.head = edgeTab[i].tail;
        ptr = (edgeType*)bsearch(&work, edgeTab, residualEdges, sizeof(edgeType),
            tailThenHead);
        if (ptr == NULL)
        {
            //printf("bsearch %d failed %d\n", i, __LINE__);
            exit(0);
        }
        edgeTab[i].inverse = ptr - edgeTab;  // ptr arithmetic to get subscript
    }
    //stopCPU = CPUtime();
    //printf("set inverses CPU %f\n", stopCPU - startCPU);

    // For each vertex i, determine first edge in edgeTab with
    // a tail >= i.
    firstEdge = (int*)malloc((n + 1) * sizeof(int));
    if (!firstEdge)
    {
        printf("malloc failed %d\n", __LINE__);
        exit(0);
    }
    j = 0;
    for (i = 0; i < n; i++)
    {
        firstEdge[i] = j;
        // Skip over edges with vertex i as their tail.
        for (;
            j < residualEdges && edgeTab[j].tail == i;
            j++)
            ;
    }
    firstEdge[n] = residualEdges;  //Sentinel
    if (n <= 20)
        dumpFinal();
}

void findValInArr(int val,int* arr) {
    for (int i = 0;i < n;i++) {
        if (arr[i] == val) {
            flag = 1;//found
        }
    }
    
}

int main()
{
    //int i, j;
    int start, end;
    float startCPU, stopCPU;
    

    read_input_file();
    //startCPU = CPUtime();
    //printf("S : %d, T : %d",1,n-1);
    //start = 1;end = n-1;
    /*for (int i = 0;i < n;i++) {
        P[i] = 1;
    }*/
    for (int k = 1;k < n;k++) {
        start = k;
        end = P[k];
        //for (int j = 0;j < n;j++) {
            //end = j;
            //****//
            //printf("\n start : %d, end : %d\n", start, end);
            int maxFlow = max_flow(start, end);
            //printf("total flow is %d", maxFlow);  // 0=source, n-1=sink

            //printf("\narray X (S): ");
            //for (int i = 0;i < Xctr;i++) {
            //    printf(" %d", arrayX[i]);
            //}

            //printf("\narray Y (T): ");
            //for (int i = 0;i < Yctr;i++) {
            //    printf(" %d", arrayY[i]);
            //}
           // printf("\n");

            F[start] = maxFlow;
            

            for (int i = 0;i < n;i++) {
                
                flag = 0;
                findValInArr(i, arrayX);
                if (flag == 1 && i != start && P[i] == end) {
                    //printf("modifyP");
                    P[i] = start;
                }
                //if i in X && i!=s && p[i]=end then=> p[i]=start 
            }

            //if P[end] in X => isPendInX=0
            int isPendInX = 0;//0=true
            flag = 0;
            findValInArr(P[end], arrayX);
            if (flag == 1) {
                P[start] = P[end];
                P[end] = start;
                F[start] = F[end];
                F[end] = maxFlow; //f[s,t]
            }
            Yctr = 0;Xctr = 0;


        //}
    }
    //**//
    free(edgeTab);
    free(firstEdge);
    printf("\noutput: ");
    for (int i = 1;i < n;i++) {
        printf(" \n%d", i);
        printf(" \t%d", P[i]);
        printf(" \t%d", F[i]);
    }

}
