//------------------------------------------------------------------------
// convolution_xloops
//------------------------------------------------------------------------

#define N 20
#define K 3

#include <stdio.h>
//#include <stdlib.h>
//extern int N;
//extern int K;

void  __attribute__ ((noinline)) convolution_xloops( int* I, int* O, int* W, int N1, int K1)
{
  for ( int y = 0; y < N1-K1+1; y++ ) {
    for ( int x = 0; x < N1-K1+1; x++ ) {
      O[y*(N1-K1+1)+x] = 0;
      #pragma unordered_for
      for ( int ky = 0; ky < K1; ky++ ) {
        for ( int kx = 0; kx < K1; kx++ ) {
          O[y*(N1-K1+1)+x] += I[(y+ky)*N1+x+kx] * W[ky*K1+kx];
        }
      }
    }
  }
}

int main() {

 // srand(45);

  int I[N][N];
  int W[3][3] = { {1,2,3}, {4,5,6}, {7,8,9} };
  int O[N-K+1][N-K+1];
  int ref;
  
  for ( int i = 0; i < N; i++ ) {
    for ( int j = 0; j < N; j++ ) {
      I[i][j] = 1; //rand();
    }
  }

  convolution_xloops(&I[0][0], &O[0][0], &W[0][0], N, K);

  for ( int y = 0; y < N-K+1; y++ ) {                                           
    for ( int x = 0; x < N-K+1; x++ ) {                                         
      ref = 0;
      for ( int ky = 0; ky < K; ky++ ) {                                      
        for ( int kx = 0; kx < K; kx++ ) {                                    
          ref += I[y+ky][x+kx]*W[ky][kx];                                  
        }                                                                     
      }       
      if ( ref!= O[y][x] ) {
        printf("\n[FAILED] at y: %d x: %d\n",y,x);                                                               
        return 0;
      }
    }                                                                         
  }  

  printf("\n[PASSED]\n");
  return 0;
}
