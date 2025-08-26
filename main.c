#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE  1024
#define PROGRAM_SIZE 512

enum op_code {
  PUSH, //done
  POP,  //done
  ADD,  //done
  SUB,  //done
  MUL,
  DIV,
  STOP, //done
};

unsigned char program[PROGRAM_SIZE] = {
  PUSH, 2, 0xff, 0xff,
  PUSH, 2, 0x01, 0x0,
  SUB, 2,
  POP, 2,
  STOP,
};

int main(void) {
  unsigned char *memory = calloc(MEMORY_SIZE, 1);
  unsigned char flags = 0;
  int pc = 0;
  int sc = MEMORY_SIZE;
  
  memcpy(memory, program, sizeof(program));

  for (;memory[pc] != STOP; pc++) {
    int byte;
    switch (memory[pc]) {
      case PUSH:
        for (int i = memory[++pc]; i > 0; i--) {
          memory[--sc] = memory[++pc];
        }
        break;
      case POP:
        for (int i = memory[++pc]; i > 0; i--) {
          printf("%hx\n", memory[sc++]);
        }
        break;
      case ADD:
        byte = memory[++pc];
        sc += byte;
        for (int i = byte; i > 0; i--) {
          sc--;
          short res = memory[sc+byte] + memory[sc] + (flags >> 7);
          if (res > 255) {
            flags |= 128;
          } else {
            flags &= 127;
          }
          memory[sc+byte] = res;
        }
        sc += byte;
        break;
      case SUB:
        byte = memory[++pc];
        sc += byte;
        for (int i = byte; i > 0; i--) {
          sc--;
          short res = memory[sc+byte] - memory[sc] - ((flags & 64) >> 6);
          if (memory[sc+byte] < memory[sc]) {
            res += 256;
            flags |= 64;
          } else {
            flags &= 191;
          }
          memory[sc+byte] = res;
        }
        sc += byte;
        break;
      case MUL:
        sc++;
        memory[sc] *= memory[sc-1];
        break;
      case DIV:
        sc++;
        memory[sc] /= memory[sc-1];
        break;
      default:
        printf("pc = %d\n", pc);
    }
  }

  free(memory);
}
