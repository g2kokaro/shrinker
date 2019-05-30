#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define bufferSize 2048

// void hexDump(char * buffer, int length){
//   for (int i = 0; i < length; i++){
//     printf("%02hhX", (unsigned char)buffer[i]);
//     if(i % 2 == 0){ printf(" "); }
//     if(i % 16 == 0){ printf("\n"); }
//   }
//   printf("\n");
// }

void writeBytes(int count, int previous, int current, FILE *out){
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
}

void shrink (FILE *in, FILE *out){
  char *inputBuffer = malloc(bufferSize);
  int bufferIndex = 0;

  int previous = 0;
  int current = fgetc(in);
  while(!feof(in)){
    inputBuffer[bufferIndex] = current - previous;
    previous = current;
    current = fgetc(in);
    bufferIndex++;
  }
  previous = inputBuffer[0];
  int count = 1;
  for(int i = 1; i < bufferIndex; i++){
    current = inputBuffer[i];
    if(current == previous && count < 255){
      count++;
    } else {
      writeBytes(count, previous, current, out);
      count = 1;
      previous = inputBuffer[i];
    }
  }
  writeBytes(count, previous, current, out);
}

void unshrink (FILE *in, FILE *out){
  char *outputBuffer = malloc(bufferSize);
  int bufferIndex = 0;
  
  unsigned char count = 0;
  unsigned char byte = fgetc(in);
  while (!feof(in)){
    if(byte == 0x00){
      byte = fgetc(in);
      if(byte == 0x00){
        outputBuffer[bufferIndex] = byte;
        bufferIndex++;
      } else {
        count = byte;
        byte = fgetc(in);
        for(int i = 0; i < count; i++){
          outputBuffer[bufferIndex] = byte;
          bufferIndex++;
        }
        
      }
    } else {
      outputBuffer[bufferIndex] = byte;
      bufferIndex++;
    }
    byte = fgetc(in);
  }
  
  unsigned char previous = 0;
  unsigned char delta = 0;
  for(int i = 0; i < bufferIndex; i++){
    delta = outputBuffer[i];
    outputBuffer[i] = previous + delta;
    previous = outputBuffer[i];
  }
  outputBuffer[bufferIndex + 1] = '\0';
  fwrite(outputBuffer, 1, bufferIndex, out);
}

int main(int argc, char *argv[]) {
  if(argc < 4){
    printf("Usage:\n");
    printf("    shrinker shrink <input-file> <output-file>\n");
    printf("    shrinker unshrink <input-file> <output-file>\n");
    return 0;
  }

  char *mode = argv[1];
  char *inputFileName = argv[2];
  char *outputFileName = argv[3];

  FILE *in = fopen(inputFileName, "r");
  if (!in){
    printf("Error opening input file %s!\n", argv[2]);
    return 0;
  }
  FILE *out = fopen(outputFileName, "a");
  if (!out){
    printf("Error opening output file %s!\n", argv[2]);
    return 0;
  }

  if(strcmp(mode, "shrink") == 0){
    shrink (in, out);
  } else if(strcmp(mode, "unshrink") == 0) {
    unshrink (in, out);
  } else {
    printf("Unknown mode: %s", mode);
  }
  return 0;
}
