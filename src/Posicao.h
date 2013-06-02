#ifndef __POSICAO_H__
#define __POSICAO_H__

struct Posicao {
    long x;
    long y;
    long z;

    void updateFastCompare(unsigned long maxDim) {
        fastCompare = (  ( (x*maxDim) + y )  *  maxDim  ) + z;
    }

    Posicao(void): 
       x(0), y(0), z(0), fastCompare(0)
    { }
    Posicao(unsigned long X, unsigned long Y, unsigned long Z):
       x(X), y(Y), z(Z)
    { }
/*   Estes metodos de comparacao sao muito demorados devido ao hiperpipeline....
    bool operator<(const Posicao &other) const {
        if(other.x < this->x)
           return true;
        if(other.x > this->x)
	   return false;
        if(other.y < this->y)
           return true;
        if(other.y > this->y)
           return false;
        if(other.z < this->z)
           return true;
        return false;
    }

    bool operator>(const Posicao &other) const {
        if(other.x > this->x)
            return true;
        if(other.x < this->x)
            return false;
        if(other.y > this->y)
            return true;
        if(other.y < this->y)
            return false;
        if(other.z < this->z)
            return true;
        return false;
    }
*/

    bool operator<(const Posicao &other) const {
        return this->fastCompare < other.fastCompare;
    }

    bool operator>(const Posicao &other) const {
        return this->fastCompare > other.fastCompare;
    }
 
    bool operator==(const Posicao &other) const {
        if(
           other.x == this->x
           && other.y == this->y
           && other.z == this->z
        )
            return true;
        return false;
    }

    bool operator!=(const Posicao &other) const {
        if(
           other.x == this->x
           && other.y == this->y
           && other.z == this->z
        ) 
            return false;
        return true;
    }

    bool operator<=(const Posicao &other) const {
        if(other == (*this))
            return true;
        if(other < (*this))
            return true;
        return false;
    }

    bool operator>=(const Posicao &other) const {
        if(other == (*this))
            return true;
        if(other > (*this))
            return true;
        return false;
    }

    protected:
       long long fastCompare;
};

#endif
