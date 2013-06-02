#ifndef __ELETRIC_POTENTIAL_COMPUTER_H__
#define __ELETRIC_POTENTIAL_COMPUTER_H__

#include <vector>
#include "Universo.h"

class EletricPotentialComputer {
    public:
       EletricPotentialComputer(void);
       void run(void);
       void setUniverso(Universe *u) { universo = u; }
       static void mutexInitialize(void);
    protected:
       struct Epair {
           Posicao *pos;
           Particula *part;
       };

       ::std::vector< Epair > pair;
       Universe *universo;
       int threadDataSize;
       int radius;

       void carregarMais(void);
       void calcular(void);
};

void* runEletricPotentialComputer(void *ptr);
void createEletricThread(Universe *universo);
void waitEletricThreads(void);

#endif
