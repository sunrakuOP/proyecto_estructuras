#ifndef URGENCIAS_HPP
#define URGENCIAS_HPP
#include <string>
#include <list>
#include "hospital/Sala.hpp"
#include "modelo/Medico.hpp"
#include "modelo/Paciente.hpp"
#include "estructuras/ColaPrioridadPacientes.hpp"
#include "estructuras/PilaAtendidos.hpp"
#include "estructuras/RegistroPacientes.hpp"
class Urgencias { // Clase general que controla el simulador
    private:
    // Atributos
    Sala salaUrgencias;
    RegistroPacientes registro;
    ColaPrioridadPacientes cola;
    PilaAtendidos pila;
    std::list<Medico> medicos;
    Medico* medicoActual;
    int turnoGlobal;
    // Metodos
    void mostrarEstadoTurno()const; // Imprime NombreDoctor | Turno: x/50 | Pacientes en cola: y
    void mostrarReporteFinal()const;
    Medico* buscarOCrearMedico(const std::string& nombre);
    void cambiarMedico();
    void generarLlegadaPaciente();

    public:
    Urgencias();
    void iniciar();
};

#endif