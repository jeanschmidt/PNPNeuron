#include "Standarts.h"
#include "Config.h"

unsigned RESOLUTION=100;

Fluid StandartElements::Sodio(1, 1.1, 0.2);
Fluid StandartElements::Potacio(1, 1.0, 0.2);
Fluid StandartElements::Cloro(-1, 1.1, 0.2);
Fluid StandartElements::Calcio(2, 0.9, 0.2);

Medium StandartElements::citoplasma(0.7);
Medium StandartElements::extracelular(0.9);

long double StandartArea::aresta = 0.0000025;// 0.25*10^(-6) 1/4 diametro médio do neuronio
long double StandartArea::volume = 0.0000025 * 0.0000025 * 0.0000025; //(0.25*10^(-6))^3

long double StandartCoeficients::omega = 8987600000; // 1/(4*pi*E0)
long double StandartCoeficients::charge_coeficient = 96485.336523; //unit charge * avogrado number
long double StandartCoeficients::psi = (8.31451/310.15); 
long double StandartCoeficients::beta = (0.00345);
long double StandartCoeficients::delta_time = 0.001;


void StandartElements::load(void) {
    const ConfigList::Config *c;
    
    c = ConfigHolder::get()->get("Na_valence");
    if(c) Sodio.valence = atoi( c->value );
    c = ConfigHolder::get()->get("Na_difusity_c");
    if(c) Sodio.difusity_coeficient = atof( c->value );
    c = ConfigHolder::get()->get("Na_osmotic_c");
    if(c) Sodio.osmotic_coeficient = atof( c->value );

    c = ConfigHolder::get()->get("K_valence");
    if(c) Potacio.valence = atoi( c->value );
    c = ConfigHolder::get()->get("K_difusity_c");
    if(c) Potacio.difusity_coeficient = atof( c->value );
    c = ConfigHolder::get()->get("K_osmotic_c");
    if(c) Potacio.osmotic_coeficient = atof( c->value );

    c = ConfigHolder::get()->get("Cl_valence");
    if(c) Cloro.valence = atoi( c->value );
    c = ConfigHolder::get()->get("Cl_difusity_c");
    if(c) Cloro.difusity_coeficient = atof( c->value );
    c = ConfigHolder::get()->get("Cl_osmotic_c");
    if(c) Cloro.osmotic_coeficient = atof( c->value );

    c = ConfigHolder::get()->get("Ca_valence");
    if(c) Calcio.valence = atoi( c->value );
    c = ConfigHolder::get()->get("Ca_difusity_c");
    if(c) Calcio.difusity_coeficient = atof( c->value );
    c = ConfigHolder::get()->get("Ca_osmotic_c");
    if(c) Calcio.osmotic_coeficient = atof( c->value );

    c = ConfigHolder::get()->get("citoplasm_difusity");
    if(c) citoplasma.difusity = atof( c->value );
    c = ConfigHolder::get()->get("extracelular_difusity");
    if(c) extracelular.difusity = atof( c->value );
}

void StandartArea::load(void) {
    const ConfigList::Config *c;

    c = ConfigHolder::get()->get("resolution");
    if(c) RESOLUTION = atoi( c->value );

    c = ConfigHolder::get()->get("unit_lenght");
    if(c) {
        aresta = atof( c->value );
        volume = aresta*aresta*aresta;
    }
}

void StandartCoeficients::load(void) {
    const ConfigList::Config *c;
    
    c = ConfigHolder::get()->get("omega");
    if(c) omega = atof( c->value );
    c = ConfigHolder::get()->get("charge_coeficient");
    if(c) charge_coeficient = atof( c->value );
    c = ConfigHolder::get()->get("psi");
    if(c) psi = atof( c->value );
    c = ConfigHolder::get()->get("beta");
    if(c) beta = atof( c->value );
    c = ConfigHolder::get()->get("delta_time");
    if(c) delta_time = atof( c->value );
}

