#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "cuda.h"
#include "functions.c"


int main (int argc, char **argv) 
{

  /* Part 2. Start this program by first copying the contents of the main function from 
     your completed decrypt.c main function. */

  /* Q4 Make the search for the secret key parallel on the GPU using CUDA. */



  //declare storage for an ElGamal cryptosystem
  unsigned int n, p, g, h, x;
  unsigned int Nints;

  //get the secret key from the user
  printf("Enter the secret key (0 if unknown): "); fflush(stdout);
  char stat = scanf("%u",&x);

  printf("Reading file.\n");

  /* Q3 Complete this function. Read in the public key data from public_key.txt
    and the cyphertexts from messages.txt. */



  FILE *readPublic = fopen("public_key.txt", "r");

  fscanf(readPublic, "%u", &n);
  fscanf(readPublic, "%u", &p);
  fscanf(readPublic, "%u", &g);
  fscanf(readPublic, "%u", &h);

  fclose(readPublic);


  
  FILE *readMessage = fopen("message.txt", "r");

  fscanf(readMessage, "%u\n", &Nints);

  unsigned int *Zmessage = (unsigned int *) malloc(Nints*sizeof(unsigned int));
  unsigned int *a = (unsigned int *) malloc(Nints*sizeof(unsigned int));

  for (int i=0; i < Nints; i++)
  {
    fscanf(readMessage, "%u %u\n", &Zmessage[i], &a[i]);
  } 

  fclose(readMessage);

  // find the secret key
  if (x==0 || modExp(g,x,p)!=h) {
    printf("Finding the secret key...\n");
    double startTime = clock();
    for (unsigned int i=0;i<p-1;i++) {
      if (modExp(g,i+1,p)==h) {
        printf("Secret key found! x = %u \n", i+1);
        x=i+1;
      } 
    }


    double endTime = clock();

    double totalTime = (endTime-startTime)/CLOCKS_PER_SEC;
    double work = (double) p;
    double throughput = work/totalTime;

    printf("Searching all keys took %g seconds, throughput was %g values tested per second.\n", totalTime, throughput);
  }

  /* Q3 After finding the secret key, decrypt the message */

  ElGamalDecrypt(Zmessage, a, Nints, p, x);
  unsigned char *newArray = (unsigned char *) malloc(1024*sizeof(unsigned char));
  unsigned int Nchars = ((n-1)/8) * Nints;

  convertZToString(Zmessage, Nints, newArray, Nchars);
 


//4.1: search for parallel key

  int Nthreads;
  dim3 Bl(32,1,1);
  dim3 Gr((p+32-1)/32,1,1);
  unsigned int *device_a;
  unsigned int host_a;
  cudaMalloc(&device_a, Nthreads*sizeof(unsigned int));
  host_a = (unsigned int *) malloc(sizeof(unsigned int));

  double start = clock();
  cudaDecrypt <<<Gr,Bl>>>(g,p,h,device_a);
  cudaDeviceSynchronize();

  cudaMemcpy(host_A,device_a,sizeof(unsigned int) , cudaMemcpyDeviceToHost);



  return 0;


}










__device__ unsigned int cudaModExp(unsigned int a, unsigned int b, unsigned p)
{
   unsigned int z = a;
   unsigned int aExpb = 1;

   while (b > 0) {
     if (b%2 == 1) aExpb = cudaModprod(aExpb, z, p);
     z = cudaModprod(z, z, p);
     b /= 2;
   }
   return aExpb;
}




__device__ unsigned int cudaModProd(unsigned int a, unsigned int b, unsigned p)
{
    unsigned int z_a = a;
    unsigned int a_b = 0;

    while (b>0)
    {
        if (b%2 == ) a_b = (a_b + z_a)%p;
        z_a = (2*z_a) % p;
        b/= 2;
    }
    return a_b;
}







// find the key
__global__ void cudaKeyDecrypt(unsigned int g, unsigned int p, unsigned int h, unsigned int *d)
{
    int thread = threadIdx.x;
    int block = blockIdx.x;
    int blockSize = blockDim.x;
    int gid = thread + block*blockSize;

    if (gid < (p-1))
    {
        if (cudaModExp(g,gid,p) == h)
        {
            d = gid-1;
        }
    }
}




