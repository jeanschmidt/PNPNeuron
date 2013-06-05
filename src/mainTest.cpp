#include <stdio.h>
#include <stdlib.h>

#include "Engine.h"

#define FAIL fprintf(stderr,"Fail in test %d in file %s\n", __LINE__, __FILE__); exit(__LINE__);

int main(int argc, char **argv) {

    float prova1 = GreatEngine::JPNP(
       5.00,  //c1
       140.0, //c2
       1,     //d1
       1,     //d2
       0.80,  //dm
       1,     //t
       0.0,   //v1
       -0.075,//v2
       5.0,   //pi1
       140.0, //pi2
       260.0, //PI1
       185.0, //PI2
       1,
       1
    );
    if( 
       prova1 > 23.99     //resultado na mao = 23.94
       || prova1 < 23.89
    ) {
        FAIL
    }

    float prova2 = GreatEngine::EPotential(
       4,
       0.4,
       2
    );
//    if(
//       prova2 > 51.25    //resultado na mao = 51.2
//       || prova2 < 51.15 
//    ) {
//        FAIL
//    }

    float prova3 = GreatEngine::OsmoticPressure(
       2,
       0.3,
       3.4,
       273
    );
    if(
       prova3 > 557.02   //resultado na mao = 556.92
       || prova3 < 556.52
    ) {
        FAIL
    }

    return 0;
}
