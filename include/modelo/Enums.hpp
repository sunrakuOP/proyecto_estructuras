#ifndef ENUMS_HPP
#define ENUMS_HPP
#include <string>

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
std::string aTexto(EstadoPaciente estado);
int aNumero(Gravedad g);
std::string aTexto(Gravedad gravedad); // Para pasar de numero a texto la gravedad
#endif
