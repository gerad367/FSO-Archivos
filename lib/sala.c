#include "sala.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct Sala {
  int capacidad;
  int ocupados;
  int* id_personas;
};

static struct Sala sala = {0, 0};

int reserva_asiento(int id_persona){
  if (sala.capacidad == 0 || id_persona <= 0) return -1; // No existe la sala
  for (int i = 0; i < sala.capacidad; i++) {
    if (sala.id_personas[i] == 0) {
      sala.id_personas[i] = id_persona;
      sala.ocupados++;
      return ++i; // Se devuelve el id del asiento seleccionado
    }
  }
  return -1; // No hay asientos libres
}

int libera_asiento(int id_asiento){
  // No existe la sala, o el id del asiento es inválido
  if (sala.capacidad == 0 || id_asiento <= 0 || id_asiento > sala.capacidad) return -1;
  int id = sala.id_personas[id_asiento - 1];
  if (id <= 0) return -1;
  sala.id_personas[id_asiento - 1] = 0;
  sala.ocupados--;
  return id;
}

int estado_asiento(int id_asiento){
  // No existe la sala, o el id del asiento es inválido
  if (sala.capacidad == 0 || id_asiento <= 0 || id_asiento > sala.capacidad) return -1;
  return sala.id_personas[id_asiento - 1];
}

int asientos_libres(){
  // No existe la sala
  if (sala.capacidad == 0) return -1;
  return sala.capacidad - sala.ocupados;
}

int asientos_ocupados(){
  // No existe la sala
  if (sala.capacidad == 0) return -1;
  return sala.ocupados;
}

int capacidad_sala(){
  // no existe la sala
  if (sala.capacidad == 0) return -1;
  return sala.capacidad;
}

int crea_sala(int capacidad){
  if (sala.capacidad != 0 || capacidad <= 0) return -1; // Ya existe una sala, se debería eliminar la anterior antes de crear otra
  sala.id_personas = malloc(capacidad * sizeof(int));
  if (sala.id_personas == NULL) return -1; // No se ha podido reservar memoria para la sala
  sala.capacidad = capacidad;
  sala.ocupados = 0;
  for (int i = 0; i < capacidad; i++) sala.id_personas[i] = 0;
  return capacidad;
}

int elimina_sala(){
  if (sala.capacidad == 0) return -1; // No existe sala que eliminar
  sala.capacidad = 0;
  sala.ocupados = 0;
  free(sala.id_personas);
  return 0;
}

// Funciones para guardar el estado de la sala
int guarda_estado_sala(const char* ruta_fichero) {

  if (sala.capacidad == 0) {
    fprintf(stderr, "No existe sala.\n");
    return -1;
  }

  int file = open(ruta_fichero, O_WRONLY | O_CREAT | O_TRUNC, 0644);

  if (file == -1) {
    perror("Error al guardar el estado de la sala");
    return -1;
  }

  if (write(file, &sala.capacidad, sizeof(int)) != 4) {
    perror("Error al guardar el estado de la sala");
    close(file);
    return -1;
  }

  if (write(file, &sala.ocupados, sizeof(int)) != 4) {
    perror("Error al guardar el estado de la sala");
    close(file);
    return -1;
  }

  int size = sala.capacidad * sizeof(int);

  if (write(file, sala.id_personas, size) != size) {
    perror("Error al guardar el estado de la sala");
    close(file);
    return -1;
  }

  close(file);

  return 0;
}

int recupera_estado_sala(const char* ruta_fichero) {

  int file = open(ruta_fichero, O_RDONLY, 0);

  if (file == -1) {
    perror("Error al cargar el estado");
    return -1;
  }

  int capacidad;
  if (read(file, &capacidad, sizeof(int)) != sizeof(int) || capacidad <= 0) {
    perror("Error al cargar el estado");
    close(file);
    return -1;
  }

  if (sala.capacidad != capacidad) {
    fprintf(stderr, "La capacidad de las salas no coinciden.\n");
    close(file);
    return -1;
  }

  if (read(file, &sala.ocupados, sizeof(int)) != sizeof(int)) {
    perror("Error al cargar el estado");
    close(file);
    return -1;
  }

  int size = sala.capacidad * sizeof(int);

  if (read(file, sala.id_personas, size) != size) {
    perror("Error al cargar el estado");
    close(file);
    return -1;
  }

  close(file);

  return 0;
}

int guarda_estado_parcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos) {

  int file = open(ruta_fichero, O_RDWR, 0);
  int data;
  int offset;

  if (file == -1) {
    perror("Error al guardar estado parcial");
    return -1;
  }

  int capacidad;
  if (read(file, &capacidad, sizeof(int)) != sizeof(int)) {
    perror("Error al guardar estado parcial");
    close(file);
    return -1;
  }

  int ocupados;
  if (read(file, &ocupados, sizeof(int)) != sizeof(int)) {
    perror("Error al guardar estado parcial");
    close(file);
    return -1;
  }

  if (sala.capacidad != capacidad) {
    fprintf(stderr, "La capacidad de las salas no coinciden.\n");
    close(file);
    return -1;
  }

  printf("pre-bucle\n");
  
  for (int i = 0; i < num_asientos; i++) {
    offset = (int)(&sala.id_personas[id_asientos[i] - 1] - sala.id_personas) * 4 + sizeof(int) * 2;

    printf("offset calculado\n");
    
    if (lseek(file, offset, SEEK_SET) == -1) {
      perror("Error al guardar estado parcial");
      close(file);
      return -1;
    }

    printf("offset aplicado\n");

    if (read(file, &data, sizeof(int)) != sizeof(int)) {
      perror("Error al guardar estado parcial");
      close(file);
      return -1;
    }

    printf("asiento leido: %d\n", data);

    if (data == 0 && sala.id_personas[id_asientos[i] - 1] != 0) {
      // El asiento estaba libre y ahora estará ocupado -> incrementamos el contador de asientos ocupados
      ocupados++;
    } else if (data != 0 && sala.id_personas[id_asientos[i] - 1] == 0) {
      // El asiento estaba ocuapdo y ahora estará libre -> decrementamos el contador de asientos ocupados
      ocupados--;
    }

    printf("contador ajustado\n");

    if (lseek(file, -sizeof(int), SEEK_CUR) == -1) {
      perror("Error al guardar estado parcial");
      close(file);
      return -1;
    }

    if (write(file, &sala.id_personas[id_asientos[i] - 1], sizeof(int)) != sizeof(int)) {
      perror("Error al guardar estado parcial");
      close(file);
      return -1;
    }

    printf("asiento escrito\n");
  }

  printf("post-bucle\n");

  // Después de haber actualizado los valores de los asientos, actualizamos la variable de asientos ocupados
  if (lseek(file, sizeof(int), SEEK_SET) == -1) {
    perror("Error al guardar estado parcial");
    close(file);
    return -1;
  }

  if (write(file, &ocupados, sizeof(int)) != sizeof(int)) {
    perror("Error al guardar estado parcial");
    close(file);
    return -1;
  }

  close(file);
  
  return 0;
}

int recupera_estado_parcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos) {

  int capacidad, ocupados, data;
  off_t offset;

  if (sala.capacidad == 0) {
    fprintf(stderr, "No existe sala donde cargar los datos.\n");
    return -1;
  }

  int file = open(ruta_fichero, O_RDONLY, 0);
  if (file == -1) {
    perror("Error al cargar estado parcial.");
    return -1;
  }

  if (read(file, &capacidad, sizeof(int)) != sizeof(int)) {
    perror("Error al cargar estado parcial");
    return -1;
  }

  if (capacidad != sala.capacidad) {
    fprintf(stderr, "La capacidad de las salas no coinciden.\n");
    return -1;
  }

  for (int i = 0; i < num_asientos; i++) {
    offset = (int)(&sala.id_personas[id_asientos[i] - 1] - sala.id_personas) * 4 + sizeof(int) * 2;
    
    if (lseek(file, offset, SEEK_SET) == -1) {
      perror("Error al cargar estado parcial");
      close(file);
      return -1;
    }

    if (read(file, &data, sizeof(int)) != sizeof(int)) {
      perror("Error al cargar estado parcial");
      close(file);
      return -1;
    }

    if (sala.id_personas[id_asientos[i] - 1] == 0 && data != 0) {
      // El asiento estaba libre y ahora estará ocupado -> incrementamos el contador de asientos ocupados
      sala.ocupados++;
    } else if (sala.id_personas[id_asientos[i] - 1] != 0 && data == 0) {
      // El asiento estaba ocuapdo y ahora estará libre -> decrementamos el contador de asientos ocupados
      sala.ocupados--;
    }

    sala.id_personas[id_asientos[i] - 1] = data;
  }

  close(file);
  return 0;
}

