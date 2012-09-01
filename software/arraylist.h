///////////////////////////////////////////////////////////////
//
//  Array List (for Pointers)
//
//  01.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////////// 

#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*AList_CmpFnc)(void*, void*);


typedef struct {
 int size;
 int len;
 void** items;
} ArrayList;

ArrayList* AList_Create();
void AList_Destroy(ArrayList* list);
void AList_Add(ArrayList* list, void* item);
void* AList_Get(ArrayList* list, int index);
void AList_Set(ArrayList* list, int index, void* item);
int AList_Length(ArrayList* list);
void AList_Delete(ArrayList* list, int index);
void AList_Sort(ArrayList* list, AList_CmpFnc compare);
ArrayList* AList_Split(char* str, char* delimiters);

#endif

