#ifndef __MEMBRANA_H__
#define __MEMBRANA_H__

#include "Posicao.h"

class Membrana {
    public:
       enum Direction {
           NOT_CONNECTED,
           FIRST_OUTSIDE,
           FIRST_INSIDE
       };

       enum Ion {
           Na,
           K,
           Cl,
           Ca
       };

       Membrana(void):
          perm_na(0.0f),
          perm_k(0.0f),
          perm_cl(0.0f),
          perm_ca(0.0f)
       { }
       Membrana(Posicao &out, Posicao &in):
          outside(out),
          inside(in),
          perm_na(0.0f),
          perm_k(0.0f),
          perm_cl(0.0f),
          perm_ca(0.0f)
       { } 
       void setInside(Posicao &p) { inside = p; }
       void setOutside(Posicao &p) { outside = p; }
       bool isOutside(Posicao &p) { return outside == p; }
       bool isInside(Posicao &p) { return inside == p; }
       Direction Direcao(Posicao &um, Posicao &dois);

       long double permeability(Ion ion);

       long double permeabilityNa(void) { return perm_na+leak_na; }
       long double permeabilityK(void) { return perm_k+leak_k; }
       long double permeabilityCl(void) { return perm_cl+leak_cl; }
       long double permeabilityCa(void) { return perm_ca+leak_ca; }

       long double pumpNa(void) { return pump_na; }
       long double pumpK(void) { return pump_k; }
       long double pumpCl(void) { return pump_cl; }
       long double pumpCa(void) { return pump_ca; }

       void update(long double e_potential);

    protected:
       Posicao outside, inside;

       long double perm_na;
       long double perm_k;
       long double perm_cl;
       long double perm_ca;

       static long double leak_na; 
       static long double leak_k;
       static long double leak_cl;
       static long double leak_ca;

       static long double pump_na;
       static long double pump_k;
       static long double pump_cl;
       static long double pump_ca;

       static long double trigger_na;
       static long double trigger_k;
       static long double trigger_cl;
       static long double trigger_ca;

       static long double conformation_na;
       static long double conformation_k;
       static long double conformation_cl;
       static long double conformation_ca;
};

#endif
