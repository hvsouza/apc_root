void denoise()
{
  FILE     *fpv_out, *fpy_out,
  *fpu_out, *fps_out, *fpx_out;               		
  char     vname[] 	= "v.txt";  
  char     uname[] 	= "u.txt";
  char     sname[] 	= "s.txt";  
  char     xname[] 	= "x.txt";  
  char     yname[] 	= "y.txt";  
  fpv_out 			= fopen(vname, "w");
  fpu_out 			= fopen(uname, "w");
  fps_out 			= fopen(sname, "w");
  fpx_out 			= fopen(xname, "w");
  fpy_out 			= fopen(yname, "w");

  // Set random seed
  // rand_val(1);

  // Set parameters for simulation  
  int      N 		= 2500;         
  double   stdv 	= 10;     
  double   stdu 	= 1;
  double   alpha 	= 0.99;
  int      M 		= 50;
  double   w[M];
  // Allocate array for correlation vector
  double   rss[M];           
  double   rvv[M];
  double   rxx[M];
  double   rxs[M];
  double   Rxx[M][M];

  // Allocate array for signal
  double   v[N];           
  double   u[N];
  double   s[N];
  double   x[N];
  double   y[N];

  // Generate random numbers
  for (int i=0; i<N; i++)
  {
    v[i] = gRandom->Gaus(0, stdv);
    u[i] = gRandom->Gaus(0, stdu);
    fprintf(fpv_out, "%d %f \n",i, v[i]);
    fprintf(fpu_out, "%d %f \n",i, u[i]);
  }


  // Generate the desired signal s(n) to be an AR(1) process
  s[0] = 0;
  for (int n=1; n<N; n++)
  {
  	y[n] = 0;
  	s[n] = alpha*s[n-1] + u[n];
  	x[n] = s[n] + v[n];
        fprintf(fps_out, "%d %f \n", n,s[n]);
        fprintf(fpx_out, "%d %f \n", n,x[n]);
  }

  // Compute autocorrelation of v(n)
  for (int k=0; k<M; k++)
  {
  	w[k] 	= 0;
  	rvv[k] 	= (k == 0) ? pow(stdv,2) : 0;
  }


  for (int k=0; k<M; k++)
  {
  	// Compute autocorrelation of s(n)
  	// rss[k] = pow(alpha,k)/(1-pow(alpha,2))*pow(stdu,2);
    rss[k]=0;
    for(Int_t j = 0; j<N-k; j++){
          rss[k] += s[j]*s[j+k]/(N-k);
        }
        // cout << pow(alpha,k)/(1-pow(alpha,2))*pow(stdu,2) << " " << rss[k] << endl;

  	// Cross-correlation of x(n) and s(n)
  	rxs[k] = rss[k];

  	// Compute autocorrelation of x(n)
  	rxx[k] = rss[k] + rvv[k];
  }

  // Autocorrelation matrix of x(n)
  for (int i=0; i<M; i++)
  {
  	for (int j=0; j<M; j++)
  	{
  		Rxx[i][j] = rxx[abs(i-j)];
  		// printf("%f\t",Rxx[i][j] );
  	}
  	// printf("\n");
  }


  int N_iter 	= 20;
  double beta 	= 0.001;
  double temp 	= 0;
  for (int k=0; k<N_iter; k++)
  {
  	for (int i=0; i<M; i++)
	{
	  // Compute the Rxx*w
	  for (int j=0; j<M; j++)
	  {
	  	temp = temp + Rxx[i][j]*w[j];
	  }
	  w[i] = w[i] - beta*(-rxs[i] + temp);
          // or this:
          // jaboc iterative method
          // w[i] = w[i] - (-rxs[i] + temp)/Rxx[i][i];
	  temp = 0;
	  // printf("%0.4f\t",w[i]);
	}
	// printf("\n");
  }


  for (int n=M-1; n<N; n++)
  {
  	y[n] = 0;
  	for (int i=0; i<M; i++)
	{
		y[n] = y[n] + w[i]*x[n-i];
	}
	fprintf(fpy_out, "%d %f \n", n, y[n]);
  }

  fclose(fpv_out);
  fclose(fpu_out);
  fclose(fps_out);
  fclose(fpx_out);
  fclose(fpy_out);
}

