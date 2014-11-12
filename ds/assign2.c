#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EDGE_SIZE 10
#define MAX_STACK_SIZE 100

typedef struct Queue {
  int rear;
  int front;
  void* items[MAX_STACK_SIZE];
} Queue;

typedef struct Edge {
  struct Vertice* start;
  struct Vertice* end;
} Edge;


typedef struct Vertice {
  char name;
  Edge* edges[MAX_EDGE_SIZE];
  int edge_size;
} Vertice;

Queue* init_queue() {
  Queue* queue;
  queue = (Queue *) malloc(sizeof(Queue));

  queue->front = 0;
  queue->rear = 0;
  return queue;
}

int queue_empty(Queue* q) {
  return q->front == q->rear;
}

int queue_full(Queue* q) {
  return q->rear == MAX_STACK_SIZE;
}

void enqueue(Queue* q, void* e) {
  if(!queue_full(q)) {
    q->items[q->rear++] = e;
  }
}

void* dequeue(Queue* q) {
  if(!queue_empty(q)) {
    return q->items[q->front++];
  }

  return NULL;
}

Vertice* init_vertice(char name) {
  Vertice* vertice;
  vertice = (Vertice *) malloc(sizeof(Vertice));

  vertice->name = name;
  vertice->edge_size = 0;

  return vertice;
}

void add_edge(Vertice* v1, Vertice* v2) {
  Edge* edge;

  edge = (Edge *) malloc(sizeof(Edge));
  edge->start = v1;
  edge->end = v2;

  if(v1->edge_size > MAX_EDGE_SIZE || v2->edge_size > MAX_EDGE_SIZE) {
    printf("MAX_EDGE_SIZE exceeded");
    exit(1);
  }
  v1->edges[v1->edge_size++] = edge;
  v2->edges[v2->edge_size++] = edge;
}

Queue* dfs(Vertice* start_v) {
  Vertice* v;
  Queue* queue = init_queue();
  Queue* result = init_queue();
  Edge* e;
  int i = 0;

  v = start_v;

  while(v->edge_size > i || !queue_empty(queue)) {
    if(v->edge_size == i && !queue_empty(queue)) {
      e = (Edge *) dequeue(queue);
      v = e->end;
    } else {
      e = v->edges[i];
    }

    if(e->start->name == v->name) {
      if(v->edge_size > i + 1) {
        for(int j = 0; j < v->edge_size; j++){
          if(j != i && v->edges[j]->start->name == v->name) {
            enqueue(queue, v->edges[j]);
          }
        }
      }
      enqueue(result, v);
      v = e->end;
      i = 0;
    } else if(v->edge_size == 1 && e->end->name == v->name) {
      enqueue(result, v);
      i += 1;
    } else {
      i += 1;
    }
  }
  return result;
}

Queue* bfs(Vertice* start_v) {
  Vertice* v;
  Edge* e;

  Queue* queue = init_queue();
  Queue* result = init_queue();

  enqueue(queue, start_v);
  enqueue(result, start_v);
  do {
    v = dequeue(queue);

    for(int j = 0; j < v->edge_size; j++) {
      Edge* cur_e = v->edges[j];
      Vertice* next_v = cur_e->end;
      if(cur_e->start->name == v->name) {
        enqueue(result, next_v);
        enqueue(queue, next_v);
      }
    }
  } while(!queue_empty(queue));

  return result;
}


int main() {
  /*

  Vertice* va;
  Vertice* vb;
  Vertice* vc;
  Vertice* vd;
  Vertice* ve;
  Vertice* vf;
  Vertice* vg;

  va = init_vertice('A');
  vb = init_vertice('B');
  vc = init_vertice('C');
  vd = init_vertice('D');
  ve = init_vertice('E');
  vf = init_vertice('F');
  vg = init_vertice('G');

  add_edge(va, vb);
  add_edge(vb, vd);
  add_edge(vd, vg);
  add_edge(vg, ve);
  add_edge(vg, vf);
  add_edge(ve, vc);

  Queue* dfs_result;
  Queue* bfs_result;
  Vertice* v;

  dfs_result = dfs(va);

  printf("DFS: ");
  while((v = (Vertice *)dequeue(dfs_result)) != NULL) {
    printf("%c ", v->name);
  }
  printf("\n");

  bfs_result = bfs(va);

  printf("BFS: ");
  while((v = (Vertice *)dequeue(bfs_result)) != NULL) {
    printf("%c ", v->name);
  }
  printf("\n");
  */
  FILE* pFile;
  char vertices[100];
  int start[100];
  int end[100];
  char e[10];
  int i = 0;
  int j = 0;


  Vertice* vs[100];

  pFile = fopen ("input.txt" , "r");
  if(pFile == NULL) {
    perror("Error opening file");
  } else {
    while(fgets(e , 10, pFile) != NULL) {
      if(i == 0) {
        vertices[j++] = e[0];
      } else if(i % 2 == 1) {
        int f1 = 0;
        int f2 = 0;
        for(int k = 0; k < j; k++) {
          f1 = f1 || (e[0] == vertices[k]);
          f2 = f2 || (e[2] == vertices[k]);
        }

        if(!f1) {
          vertices[j++] = e[0];
        }

        if(!f2) {
          vertices[j++] = e[2];
        }

        for(int z = 0; z < j; z ++) {
          if(e[0] == vertices[z]) {
            start[i] = z;
          }
        }
        for(int z = 0; z < j; z ++) {
          if(e[2] == vertices[z]) {
            end[i] = z;
          }
        }

      }
      i++;
    }
    fclose(pFile);
  }

  Vertice* start_v = init_vertice(vertices[0]);

  for(int z = 0; z < j; z++) {
    vs[z] = init_vertice(vertices[z]);
  }


  for(int z = 0; z < i; z++) {
    if(z % 2 != 0) {
      add_edge(vs[start[z]], vs[end[z]]);
    }
  }


  Queue* dfs_result;
  Queue* bfs_result;
  Vertice* v;

  dfs_result = dfs(vs[0]);
  printf("DFS: ");
  while((v = (Vertice *)dequeue(dfs_result)) != NULL) {
    printf("%c ", v->name);
  }
  printf("\n");

  bfs_result = bfs(vs[0]);

  printf("BFS: ");
  while((v = (Vertice *)dequeue(bfs_result)) != NULL) {
    printf("%c ", v->name);
  }
  printf("\n");

  return 0;
}
