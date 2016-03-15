#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


static void vectorGrow(vector *v)
{
  //expand to double size
  v->allocLength *=2;
  v->elems = realloc(v->elems, v->allocLength * v->elemSize);
}

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
  assert(elemSize > 0);
  v->elemSize = elemSize;
  v->logLength = 0;
  v->allocLength = initialAllocation;
  v->elems = malloc(initialAllocation * elemSize);
  v->freeFn = freeFn;
  assert(v->elems != NULL);
}

void VectorDispose(vector *v)
{
  assert(v->elems != NULL);
  free(v->elems);
}

int VectorLength(const vector *v)
{
  return v->logLength;
}

void *VectorNth(const vector *v, int position)
{
  return (char*)v->elems + (v->elemSize * position);
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
  assert(elemAddr != NULL);
  assert(position < v->allocLength);
  memcpy((char*)v->elems + (v->elemSize * position), elemAddr, v->elemSize);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
  assert(elemAddr != NULL);
  assert((position >= 0) && (position <= v->logLength));
  if(v->logLength == v->allocLength)
    vectorGrow(v);
  memcpy((char*)v->elems + ((position+1) * v->elemSize), 
    (char*)v->elems + (position * v->elemSize), 
	(v->logLength - position) * v->elemSize);
  memcpy((char*)v->elems + (position * v->elemSize), elemAddr, v->elemSize);
  v->logLength++;
}

void VectorAppend(vector *v, const void *elemAddr)
{
  assert(elemAddr != NULL);
  if(v->logLength == v->allocLength)
    vectorGrow(v);
  memcpy((char*)v->elems + (v->logLength * v->elemSize), elemAddr, v->elemSize);
}

void VectorDelete(vector *v, int position)
{
  assert((position >= 0) && (position < v->logLength));
  if(v->freeFn != NULL) 
	v->freeFn((char*)v->elems + (position * v->elemSize));
  memmove((char*)v->elems + (position * v->elemSize), (char*)v->elems + ((position+1) * v->elemSize), 
    (v->logLength - position) * v->elemSize);
  v->logLength--;
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
  qsort(v->elems, v->logLength, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{ return -1; }
