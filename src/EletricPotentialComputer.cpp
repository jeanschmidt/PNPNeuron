#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "EletricPotentialComputer.h"
#include "Standarts.h"
#include "Engine.h"
#include "Config.h"

pthread_mutex_t carregar_mutex;
pthread_mutex_t terminar_mutex;

volatile long int semaphore;

EletricPotentialComputer::EletricPotentialComputer(void) {
    universo=NULL;

    radius = 0;
    const ConfigList::Config *c = ConfigHolder::get()->get("e_potential_radius");
    if(c) radius = atoi(c->value);
    if(radius < 1)
        radius = 1;
    
    threadDataSize = 0;
    c = ConfigHolder::get()->get("thread_data_size");
    if(c) threadDataSize = atoi(c->value);
    if(threadDataSize<1)
        threadDataSize=100;
}

void EletricPotentialComputer::run(void) {
    while(true) {
        carregarMais();
        if(pair.empty())
            break;
        calcular();
    }
}

void EletricPotentialComputer::mutexInitialize(void) {
    pthread_mutex_init(&carregar_mutex, NULL);
    pthread_mutex_init(&terminar_mutex, NULL);
    semaphore = 0;
}

void EletricPotentialComputer::carregarMais(void) {
    pair.clear();
    pair.reserve(threadDataSize);

    pthread_mutex_lock(&carregar_mutex);
    for(int a=0; a<threadDataSize; a++) {
       Epair p;
       p.pos = universo->getPosicao();
       p.part = universo->getParticula();

       if(!p.pos || !p.part)
           break;

       pair.push_back(p); 
       universo->next();
    }
    pthread_mutex_unlock(&carregar_mutex);
}

void EletricPotentialComputer::calcular(void) {
    for(int begin=0, tam=pair.size(); begin<tam; begin++) {

        Posicao *pos = pair[begin].pos;
        Particula *part = pair[begin].part;

        if(!pos || !part) 
            break;

        part->eletric_potential = 0;

        for(int ax=-radius; ax<=radius; ax++) {
            for(int ay=-radius; ay<=radius; ay++) {
                for(int az=-radius; az<=radius; az++) {

                    //nao estamos calculando nossa propria carga.
                    if(!ax && !ay && !az)
                        continue;

                    //pega o ponto de referencia
                    Posicao point(*pos);
                    point.x += ax;
                    point.y += ay;
                    point.z += az;
                    point.updateFastCompare(RESOLUTION);
                    Particula *particle = universo->getPosicao(point);
                    if(!particle)
                        continue;

                    //calcula a distancia
                    long double dax = (long double)(ax)*StandartArea::aresta;
                    long double day = (long double)(ay)*StandartArea::aresta;
                    long double daz = (long double)(az)*StandartArea::aresta;
                    long double dist = dax*dax + day*day + daz*daz;
                    dist = sqrt(dist);

                    //calcula a carga disposta no ponto
                    long double charge = particle->c_sodio + particle->c_potacio - particle->c_cloro + (2 * particle->c_calcio); 
                    charge *= StandartCoeficients::charge_coeficient * StandartArea::volume;

                    //calcula o potencial eletrico para o dado ponto
                    part->eletric_potential += (
                        GreatEngine::EPotential(
                            dist,
                            charge,
                            StandartCoeficients::omega
                        )
                    );

                }
            }
        }

        if(part->injecao) {
            part->eletric_potential += part->injecao->getInjectedCurrent();
        }
    }
}

void* runEletricPotentialComputer(void *ptr) {
    EletricPotentialComputer computer;
    computer.setUniverso((Universe*)ptr);
    computer.run();

    pthread_mutex_lock(&terminar_mutex);
    semaphore--;
    pthread_mutex_unlock(&terminar_mutex);
    return NULL;
}

void createEletricThread(Universe *universo) {

    pthread_mutex_lock(&terminar_mutex);
    semaphore++;
    pthread_mutex_unlock(&terminar_mutex);

    pthread_t pt;
    pthread_create(&pt, NULL, runEletricPotentialComputer, universo);
    pthread_detach(pt);
}

void waitEletricThreads(void) {
    while(true) {
        pthread_mutex_lock(&terminar_mutex);
        if(semaphore == 0)
            break;
        pthread_mutex_unlock(&terminar_mutex);
        sleep(1); //0.5 s de espera media é totalmente razoavel
    }
    pthread_mutex_unlock(&terminar_mutex);
}
