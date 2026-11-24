cmake_minimum_required(VERSION 3.10)
project(urgencias CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# genera build/compile_commands.json para que el editor (clangd) encuentre la carpeta include/
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

add_executable(urgencias
    main.cpp
    src/modelo/Enums.cpp
    src/modelo/Paciente.cpp
    src/modelo/Medico.cpp
    src/hospital/Cama.cpp
    src/hospital/Sala.cpp
    src/hospital/Urgencias.cpp
    src/estructuras/RegistroPacientes.cpp
    src/estructuras/PilaAtendidos.cpp
    src/estructuras/ColaPrioridadPacientes.cpp
)

# permite escribir #include "modelo/Paciente.hpp" desde cualquier archivo
target_include_directories(urgencias PRIVATE include)
