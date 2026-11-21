# Simulador de Urgencias

Proyecto final de Estructuras de Datos (2026-2). Simulador en C++ (POO) que se ejecuta en modo texto:
un médico recorre una sala de 10 camas y atiende pacientes que llegan según su gravedad.

**Estado:** en desarrollo. Por ahora `main.cpp` está vacío: las clases están hechas, falta el ciclo de turnos
que las usa, así que al ejecutar el programa todavía no se imprime nada.

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
  hospital/         Cama, Sala
  estructuras/      RegistroPacientes (tabla hash), PilaAtendidos (pila)
src/                implementaciones (.cpp), misma organización que include/
main.cpp
CMakeLists.txt
largo/urgencias.cpp archivo único de trabajo con todas las clases juntas
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

## Pendiente

- `ColaPrioridadPacientes` (cola de prioridad por gravedad): ya está escrita en `largo/urgencias.cpp`, pero
  todavía no está separada en `include/` y `src/`, así que el programa compilado con CMake aún no la incluye.
- Ciclo de turnos con las acciones del médico, métricas y auditoría final.

## Trabajo en equipo

Antes de empezar: `git pull`. Al terminar: `git add <archivos>`, `git commit` y `git push`.
Antes de cada commit conviene revisar `git status`: lo que aparece con la letra en la columna izquierda es lo que entra.
