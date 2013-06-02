#ifndef __GRADIENT_COMPUTER_H__
#define __GRADIENT_COMPUTER_H__

#include <vector>
#include "Universo.h"

class GradientComputer {
    public:
       GradientComputer(void);
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

       void carregarMais(void);
       void updateMembrane(Particula &um, Posicao &um_p, Particula &dois, Posicao &dois_p, unsigned short index);
       void updateGradientMembrane(Particula &um, Posicao &um_p, Particula &dois, Posicao &dois_p, unsigned short index);
       void multiIonicGradient(Particula *part, Particula *part1, unsigned short index);
       void calcular(void);
       void log(Particula *part);
};

void* runGradientComputer(void *ptr);
void createGradientThread(Universe *universo);
void waitGradientThreads(void);

#endif
