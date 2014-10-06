#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct ListNode {
  float coef;
  int expo;
  struct ListNode* link;
} ListNode;

typedef struct ListHead {
  ListNode* head;
} ListHead;

ListHead* createLinkedList(void)
{
  ListHead* L;
  L = (ListHead *)malloc(sizeof(ListHead));
  L->head = NULL;
  return L;
}

void addNode(ListHead* L, ListNode* newNode) {
  ListNode* p;

  p = (ListNode *)malloc(sizeof(ListNode));
  if(L->head == NULL) {
    L->head = newNode;
    return;
  } else {
    p = L->head;
    while(p->link != NULL) {
      p = p->link;
    }
  }

  p->link = newNode;
}

void addLastNode(ListHead* L, float coef, int expo)
{
  ListNode* newNode;
  ListNode* p;
  newNode = (ListNode *)malloc(sizeof(ListNode));
  newNode->coef = coef;

  newNode->expo = expo;
  newNode->link = NULL;

  if(L->head == NULL) {
    L->head = newNode;
    return;
  } else {
    p = L->head;
    while(p->link != NULL) {
      p = p->link;
    }
  }
  p->link = newNode;
}

void addPoly(ListHead* A, ListHead* B, ListHead* C)
{
  ListNode* pA = A->head;
  ListNode* pB = B->head;
  float sum;

  while(pA && pB){
    if(pA->expo == pB->expo){
      sum = pA->coef + pB->coef;
      addLastNode(C, sum, pA->expo);
      pA=pA->link; pB=pB->link;
    }
    else if(pA->expo > pB->expo){
      addLastNode(C, pA->coef, pA->expo);
      pA=pA->link;
    }
    else {
      addLastNode(C, pB->coef, pB->expo);
      pB=pB->link;
    }
  }

  for( ; pA!=NULL; pA=pA->link)
    addLastNode(C, pA->coef, pA->expo);

  for( ; pB!=NULL; pB=pB->link)
    addLastNode(C, pB->coef, pB->expo);
}

void printPoly(ListHead* L)
{
  ListNode* p = L->head;
  for(;p;p=p->link) {
    printf("%3.0fx^%d", p->coef, p->expo);
  }
}

ListHead* multPoly(ListHead* l1, ListHead* l2)
{
  ListHead *A;
  ListNode* x = l1->head;

  A = createLinkedList();
  for(; x; x=x->link) {
    ListNode* y = l2->head;
    for(; y; y=y->link) {
      addLastNode(A, x->coef * y->coef, x->expo + y->expo);
    }
  }

  return A;
}

ListHead* tokenize(char* sources) {
  ListHead *A;
  char buf[1024];
  float coef;
  int expo;
  char *temp;
  int i = 0;
  int is_coef = 1;

  A = createLinkedList();

  while(i < strlen(sources)) {
    char token = sources[i];
    i += 1;

    if(token == ' ' || token == '^') {
      continue;
    } else if(token == 'x') {
      if(strcmp(buf, "") == 0) {
        coef = 1.0;
      } else {
        coef = strtof(buf, &temp);
      }

      memset(buf, 0, sizeof(buf));
      is_coef = !is_coef;
    } else if(token == '+' || token == '-') {
      is_coef = !is_coef;
      if(strcmp(buf, "") == 0) {
        expo = 1;
      } else {
        expo = strtod(buf, &temp);
      }

      addLastNode(A, coef, expo);
      memset(buf, 0, sizeof(buf));
      strncat(buf, &token, 1);
    } else {
      strncat(buf, &token, 1);
    }
  }

  if(strlen(sources) == i && !is_coef) {
    addLastNode(A, coef, 1);
  }

  if(strcmp(buf, "") != 0) {
    addLastNode(A,  strtof(buf, &temp), 0);
    memset(buf, 0, sizeof(buf));
  }

  return A;
}


int main() {
  char s[256];
  strcpy(s, "x + 1");
  ListHead* l1 = tokenize(s);
  ListHead* l2 = tokenize(s);

  ListHead* l3 = multPoly(l1, l2);

  printPoly(l3);
  return 0;
}
