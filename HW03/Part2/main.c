#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "mpi.h"
#include "functions.h"

int main (int argc, char **argv) {

  MPI_Init(&argc,&argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  //seed value for the randomizer 
  double seed = clock()+rank; //this will make your program run differently everytime
  //double seed = rank; //uncomment this and your program will behave the same everytime it's run

  srand(seed);

  //begin with rank 0 getting user's input
  unsigned int n;

  





/* Q3.1 Make rank 0 setup the ELGamal system and
    broadcast the public key information */

  if (rank == 0)
  {
    printf("Enter a number of bits: "); fflush(stdout);
    char status = scanf("%u",&n);

    //make sure the input makes sense
    if ((n<3)||(n>31)) {//Updated bounds. 2 is no good, 31 is actually ok
      printf("Unsupported bit size.\n");
      return 0;   
    }
    printf("\n");
  }

  //declare storage for an ElGamal cryptosytem
  unsigned int p, g, h, x;

  //setup an ElGamal cryptosystem
  if (rank == 0)
  {
    setupElGamal(n,&p,&g,&h,&x);
  }

  /* Q1.3 Share the public key information */
  
  // here, I broadcast three of the variables given to us

  MPI_Bcast(&p,1,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Bcast(&g,1,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Bcast(&h,1,MPI_INT,0,MPI_COMM_WORLD);



  



//Suppose we don't know the secret key. Use all the ranks to try and find it in parallel
   
  if (rank == 0)
    printf("Using %d processes to find the secret key...\n", size);

  
/*Q3.2 We want to loop through values i=0 .. p-2
     determine start and end values so this loop is 
     distributed amounst the MPI ranks  */

    // here, I create three separate variables: one stores the total size of the loop,    // one keeps track of the small portion added, and the last variable keeps track
    // of the remainder

    unsigned int totalLoopSize = p-1; //total loop size
    unsigned int chunk = totalLoopSize / size;
    unsigned int leftOvers = totalLoopSize % size;


    unsigned int start, end, thread;
  
    start = 0; 
    end = start + chunk;
    thread = 0;

    // this checks that while we are not at the end, the end variable will continue to
    // increment, as long as the thread does not surpass the remainder
    while (end != totalLoopSize)
    {
      if (thread<leftOvers)
      {
        end = end + 1;
      }
    }

    // if the thread is equal to the rank, we will go through the entire loop and
    // print the secret keys, then the variables will be updated/incremented
    if (thread == rank)
    {

    //loop through the values from 'start' to 'end'
      for (unsigned int i=start;i<end;i++) 
      {
        if (modExp(g,i+1,p)==h)
          printf("Secret key found! x = %u \n", i+1);
      }

    }
    
    start = end + 1;         
    end = start + chunk;
    thread = thread + 1;

 

  MPI_Finalize();

  return 0;

}
