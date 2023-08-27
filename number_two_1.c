#include <stdio.h>
#include <stdlib.h>

struct listNode{
    int vertex;
    struct listNode * linked;
    struct listNode * nextNode;
    struct listNode * prevNode;
};

struct node {
    int vertex;
    int status;
    struct listNode * nextHead;
    struct listNode * prevHead;
};

struct Graph {
    int size;
    int numVertices;
    int numEdges;
    int* visited; // ????????
    struct node** nodes;
};

void assignListNode(struct listNode * assigned, int vertex, struct listNode * linked, struct listNode * nextNode, struct listNode * prevNode){
    assigned -> vertex = vertex;
    assigned -> linked = linked;
    assigned -> nextNode = nextNode;
    assigned -> prevNode = prevNode;
}

struct listNode* createListNode(){
    struct listNode * newNode = calloc(1, sizeof(struct listNode));
    newNode -> vertex = 0;    
    newNode -> linked = NULL;
    newNode -> nextNode = NULL;
    newNode -> prevNode = NULL;
    return newNode; 
}

// Создание вершины
struct node* createNode(int v) {
    struct node* newNode = calloc(1, sizeof(struct node));
    newNode -> vertex = v;
    newNode -> status = 0; 
    newNode -> nextHead = createListNode();
    newNode -> prevHead = createListNode();
    assignListNode(newNode -> nextHead, 0xdeadfa11, NULL, NULL, NULL);
    assignListNode(newNode -> prevHead, 0xdeadfa11, NULL, NULL, NULL);
    return newNode;
}

// Создание графа
struct Graph* createGraph(int vertices, int edges) {
    struct Graph* graph = calloc(1, sizeof(struct Graph));
    graph -> size = vertices;
    graph -> numVertices = vertices;
    graph -> numEdges = edges;
    graph -> nodes = calloc(vertices, sizeof(struct node*));
    graph -> visited = calloc(vertices, sizeof(int)); //  ???????????
 
    int i;
    for (i = 0; i < vertices; i++) {
        graph -> nodes[i] = createNode(i);
        graph -> visited[i] = 0;
    }
    return graph;
}
 
// naming of vars!!!!!!!!!!!
void addEdge(struct Graph* graph, int src, int dest) {
    struct node * source = graph -> nodes[src];
    struct node * destination = graph -> nodes[dest];

    struct listNode * newNext = createListNode();
    struct listNode * newPrev = createListNode();
    //
    assignListNode(newNext, destination -> vertex, newPrev, source -> nextHead -> nextNode, source -> nextHead);
    if (source -> nextHead -> nextNode != NULL){
        (source -> nextHead -> nextNode) -> prevNode = newNext;
    }
    source -> nextHead -> nextNode = newNext;


    //
    assignListNode(newPrev, source -> vertex, newNext, destination -> prevHead -> nextNode, destination -> prevHead);
    if (destination -> prevHead -> nextNode != NULL){
        (destination -> prevHead -> nextNode) -> prevNode = newPrev;
    }
    destination -> prevHead -> nextNode = newPrev;
}
 
// Выводим граф
void printGraph(struct Graph* graph) {
    int v;
    printf("%d\n%d\n", graph -> numVertices, graph -> numEdges);
    for (v = 0; v < graph -> size; v++) {
        if (graph -> nodes[v] == NULL){
            continue;
        }
        struct listNode* temp = graph -> nodes[v] -> nextHead;
        while (temp -> nextNode != NULL) {
            printf("%d %d\n", (graph -> nodes[v] -> vertex) + 1, (temp -> nextNode -> vertex) + 1);
            temp = temp -> nextNode;
        }
    }
}

void deleteListNode(struct listNode * deleted){
    struct listNode * linked = deleted -> linked;

    if (linked -> prevNode != NULL){
        linked -> prevNode -> nextNode = linked -> nextNode;
    }
    if (linked -> nextNode != NULL){
        linked -> nextNode -> prevNode = linked -> prevNode;
    }
    free(linked);

    if (deleted -> prevNode != NULL){
        deleted -> prevNode -> nextNode = deleted -> nextNode;
    }
    if (deleted -> nextNode != NULL){
        deleted -> nextNode -> prevNode = deleted -> prevNode;
    }
    free(deleted);
}

//
void deleteNode(struct Graph * graph, int vertex){

    //delete previous 
    struct node *deleted = graph -> nodes[vertex];
    struct listNode * temp = deleted -> prevHead -> nextNode;
    while (temp != NULL){
        deleteListNode(temp);
        (graph -> numEdges) --;
        temp = deleted -> prevHead -> nextNode;
    }
    free(deleted -> prevHead);

    //delete next
    temp = deleted -> nextHead -> nextNode;
    while (temp != NULL){
        deleteListNode(temp);
        (graph -> numEdges) --;
        temp = deleted -> nextHead -> nextNode;      
    }
    free(deleted -> nextHead);

    //graph quest?
    graph -> nodes[vertex] = NULL;

    free(deleted);
}

void deleteGraph(struct Graph * graph){
    for (int i = 0; i < graph -> size; i ++){
        if (graph -> nodes[i] != NULL){
            deleteNode(graph, i);
        }
    }
    free(graph);
}

void DFS(struct Graph* graph, int vertex, int direction) {

    struct listNode* temp = NULL;
    if (direction){
        temp = graph -> nodes[vertex] -> nextHead;
    } else {
        temp = graph -> nodes[vertex] -> prevHead;
    }
    graph -> visited[vertex] = 1;
    graph -> nodes[vertex] -> status ++;

    while (temp -> nextNode != NULL){
        temp = temp -> nextNode;
        int connectedVertex = temp -> vertex;
        if (graph -> visited[connectedVertex] == 0){
            DFS(graph, connectedVertex, direction);
        }
    }
}

void clearVisitedList(struct Graph * graph) {
    for (int i = 0; i < graph -> size; i++){
        graph -> visited[i] = 0;
    }
}

void deleteDead(struct Graph * graph) {
    for (int i = 1; i < (graph -> size) - 1; i++){
        if (graph -> nodes[i] -> status < 2){
            deleteNode(graph, i);
            (graph -> numVertices) --;
        }
    }
}

struct Graph* scanGraph(){
    int N = 0, M = 0;
    int src, dest;
    scanf("%d %d", &N, &M);
    struct Graph * graph = createGraph(N, M);
    for (int i =0; i < M; i++){
        scanf("%d %d", &src, &dest);
        addEdge(graph, src - 1, dest - 1);
    }

    return graph;
}

int main() { 
    struct Graph* graph =  scanGraph();

    DFS (graph, 0, 1);
    clearVisitedList(graph);
    DFS (graph, (graph -> size) - 1, 0);
    deleteDead(graph);
 
    printGraph(graph);
    deleteGraph(graph);
    
    return 0;
}





