#include "ModelLoad.h"
#include "Standarts.h"
#include <stdio.h>
#include <string.h>

ModelLoad::ModelLoad(void):
   name(NULL),
   scan_path(NULL),
   steep(0),
   universo(NULL)
{ }

ModelLoad::ModelLoad(const char *name_s, const char *scan_path_s, short step_s, Universe *universo_s):
   name(NULL),
   scan_path(NULL),
   steep(step_s),
   universo(universo_s)
{
    setName(name_s);
    setScanPath(scan_path_s);
}

ModelLoad::~ModelLoad() {
    setName(NULL);
    setScanPath(NULL);
}

void ModelLoad::setName(const char *n) {
    if(name) free(name);

    if(!n) {
        name=NULL;
        return;
    }
    
    size_t tam = strlen(n)+1;
    name = (char*)malloc(sizeof(char)*tam);
    memcpy(name, n, tam);
}

void ModelLoad::setScanPath(const char *n) {
   if(scan_path) free(scan_path);

   if(!n) {
       name=NULL;
       return;
   }

   size_t tam = strlen(n)+1;
   scan_path = (char*)malloc(sizeof(char)*tam);
   memcpy(scan_path, n, tam);
}

ModelLoad::LoadStatus ModelLoad::load(void) {
    if(!universo)
        return UNIVERSO_ERR;
    if(!scan_path || !name)
        return NAME_PATH_ERR;
     
    char path[512];
   
    for(int a=0; a<steep; a++) {
        snprintf(path, sizeof(path), "%s/%s%d.bmp", scan_path, name, a);

        SDL_Surface *surf = IMG_Load(path);
        fprintf(stderr,"  ->%s\n", path);
        if(!surf) {
            fprintf(stderr,"Error on opening file! %s\n", path);
            return GENERAL_ERR;
        }
        if(surf->w != RESOLUTION || surf->h != RESOLUTION) {
            fprintf(stderr,"Error on opening file! %s - Image size don't match.\n", path);
            return IMG_DIMEN_ERR;
        }
        if(surf->format->BitsPerPixel != 24) {
            fprintf(stderr,"Error, file %s don't have 24 bpp! (it is %d)\n", path, surf->format->BitsPerPixel);
            return IMG_DIMEN_ERR;
        }

        loadImage(surf, a);
    }

    createCitoplasm();

    return LOAD_OK;
}

void ModelLoad::loadImage(SDL_Surface *surf, int index) {
    char *ptr = (char*)(surf->pixels);

    //monta a particula intracellular
    Particula part;
    part.c_sodio = 0.010;
    part.c_potacio = 0.135;
    part.c_cloro = 0.010;
    part.c_calcio = 0.001;
    part.meio = &StandartElements::citoplasma;

    for(int x=0; x<RESOLUTION; x++) {
        for(int y=0; y<RESOLUTION; y++) {
          
            //Pontos pretos definem o neuronio
            if(
               ptr[0] == 0
               && ptr[1] == 0
               && ptr[2] == 0
            ) {
                Posicao pos;
                
                //apenas para encontrar a posicao de que corte
                //o plano se refere
                switch(index) {
                    case 0:
                       pos.x = y;
                       pos.y = x;
                       pos.z = RESOLUTION/2;
                       break; 
                    case 1:
                       pos.x = RESOLUTION/2;
                       pos.y = x;
                       pos.z = y;
                       break;
                    case 2:
                       pos.x = y;
                       pos.y = x;
                       pos.z = y;
                       break; 
                    case 3:
                       pos.x = RESOLUTION-y;
                       pos.y = x;
                       pos.z = y;
                       break; 
                }
                addExpanded(pos, part);
            }

            ptr+=3;
        }
    }
}

void ModelLoad::addExpanded(Posicao &pos, Particula &part) {
    for(int a=-1; a<2; a++) {
        for(int b=-1; b<2; b++) {
            for(int c=-1; c<2; c++) {
                Posicao p(pos);
                p.x += a;
                p.y += b;
                p.z += c;
                p.updateFastCompare(RESOLUTION);
                universo->addPosicao(p, part);
            }
        }
    }
}

void ModelLoad::createCitoplasm(void) {
     ::std::map< Posicao, Particula > tempMap;
     discoveryEdges( tempMap );
     fprintf(stderr,"*Merging new edges on actual memory\n");
     universo->mergeWith( tempMap );

     fprintf(stderr,"*Now creating citoplasm...\n");
     for(int a=0; a<10; a++) {
         tempMap.clear();
         coverWithSurface( tempMap );
         universo->mergeWith( tempMap );
     }
}

void ModelLoad::coverWithSurface( ::std::map< Posicao, Particula > &tmpmem ) {
     Particula extracelular;
     extracelular.c_sodio = 0.145;
     extracelular.c_potacio = 0.005;
     extracelular.c_cloro = 0.120;
     extracelular.c_calcio = 0.1; 
     extracelular.meio = &(StandartElements::extracelular);

     universo->rewind();
     while(true) {
         Posicao* pos = universo->getPosicao();
        
         if(!pos)
             break;
 
         Posicao p1(*pos);
         Posicao p2(*pos);
         Posicao p3(*pos);
         Posicao p4(*pos);
         Posicao p5(*pos);
         Posicao p6(*pos);

         p1.x++;
         p2.x--;
         p3.y++;
         p4.y--;
         p5.z++;
         p6.z--;

         p1.updateFastCompare(RESOLUTION);
         p2.updateFastCompare(RESOLUTION);
         p3.updateFastCompare(RESOLUTION);
         p4.updateFastCompare(RESOLUTION);
         p5.updateFastCompare(RESOLUTION);
         p6.updateFastCompare(RESOLUTION);

         if( !(universo->getPosicao(p1)) ) 
             tmpmem[p1] = extracelular;

         if( !(universo->getPosicao(p2)) )
             tmpmem[p2] = extracelular;

         if( !(universo->getPosicao(p3)) )
             tmpmem[p3] = extracelular; 

         if( !(universo->getPosicao(p4)) )
             tmpmem[p4] = extracelular;

         if( !(universo->getPosicao(p5)) )
             tmpmem[p5] = extracelular;

         if( !(universo->getPosicao(p6)) )
             tmpmem[p6] = extracelular;

         universo->next();
     }
}

void ModelLoad::discoveryEdges( ::std::map< Posicao, Particula > &tmpmem ) {
     fprintf(stderr,"*Loading and discovering edges..\n");

     Particula extracelular;
     extracelular.c_sodio = 1.1;
     extracelular.c_potacio = 0.9;
     extracelular.c_cloro = 0.6;
     extracelular.c_calcio = 1.0; 
     extracelular.meio = &(StandartElements::extracelular);

     universo->rewind();
     while(true) {
         Posicao* pos = universo->getPosicao();

         if(!pos)
             break;

         Posicao p1(*pos);
         Posicao p2(*pos);
         Posicao p3(*pos);
         Posicao p4(*pos);
         Posicao p5(*pos);
         Posicao p6(*pos);

         p1.x++;
         p2.x--;
         p3.y++;
         p4.y--;
         p5.z++;
         p6.z--;

         p1.updateFastCompare(RESOLUTION);
         p2.updateFastCompare(RESOLUTION);
         p3.updateFastCompare(RESOLUTION);
         p4.updateFastCompare(RESOLUTION);
         p5.updateFastCompare(RESOLUTION);
         p6.updateFastCompare(RESOLUTION);

         Particula *partic;
         partic = universo->getPosicao(*pos);

         if( !(universo->getPosicao(p1)) ) { 
             partic->paredeCelularSuperficies[0] = true;
             partic->paredeCelular[0] = Membrana(p1, *pos);
             tmpmem[p1] = extracelular;
             (tmpmem[p1]).paredeCelularSuperficies[1] = true;
         }

         if( !(universo->getPosicao(p2)) ) {
             partic->paredeCelularSuperficies[1] = true;
             tmpmem[p2] = extracelular;
             (tmpmem[p2]).paredeCelularSuperficies[0] = true;
             (tmpmem[p2]).paredeCelular[0] = Membrana(p2, *pos);
         }

         if( !(universo->getPosicao(p3)) ) {
             partic->paredeCelularSuperficies[2] = true;
             partic->paredeCelular[1] = Membrana(p3, *pos);
             tmpmem[p3] = extracelular; 
             (tmpmem[p3]).paredeCelularSuperficies[3] = true;
         }

         if( !(universo->getPosicao(p4)) ) {
             partic->paredeCelularSuperficies[3] = true;
             tmpmem[p4] = extracelular;
             (tmpmem[p4]).paredeCelularSuperficies[2] = true;
             (tmpmem[p4]).paredeCelular[1] = Membrana(p4, *pos);
         }

         if( !(universo->getPosicao(p5)) ) {
             partic->paredeCelularSuperficies[4] = true;
             partic->paredeCelular[2] = Membrana(p5, *pos);
             tmpmem[p5] = extracelular;
             (tmpmem[p5]).paredeCelularSuperficies[5] = true;
         }

         if( !(universo->getPosicao(p6)) ) {
             partic->paredeCelularSuperficies[5] = true;
             tmpmem[p6] = extracelular;
             (tmpmem[p6]).paredeCelularSuperficies[4] = true;
             (tmpmem[p6]).paredeCelular[2] = Membrana(p6, *pos);
         }

         universo->next();
     }
}




