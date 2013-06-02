#ifndef __INJECTION_CURRENT_H__
#define __INJECTION_CURRENT_H__

// Esta classe mantem um DDP fixo entre dois pontos.
class InjectCurrent {
    public:
       InjectCurrent(int b, int e, long double v):
          begin(b),
          end(e),
          atual(0),
          valor(v)
       { }
       long double getInjectedCurrent(void) {
           if(end < atual) 
               return 0.0f;
           atual++;
           if(begin > atual)
               return 0.0f;
           return valor;
       }
    protected:
       int begin;
       int end;
       int atual;
       long double valor;
};

#endif
