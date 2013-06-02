#ifndef __ENGINE_H__
#define __ENGINE_H__

class GreatEngine {
    public:
       //realiza o calculo do Poison-Nernst-Plank =)
       static const long double JPNP(
          long double c1,   //concentracao em 1
          long double c2,   //concentracao em 2
          long double d1,   //coeficiente difusao em 1
          long double d2,   //coeficiente difusao em 2
          long double dm,   //coeficiente de difusao da membrana
          long double t,    //variacao de tempo
          long double v1,   //potencial eletrico em 1
          long double v2,   //potencial eletrico em 2
          long double pi1,  //pressao osmotica especifica em 1
          long double pi2,  //pressao osmotica especifica em 2
          long double PI1,  //pressao osmotica em 1
          long double PI2,  //pressao osmotica em 2
          long double psi,  //coeficiente 
          long double beta  //coeficiente
       ) {
           
#ifdef __TEST__
           long double blah = (d1+d2)*0.5f*dm;
           if(blah > 1.0f) {
               printf("ERRO\n");
               printf(" %f %f %f\n", d1, d2, dm);
               exit(__LINE__);
           }
#endif

           return ( 
                     (d1+d2)*0.5f*dm
                  ) * (
                     ( (pi1+pi2)/(PI1+PI2) ) * psi * (PI1-PI2)
                     + (beta * (v1 - v2) * (c1 + c2) * 0.5)
                  );
       }

       static const long double EPotential( 
          long double r,    //distancia
          long double c,    //concentracao de cargas
          long double omega //todos os coeficientes juntos
       ) {
           return (c * omega)/r;
       }

       static const long double OsmoticPressure(
          long double i, //coeficiente osmotico
          long double c, //concentracao
          long double R, //blah dos gases
          long double T  //temperatura em K
       ) {
           return i * c * R * T;
       } 
};

#endif
