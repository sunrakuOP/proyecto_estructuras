# Simulador de Urgencias

Proyecto final de Estructuras de Datos (2026-2). Simulador en C++ (POO) que se ejecuta en modo texto:
un médico recorre una sala de 10 camas y atiende pacientes que llegan según su gravedad.

**Estado:** funcional. Están implementadas todas las clases, el ciclo interactivo de turnos (Urgencias::iniciar()), las alertas de riesgo y el reporte final con métricas y auditoría.

## Cómo compilar

Requiere CMake (3.10 o superior) y un compilador con C++17.

```
cmake -S . -B build
cmake --build build
./build/urgencias
```

## Estructura

```
include/            declaraciones (.hpp)
  modelo/           Enums, Paciente, Medico
  hospital/         Cama, Sala, Urgencias
  estructuras/      RegistroPacientes (tabla hash), PilaAtendidos (pila), ColaPrioridadPacientes (cola de prioridad)
src/                implementaciones (.cpp), misma organización que include/
main.cpp
CMakeLists.txt
largo/largo.cpp archivo único de trabajo con todas las clases juntas
```

Los `#include` se escriben con la carpeta, por ejemplo `#include "modelo/Paciente.hpp"`.

## Cómo agregar una clase

1. Declaración en `include/<carpeta>/Nombre.hpp` (con guardas `#ifndef`) e implementación en `src/<carpeta>/Nombre.cpp`.
2. Sumar el `.cpp` a la lista de `add_executable` en `CMakeLists.txt` (si no, el linker no la encuentra).
3. Incluirla con la carpeta: `#include "<carpeta>/Nombre.hpp"`.

## Clases hechas

| Clase | Qué hace |
|---|---|
| `Paciente` | datos del paciente, estado (espera, cama, alta, perdido) y turno de cierre |
| `Cama` | nodo de la lista doblemente enlazada: estado, paciente actual y turnos restantes |
| `Sala` | las 10 camas, la posición del médico y su desplazamiento a izquierda y derecha |
| `Medico` | jornada de hasta 50 turnos e historial de jornadas |
| `RegistroPacientes` | tabla hash de pacientes por cédula |
| `PilaAtendidos` | pila (LIFO) de los últimos pacientes dados de alta |
| `ColaPrioridadPacientes` | cola de prioridad por gravedad (FIFO dentro del mismo nivel); también envejece a los pacientes y saca a los perdidos |
| 'Urgencias' | controlador central del simulador: gestiona el ciclo de turnos, el ingreso de pacientes, las acciones del médico (A, D, R, T, X, U, C, O, E), el estado durante cada turno y el reporte final con métricas y auditoría |


## Trabajo en equipo

Antes de empezar: `git pull`. Al terminar: `git add <archivos>`, `git commit` y `git push`.
Antes de cada commit conviene revisar `git status`: lo que aparece con la letra en la columna izquierda es lo que entra.
