#include <iostream>
#include <string>
#include <list>
#include <cctype>
#include <stack>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include "hospital/Urgencias.hpp"

Urgencias::Urgencias(){ // Contructor
    medicoActual = nullptr; // Aun no se ha asignado ningun medico
    turnoGlobal = 1;
    turnoJornada = 1;
    totalPacientesRecibidos = 0;
    totalDadosDeAlta = 0;
    sumaEsperaRecibidos = 0;
}

Urgencias::~Urgencias(){ // Destructor
    for (Medico* m : medicos) {
        delete m;
    }
}
Medico* Urgencias::buscarOCrearMedico(const std::string& nombre){
    for (Medico* m : medicos){
        if (m->getNombreMedico() == nombre){ // Busca al medico con el nombre
            return m;
        }
    } // Si no lo encuentra lo crea y agrega a la lista de medicos
    Medico* nuevoMedico = new Medico(nombre);
    medicos.push_back(nuevoMedico);
    return nuevoMedico;
}

void Urgencias::cambiarMedico(){
    if (medicoActual != nullptr) {
        medicoActual->terminarJornada();
        pila.vaciar();
        std::cout << "\nJornada de " << medicoActual->getNombreMedico() << " finalizada.\n";
    }
    std::string nombreNuevo;
    std::cout << "\nIngrese el nombre del nuevo medico: ";
    std::getline(std::cin >> std::ws, nombreNuevo);

    medicoActual = buscarOCrearMedico(nombreNuevo);
    medicoActual->iniciarJornada();
    turnoJornada = 1;
    salaUrgencias.reiniciarPosicionM(); // Para que inicie en la cama #1
    std::cout << "Bienvenido/a, " << medicoActual->getNombreMedico() << ". Inicia nueva jornada (Turno 1/50).\n";
}

void Urgencias::mostrarEstadoTurno() const {
    std::cout<<"\n";
    if (medicoActual==nullptr){
        std::cout<< "Sin asignar medico." << "  |  ";
    } else {
        std::cout<< "Dr. " << medicoActual->getNombreMedico() << "  |  ";
    }
    std::cout<< "Turno: " << turnoJornada << "/50" << "  |  ";
    if (cola.estaVacia()){
        std::cout<< "Sin pacientes en cola." << std::endl;
    } else {
        std::cout<< "Pacientes en cola: " << cola.totalPacientesCola() << std::endl;
    }
    salaUrgencias.mostrar();
    Cama& camaActual = salaUrgencias.getCamaActual();
    std::cout<< "Cama actual: #" << camaActual.numeroCama << " - ";
    if (camaActual.estaVacia()){
        std::cout<< "Vacia." << std::endl;
    } else if (camaActual.getEstadoCama()==EstadoCama::mantenimiento){
        std::cout<< "En mantenimiento." << std::endl;
    } else { // Ocupada
        Paciente* p = camaActual.getPaciente();
        int turnosTotales = p->turnoDeTratamiento();
        int completados = turnosTotales - camaActual.getTurnosRestantes();
        std::cout<< "Paciente: " << p->getNombre() << 
        "  (Gravedad: " << aNumero(p->getGravedad()) << ", " <<
        aTexto(p->getGravedad()) << ")\n" << 
        "Tratamiento: " << completados << "/" << turnosTotales << " turnos completados.\n\n";
    }
    if (cola.estaVacia()){
        std::cout<< "Sin pacientes en la cola de prioridad." <<std::endl;
    } else { // Resumen cola de prioridad
        std::cout<< "Cola de prioridad:\n";
        std::list<Gravedad> niveles = {
            Gravedad::critico,
            Gravedad::severo,
            Gravedad::moderado,
            Gravedad::leve,
            Gravedad::consulta
        };
        for (Gravedad g : niveles) {
            int cantidad = cola.cantidadPorGravedad(g);
            if (cantidad > 0) {
                int sumaMinutos = cola.sumaEsperaPorGravedad(g);
                int divisor = cantidad * 5;
                float promedioTurnos = static_cast<float>(sumaMinutos) / divisor;

                std::cout << "- " << cantidad
                        << (cantidad == 1 ? " paciente " : " pacientes ")
                        << "gravedad " << aNumero(g)
                        << " (esperando ";

                if (sumaMinutos % divisor == 0) {
                    std::cout << (sumaMinutos / divisor); // Imprime como int exacto: 0, 1, 2...
                } else {
                    std::cout << std::fixed << std::setprecision(1) << promedioTurnos; // Imprime 1 decimal: 1.5, 2.3...
                }

                std::cout << (cantidad == 1
                                ? (promedioTurnos == 1.0f ? " turno" : " turnos")
                                : (promedioTurnos == 1.0f ? " turno prom." : " turnos en promedio"))
                        << ")";

                if (g == Gravedad::critico && cola.hayAlertaEnGravedad()) {
                    std::cout << " [ALERTA]";
                }
                std::cout << "\n";
            }
        }
    }
}

void Urgencias::generarLlegadaPaciente() {
    if (turnoGlobal > 1 && (rand() % 100 >= 65)) {
        return; // Probabilidad de que no entre un paciente en este turno
    }
    std::vector<std::string> nombres = {"Ana", "Carlos", "Luis", "Marta", "Sofia", "Jorge", "Elena", "Mateo", "Angela", "Samuel"};
    std::vector<std::string> apellidos = {"Ruiz", "Perez", "Mejia", "Velez", "Gomez", "Diaz", "Castro", "Rojas", "Mendoza", "Posso"};

    std::string nombreCompleto = nombres[rand() % nombres.size()] + " " + apellidos[rand() % apellidos.size()]; // Mezcla un nombre con un apellido al azar

    static int contadorCedula = 1000001; // Static para asegurar que no existan dos pacientes con la misma cedula
    std::string cedula = std::to_string(contadorCedula++);
    
    // Definir la gravedad y condicion aleatoriamente
    int dado = rand() % 100;
    Gravedad gravedad;
    std::vector<std::string> posiblesCondiciones;
    if (dado < 15) {
        gravedad = Gravedad::critico;
        posiblesCondiciones = {
            "Dolor toracico agudo",
            "Trauma craneoencefalico severo",
            "Dificultad respiratoria critica",
            "Hemorragia interna"
        };
    } else if (dado < 35) {
        gravedad = Gravedad::severo;
        posiblesCondiciones = {
            "Fractura expuesta",
            "Quemadura de segundo grado",
            "Crisis asmatica aguda",
            "Dolor abdominal intenso"
        };
    } else if (dado < 60) {
        gravedad = Gravedad::moderado;
        posiblesCondiciones = {
            "Fiebre alta y deshidratacion",
            "Herida profunda con sutura",
            "Migrana severa con vomito",
            "Infeccion urinaria aguda"
        };
    } else if (dado < 85) {
        gravedad = Gravedad::leve;
        posiblesCondiciones = {
            "Esguince de tobillo",
            "Reaccion alergica leve",
            "Lumbalgia mecanica",
            "Contusion en hombro",
            "Tos"
        };
    } else {
        gravedad = Gravedad::consulta;
        posiblesCondiciones = {
            "Control medico general",
            "Renovacion de formula",
            "Chequeo de presion arterial",
            "Dolor de garganta leve"
        };
    }
    std::string condicion = posiblesCondiciones[rand() % posiblesCondiciones.size()]; // Elige aleatoriamente una condicion segun la gravedad

    // Crear el paciente, registrarlo en la Tabla Hash y encolarlo
    Paciente nuevoPaciente(nombreCompleto, cedula, gravedad, condicion, turnoGlobal);
    Paciente* guardado = registro.registrar(nuevoPaciente);
    if (guardado != nullptr) {
        cola.encolar(guardado);
        std::cout << "\n[NUEVO PACIENTE EN LA COLA]\nNombre: " << guardado->getNombre()
                  << " (CC: " << guardado->getCedula()
                  << ", Condicion: " << guardado->getCondicion()
                  << ", Gravedad: " << aNumero(guardado->getGravedad()) << ")\n";
    }
}

void Urgencias::mostrarReporteFinal() const{ 
    std::list<const Paciente*> historial = registro.todos(); /* Lista con el 
    historial de pacientes para no modificar nada, solo leer */
    int recibidos = 0;
    int dadosDeAlta = 0;
    int perdidos = 0;
    int sumaEsperaRecibidos = 0;
    for (const Paciente* p : historial) {
        EstadoPaciente est = p->getEstadoPaciente();
        if (est == EstadoPaciente::cama || est == EstadoPaciente::alta) {
            recibidos++; // Cuenta los que estan actualmente en cama y los que fueron dados de alta
            sumaEsperaRecibidos += p->getTiempoEspera();
        }
        if (est == EstadoPaciente::alta) {
            dadosDeAlta++;
        }
        if (est == EstadoPaciente::perdido) {
            perdidos++;
        }
    }
    double promedioEspera = 0.0;
    if (recibidos > 0) {
        promedioEspera = static_cast<double>(sumaEsperaRecibidos) / recibidos;
    } else {
        promedioEspera = 0.0;
    }

    std::cout << "\n========================================\n";
    std::cout << "      REPORTE FINAL DEL SIMULADOR\n";
    std::cout << "========================================\n";
    std::cout << "Pacientes recibidos:        " << recibidos << "\n";
    std::cout << "Pacientes dados de alta:    " << dadosDeAlta << "\n";
    std::cout << "Pacientes perdidos:         " << perdidos << "\n";
    std::cout << "Tiempo promedio de espera:  " << promedioEspera << " minutos\n";
    std::cout << "Camas desocupadas:          " << salaUrgencias.camasDesocupadas() << "\n";
    std::cout << "Cantidad de medicos:        " << medicos.size() << "\n";

    // Historial de jornadas y turnos por medico
    std::cout << "\n--- HISTORIAL DE MEDICOS ---\n";
    for (const Medico* m : medicos) {
        m->mostrar();
    }

    // Auditoria de pacientes perdidos
    std::cout << "\n--- AUDITORIA DE PACIENTES PERDIDOS ---\n";
    if (perdidos == 0) {
        std::cout << "No se perdieron pacientes durante la ejecucion.\n";
    } else {
        std::cout << "Total de pacientes perdidos: " << perdidos << "\n";
        for (const Paciente* p : historial) {
            if (p->getEstadoPaciente() == EstadoPaciente::perdido) {
                std::cout << "- " << p->getNombre() << " (CC: " << p->getCedula()
                          << ", Gravedad " << aNumero(p->getGravedad()) << ")"
                          << " | Medico de turno: " << p->getNombreMedico() << "\n";
            }
        }
    }
    std::cout << "========================================\n";
}

void Urgencias::iniciar() {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::cout << "===========================================\n";
    std::cout << "   BIENVENIDO/A AL SIMULADOR DE URGENCIAS   \n";
    std::cout << "===========================================\n";

    // Iniciar con el primer médico
    cambiarMedico();

    std::list<char> acciones = { 'A', 'D', 'R', 'T', 'X', 'U', 'C', 'E', 'O' };
    bool simulacionActiva = true;
    bool turnoAvanzo = true;

    while (simulacionActiva) {
        // Solo si el turno avanzó, llega (posiblemente) un paciente y se imprime el estado
        if (turnoAvanzo) {
            std::cout << "\n----------------------------------------\n";
            generarLlegadaPaciente();
            mostrarEstadoTurno();
        }

        // SIEMPRE pedir y validar la siguiente acción (aunque el turno no haya avanzado)
        char accion;
        bool accionValida = false;
        while (!accionValida) {
            std::cout << "\nAcciones: [A] [D] [R] [T] [X] [U] [C] [E] [O]\n" <<
                        "[A] =  Moverse a la izquierda.\n" << 
                        "[D] =  Moverse a la derecha.\n" <<
                        "[R] =  Recibir al siguiente paciente de la Cola de Prioridad y asignarlo a la cama actual.\n" << 
                        "[T] =  Tratar al paciente de la cama actual.\n" << 
                        "[X] =  Dar de alta al paciente de la cama actual.\n" << 
                        "[U] =  Ver la Pila de los Últimos pacientes atendidos.\n" << 
                        "[C] =  Consultar la Tabla Hash buscando un paciente por su cédula.\n" << 
                        "[E] =  Terminar el turno anticipadamente y guardar las métricas.\n" <<
                        "[O] =  Cambiar medico.\n"<<
                        "--> ";
            std::cin >> accion;
            std::cin.ignore(10000, '\n'); // Descarta caracteres extra en la misma linea
            accion = static_cast<char>(std::toupper(static_cast<unsigned char>(accion)));

            if (std::find(acciones.begin(), acciones.end(), accion) != acciones.end()) {
                accionValida = true; // Si encuentra la letra en la lista de acciones
            } else {
                std::cout << "Accion no valida. Intenta de nuevo.\n";
            }
        }

        // Por defecto asumimos que NO consume turno; solo las acciones exitosas lo ponen en true
        turnoAvanzo = false;
        Cama& camaActual = salaUrgencias.getCamaActual();

        switch (accion) {
            case 'A': { // Moverse a la izquierda
                if (salaUrgencias.moverIzquierda()) { /* El metodo comprueba si es posible 
                    el movimiento, si es posible lo efectua y retorna true */ 
                    turnoAvanzo = true;
                } else {
                    std::cout << "Ya te encuentras en la primera cama (#1). No se consume turno.\n";
                }
                break;
            }
            case 'D': { // Moverse a la derecha
                if (salaUrgencias.moverDerecha()) {
                    turnoAvanzo = true;
                } else {
                    std::cout << "Ya te encuentras en la ultima cama (#10). No se consume turno.\n";
                }
                break;
            }
            case 'R': { // Recibir al siguiente paciente de la Cola de Prioridad y asignarlo a la cama actual
                if (camaActual.getEstadoCama() == EstadoCama::mantenimiento) {
                    std::cout << "La cama actual esta en mantenimiento. No se consume turno.\n";
                } else if (!camaActual.estaVacia()) {
                    std::cout << "La cama actual ya esta ocupada. No se consume turno.\n";
                } else if (cola.estaVacia()) {
                    std::cout << "No hay pacientes esperando en la cola de prioridad. No se consume turno.\n";
                } else {
                    Paciente* siguiente = cola.desencolar();
                    if (camaActual.asignarPaciente(siguiente)) {
                        siguiente->marcarEnCama();
                        std::cout << "Paciente " << siguiente->getNombre()
                                  << " asignado a la cama #" << camaActual.numeroCama << ".\n";
                        turnoAvanzo = true;
                    }
                }
                break;
            }
            case 'T': { // Tratar al paciente de la cama actual
                if (camaActual.getEstadoCama() == EstadoCama::mantenimiento || camaActual.estaVacia()) {
                    std::cout << "No hay un paciente en esta cama para tratar. No se consume turno.\n";
                } else if (camaActual.tratamientoCompleto()) {
                    std::cout << "El tratamiento ya esta completo. Usa [X] para dar de alta. No se consume turno.\n";
                } else {
                    camaActual.tratar();
                    std::cout << "Tratamiento aplicado en cama #" << camaActual.numeroCama
                              << ". Turnos restantes: " << camaActual.getTurnosRestantes() << ".\n";
                    turnoAvanzo = true;
                }
                break;
            }
            case 'X': { // Dar de alta al paciente de la cama actual
                if (camaActual.getEstadoCama() == EstadoCama::mantenimiento || camaActual.estaVacia()) {
                    std::cout << "La cama actual no tiene paciente. No se consume turno.\n";
                } else if (!camaActual.tratamientoCompleto()) {
                    std::cout << "El tratamiento aun no esta completo (faltan "
                              << camaActual.getTurnosRestantes() << " turnos). No se consume turno.\n";
                } else { // Paciente listo para ser dado de alta
                    Paciente* dadoDeAlta = camaActual.liberar();
                    dadoDeAlta->marcarAlta(medicoActual->getNombreMedico(), turnoGlobal);
                    pila.apilar(*dadoDeAlta);
                    std::cout << "Paciente " << dadoDeAlta->getNombre() << " dado de alta.\n";
                    turnoAvanzo = true;
                }
                break;
            }
            case 'U': { // Ver la Pila de los Últimos pacientes atendidos
                std::cout << "\n" << medicoActual->getNombreMedico() << ", ultimos pacientes dados de alta:\n";
                if (pila.estaVacia()) {
                    std::cout << "  (Ningun paciente dado de alta en esta jornada)\n";
                } else {
                    std::list<const Paciente*> listaUltimos = pila.ultimos();
                    int pos = 1;
                    for (const Paciente* p : listaUltimos) {
                        std::string etiqueta = (pos == 1) ? "[ultimo]" : (pos == 2) ? "[penultimo]" : "[anterior]";
                        std::cout << "> " << etiqueta << " " << p->getNombre()
                                  << " (CC " << p->getCedula()
                                  << ", gravedad " << aNumero(p->getGravedad()) << ")\n";
                        pos++;
                    }
                }
                break;
            }
            case 'C': { // Consultar la Tabla Hash buscando un paciente por su cédula
                std::string cedulaBuscada;
                std::cout << "Ingrese la cedula del paciente a consultar: ";
                std::cin >> cedulaBuscada;
                std::cin.ignore(10000, '\n');

                Paciente* encontrado = registro.buscar(cedulaBuscada);
                if (encontrado == nullptr) {
                    std::cout << "No se encontro ningun paciente con la cedula " << cedulaBuscada << ".\n";
                } else {
                    std::cout << "\nResultado de busqueda en Tabla Hash:\n";
                    encontrado->mostrarDatosPaciente();
                }
                break;
            }
            case 'E': { // Terminar el turno anticipadamente y guardar las métricas
                std::cout << "\nTerminando turno del Dr. " << medicoActual->getNombreMedico() << " anticipadamente...\n";
                medicoActual->terminarJornada();
                pila.vaciar();
                simulacionActiva = false;
                break;
            }
            case 'O': { // Cambiar medico
                cambiarMedico();
                turnoAvanzo = true;
                break;
            }
        }

        // Solo si la acción consumió turno (A, D, R, T, X), avanzar reloj y envejecer cola
        if (turnoAvanzo && accion != 'O') {
            medicoActual->registrarTurno();

            std::list<Paciente*> perdidosEsteTurno = cola.envejecer();
            for (Paciente* p : perdidosEsteTurno) {
                p->marcarPerdido(medicoActual->getNombreMedico(), turnoGlobal);
                std::cout << "[ALERTA CRITICA] El paciente " << p->getNombre()
                          << " (CC: " << p->getCedula() << ") supero el limite de espera y se ha perdido.\n";
            }
            turnoJornada++;
            turnoGlobal++;

            // Verificar si el médico completó sus 50 turnos de jornada
            if (medicoActual->jornadaCompleta() || turnoJornada > 50) {
                std::cout << "\n========================================\n";
                std::cout << medicoActual->getNombreMedico() << " ha completado sus 50 turnos de jornada.\n";
                std::cout << "========================================\n";
                medicoActual->terminarJornada();
                pila.vaciar();
                char continuar;
                std::cout << "¿Desea iniciar jornada con otro medico? (S/N): ";
                std::cin >> continuar;
                std::cin.ignore(10000, '\n'); // Limpiar buffer
                if (std::toupper(static_cast<unsigned char>(continuar)) == 'S') {
                    cambiarMedico();
                    turnoAvanzo = true;
                } else {
                    simulacionActiva = false;
                }
            }
        }
    }

    mostrarReporteFinal();
}