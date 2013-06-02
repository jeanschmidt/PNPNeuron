#ifndef __STANDARTS_H__
#define __STANDARTS_H__

#include "Fluido.h"

extern unsigned RESOLUTION;

class StandartElements {
    public:
       //Ions celulares basicos
       static Fluid Sodio;
       static Fluid Potacio;
       static Fluid Cloro;
       static Fluid Calcio;

       static Medium citoplasma;
       static Medium extracelular;

       static void load(void);
};

class StandartArea {
    public:
       static long double aresta;
       static long double volume;

       static void load(void);
};

class StandartCoeficients {
    public:
       static long double omega;
       static long double charge_coeficient;
       static long double psi;
       static long double beta;
       static long double delta_time;

       static void load(void);
};

#endif
