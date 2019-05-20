#include <stdio.h>
#include <string.h>

void shrink (FILE * in, FILE * out){
  int previous = 0;
  int current = fgetc(in);
  while(!feof(in)){
    fputc(current - previous, out);
    previous = current;
    current = fgetc(in);
  }
}

void unshrink (FILE * in, FILE * out){
  int previous = 0;
  int delta = fgetc(in);
  while(!feof(in)){
    fputc(previous + delta, out);
    previous = previous + delta;
    delta = fgetc(in);
  }
}

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

  if(strcmp(mode, "shrink") == 0){
    shrink (in, out);
  } else if (strcmp(mode, "unshrink") == 0) {
    unshrink (in, out);
  } else {
    printf("Unknown mode: %s", mode);
  }
  return 0;
}
