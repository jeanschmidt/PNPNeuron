#ifndef __RENDER_VIEW_H__

#include "stdio.h"
#include "Universo.h"

class RenderView {
    public:
        RenderView(void):
            universo(NULL)
        { }
	RenderView(Universe *u):
            universo(u)
        { }

        void setUniverso(Universe *u) { universo = u; }
        void renderArea(void);
        void renderMembrane(void);
        void renderEPotential(unsigned frame);

    protected:
        Universe *universo; 
};

#endif
