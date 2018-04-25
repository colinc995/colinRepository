#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "functions.h"

int main (int argc, char **argv) {

  //seed value for the randomizer 
  double seed = clock(); //this will make your program run differently everytime
  //double seed = 0; //uncomment this and your program will behave the same everytime it's run

  srand(seed);

  //declare storage for an ElGamal cryptosytem
  unsigned int n,p, g, h, x;

  //begin with getting user's input

  printf("Enter a number of bits: "); fflush(stdout);
  char status = scanf("%u",&n);

  //make sure the input makes sense
  if ((n<9)||(n>31)) {//Updated bounds. 2 is no good, 31 is actually ok
    printf("Unsupported bit size.\n");
    return 0;   
  }
  printf("\n");

  //setup an ElGamal cryptosystem
  setupElGamal(n,&p,&g,&h,&x);


  printf("Writing to file.\n");

  /* Q1 Complete this function and write the puclib key data to the file public_key.txt */

   
   //tells c to open a file call public_key.txt for reading  
  FILE *file = fopen("public_key.txt","w");  //r stands for read, w stands for write

  
  //read the size of the data in the first entry
  fprintf(file, "%u\n", n); //%u specifies an unsigned integer
  fprintf(file, "%u\n", p);
  fprintf(file, "%u\n", g);
  fprintf(file, "%u\n", h);
  //close the file when we're done
  fclose(file);

  
  return 0;
}
