#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 20
#define NMAX MAX_VERTICES * (MAX_VERTICES - 1) / 2

typedef struct Node {
    int vertex;
    int weight;
    int prev;
    struct Node* next;
} Node;

typedef struct {
    Node* tab[NMAX];
    int first;
} prioQueue;

void exchange(Node** a, Node** b) {
    Node* temp = *a;
    *a = *b;
    *b = temp;
}

void upHeap(Node* tab[], int n) {
    while (n > 0) {
        int i = (n - 1) / 2;
        if (tab[n]->weight < tab[i]->weight) {
            exchange(&tab[n], &tab[i]);
            n = i;
        } else {
            break;
        }
    }
}

void insertPrioQueue(prioQueue* q, Node* el) {
    if (q->first < NMAX) {
        q->tab[q->first] = el;
        upHeap(q->tab, q->first++);
    }
}

void downHeap(Node* tab[], int f, int l) {
    int i = 2 * f + 1;
    while (i <= l) {
        if (i + 1 <= l && tab[i + 1]->weight < tab[i]->weight) {
            i++;
        }
        if (tab[i]->weight < tab[f]->weight) {
            exchange(&tab[i], &tab[f]);
            f = i;
            i = 2 * f + 1;
        } else {
            break;
        }
    }
}

void removeMinPrioQueue(prioQueue* q, Node** el) {
    if (q->first > 0) {
        *el = q->tab[0];
        q->tab[0] = q->tab[--q->first];
        downHeap(q->tab, 0, q->first - 1);
    }
}

typedef struct Graph {
    int numOfVertices;
    Node* adjLists[MAX_VERTICES];
} Graph;

Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numOfVertices = vertices;
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dst, int weight) {
    Node* newNode1 = (Node*)malloc(sizeof(Node));
    newNode1->vertex = dst;
    newNode1->weight = weight;
    newNode1->prev = src;
    newNode1->next = graph->adjLists[src];
    graph->adjLists[src] = newNode1;

    Node* newNode2 = (Node*)malloc(sizeof(Node));
    newNode2->vertex = src;
    newNode2->weight = weight;
    newNode2->prev = dst;
    newNode2->next = graph->adjLists[dst];
    graph->adjLists[dst] = newNode2;
}

void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->numOfVertices; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            Node* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }
    free(graph);
}

void mstPrint(Node* mstEdges[], int numOfVertices) {
    for (int i = 0; i < numOfVertices; i++) {
        if (mstEdges[i] != NULL) {
            printf("Edge: %d - %d (weight: %d)\n", mstEdges[i]->prev, i, mstEdges[i]->weight);
        }
    }
}

void Prim(Graph* graph, int startVertex) {
    prioQueue pq;
    pq.first = 0;

    bool* inMST = (bool*)calloc(graph->numOfVertices, sizeof(bool));
    Node** mstEdges = (Node**)calloc(graph->numOfVertices, sizeof(Node*));

    inMST[startVertex] = true;
    for (Node* temp = graph->adjLists[startVertex]; temp; temp = temp->next) {
        insertPrioQueue(&pq, temp);
    }

    for (int count = 0; count < graph->numOfVertices - 1; count++) {
        Node* minEdge;
        do {
            removeMinPrioQueue(&pq, &minEdge);
        } while (inMST[minEdge->vertex]);

        inMST[minEdge->vertex] = true;
        mstEdges[minEdge->vertex] = minEdge;

        for (Node* temp = graph->adjLists[minEdge->vertex]; temp; temp = temp->next) {
            if (!inMST[temp->vertex]) {
                insertPrioQueue(&pq, temp);
            }
        }
    }

    mstPrint(mstEdges, graph->numOfVertices);

    free(inMST);
    free(mstEdges);
}

int main() {
    Graph* graph = createGraph(6);
    addEdge(graph, 0, 5, 9);
    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 2, 10);
    addEdge(graph, 1, 2, 6);
    addEdge(graph, 1, 5, 5);
    addEdge(graph, 2, 5, 1);
    addEdge(graph, 2, 3, 7);
    addEdge(graph, 2, 4, 2);
    addEdge(graph, 3, 4, 11);
    addEdge(graph, 3, 5, 8);
    addEdge(graph, 4, 5, 3);

    Prim(graph, 0);
    freeGraph(graph);
    return 0;
}
