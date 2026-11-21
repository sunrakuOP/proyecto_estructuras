#include "modelo/Paciente.hpp"
#include <iostream>
#include <cctype>

std::string Paciente::getNombre() const { return nombre; }
std::string Paciente::getCedula() const { return cedula; }
Gravedad Paciente::getGravedad() const { return x; }
std::string Paciente::getCondicion() const { return condicion; }
int Paciente::getTurnoLlegada() const { return turnoLlegada; }
int Paciente::getTiempoEspera() const { return tiempoEspera; }
int Paciente::getContadorRiesgo() const { return contadorRiesgo; }
EstadoPaciente Paciente::getEstadoPaciente() const { return estadoP; }
std::string Paciente::getNombreMedico() const { return nombreMedico; }
int Paciente::getTurnoCierre() const { return turnoCierre; }


bool Paciente::esCedulaValida() const {
    if(cedula == "" || (int)cedula.size() > 10) {
        return false;
    }

    for(int i=0; i < (int)cedula.size(); ++i) {
        if(!std::isdigit(static_cast<unsigned char>(cedula.at(i)))) {
            return false;
        }
    }
    return true;
}

void Paciente::esperarUnTurno() {
    tiempoEspera = tiempoEspera + minutosPorTurno;
    if(x == Gravedad::critico) {
        contadorRiesgo++;
    }
}

bool Paciente::estaPerdido() const {
    if(x != Gravedad::critico) {
        return false;
    }
    if(contadorRiesgo > limiteRiesgo) {
        return true;
    }
    return false;
}

void Paciente::marcarEnCama() {
    estadoP = EstadoPaciente::cama;
}

void Paciente::marcarAlta(const std::string& nomMed, int turno) {
    estadoP = EstadoPaciente::alta;
    nombreMedico = nomMed;
    turnoCierre = turno;
}

void Paciente::marcarPerdido(const std::string& nomMed, int turno) {
    estadoP = EstadoPaciente::perdido;
    nombreMedico = nomMed;
    turnoCierre = turno;
}

int Paciente::turnoDeTratamiento() const { return 5 - static_cast<int>(x); }

void Paciente::mostrarDatosPaciente() const {
    std::cout<<"Nombre: "<< nombre <<std::endl;
    std::cout<<"Cedula: "<< cedula <<std::endl;
    std::cout<<"Gravedad: "<< aNumero(x) <<std::endl;
    std::cout<<"Condicion: "<< condicion <<std::endl;
    std::cout<<"Turno llegado: "<< turnoLlegada <<std::endl;
    std::cout<<"Tiempo espera: "<< tiempoEspera <<std::endl;
    std::cout<<"Contador riesgo: "<< contadorRiesgo <<std::endl;
    std::cout<<"Estado paciente: "<< aTexto(estadoP) <<std::endl;

    // lo que sigue depende del estado: en espera o en cama todavia no hay medico ni turno de cierre, no se imprime nada
    if(estadoP == EstadoPaciente::alta) {
        std::cout<<"Atendido por: "<< nombreMedico <<std::endl;
        std::cout<<"Turno de alta: "<< turnoCierre <<std::endl;
    } else if(estadoP == EstadoPaciente::perdido) {
        std::cout<<"Medico de turno: "<< nombreMedico <<std::endl;
        std::cout<<"Turno de perdida: "<< turnoCierre <<std::endl;
    }
}
