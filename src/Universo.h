#ifndef __UNIVERSO_H__
#define __UNIVERSO_H__

#include <stdlib.h>
#include <stdio.h>
#include <map>
#include "Posicao.h"
#include "Particula.h"

class Universe {
    public:
        void addPosicao(Posicao &po, Particula &pa) { 
            memory[po] = pa;
        }
        Particula* getParticula(void) {
            if(att == memory.end())
                return NULL;
            return (Particula*)(&(att->second));
         }
        Posicao* getPosicao(void) {
            if(att == memory.end())
                return NULL;
            return (Posicao*)(&(att->first));
        }
        void next(void) { ++att; }
        Particula* getPosicao(Posicao &po) { 
	    ::std::map< Posicao, Particula >::iterator it;
            it = memory.find(po);
            if(it == memory.end())
	        return NULL;
            else
                return &(it->second);
	}
        ::std::map< Posicao, Particula >::iterator getBegin(void) { return memory.begin(); } 
        void rewind(void) { att = getBegin(); }
        void mergeWith( ::std::map< Posicao, Particula > &map ) {
            memory.insert( map.begin(), map.end() );
        }
        
    protected:
        ::std::map< Posicao, Particula > memory;
        ::std::map< Posicao, Particula >::iterator att;
};

#endif
