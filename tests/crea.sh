#!/bin/bash

test () {
  if [ $1 -eq 0 ]; then
    echo "Prueba exitosa"
    exitos=$((exitos + 1))
  else
    echo "Prueba fallida"
  fi
  total=$((total + 1))
}

testFail () {
  if [ $1 != 0 ]; then
    echo "Prueba exitosa"
    exitos=$((exitos + 1))
  else
    echo "Prueba fallida"
  fi
  total=$((total + 1))
}

limpia () {
  if [ -e fichero ]; then
    rm fichero
  fi
}

# Pruebas de "misala crea"

exitos=0
total=0

limpia

# Crear sala
echo "Creando una sala con un tamaño normal."
../misala crea -f fichero -c 5
test $(echo $?)
echo ""

# Crear sala ya existente sobreescribiendo
echo "Creando una sala previamente existente sobreescribiendo"
../misala crea -of fichero -c 5
test $(echo $?)
echo ""

# Intentando crear sala que ya existe sin sobreescribir
echo "Creando una sala previemente existente sin sobreescribir"
../misala crea -f fichero -c 5
testFail $(echo $?)
echo ""

# Intentando crear sala sin indicar la ruta
echo "Creando una sala sin indicar la ruta"
limpia
../misala crea -c 5
testFail $(echo $?)
echo ""

# Intentando crear sala sin indicar la capacidad
echo "Creando una sala sin indicar la capacidad"
limpia
../misala crea -f fichero
testFail $(echo $?)
echo ""

# Intentando crear sala con capacidad 0
echo "Creando una sala con capacidad cero"
limpia
../misala crea -f fichero -c 0
testFail $(echo $?)
echo ""

echo "$exitos pruebas exitosas de $total pruebas realizadas"
