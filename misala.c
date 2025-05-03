#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "lib/sala.h"


int main(int argc, char** argv) {

  if (argc == 1) {
    fprintf(stderr, "Faltan argumentos.\n");
  }

  extern char *optarg;
  extern int optind;
  optind = 2;
  int file;
  char* filename = NULL;
  int capacity;


  int overwrite = 0;
  
  char c;
  while ((c = getopt(argc, argv, "f:oc:")) != -1)  {
    switch (c) {
      case 'f':
        printf("detectado f: optind -> %d\n", optind);
        printf("%s\n", optarg);
        filename = optarg;
        break;
      case 'o':
        printf("detectado o: optind -> %d\n", optind);
        overwrite = 1;
        break;
      case 'c':
        printf("detectado c: optind -> %d\n", optind);
        capacity = atoi(optarg);
        break;
      default:
        printf("No reconocido.\n");
        break;
    }
  }

  for (int i = 0; i < argc; i++) {
    printf("argv[%d] -> %s\n", i, argv[i]);
  }


  if (filename == NULL) {
    fprintf(stderr, "No se ha indicado fichero,\n");
    return -1;
  }

  if (strcmp(argv[1], "crea") == 0) {
    if (capacity < 0) {
      fprintf(stderr, "Introduzca un valor de capacidad correto,\n");
      return -1;
    }
    
    if (access(filename, F_OK) == 0 && !overwrite) {
      fprintf(stderr, "El fichero ya existe y no se ha especificado sobreescritura.\n");
      return -1;
    }
    crea_sala(capacity);
    guarda_estado_sala(filename);
   
  }


  return 0;
}
