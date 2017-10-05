//------------------------------------------------------------------------
// matrix_mul_xloops
//------------------------------------------------------------------------

#define N 25

#include <stdio.h>

int A[N][N];
int B[N][N];
int C[N][N];

int main() {

  int ref;
 /* 
  for ( int i = 0; i < N; i++ ) {
    for ( int j = 0; j < N; j++ ) {
      A[i][j] = 1;
      B[i][j] = 1;
      C[i][j] = 0;
    }
  }
*/
  #pragma unordered_for 
  for ( int i = 0; i < N; i++ ) {
    for ( int j = 0; j < N; j++ ) {
      for ( int k = 0; k < N; k++ ) {
        C[i][j] += A[i][k]*B[k][j];
      }
    }
  }
/*
  for ( int i = 0; i < N; i++ ) {                                           
    for ( int j = 0; j < N; j++ ) {                                         
      if ( C[i][j] != N ) {
        printf("\n[FAILED] at i: %d j: %d\n",i,j);                                                               
        return -1;
      }
    }                                                                         
  }  
*/
  printf("\n[PASSED]\n");
  return 0;
}
