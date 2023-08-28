// Основная идея: 
//  Граф проходится (DFS) в двух направлениях - от вершины 1 до N с пометкой в статусе вершины о посещении
//  Затем граф проходится в обратном направлении - от вершины N до 1 с пометкой в статусе вершины о посещении
//  Вершины, имеющие status < 2 - мертвые, т.к. посещались менее 2-х раз. Такие вершины удаляются


#include <stdio.h>
#include <stdlib.h>


//узел списка, необходимый для хранения взаимосвязей в графе. 
//  vertex - идентификатор вершины либо 0xdeadfa11 для головы списка
//  linked - указывает на элемент списка, соответствующий данному ребру графа, но с началом в другой вершине
//  nextNode - следующий элемент списка
//  prevNode - предыдущий элемент списка
struct listNode{
    int vertex;
    struct listNode * linked;
    struct listNode * nextNode;
    struct listNode * prevNode;
};

//вершина графа
//  vertex - идентификатор вершины
//  status - необходим для выполнения алгоритма, маркирует "живость" вершины
//  nextHead - указатель на голову списка соседних вершин, достижимых из данной
//  prevHead - указатель на голову списка соседних вершин, из которых достижима данная
struct node {
    int vertex;
    int status;
    struct listNode * nextHead;
    struct listNode * prevHead;
};

//граф
//  size - размер графа, общее число вершин
//  numVertices - число живых вершин
//  numEdges - число ребер графа
//  visited - массив посещенных вершин для реализации DFS
//  nodes - массив указателей на вершины графа
struct Graph {
    int size;
    int numVertices;
    int numEdges;
    int* visited;
    struct node** nodes;
};

//создает пустой узел списка
struct listNode* createListNode(){
    struct listNode * newNode = calloc(1, sizeof(struct listNode));
    newNode -> vertex = 0;    
    newNode -> linked = NULL;
    newNode -> nextNode = NULL;
    newNode -> prevNode = NULL;
    return newNode; 
}

//идентифицирует существующий узел списка переданными значениями 
void assignListNode(struct listNode * assigned, int vertex, struct listNode * linked, struct listNode * nextNode, struct listNode * prevNode){
    assigned -> vertex = vertex;
    assigned -> linked = linked;
    assigned -> nextNode = nextNode;
    assigned -> prevNode = prevNode;
}

//помещает в список элемент pushed после элемента prevInList
void pushInList(struct listNode* pushed, struct listNode * prevInList){
    pushed -> nextNode = prevInList -> nextNode;
    pushed -> prevNode = prevInList;
    if (prevInList -> nextNode != NULL){
        prevInList -> nextNode -> prevNode = pushed;
    }
    prevInList -> nextNode = pushed;
}

// Создает вершину графа с данным идентификатором вершины
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

//  Создает граф с заданным числом вершин
struct Graph* createGraph(int vertices, int edges) {
    struct Graph* graph = calloc(1, sizeof(struct Graph));
    graph -> size = vertices;
    graph -> numVertices = vertices;
    graph -> numEdges = edges;
    graph -> nodes = calloc(vertices, sizeof(struct node*));
    graph -> visited = calloc(vertices, sizeof(int));
 
    int i;
    for (i = 0; i < vertices; i++) {
        graph -> nodes[i] = createNode(i);
        graph -> visited[i] = 0;
    }
    return graph;
}
 
//  Добавляет новое ребро src -> dest в граф graph . Ребра в список исходящих вершин добавляются в порядке возрастания идентификатора ребра
void addEdge(struct Graph* graph, int src, int dest) {
    struct node * source = graph -> nodes[src];
    struct node * destination = graph -> nodes[dest];

    struct listNode * destNewListNode = createListNode();
    struct listNode * srcNewListNode = createListNode();
    
    //поиск позиции в списке, в которую следует поместить новый узел
    struct listNode* temp = source -> nextHead;
    while (temp -> nextNode != NULL && (temp -> vertex < dest || temp -> vertex == 0xdeadfa11)){
        temp = temp -> nextNode;
    }
    //добавление новой вершины (vertex == dest) в список вершин, достижимых из source
        if (temp -> vertex < dest || temp == source -> nextHead){
            assignListNode(destNewListNode, dest, srcNewListNode, NULL, NULL);
            pushInList(destNewListNode, temp);
        } else {
            assignListNode(destNewListNode, dest, srcNewListNode, NULL, NULL);
            pushInList(destNewListNode, temp -> prevNode);

        }

    //добавление новой вершины (vertex == src) в список вершин, из которых достижима destination
    assignListNode(srcNewListNode, src, destNewListNode, NULL, NULL);
    pushInList(srcNewListNode, destination -> prevHead);
}

//  Считывает граф в формате задачи
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

//  Выводит графа в формате, заданном в задаче
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
 

//  Удаляет узел списка (с удалением привязанного к нему элемента в другом списке
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

//  Удаляет вершину графа
void deleteNode(struct Graph * graph, int vertex){

    //delete list of previous
    struct node *deleted = graph -> nodes[vertex];
    struct listNode * temp = deleted -> prevHead -> nextNode;
    while (temp != NULL){
        deleteListNode(temp);
        (graph -> numEdges) -= 1;
        temp = deleted -> prevHead -> nextNode;
    }
    free(deleted -> prevHead);

    //delete list of next
    temp = deleted -> nextHead -> nextNode;
    while (temp != NULL){
        deleteListNode(temp);
        (graph -> numEdges) -= 1;
        temp = deleted -> nextHead -> nextNode;      
    }
    free(deleted -> nextHead);

    //delete pointer to node in graph
    graph -> nodes[vertex] = NULL;

    free(deleted);
}

//Удаляет мертвые вершины по принципу:
//      если статус меньше двух (вершина посещена менее двух раз), то вершина - мертвая
void deleteDead(struct Graph * graph) {
    for (int i = 1; i < (graph -> size) - 1; i++){
        if (graph -> nodes[i] -> status < 2){
            deleteNode(graph, i);
            (graph -> numVertices) -= 1;
        }
    }
}

//  Удаляет граф
void deleteGraph(struct Graph * graph){
    for (int i = 0; i < graph -> size; i ++){
        if (graph -> nodes[i] != NULL){
            deleteNode(graph, i);
        }
    }
    free(graph -> visited);
    free(graph -> nodes);
    free(graph);
}


//  Осуществляет поиск в глубину в заданном направлении.
//      direction == 1 - по направлению ребер графа
//      direction == 0 - против направления ребер графа
//      при обходе делается пометка о статусе "посещенности" в вершине. 
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

// Очищает список посещенных вершин в графе, заполнямый по ходу DFS
void clearVisitedList(struct Graph * graph) {
    for (int i = 0; i < graph -> size; i++){
        graph -> visited[i] = 0;
    }
}

int main() { 
    struct Graph* graph =  scanGraph();

    //  Граф проходится в двух направлениях - от вершины 1 до N с пометкой в статусе вершины о посещении
    //  Затем граф проходится в обратном направлении - от вершины N до 1 с пометкой в статусе вершины о посещении
    //  Вершины, имеющие status < 2 - мертвые, т.к. посещались менее 2-х раз. Такие вершины удаляются
    DFS (graph, 0, 1);
    clearVisitedList(graph);
    DFS (graph, (graph -> size) - 1, 0);
    deleteDead(graph);
 
    printGraph(graph);
    deleteGraph(graph);
    
    return 0;
}





