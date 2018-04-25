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

  int bufferSize = 1024;
  unsigned char *message = (unsigned char *) malloc(bufferSize*sizeof(unsigned char));

  printf("Enter a message to encrypt: ");
  int stat = scanf (" %[^\n]%*c", message); //reads in a full line from terminal, including spa

  //declare storage for an ElGamal cryptosytem
  unsigned int n, p, g, h;

  printf("Reading file.\n");

  /* Q2 Complete this function. Read in the public key data from public_key.txt,
    convert the string to elements of Z_p, encrypt them, and write the cyphertexts to 
    message.txt */

  
  FILE *public = fopen("public_key.txt","r");  //r stands for read, w stands for write

  
  //read the size of the data in the first entry
  fscanf(public, "%u", &n); //%u specifies an unsigned integer
  fscanf(public, "%u", &p);
  fscanf(public, "%u", &g);
  fscanf(public, "%u", &h);
  
  
  //close the file when we're done
  fclose(public); 



  unsigned int charsPerInt = (n-1)/8;

  padString(message, charsPerInt);
  printf("Padded Message = \"%s\"\n", message);

  unsigned int Nchars = strlen(message);
  unsigned int Nints  = strlen(message)/charsPerInt;

  //storage for message as elements of Z_p
  unsigned int *Zmessage = 
      (unsigned int *) malloc(Nints*sizeof(unsigned int)); 
  
  //storage for extra encryption coefficient 
  unsigned int *a = 
      (unsigned int *) malloc(Nints*sizeof(unsigned int)); 

  // cast the string into an unsigned int array
  convertStringToZ(message, Nchars, Zmessage, Nints);
  
  //Encrypt the Zmessage with the ElGamal cyrptographic system
  ElGamalEncrypt(Zmessage,a,Nints,p,g,h);



  //take entries of m and a and put then
  FILE *newMessage = fopen("message.txt", "w");
  fprintf(newMessage, "%u\n", Nints);
  
  for (unsigned int m=0;m<Nints;m++)
  {
      fprintf(newMessage, "%u %u\n", Zmessage[m], a[m]);
  }

  fclose(newMessage);
 

    




  return 0;
}
