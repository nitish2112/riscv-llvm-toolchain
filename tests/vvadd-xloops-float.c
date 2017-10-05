//------------------------------------------------------------------------
// vvadd_xloops
//------------------------------------------------------------------------
#include <stdio.h>

void vvadd_xloops( float C[], float A[], float B[], float size );

void  __attribute__ ((noinline)) vvadd_xloops( float C[], float A[], float B[], float size )
{
  #pragma unordered_for
  for ( int i = 0; i < size; i++ ) {
    C[i] = A[i] + B[i];
  }
}

int main() {

  float A[10] = { 1,2,3,4,5,6,7,8,9,10 };
  float B[10] = { 1,2,3,4,5,6,7,8,9,10 };
  float C[10];

  vvadd_xloops(C, A, B, 10);

  for ( int i = 0; i < 10; i++ ) {
    if ( C[i] != 2*(i+1) ) {
      printf("\n[FAILED]\n");
      return 0;
    }
  }
  
  printf("\n[PASSED]\n");
  return 0;

}

