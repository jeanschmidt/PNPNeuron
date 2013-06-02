#ifndef __MODEL_LOAD_H__
#define __MODEL_LOAD_H__

#include "Universo.h"
#include <SDL/SDL_image.h> 

class ModelLoad {
    public:
       enum LoadStatus {
           LOAD_OK,
           NAME_PATH_ERR,
           UNIVERSO_ERR,
           IMG_DIMEN_ERR,
           GENERAL_ERR
       };

       ModelLoad(void);
       ModelLoad(const char *name_s, const char *scan_path_s, short step_s, Universe *universo_s);

       ~ModelLoad();

       void setName(const char *n);
       void setSteep(short n) { steep = n; }
       void setScanPath(const char *n);
       void setUniverso(Universe *u) { universo = u; }

       LoadStatus load(void);
    protected:
       char *name;
       char *scan_path;
       short steep;
       Universe *universo;

       void loadImage(SDL_Surface *surf, int index);
       void addExpanded(Posicao &pos, Particula &part);
       void createCitoplasm(void);
       void discoveryEdges( ::std::map< Posicao, Particula > &tmpmem );
       void coverWithSurface( ::std::map< Posicao, Particula > &tmpmem );
};

#endif
