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
/* ARCHIVO DE ORGANIZACION

Aca voy a organizar todas las clases para tener un idea y construccion mas rapida, 
conforme vaya escribiendo este archivo voy creando las construcciones de los .hpp y .cpp separados

Iba a hacer la clase persona y que de ahi heredara Paciente y Medico pero no se justifica
*/


// Clase de enums
enum class Gravedad {
    critico,
    severo,
    moderado,
    leve,
    consulta
};


enum class EstadoPaciente {
    espera, 
    cama,
    alta,
    perdido
};


enum class EstadoCama {
    vacia,
    ocupada,
    mantenimiento
};


// forma de mostrar EstadoPaciente y Gravedad
std::string aTexto(EstadoPaciente estado) {
    switch (estado) {
    case EstadoPaciente::espera:     return "espera";
    case EstadoPaciente::cama:       return "cama";
    case EstadoPaciente::alta:       return "alta";
    case EstadoPaciente::perdido:    return "perdido";
    }
return "desconocido"; 
}


int aNumero(Gravedad g) {
    switch (g) {
    case Gravedad::critico:     return 1;
    case Gravedad::severo:      return 2;
    case Gravedad::moderado:    return 3;
    case Gravedad::leve:        return 4;
    case Gravedad::consulta:    return 5;
    }
return -1;
}


// clase paciente en su propio archivo .hpp
class Paciente {
private:
    std::string nombre;
    std::string cedula;
    Gravedad x;
    std::string condicion;
    int turnoLlegada;
    int tiempoEspera;
    int contadorRiesgo;
    EstadoPaciente estadoP;
    std::string nombreMedico;
    int turnoCierre;

    // constantes para la comparacion
    static const int minutosPorTurno = 5;
    static const int limiteRiesgo = 6;
public:
    Paciente(const std::string& n, const std::string& c, Gravedad y, const std::string& co, int tl):
        nombre(n), cedula(c), x(y), condicion(co), turnoLlegada(tl), tiempoEspera(0), contadorRiesgo(0), 
        estadoP(EstadoPaciente::espera), nombreMedico(""), turnoCierre(0) {}


    // getters de los atributos para usar fuera de la funcion
    std::string getNombre() const { return nombre; }
    std::string getCedula() const { return cedula; }
    Gravedad getGravedad() const { return x; }
    std::string getCondicion() const { return condicion; }
    int getTurnoLlegada() const { return turnoLlegada; }
    int getTiempoEspera() const { return tiempoEspera; }
    int getContadorRiesgo() const { return contadorRiesgo; }
    EstadoPaciente getEstadoPaciente() const { return estadoP; }
    std::string getNombreMedico() const { return nombreMedico; }
    int getTurnoCierre() const { return turnoCierre; }


    // validar si la cedula tiene un formato correcto
    bool esCedulaValida() const {
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
    
    // resto de metodos
    void esperarUnTurno() {
        tiempoEspera = tiempoEspera + minutosPorTurno;
        if(x == Gravedad::critico) {
            contadorRiesgo++;
        }
    }

    bool estaPerdido() const {
        if(x != Gravedad::critico) {
            return false;
        }
        if(contadorRiesgo > limiteRiesgo) {
            return true;
        }
        return false;
    }

    void marcarEnCama() {
        estadoP = EstadoPaciente::cama;
    }

    void marcarAlta(const std::string& nomMed, int turno) {
        estadoP = EstadoPaciente::alta;
        nombreMedico = nomMed;
        turnoCierre = turno;
    }

    void marcarPerdido(const std::string& nomMed, int turno) {
        estadoP = EstadoPaciente::perdido;
        nombreMedico = nomMed;
        turnoCierre = turno;
    }

    int turnoDeTratamiento() const { return 5 - static_cast<int>(x); }

    void mostrarDatosPaciente() const {
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
};


// clase Cama en su propio archivo .hpp
class Cama {
private:
    EstadoCama estadoC;
    Paciente* paciente; // inicia vacia
    int turnosRestantes;

public: 
    int numeroCama; // de default mientras leo que tipo debe tener

    Cama* siguiente;
    Cama* anterior;

    Cama(int nc, Cama* s, Cama* a): 
        estadoC(EstadoCama::vacia), paciente(nullptr), turnosRestantes(0), numeroCama(nc), siguiente(s), anterior(a) {}

    // getters de los atributos privados para usar fuera de la clase
    EstadoCama getEstadoCama() const { return estadoC; }
    Paciente* getPaciente() const { return paciente; }
    int getTurnosRestantes() const { return turnosRestantes; }


    bool estaVacia() {
        if(this->estadoC == EstadoCama::vacia) {
            return true;
        }
        return false;
    }

    void cambiarEstadoCama(EstadoCama nuevoEstado) {
        // primero comparar que no sea el mismo estado porque da lo mismo
        if(this->estadoC == nuevoEstado) {
            return;
        } else if(this->paciente != nullptr && nuevoEstado == EstadoCama::mantenimiento) {
            return;
        } else if(this->paciente != nullptr && nuevoEstado == EstadoCama::vacia) {
            return;
        } else if(this->paciente == nullptr && nuevoEstado == EstadoCama::ocupada) {
            return;
        }
        this->estadoC = nuevoEstado;
    }

/*
voy a usar el "this" en vez del atributo (solo para practicar la sintaxis, se ve mas profesional)
cambie algunos metodos de void a bool para que no hayan salidas silenciosas del programa
(antes solo los dejaba en return, ahora avisa)
*/

    bool asignarPaciente(Paciente* pacienteP) {
        if(this->estadoC != EstadoCama::vacia) {
            return false;
        } else if(this->paciente == pacienteP) {
            return false;
        }
        paciente = pacienteP;
        this->estadoC = EstadoCama::ocupada;
        turnosRestantes = pacienteP->turnoDeTratamiento();
        return true;
    }

    bool tratar() {
        if(this->paciente == nullptr || turnosRestantes <= 0) {
            return false;
        }
        turnosRestantes--;
        return true;
    }

    bool tratamientoCompleto() {
        // para que la cama solo dependa de los metodos de la cama, si no compararia con los del paciente y rompe la regla de encapsulacion que enseño la profe Luisa ( de algo me acuerdo )
        if(this->paciente == nullptr || turnosRestantes != 0) {
            return false;
        }
        return true;
    }

    Paciente* liberar() {
        if(this->paciente == nullptr) {
            return nullptr;
        } else if(!tratamientoCompleto()) {
            return nullptr;
        }
        this->turnosRestantes = 0;
        Paciente* copia = this->paciente;
        this->paciente = nullptr;
        this->estadoC = EstadoCama::vacia;
        return copia;
    }
};



// clase Sala en su propio archivo .hpp
class Sala {    
private:
    Cama* cabeza; 
    Cama* cola;
    Cama* camaActual; // donde esta parado el doctor en este momento

    static const int cantidadCamas = 10;

    void agregarCama(int nc) {
        if(nc <= 0 || nc > cantidadCamas) {
            return;
        }

        if(cabeza == nullptr) {
            Cama* nueva = new Cama(nc, nullptr, nullptr);
            cabeza = nueva;
            cola = nueva;
            return;
        }
        Cama* nueva = new Cama(nc, nullptr, cola);
        cola->siguiente = nueva;
        cola = nueva;
    }

public:
    Sala(): cabeza(nullptr), cola(nullptr), camaActual(nullptr) {
        for(int i=1; i < cantidadCamas + 1; ++i) {
            agregarCama(i);
        }
        camaActual = cabeza;
    }
    
    ~Sala() {
        Cama* auxiliar = cabeza;
        while(auxiliar != nullptr) {
            Cama* siguienteCama = auxiliar->siguiente;
            delete auxiliar;
            auxiliar = siguienteCama;
        }
    }

    Cama& getCamaActual() const { return *(camaActual); } // desreferenciando

    bool moverDerecha() {
        if(this->camaActual->siguiente == nullptr) {
            return false;
        }
        camaActual = camaActual->siguiente;
        return true;
    }

    bool moverIzquierda() {
        if(this->camaActual->anterior == nullptr) {
            return false;
        }
        camaActual = camaActual->anterior;
        return true;
    }


    int camasDesocupadas() const {
        Cama* auxiliar = cabeza;
        int cuenta = 0;
        while(auxiliar != nullptr) {
            if(auxiliar->getEstadoCama() == EstadoCama::vacia) {
                cuenta++;
            }
            auxiliar = auxiliar->siguiente;
        }
        return cuenta;
    }


    void mostrar() const {
        // linea 1: las camas, con "=" solo entre ellas
        Cama* auxiliar = cabeza;
        while(auxiliar != nullptr) {
            if(auxiliar->numeroCama < 10) {
                std::cout<<"[ #"<< auxiliar->numeroCama <<":";
            } else {
                std::cout<<"[#"<< auxiliar->numeroCama <<":";
            }

            if(auxiliar->getEstadoCama() == EstadoCama::vacia) {
                std::cout<<"V  ]";
            } else if(auxiliar->getEstadoCama() == EstadoCama::mantenimiento) {
                std::cout<<"X  ]";
            } else if(auxiliar->getPaciente()->getGravedad() == Gravedad::critico) {
                std::cout<<"P1*]";
            } else {
                std::cout<<"P"<< aNumero(auxiliar->getPaciente()->getGravedad()) <<" ]";
            }

            if(auxiliar->siguiente != nullptr) {
                std::cout<<"=";
            }
            auxiliar = auxiliar->siguiente;
        }
        std::cout<<std::endl;

        // linea 2: la M debajo de la cama actual (cada celda mide 9, igual que arriba)
        auxiliar = cabeza;
        while(auxiliar != nullptr) {
            if(auxiliar == camaActual) {
                std::cout<<"    M    ";
            } else {
                std::cout<<"         ";
            }

            if(auxiliar->siguiente != nullptr) {
                std::cout<<" ";
            }
            auxiliar = auxiliar->siguiente;
        }
        std::cout<<std::endl;
    }
    void Sala::reiniciarPosicionM(){
        camaActual = cabeza;
    }
};



class Medico {
private:
    std::string nombreMedico;
    int turnosJornadaActual;
    bool enJornada;

    // iba a hacer yo mismo la implementacion de la lista pero quiero aprender a usar la stl entonces lo hare asi
    std::list<int> historialJornadas;

    static const int maxTurnosJornada = 50;

public:
    Medico(const std::string& n): nombreMedico(n), turnosJornadaActual(0), enJornada(false) {}

    std::string getNombreMedico() const { return nombreMedico; }

    bool iniciarJornada() {
        if(enJornada) {
            return false;
        }
        enJornada = true;
        turnosJornadaActual = 0;
        return true;
    }

    bool registrarTurno() {
        if(this->turnosJornadaActual >= maxTurnosJornada || !enJornada) {
            return false;
        }
        turnosJornadaActual++;
        return true;
    }

    bool jornadaCompleta() const {
        if(turnosJornadaActual == maxTurnosJornada) {
            return true;
        }
        return false;
    }

    bool terminarJornada() {
        if(!enJornada) {
            return false;
        }
        historialJornadas.push_back(turnosJornadaActual);
        enJornada = false;
        return true;
    }

    int jornadasTrabajadas() const {
        return (int)historialJornadas.size();
    }

    int turnoDeJornada(int indice) const {
        int tamanoLista = (int)historialJornadas.size();
        if(indice > tamanoLista - 1 || indice < 0) {
            return -1;
        }

        // usar iteradores para moverse por la lista
        auto it = historialJornadas.begin();
        for(int i=0; i < indice && it != historialJornadas.end(); ++i) {
            it++;
        }   
        return *it;
    }

    void mostrar() const {
        std::cout<<"Nombre medico: "<< nombreMedico <<std::endl;
        std::cout<<"Jornadas: "<< jornadasTrabajadas() <<std::endl;
        int i = 0;
        for(auto it = historialJornadas.begin(); it != historialJornadas.end(); ++it) {
            std::cout<<"Jornada "<< i <<": "<< *it <<std::endl;
            ++i;
        }
    }

};







// hace dos semanas no adelantamos, continuamos con las tablas hash
class RegistroPacientes {
private:
    std::vector<std::list<std::pair<std::string, Paciente>>> registroPacientes;
    int m; // numero de posiciones
    int cantidad; // para hacer O(1) el proceso de contar numero de pacientes

    int funcionHash(const std::string& clave) const { // la clave es la cedula que es un string, hacemos la funcion polinomica
        unsigned long long h = 0;
        for(int i=0; i < (int)clave.size(); ++i) {
            h = ((31*h) + static_cast<unsigned char>(clave[i])) % (m);
        }
        return h;

    }

public: 
    RegistroPacientes(int t = 211): registroPacientes(std::vector<std::list<std::pair<std::string, Paciente>>> (t)), m(t), cantidad(0) {}

    // prohibir la copia al crear y al asignar
    RegistroPacientes(const RegistroPacientes&) = delete;
    RegistroPacientes& operator=(const RegistroPacientes&) = delete;

    // Devolver la cantidad de pacientes en O(1)
    int cantidadRegistros() const {
        return cantidad;  
    }


    Paciente* registrar(const Paciente& p) {
        if(!p.esCedulaValida()) {
            return nullptr;
        }

        std::string cedula = p.getCedula();
        int indice = funcionHash(cedula);
        
        // revisar que no este ya agregado
        for(auto& par: registroPacientes[indice]) {
            if(par.first == cedula) {
                return nullptr;
            }
        }

        registroPacientes[indice].push_back({cedula, p});
        cantidad++;

        /* Que devuelve????

        El puntero al ultimo elemento creado, con registro[indice] nos paramos en la lista,
        el .back() devuelve el ultimo elemento agregado, y el .second es el Paciente

        Con el ampersand & le sacamos la direccion
        */
        return &(registroPacientes[indice].back().second);
    }


    // dos metodos con mismo modelo pero devuelven cosas diferentes
    Paciente* buscar(const std::string& cedula) {
        int indice = funcionHash(cedula);
        for(auto& par: registroPacientes[indice]) {
            if(par.first == cedula) {
                // mismo uso de las direcciones que en metodo registrar
                return &(par.second); 
            }
        }
        return nullptr;
    }



    bool existe(const std::string& cedula) const { 
        int indice = funcionHash(cedula);

        for(auto& par: registroPacientes[indice]) {
            if(par.first == cedula) {
                // mismo uso de las direcciones que en metodo registrar
                return true; 
            }
        }
        return false;
    }


    /* AUDITORIA AL FINAL DEL TURNO

    Hacer dos for (anidados) que al final es casi O(n) por los pocos indicies compartidos (colisiones)

    Metodo que devuelva una lista con la direccion de cada uno de los pacientes atendidos
    */

    std::list<const Paciente*> todos() const { 
        std::list<const Paciente*> reporte; 

        for(auto& celda: registroPacientes) {
            for(auto& nodo: celda) {
                reporte.push_back(&(nodo.second));
            }
        }

        return reporte;
    }
};



class PilaAtendidos {
private:
    /* Por que puntero y no copia ??
    Porque hay una sola copia real de los pacientes, aca solo es para usar el mismo sabiendo donde vive en memoria
    Si algo cambiara, quedaria desactualizado usando copia

    (solo acepta pacientes cuyo estado sea alta)
    */

    std::stack<const Paciente*> pilaAtendidos;
public:
    PilaAtendidos() {}

    bool apilar(const Paciente& p)  {
        if(p.getEstadoPaciente() != EstadoPaciente::alta) {
            return false;
        }
        pilaAtendidos.push(&p); // recordar que agrega el puntero
        return true;   
    }

    bool estaVacia() const {
        return pilaAtendidos.empty();
    }

    void vaciar() {
        while(!pilaAtendidos.empty()){
            pilaAtendidos.pop();
        }   
    }

    int cantidad() const {
        return (int)pilaAtendidos.size();
    }

    // ahora el metodo mas dificil de la pila, uso una copia para no modificar la original
    std::list<const Paciente*> ultimos() const {
        std::list<const Paciente*> listaInvertida;
        std::stack<const Paciente*> copiaPila = pilaAtendidos;

        while(!copiaPila.empty()) {
            listaInvertida.push_back(copiaPila.top());
            copiaPila.pop();
        }

        return listaInvertida;
    }
};


/* COLAS DE PRIORIDAD (TEORIA):

La cola normal solo saca el primer elemento en ella, FIFO
La cola de prioridad saca el elemento con mayor prioridad.

Como ???? 
Se puede hacer de varias formas:

1. primero organizando segun la prioridad al encolar O(n) al agregar y O(1) al desencolar
2. agregar normal (O(1)) y recorrer hasta encontrar O(n)

En mi caso:
COLA SOLO GUARDA Paciente*, entonces ellos tienen su propia prioridad

EL cast de aNumero(Gravedad) ya lo tenemos.

Como la implementamos? Una lista y yo manejo el orden de agregar y sacar



AL AGREGAR RECORREMOS Y COLOCAMOS DONDE VA (Segun prioridad) => O(n)
PERO EL DESENCOLAR Y FRONT ES => O(1)
*/

class ColaPrioridadPacientes {
private:
    std::list<Paciente*> colaPrioridad;
public:
    ColaPrioridadPacientes() {}

    ~ColaPrioridadPacientes() {}

    bool estaVacia() const {
        return colaPrioridad.empty();
    }

    int totalPacientesCola();

    int sumaEsperaPorGravedad(Gravedad g);
    
    bool hayAlertaEnGravedad();
    

    bool encolar(Paciente* p) {
        // chequear que el paciente no sea null
        if(p == nullptr) {
            return false;
        }
        int prioridad = aNumero(p->getGravedad());

        // recorrer en listas se hace con iterador
        for(auto it = colaPrioridad.begin(); it != colaPrioridad.end(); ++it) {
            if(prioridad < aNumero((*it)->getGravedad())) {
                colaPrioridad.insert(it, p);
                return true;
            }
        }
        // si llega al final, pues se hace push_back porque es el de menor priroidad
        colaPrioridad.push_back(p);
        return true;
    }

    Paciente* desencolar() {
        if(estaVacia()) {
            return nullptr;
        }

        Paciente* p = colaPrioridad.front();
        colaPrioridad.pop_front();
        return p;
    }

    // recorrer la lista, envejecer a todos y sacar los perdidos
    std::list<Paciente*> envejecer() {
        std::list<Paciente*> listaPerdidos;

        for(auto it = colaPrioridad.begin(); it != colaPrioridad.end();) {
            (*it)->esperarUnTurno(); // esperar mientras el medico atiende
            if((*it)->estaPerdido()) {
                listaPerdidos.push_back(*it);
                it = colaPrioridad.erase(it);
            } else {
                ++it;
            }
        }

        return listaPerdidos;
    }

    int ColaPrioridadPacientes::totalPacientesCola() const {
        return colaPrioridad.size();
    }

    int cantidadPorGravedad(Gravedad g) const {
        int contador = 0;
        for(auto& p: colaPrioridad) {
            if(aNumero(g) == aNumero(p->getGravedad())) {
                contador++;
            }
        }   
        return contador;
    }
    int ColaPrioridadPacientes::sumaEsperaPorGravedad(Gravedad g) const{
        int suma = 0;
        for (Paciente* p : colaPrioridad){
            if (p->getGravedad() == g){
                suma = suma + p->getTiempoEspera();
            }
        }
        return suma;
    }
    
    bool ColaPrioridadPacientes::hayAlertaEnGravedad() const{
        for (Paciente* p : colaPrioridad){
            if (p->getGravedad() == Gravedad::critico && p->getTiempoEspera()>=5){
                return true;
            }
        }
        return false;
    }
};
// Urgencias.cpp
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

// MAIN
int main() {
    Urgencias hospital;
    hospital.iniciar();
    return 0;
}