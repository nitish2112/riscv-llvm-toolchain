//------------------------------------------------------------------------
// vvadd_xloops
//------------------------------------------------------------------------

#include <stdio.h>

int main() {

  int A[100];
  int B[100]; 
  int C[100];
 
  for ( int i = 0; i < 100; i++ ) {
    A[i] = i+1;
    B[i] = i+1;
  }

  #pragma unroll//unordered_for
  for ( int i = 0; i < 100; i++ ) {
    C[i] = A[i] + B[i];
  }

  for ( int i = 0; i < 100; i++ ) {
    if ( C[i] != 2*(i+1) ) {
      printf("\n[FAILED]\n");
      return 0;
    }
  }
  
  printf("\n[PASSED]\n");
  return 0;

}
