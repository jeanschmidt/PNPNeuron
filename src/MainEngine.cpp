#include "MainEngine.h"
#include "Standarts.h"
#include <math.h>

void MainEngine::run(void) {
    if(!universo)
        return;

    populateEletricPotential();
    populateGradient();
    commitChanges();
}

void MainEngine::populateEletricPotential(void) {
    fprintf(stderr,"*Calculating eletric potencial...\n");
    universo->rewind();
    for(int a=0; a<num_processor; a++)
        createEletricThread(universo);
    waitEletricThreads();
}

void MainEngine::populateGradient(void) {
    fprintf(stderr,"*Calculating ionic flux gradient...\n");
    universo->rewind();
    for(int a=0; a<num_processor; a++)
        createGradientThread(universo);
    waitGradientThreads();
} 

void MainEngine::commitChanges(void) {
    fprintf(stderr,"*Commiting changes..\n");
    universo->rewind();
    while(true) {
        Particula *part = universo->getParticula();
        Posicao *pos = universo->getPosicao();

        if(!part || !pos)
            break;
        
        Posicao p1(*pos);
        Posicao p2(*pos);
        Posicao p3(*pos);

        p1.x++;
        p2.y++;
        p3.z++;

        p1.updateFastCompare(RESOLUTION);
        p2.updateFastCompare(RESOLUTION);
        p3.updateFastCompare(RESOLUTION);

        Particula *part1 = universo->getPosicao(p1);
        Particula *part2 = universo->getPosicao(p2);
        Particula *part3 = universo->getPosicao(p3);

        multiIonicCommit(part, part1, 0);
        multiIonicCommit(part, part2, 1);
        multiIonicCommit(part, part3, 2);

        universo->next();
    }
}

void MainEngine::multiIonicCommit(Particula *part, Particula *part1, unsigned short index) {
    if(!part1 || !part)
        return;

#ifdef __TEST__
    long double anterior, atual;
    
    anterior = part->c_sodio + part1->c_sodio;
#endif

    part->c_sodio += part->g_sodio[index];
    part1->c_sodio -= part->g_sodio[index];

#ifdef __TEST__
    atual = part->c_sodio + part1->c_sodio;

    if(anterior > (atual+0.01) || anterior < (atual-0.01)) {
        fprintf(stderr,"ERROR! %f %f\n", anterior, atual);
        exit(__LINE__);
    }
#endif



#ifdef __TEST__
    anterior = part->c_potacio + part1->c_potacio;
#endif

    part->c_potacio += part->g_potacio[index];
    part1->c_potacio -= part->g_potacio[index];
 
#ifdef __TEST__
    atual = part->c_potacio + part1->c_potacio;

    if(anterior > (atual+0.01) || anterior < (atual-0.01)) {
        fprintf(stderr,"ERROR! %f %f\n", anterior, atual);
        exit(__LINE__);
    }
#endif



#ifdef __TEST__
    anterior = part->c_cloro + part1->c_cloro;
#endif

    part->c_cloro += part->g_cloro[index];
    part1->c_cloro -= part->g_cloro[index];

#ifdef __TEST__
    atual = part->c_cloro + part1->c_cloro;

    if(anterior > (atual+0.01) || anterior < (atual-0.01)) {
        fprintf(stderr,"ERROR! %f %f\n", anterior, atual);
        exit(__LINE__);
    }
#endif
 


#ifdef __TEST__
    anterior = part->c_calcio + part1->c_calcio;
#endif

    part->c_calcio += part->g_calcio[index];
    part1->c_calcio -= part->g_calcio[index];

#ifdef __TEST__
    atual = part->c_calcio + part1->c_calcio;
    
    if(anterior > (atual+0.01) || anterior < (atual-0.01)) {
        fprintf(stderr,"ERROR! %f %f\n", anterior, atual);
        exit(__LINE__);
    }
#endif
}


