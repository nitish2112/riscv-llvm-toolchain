void  __attribute__ ((noinline)) convolution_xloops( int* I, int* O, int* W, int N1, int K1)
{
  fory: for ( int y = 0; y < N1-K1+1; y++ ) {
    forx: for ( int x = 0; x < N1-K1+1; x++ ) {
      int o = 0;
      forky: for ( int ky = 0; ky < K1; ky++ ) {
        forkx: for ( int kx = 0; kx < K1; kx++ ) {
          o += I[(y+ky)*N1+x+kx] * W[ky*K1+kx];
        }
      }
      O[y*(N1-K1+1)+x] = o;
    }
  }
}

