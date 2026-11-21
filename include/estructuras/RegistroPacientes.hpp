#ifndef REGISTROPACIENTES_HPP
#define REGISTROPACIENTES_HPP
#include <string>
#include <vector>
#include <list>
#include <utility>
#include "modelo/Paciente.hpp"

class RegistroPacientes {
private:
    std::vector<std::list<std::pair<std::string, Paciente>>> registroPacientes;
    int m; // numero de posiciones
    int cantidad; // para hacer O(1) el proceso de contar numero de pacientes

    int funcionHash(const std::string& clave) const; // la clave es la cedula que es un string, hacemos la funcion polinomica

public:
    RegistroPacientes(int t = 211): registroPacientes(std::vector<std::list<std::pair<std::string, Paciente>>> (t)), m(t), cantidad(0) {}

    // prohibir la copia al crear y al asignar
    RegistroPacientes(const RegistroPacientes&) = delete;
    RegistroPacientes& operator=(const RegistroPacientes&) = delete;

    // Devolver la cantidad de pacientes en O(1)
    int cantidadRegistros() const;


    Paciente* registrar(const Paciente& p);


    // dos metodos con mismo modelo pero devuelven cosas diferentes
    Paciente* buscar(const std::string& cedula);
    bool existe(const std::string& cedula) const;


    /* AUDITORIA AL FINAL DEL TURNO

    Hacer dos for (anidados) que al final es casi O(n) por los pocos indicies compartidos (colisiones)

    Metodo que devuelva una lista con la direccion de cada uno de los pacientes atendidos
    */

    std::list<const Paciente*> todos() const;
};

#endif
