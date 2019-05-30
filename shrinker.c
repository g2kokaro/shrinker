#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define bufferSize 4096

void shrink (FILE *in, FILE *out){
  char *inputBuffer = malloc(bufferSize);
  unsigned char bufferIndex = 0;

  int previous = 0;
  int current = fgetc(in);
  while(!feof(in)){
    inputBuffer[bufferIndex] = current - previous;
    previous = current;
    current = fgetc(in);
    bufferIndex++;
  }

  previous = inputBuffer[0];
  current = inputBuffer[1];
  unsigned char count = 1;
  for(int i = 1; i < bufferIndex; i++){
    current = inputBuffer[i];
    if(current == previous && count < 255){
      count++;
    } else {
      if(count == 1){
        if(previous == 0x00){
          fputc(0x00, out);
        }
        fputc(previous, out);
      } else {
        fputc(0x00, out);
        fputc(count, out);
        fputc(previous, out);
      }
      count = 1;
      previous = inputBuffer[i];
    }
  }
}

void unshrink (FILE *in, FILE *out){
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

  FILE *in = fopen(inputFileName, "r");
  FILE *out = fopen(outputFileName, "a");

  if(strcmp(mode, "shrink") == 0){
    shrink (in, out);
  } else if(strcmp(mode, "unshrink") == 0) {
    unshrink (in, out);
  } else {
    printf("Unknown mode: %s", mode);
  }
  return 0;
}
