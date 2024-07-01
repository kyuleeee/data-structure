#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define INF INT_MAX

typedef struct {
    int vertex;
    int weight;
} Edge; //엣지

typedef struct {
    Edge* edges;
    int edge_count;
    int edge_capacity;
} Data; //엣지들을 담은 데이터 

typedef struct {
    int vertex;
    int dist;
} PriorityQueueNode; //priority queue의 노드

typedef struct {
    PriorityQueueNode* nodes;
    int size;
    int capacity;
} PriorityQueue; //priority queue

PriorityQueue* create_priority_queue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->nodes = (PriorityQueueNode*)malloc(capacity * sizeof(PriorityQueueNode));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
} //priority_queue만드는 노드 

void swap(PriorityQueueNode* a, PriorityQueueNode* b) {
    PriorityQueueNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PriorityQueue* pq, int idx) {
    while (idx > 0 && pq->nodes[idx].dist < pq->nodes[(idx - 1) / 2].dist) {
        swap(&pq->nodes[idx], &pq->nodes[(idx - 1) / 2]);
        idx = (idx - 1) / 2;
    }
} //값이 insert되거나 바뀌면 위로 올라가서 heapify_up하는 코드 

void heapify_down(PriorityQueue* pq, int idx) {
    int MIN = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < pq->size && pq->nodes[left].dist < pq->nodes[MIN].dist) { //heapify를 하는 데에 기준은 그 노드의 dist! 
        MIN = left;
    }
    if (right < pq->size && pq->nodes[right].dist < pq->nodes[MIN].dist) {
        MIN = right;
    }
    if (MIN != idx) {
        swap(&pq->nodes[idx], &pq->nodes[MIN]);
        heapify_down(pq, MIN);
    }
}

void insert(PriorityQueue* pq, int vertex, int dist) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->nodes = (PriorityQueueNode*)realloc(pq->nodes, pq->capacity * sizeof(PriorityQueueNode)); //동적으로 사이즈 늘려주기 
    }
    pq->nodes[pq->size].vertex = vertex; //마지막 노드에 추가하기 
    pq->nodes[pq->size].dist = dist; //마지막 노드에 추가하기 
    heapify_up(pq, pq->size); //아래에서부터 heapify를 해야함. 
    pq->size++;
} 

PriorityQueueNode extract_min(PriorityQueue* pq) {
    PriorityQueueNode min_node = pq->nodes[0]; //맨 첫번째에 있는 거 뽑기 
    pq->nodes[0] = pq->nodes[--pq->size]; //마지막에 있는 걸 맨 위로 올리기 
    heapify_down(pq, 0);  //다시 heapify하기 
    return min_node;
}

void decrease_key(PriorityQueue* pq, int vertex, int new_dist) {
    for (int i = 0; i < pq->size; i++) {
        if (pq->nodes[i].vertex == vertex) {
            pq->nodes[i].dist = new_dist; 
            heapify_up(pq, i); //아래쪽에서 값이 바뀌면, insert처럼 heapify_up이 필요 
            break;
        }
    }
}

bool is_empty(PriorityQueue* pq) {
    return pq->size == 0;
}

void free_priority_queue(PriorityQueue* pq) {
    free(pq->nodes);
    free(pq);
}

void add_edge(Data* graph, int u, int v, int weight) { 
    if (graph[u].edge_count == graph[u].edge_capacity) {
        graph[u].edge_capacity *= 2;
        graph[u].edges = (Edge*)realloc(graph[u].edges, graph[u].edge_capacity * sizeof(Edge));
    }
    graph[u].edges[graph[u].edge_count].vertex = v;
    graph[u].edges[graph[u].edge_count].weight = weight;
    graph[u].edge_count++;
} //엣지 추가하는 함수 

void read_graph_from_file(const char* filename, Data** graph, int* size, int* source) {
    
    //1. 파일 불러들이기 
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    //2. input file의 첫번째 줄 scan하기 
    int num_edges;
    fscanf(file, "%d %d %d", size, &num_edges, source);

    //3. 스캔한 num_edge 가지고 graph 초기화하기 
    *graph = (Data*)malloc(*size * sizeof(Data));
    for (int i = 0; i < *size; i++) {
        (*graph)[i].edges = (Edge*)malloc(10 * sizeof(Edge));  // 초기 용량 설정
        (*graph)[i].edge_count = 0;
        (*graph)[i].edge_capacity = 10;
    }
    
    //4. input file의 두번째줄부터 스캔하기 
    int u, v, weight;
    for (int i = 0; i < num_edges; i++) {
        fscanf(file, "%d %d %d", &u, &v, &weight);
        add_edge(*graph, u, v, weight);
    }

    fclose(file);
}

void dijkstra(Data* graph, int size, int source, const char* output_filename) {
    
    //1. dist , prev 초기화하기 (여기서의 dist,prev가 수도코드에서의 S와 같음)
    int* dist = (int*)malloc(size * sizeof(int));
    int* prev = (int*)malloc(size * sizeof(int));
    PriorityQueue* pq = create_priority_queue(size); //pq가 수도코드에서 Q와 같음


    for (int i = 0; i < size; i++) {
        dist[i] = INF;
        prev[i] = -1;
        insert(pq, i, INF);
    }

    //2. source노드의 dist는 0으로 만들어두기 
    dist[source] = 0;

    //3. source노드의 dist를 가준으로 힙성질 유지하게 하기 
    decrease_key(pq, source, 0);

    //4. 인접한 노드에 relaxation해주는 과정 
    while (!is_empty(pq)) {
        PriorityQueueNode u = extract_min(pq); //이렇게 해서 점점 pq에서 node를 제거해나간다. 
        for (int i = 0; i < graph[u.vertex].edge_count; i++) {  
            Edge edge = graph[u.vertex].edges[i]; 
            int v = edge.vertex;
            int weight = edge.weight;
            if (dist[v] > dist[u.vertex] + weight) {
                dist[v] = dist[u.vertex] + weight;
                prev[v] = u.vertex;
                decrease_key(pq, v, dist[v]);
            }
        }
    }


    //5. output_file열기 
    FILE* output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    //6. output_file에 S (dist,prev) print하기 
    for (int i = 0; i < size; i++) {
        
        if (prev[i] == -1) {
            fprintf(output_file, "%d %d %s\n", i, dist[i], "NIL");
        }else{
            fprintf(output_file, "%d %d %d\n", i, dist[i], prev[i]);
        }
    }

    fclose(output_file);

    free(dist);
    free(prev);
    free_priority_queue(pq);
}

int main() {
    Data* graph;
    int size;
    int source;

    read_graph_from_file("input.txt", &graph, &size, &source);
    dijkstra(graph, size, source, "output.txt");

    for (int i = 0; i < size; i++) {
        free(graph[i].edges);
    }
    free(graph);

    return 0;
}
