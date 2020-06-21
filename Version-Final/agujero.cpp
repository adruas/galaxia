#include "agujero.hpp"


//MÉTODOS DE cAgujero

//Constructor del agujero negro
cAgujero::cAgujero(void)
{
    /*Valores iniciales para el agujero negro*/
    aMasa=4.154e6; //La masa del agujero negro en masas solares
    aPosicionX=0; aPosicionY=0; //Lo coloco en el centro
    aVelocidadX=0; aVelocidadY=0; //Y sin velocidad
}

//Métodos para la muestra de los datos de posiciones (BH=blackhole)
double cAgujero::getPosXBH(void){return aPosicionX;}
double cAgujero::getPosYBH(void){return aPosicionY;}
//Métodos para la muestra de los datos de velocidades (BH=blackhole)
double cAgujero::getVelXBH(void){return aVelocidadX;}
double cAgujero::getVelYBH(void){return aVelocidadY;}
//Método para la muestra de la masa (BH=blackhole)
double cAgujero::masaBH(){return aMasa;}
//Métodos para establecer las posiciones, las velocidades y la masa
void cAgujero::setPosXBH(double pxBH){aPosicionX=pxBH;}
void cAgujero::setPosYBH(double pyBH){aPosicionY=pyBH;}
void cAgujero::setVelXBH(double vxBH){aVelocidadX=vxBH;}
void cAgujero::setVelYBH(double vyBH){aVelocidadY=vyBH;}
void cAgujero::setMasaBH(double mBH){aMasa=mBH;}
cAgujero::~cAgujero(void){}; //Destructor