///////////////////////////////////////////////////////////////
//
//  Array List (for Pointers)
//
//  01.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////////// 

#include "arraylist.h"

ArrayList* AList_Create() {
  ArrayList* list = (ArrayList*)malloc(sizeof(ArrayList));
  list->len = 0;
  list->size = 8;
  list->items = malloc(sizeof(void**) * list->size);
}

void AList_Destroy(ArrayList* list) {
  list->len = 0;
  free(list->items);
  free(list);
}

void AList_Add(ArrayList* list, void* item) {
  list->len++;
  if(list->len > list->size) {
    list->size *= 2;
    list->items = realloc(list->items, list->size * sizeof(void**));
  }
  list->items[list->len - 1] = item;
}

void* AList_Get(ArrayList* list, int index) {
  if(index >= list->len || index < 0) return NULL;
  return list->items[index];
}

void AList_Set(ArrayList* list, int index, void* item) {
  if(index >= list->len || index < 0) return;
  list->items[index] = item;
}

int AList_Length(ArrayList* list) {
  return list->len; 
}

void AList_Delete(ArrayList* list, int index) {
  int i;
  for(i = index; i < list->len - 1; i++) {
   list->items[index] = list->items[index + 1]; 
  }
  list->len--;
}

void AList_Sort(ArrayList* list, AList_CmpFnc compare) {
  // TODO: better sort algorithm than bubble sort (better than O(n^2)
  int j, k;
  for(j = 0; j < list->len; j++) {
   for(k = 0; k < list->len - 1; k++) {
     if(compare(list->items[k], list->items[k + 1]) >= 1) {
       // first value is bigger -> swap
       void* tmp = list->items[k];
       list->items[k] = list->items[k + 1];
       list->items[k + 1] = tmp;
     }
   }
  }
}

ArrayList* AList_Split(char* str, char* delimiters) {
  ArrayList* list = AList_Create();
  char* tok = strtok(str, delimiters);
  while(tok != NULL) {
   AList_Add(list, tok);
   tok = strtok(NULL, delimiters);
  }
  return list;
}



/*
// testing
int cmp(void* a, void* b) {
 return strcmp(a, b); 
}

int main() {
  ArrayList* list = AList_Create();
  AList_Add(list, "Hallo");
  AList_Add(list, "schöne");
  AList_Add(list, "Welt!");
  
  int i;
  for(i = 0; i < AList_Length(list); i++) {
    printf("%s\r\n", AList_Get(list, i)); 
  }
  printf("------\r\n");

  AList_Set(list, 1, "du");
  for(i = 0; i < AList_Length(list); i++) {
    printf("%s\r\n", AList_Get(list, i)); 
  }
  printf("------\r\n");
  
  AList_Delete(list, 1);
  for(i = 0; i < AList_Length(list); i++) {
    printf("%s\r\n", AList_Get(list, i)); 
  }
  printf("------\r\n");

  
  AList_Add(list, "7");
  AList_Add(list, "1");
  AList_Add(list, "8");
  AList_Add(list, "3");
  AList_Add(list, "5");
  AList_Add(list, "4");
  AList_Add(list, "2");
  AList_Add(list, "6");
  AList_Add(list, "9");
    
  for(i = 0; i < AList_Length(list); i++) {
    printf("%s\r\n", AList_Get(list, i)); 
  }
  printf("------\r\n");


  AList_Sort(list, cmp);
  for(i = 0; i < AList_Length(list); i++) {
    printf("%s\r\n", AList_Get(list, i)); 
  }
  printf("------\r\n");

  
  AList_Destroy(list);
  
}
*/
