#include <iostream>
#include <string>
#include <list>
#include <cctype>
#include <stack>

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



    int cantidadPorGravedad(Gravedad g) const {
        int contador = 0;
        for(auto& p: colaPrioridad) {
            if(aNumero(g) == aNumero(p->getGravedad())) {
                contador++;
            }
        }   
        return contador;
    }
};





int main() {

    return 0;
}