#ifndef __RENDER_VIEW_H__

#include "stdio.h"
#include "Universo.h"

class RenderView {
    public:
        RenderView(void):
           res_x(800),
           res_y(800),
           rotate_x(60),
           rotate_y(40),
           universo(NULL)
        { 
            init();
        }
	RenderView(Universe *u):
           res_x(800),
           res_y(800),
           rotate_x(60),
           rotate_y(40),
           universo(u)
        {
            init(); 
        }

        void setUniverso(Universe *u) { universo = u; }
        void renderArea(void);
        void renderMembrane(void);
        void renderEPotential(unsigned frame);

    protected:
        const char* neuralModelTitle;
        const char* neuralMembraneTitle;
        const char* neuralEletricTitle;

        const char* neuralModelFile;
        const char* neuralMembraneFile;
        const char* neuralEletricFile;

        const char* defaultDirectory;

        const char* colorPattern;
        unsigned int res_x;
        unsigned int res_y;
        unsigned int rotate_x;
        unsigned int rotate_y;

        Universe *universo;

        void init(void); 
};

#endif
