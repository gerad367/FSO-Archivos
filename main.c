#include <stdio.h>
#include "lib/sala.h"

int estado;

void result(){
  printf("%s\n", (estado == -1)? "Operacion fallida." : "Operacion correcta.");
}

int main() {

  int indexes[] = {1, 2, 4};

  crea_sala(5);

  for (int i = 0; i < 5; i++) {
    reserva_asiento(10);
  }

  estado = guarda_estado_sala("temp");
  result();

  estado = elimina_sala();
  result();

  estado = crea_sala(5);
  result();

  printf("Hay %d asientos ocupados.\n", asientos_ocupados());

  estado = recupera_estado_parcial_sala("temp", 3, indexes);
  result();

  printf("Hay %d, asientos ocupados.\n", asientos_ocupados());
  for (int i = 1; i <= 5; i++) {
    printf("El asiento %d está %s\n", i, estado_asiento(i)==0? "libre":"ocupado");
  }

  return 0;
}
