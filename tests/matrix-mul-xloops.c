//------------------------------------------------------------------------
// matrix_mul_xloops
//------------------------------------------------------------------------

#define N 20

#include <stdio.h>

int A[N][N];
int B[N][N];
int C[N][N];

void  __attribute__ ((noinline))  matrix_mult_xloops( int* A, int* B, int* C, int N1)
{
  #pragma unordered_for
  for ( int i = 0; i < N1; i++ ) {
//    #pragma unordered_for
    for ( int j = 0; j < N1; j++ ) {
      int c = 0;
      for ( int k = 0; k < N1; k++ ) {
        c += A[i*N1+k]*B[k*N1+j];
      }
      C[i*N1+j] = c;
    }
  }
}

int main() {

  for ( int i = 0; i < N; i++ ) {
    for ( int j = 0; j < N; j++ ) {
      A[i][j] = 1;
      B[i][j] = 1;
      C[i][j] = 0;
    }
  }

  matrix_mult_xloops(&A[0][0], &B[0][0], &C[0][0], N);

  for ( int i = 0; i < N; i++ ) {                                           
    for ( int j = 0; j < N; j++ ) {                                         
      if ( C[i][j] != N ) {
        printf("\n[FAILED] at i: %d j: %d\n",i,j);                                                               
        return -1;
      }
    }                                                                         
  }  

  printf("\n[PASSED]\n");
  return 0;
}
