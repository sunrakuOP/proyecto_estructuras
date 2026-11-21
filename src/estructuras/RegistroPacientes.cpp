#include "estructuras/RegistroPacientes.hpp"

int RegistroPacientes::funcionHash(const std::string& clave) const {
    unsigned long long h = 0;
    for(int i=0; i < (int)clave.size(); ++i) {
        h = ((31*h) + static_cast<unsigned char>(clave[i])) % (m);
    }
    return h;

}

int RegistroPacientes::cantidadRegistros() const {
    return cantidad;
}


Paciente* RegistroPacientes::registrar(const Paciente& p) {
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


Paciente* RegistroPacientes::buscar(const std::string& cedula) {
    int indice = funcionHash(cedula);
    for(auto& par: registroPacientes[indice]) {
        if(par.first == cedula) {
            // mismo uso de las direcciones que en metodo registrar
            return &(par.second);
        }
    }
    return nullptr;
}

bool RegistroPacientes::existe(const std::string& cedula) const {
    int indice = funcionHash(cedula);

    for(auto& par: registroPacientes[indice]) {
        if(par.first == cedula) {
            // mismo uso de las direcciones que en metodo registrar
            return true;
        }
    }
    return false;
}


std::list<const Paciente*> RegistroPacientes::todos() const {
    std::list<const Paciente*> reporte;

    for(auto& celda: registroPacientes) {
        for(auto& nodo: celda) {
            reporte.push_back(&(nodo.second));
        }
    }

    return reporte;
}
