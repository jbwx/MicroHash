// Takes 64-bit sample from input file, outputs 64-bit hash
// Written by Jacob Westra (jwbx.github.io)
// Compile: gcc -Wall src.c -o microhash
// Usage: ./microhash <filename>
// Last updated on 3/6/23

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char * argv[]) {

  if (argc != 2 || access(argv[1], F_OK) != 0) {
    return -1; // invalid # of args, or file doesn't exist
  }

  FILE * srcPtr; // initialize pointer to data source
  srcPtr = fopen(argv[1], "r");

  fseek(srcPtr, 0, SEEK_END); // seek to end and get index, acquire filesize in bytes
  unsigned filesize = ftell(srcPtr);

  unsigned segsize = 1 + ceil(filesize / 8); // divide into 8 segments, always guarantee segment size isn't zero
  int prev = filesize + 1; // removing the +1 creates weird harmonics between prev and segsize
  int accumulator = 5; // acts as a pseudo-sum. 5 chosen as starting value because prime

  int sample[8] = { // 7-digit primes, serve as mask / default input
    9854023,
    2024219,
    7004737,
    3616397,
    2032307,
    7450127,
    1969969,
    9195427
  };

  // read 8 samples from input
  for (int x = 0; x < 8 && x < filesize; x++) { //O(8) time, technically O(16) since there's another loop
    fseek(srcPtr, ((x * segsize) + (prev % segsize)) % filesize, 0); // read from index + pseudo-random offset
    char sampled = fgetc(srcPtr); // read into variable, since calling fgetc() messes with pointer index
    sample[x] ^= (int) sampled % 256; // xor with mask
    prev = (int) sampled;
    accumulator += abs((int) sampled);
  }
  accumulator *= 609241; // multiply by 609241 (prime), ensure subtle changes get magnified
  fclose(srcPtr); // done reading

  // mash together and print out
  for (int x = 0; x < 8; x++) {
    int result = abs((sample[x] ^ accumulator) % 256); // xor each with accumulator, 256 is max for 2 hex characters (16^2)
    if (result < 16) { // ensure length doesn't vary
      printf("0");
    }
    printf("%x", result);
  }
  printf("\n");
}