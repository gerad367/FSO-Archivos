#!/bin/bash

# Pruebas de "misala crea"

res=0
exitos=0

# Crear sala
echo "Creando una sala."
./misala crea -f fichero -c 5
if [ (echo $_) -eq 0 ]; then
  echo "Prueba exitosa"
  exitos=$exitos + 1
else
  echo "Prueba fallida"
  return 1
fi

# # Crear sala ya existente sobreescribiendo
# ./misala crea -f -o fichero -c 5

# # Intentando crear sala que ya existe sin sobreescribir
# ./misala crea -f fichero -c 5

# # Intentando crear sala sin indicar la ruta
# rm ./fichero
# ./misala crea -c 5

# # Intentando crear sala sin indicar la capacidad
# rm ./fichero
# ./misala crea -f fichero

# # Intentando crear una sala cambiando el orden de los parámetros
# rm ./fichero
# ./misala -f fichero -c 5 crea
