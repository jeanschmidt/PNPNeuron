#include "GradientComputer.h"
#include "Standarts.h"
#include "Engine.h"
#include "Config.h"
#include <pthread.h>
#include <unistd.h>
#include <math.h>

pthread_mutex_t g_carregar_mutex;
pthread_mutex_t g_terminar_mutex;

volatile long int g_semaphore;

GradientComputer::GradientComputer(void) {
    universo=NULL;
    
    threadDataSize = 0;
    const ConfigList::Config *c = ConfigHolder::get()->get("thread_data_size");
    if(c) threadDataSize = atoi(c->value);
    if(threadDataSize<1)
        threadDataSize=100;
}

void GradientComputer::run(void) {
    while(true) {
        carregarMais();
        if(pair.empty())
            break;
        calcular();
    }
}

void GradientComputer::mutexInitialize(void) {
    pthread_mutex_init(&g_carregar_mutex, NULL);
    pthread_mutex_init(&g_terminar_mutex, NULL);
    g_semaphore = 0;
}

void GradientComputer::multiIonicGradient(Particula *part, Particula *part1, unsigned short index) {
        if(!part) return;
	if(part1) {
		long double PI1 = part->c_sodio * StandartElements::Sodio.osmotic_coeficient
			+ part->c_potacio * StandartElements::Potacio.osmotic_coeficient
			+ part->c_cloro * StandartElements::Cloro.osmotic_coeficient
			+ part->c_calcio * StandartElements::Calcio.osmotic_coeficient
			;

		long double PI2 = part1->c_sodio * StandartElements::Sodio.osmotic_coeficient
			+ part1->c_potacio * StandartElements::Potacio.osmotic_coeficient
			+ part1->c_cloro * StandartElements::Cloro.osmotic_coeficient
			+ part1->c_calcio * StandartElements::Calcio.osmotic_coeficient
			;

                long double dm_na = 1.0f;
                long double dm_k  = 1.0f; 
                long double dm_cl = 1.0f;
                long double dm_ca = 1.0f;
                  
                if( part->paredeCelularSuperficies[index*2] ) {
                    Membrana &mem = part->paredeCelular[index];
                    dm_na = mem.permeabilityNa();
                    dm_k  = mem.permeabilityK();
                    dm_cl = mem.permeabilityCl();
                    dm_ca = mem.permeabilityCa();
                }

		part->g_sodio[index] = GreatEngine::JPNP(
				part->c_sodio,
				part1->c_sodio,

				part->meio->difusity,
				part1->meio->difusity,
                                dm_na,

                                StandartCoeficients::delta_time,

				part->eletric_potential,
				part1->eletric_potential,

				part->c_sodio * StandartElements::Sodio.osmotic_coeficient,
				part1->c_sodio * StandartElements::Sodio.osmotic_coeficient,

				PI2,
				PI1,

				StandartCoeficients::psi,
				StandartCoeficients::beta
				);

		part->g_potacio[index] = GreatEngine::JPNP(
				part->c_potacio,
				part1->c_potacio,

				part->meio->difusity,
				part1->meio->difusity,
                                dm_k,

                                StandartCoeficients::delta_time,

				part->eletric_potential,
				part1->eletric_potential,

				part->c_potacio * StandartElements::Potacio.osmotic_coeficient,
				part1->c_potacio * StandartElements::Potacio.osmotic_coeficient,

				PI2,
				PI1,

				StandartCoeficients::psi,
				StandartCoeficients::beta
				);

		part->g_cloro[index] = GreatEngine::JPNP(
				part->c_cloro,
				part1->c_cloro,

				part->meio->difusity,
				part1->meio->difusity,
                                dm_cl,

                                StandartCoeficients::delta_time,

				part->eletric_potential,
				part1->eletric_potential,

				part->c_cloro * StandartElements::Cloro.osmotic_coeficient,
				part1->c_cloro * StandartElements::Cloro.osmotic_coeficient,

				PI2,
				PI1,

				StandartCoeficients::psi,
				StandartCoeficients::beta
				);

		part->g_calcio[index] = GreatEngine::JPNP(
				part->c_calcio,
				part1->c_calcio,

				part->meio->difusity,
				part1->meio->difusity,
                                dm_ca,

                                StandartCoeficients::delta_time,

				part->eletric_potential,
				part1->eletric_potential,

				part->c_calcio * StandartElements::Calcio.osmotic_coeficient,
				part1->c_calcio * StandartElements::Calcio.osmotic_coeficient,

				PI2,
				PI1,

				StandartCoeficients::psi,
				StandartCoeficients::beta
				); 

	} else {
		part->g_sodio[index] = 0;
		part->g_potacio[index] = 0;
		part->g_cloro[index] = 0;
		part->g_calcio[index] = 0;
	}
}


void GradientComputer::carregarMais(void) {
    pair.clear();
    pair.reserve(threadDataSize);

    pthread_mutex_lock(&g_carregar_mutex);
    for(int a=0; a<threadDataSize; a++) {
       Epair p;
       p.pos = universo->getPosicao();
       p.part = universo->getParticula();

       if(!p.pos || !p.part)
           break;

       pair.push_back(p); 
       universo->next();
    }
    pthread_mutex_unlock(&g_carregar_mutex);
}

void GradientComputer::calcular(void) {
    for(int begin=0, tam=pair.size(); begin<tam; begin++) {

        Posicao *pos = pair[begin].pos;
        Particula *part = pair[begin].part;

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

        updateMembrane(*part, *pos, *part1, p1, 0);
        updateMembrane(*part, *pos, *part2, p2, 1);
        updateMembrane(*part, *pos, *part3, p3, 2);

        multiIonicGradient(part, part1, 0);
        multiIonicGradient(part, part2, 1);
        multiIonicGradient(part, part3, 2);

        updateGradientMembrane(*part, *pos, *part1, p1, 0);
        updateGradientMembrane(*part, *pos, *part2, p2, 1);
        updateGradientMembrane(*part, *pos, *part3, p3, 2);

#ifdef __TEST__        
        log(part);
#endif
    }
}

void GradientComputer::log(Particula *part) {
    if(rand()%100)
        return;

    FILE *f = fopen("log.txt", "a");
    if(!f)
        return;

    char line[512];
//    snfprintf(stderr,line, 512, 
//       "Na %f K %f Cl %f Ca %f -- %f Volts"
//       " -- %d %d %d %d %d %d --- "
//       "g Na %f-%f-%f g K (%f)-(%f)-(%f)\n", 
//       part->c_sodio,
//       part->c_potacio,
//       part->c_cloro,
//       part->c_calcio,
//       part->eletric_potential,
//       part->paredeCelularSuperficies[0],
//       part->paredeCelularSuperficies[1],
//       part->paredeCelularSuperficies[2],
//       part->paredeCelularSuperficies[3],
//       part->paredeCelularSuperficies[4],
//       part->paredeCelularSuperficies[5],
//       part->g_sodio[0],
//       part->g_sodio[1],
//       part->g_sodio[2],
//       part->g_potacio[0],
//       part->g_potacio[1],
//       part->g_potacio[2]
//       );
    fwrite(line, strlen(line), sizeof(char), f);
    fclose(f);
}

void* runGradientComputer(void *ptr) {
    GradientComputer computer;
    computer.setUniverso((Universe*)ptr);
    computer.run();

    pthread_mutex_lock(&g_terminar_mutex);
    g_semaphore--;
    pthread_mutex_unlock(&g_terminar_mutex);
    return NULL;
}

void createGradientThread(Universe *universo) {

    pthread_mutex_lock(&g_terminar_mutex);
    g_semaphore++;
    pthread_mutex_unlock(&g_terminar_mutex);

    pthread_t pt;
    pthread_create(&pt, NULL, runGradientComputer, universo);
    pthread_detach(pt);
}

void waitGradientThreads(void) {
    while(true) {
        pthread_mutex_lock(&g_terminar_mutex);
        if(g_semaphore == 0)
            break;
        pthread_mutex_unlock(&g_terminar_mutex);
        sleep(0.5); //0.25 s de espera media é totalmente razoavel
    }
    pthread_mutex_unlock(&g_terminar_mutex);
}

void GradientComputer::updateMembrane(Particula &um, Posicao &um_p, Particula &dois, Posicao &dois_p, unsigned short index) {
    //0-2-4
    if(um.paredeCelularSuperficies[index*2]) {
        Membrana &mem = um.paredeCelular[index];
        switch( mem.Direcao(um_p, dois_p) ) {
            case Membrana::NOT_CONNECTED:
               fprintf(stderr,"   *ERRO SEM SENTIDO!\n");
               exit(__LINE__);
            case Membrana::FIRST_OUTSIDE:
               mem.update(um.eletric_potential - dois.eletric_potential );
               break;
            case Membrana::FIRST_INSIDE:
               mem.update(dois.eletric_potential - um.eletric_potential );
               break;
        }
    }
}

void GradientComputer::updateGradientMembrane(Particula &um, Posicao &um_p, Particula &dois, Posicao &dois_p, unsigned short index) {
    if(um.paredeCelularSuperficies[index*2]) {
        Membrana &mem = um.paredeCelular[index];
        switch( mem.Direcao(um_p, dois_p) ) {
            case Membrana::NOT_CONNECTED:
                fprintf(stderr,"     *ERRO SEM SENTIDO 2\n");
                exit(__LINE__);
            case Membrana::FIRST_OUTSIDE:
                um.g_sodio[index]     += mem.pumpNa();
                um.g_potacio[index]   += mem.pumpK();
                um.g_cloro[index]     += mem.pumpCl();
                um.g_calcio[index]    += mem.pumpCa();
                break;
            case Membrana::FIRST_INSIDE:
                um.g_sodio[index]     -= mem.pumpNa();
                um.g_potacio[index]   -= mem.pumpK();
                um.g_cloro[index]     -= mem.pumpCl();
                um.g_calcio[index]    -= mem.pumpCa();
                break;
        }
    }
}


