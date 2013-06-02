#ifndef __PARTICULAS_H__
#define __PARTICULAS_H__

#include "Fluido.h"
#include "Membrana.h"
#include "InjectCurrent.h"

#include <string.h>

class Particula {
    public:
       Particula(void):
          c_sodio(0.0),
          c_potacio(0.0),
          c_cloro(0.0),
          c_calcio(0.0),
          eletric_potential(0.0),
          injecao(NULL)
       { 
           bzero(g_sodio, sizeof(long double)*3);
           bzero(g_potacio, sizeof(long double)*3);
           bzero(g_cloro, sizeof(long double)*3);
           bzero(g_calcio, sizeof(long double)*3);
           bzero(paredeCelularSuperficies, sizeof(bool)*6); 
       }

       Particula(long double Na, long double K, long double Cl, long double Ca):
          c_sodio(Na),
          c_potacio(K),
          c_cloro(Cl),
          c_calcio(Ca),
          eletric_potential(0.0),
          injecao(NULL)
       { 
           bzero(g_sodio, sizeof(long double)*3);
           bzero(g_potacio, sizeof(long double)*3);
           bzero(g_cloro, sizeof(long double)*3);
           bzero(g_calcio, sizeof(long double)*3);
           bzero(paredeCelularSuperficies, sizeof(bool)*6); 
       }

       ~Particula() {
           if(injecao) delete injecao;
       }

       long double c_sodio;
       long double c_potacio;
       long double c_cloro;
       long double c_calcio;

       long double eletric_potential;

       long double g_sodio[3];
       long double g_potacio[3];
       long double g_cloro[3];
       long double g_calcio[3];

       Medium *meio;
       InjectCurrent *injecao;
       bool  paredeCelularSuperficies[6]; //index que armazena quais superficies sao paredes celulares.
       ::std::map< unsigned short, Membrana > paredeCelular; 
};

#endif
