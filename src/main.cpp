#include <stdio.h>
#include <stdlib.h>
#include "RenderView.h"
#include "Universo.h"
#include "ModelLoad.h"
#include "MainEngine.h"
#include "Standarts.h"
#include "Config.h"

void load(Universe *universo) {
    fprintf(stderr,"*Loading neuron model from file...\n");
    const ConfigList::Config *path = ConfigHolder::get()->get("neuron_path");
    const ConfigList::Config *file = ConfigHolder::get()->get("neuron_file");
    const ConfigList::Config *number = ConfigHolder::get()->get("neuron_number");

    const char *c_path, *c_file;
    int valor = 0;

    if(path) 
        c_path = path->value;
    else
        c_path = "neuronModel";

    if(file)
        c_file = file->value;
    else
        c_file = "angle";    
   
    if(number)
        valor = atoi(number->value); 
    if(valor < 1)
        valor = 4;

    ModelLoad ml(
                 c_file, 
                 c_path, 
                 valor, 
                 universo
              );
    
    ModelLoad::LoadStatus stat = ml.load();
    if( stat != ModelLoad::LOAD_OK ) {
        fprintf(stderr,"\tError number %d on loading file!\n", stat);
        exit(stat);
    }
}

void injectCurrent(Universe *universo, unsigned pontos, float quanto) {
    if(!universo || !pontos)
        return;
 
    universo->rewind();
    while(true) {
        Particula *p = universo->getParticula();
        universo->next();
        if(!p)
            break;
        if(p->meio != &(StandartElements::citoplasma))
            continue;

        p->injecao = new InjectCurrent(2, 5, quanto);

        pontos--;
        if(pontos==0)
            break;
    }
}

int main(int argc, char **argv) {
    fprintf(stderr,"  ---  PNPNeuron - GNU-GPL v3   ---\n");
    fprintf(stderr,"  ---  by Jean Marcel D Schmidt ---\n");
    fprintf(stderr,"  ---       contact@jschmidt.me ---\n\n\n");

    fprintf(stderr," Atual float precision %lu\n", sizeof(long double));
    
    for(int a=1; a<argc; a++) {
        if(strcmp("stdin", argv[a])==0) {
            if( ConfigHolder::get()->parseConfig(stdin) == false ) {
                fprintf(stderr,"    ERROR ON PARSING stdin\n");
                exit(__LINE__);
            }
        } else {
            fprintf(stderr," Parsing config file %s...\n", argv[a]);
            if( ConfigHolder::get()->parseConfig(argv[a]) == false ) {
                fprintf(stderr,"    ERROR ON PARSING %s\n", argv[a]);
                exit(__LINE__);
            }
            fprintf(stderr,"\n");
        }
    }
    fprintf(stderr,"\n");

    StandartElements::load();
    StandartArea::load();
    StandartCoeficients::load();

    Universe *universo = new Universe();
    RenderView r(universo);

    load(universo);
    fprintf(stderr,"*Rendering preview...\n");
    r.renderArea();
    r.renderMembrane();

    injectCurrent(universo, 300, 500.0);

    MainEngine *me = new MainEngine();  
    me->setUniverso(universo);
 
    for(int a=0; a<15; a++) {
        me->run();
        r.renderEPotential(a);
    }

    delete me;
    delete universo;
}
