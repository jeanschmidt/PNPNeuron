#ifndef __FLUIDO_H__
#define __FLUIDO_H__

struct Fluid {
    Fluid(int v, float dc, float oc):
       valence(v),
       difusity_coeficient(dc),
       osmotic_coeficient(oc)
    { }

    int valence;
    float difusity_coeficient;
    float osmotic_coeficient; 
};

struct Medium {
    Medium(float d):
        difusity(d)
    { }
    float difusity;
};

#endif
