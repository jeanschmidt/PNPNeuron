#include "Membrana.h"
#include <stdio.h>

long double Membrana::leak_na = 0.03f;
long double Membrana::leak_k = -0.03f;
long double Membrana::leak_cl = 0.02f;
long double Membrana::leak_ca = 0.00f;

long double Membrana::pump_na = 0.029f * 100;
long double Membrana::pump_k = -0.28f * 100;
long double Membrana::pump_cl = 0.036f * 100;
long double Membrana::pump_ca = -0.1f * 100;

long double Membrana::trigger_na = 0.020;  //65mV
long double Membrana::trigger_k = 0.015;   //75mV
long double Membrana::trigger_cl = 999.999; //nao tem voltage gated
long double Membrana::trigger_ca = 0.010;  //80mV

long double Membrana::conformation_na = 0.5;
long double Membrana::conformation_k = 0.3;
long double Membrana::conformation_cl = 0.1;
long double Membrana::conformation_ca = 0.1;

Membrana::Direction Membrana::Direcao(Posicao &um, Posicao &dois) {
    if(outside == um) {
        if(inside == dois) {
            return FIRST_OUTSIDE;
        } else {
            return NOT_CONNECTED;
        }
    } else {
        if(inside == um) {
            if(outside == dois)
                return FIRST_INSIDE;
            else
                return NOT_CONNECTED;
        } else {
            return NOT_CONNECTED;
        }
    }
}

long double Membrana::permeability(Ion ion) {
    switch(ion) {
        case Na:
           return permeabilityNa();
        case K:
           return permeabilityK();
        case Cl:
           return permeabilityCl();
        case Ca:
           return permeabilityCa();
    }
}

void Membrana::update(long double e_potential) {
    if(e_potential < trigger_na) {
        perm_na += (1.0f-perm_na)*conformation_na;
    } else {
        perm_na *= conformation_na;
    }
    
    if(e_potential < trigger_k) {
        perm_k += (1.0f-perm_k)*conformation_k;
    } else {
        perm_k *= conformation_k;
    }

    if(e_potential < trigger_cl) {
        perm_cl += (1.0f-perm_cl)*conformation_cl;
    } else {
        perm_cl *= conformation_cl;
    }

    if(e_potential < trigger_ca) {
        perm_ca += (1.0f-perm_ca)*conformation_ca;
    } else {
        perm_ca *= conformation_ca;
    }
}


