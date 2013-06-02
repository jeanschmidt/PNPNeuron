#ifndef __MAIN_ENGINE_H__
#define __MAIN_ENGINE_H__

#include "Universo.h"
#include "Engine.h"
#include "EletricPotentialComputer.h"
#include "GradientComputer.h"
#include "Config.h"

class MainEngine : public GreatEngine {
    public:
       MainEngine(void):
          universo(NULL)
       { 
          EletricPotentialComputer::mutexInitialize();
          GradientComputer::mutexInitialize();

          num_processor = 1;
          const ConfigList::Config *c = ConfigHolder::get()->get("threads");
          if(c) num_processor = atoi(c->value);
          if(num_processor < 1) num_processor = 1;
       }

       void run(void);
       void setUniverso(Universe *u) { 
           universo = u;
       }

    protected:
       Universe *universo;
       unsigned num_processor;

       void populateEletricPotential(void);
       void populateGradient(void);
       void commitChanges(void);

       void multiIonicCommit(Particula *part, Particula *part1, unsigned short index);
};

#endif
