#include "divers.hpp"



std::string IntToBinary(int n, int nLengh)
{
    std::string sOut("");

    int nBitValue=1;
    int nNecessaryDigits = 1;
    while(nBitValue<n)
    {
        nBitValue*=2;
        nNecessaryDigits++;
    }
    nBitValue/=2;
    nNecessaryDigits--;

    //Remplissage
    if(nLengh!=0)
        for(int i=0 ; nLengh>nNecessaryDigits+i ; i++)
            sOut+='0';

    while(nBitValue>=1)
    {
        if(n-nBitValue >=0)
        {
            n-=nBitValue;
            sOut+='1';
        }
        else
            sOut+='0';

        nBitValue/=2;
    }


    return sOut;

}
