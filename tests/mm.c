void matrix_mult_xloops( int* A, int* B, int* C, int N1)
{
  for ( int i = 0; i < N1; i++ ) {
    #pragma unordered_for
    for ( int j = 0; j < N1; j++ ) {
      for ( int k = 0; k < N1; k++ ) {
        C[i*N1+j] += A[i*N1+k]*B[k*N1+j];
      }
    }
  }
}
