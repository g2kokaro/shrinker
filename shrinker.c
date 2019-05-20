#include <stdio.h>

int main(int argc, char *argv[]) {
  if(argc < 4){
    printf("Usage:\n");
    printf("    node shrinker.js shrink <input-file> <output-file>\n");
    printf("    node shrinker.js unshrink <input-file> <output-file>\n");
  }

  char *mode = argv[1];
  char *inputFileName = argv[2];
  char *outputFileName = argv[3];

  FILE * in = fopen(inputFileName, "r");
  FILE * out = fopen(outputFileName, "a");

  
  return 0;
}
