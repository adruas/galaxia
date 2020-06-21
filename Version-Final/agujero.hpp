#pragma once

//CLASE del agujero negro
class cAgujero
{
private:
    //Características del agujero negro (comienza por a de Agujero Negro)
    double aPosicionX, aVelocidadX, aPosicionY;
    double  aVelocidadY, aMasa, aExcentricidad;
public:
    cAgujero(void); //Constructor
    //Métodos para la muestra de los datos de posiciones (BH=blackhole)
    double getPosXBH(void); double getPosYBH(void);
    //Métodos para la muestra de los datos de velocidades
    double getVelXBH(void); double getVelYBH(void);
    //Método para la muestra de la masa
    double masaBH();
    //Métodos para establecer las posiciones, las velocidades y la masa
    void setPosXBH(double pxBH); void setPosYBH(double pyBH);
    void setVelXBH(double vxBH); void setVelYBH(double vyBH);
    void setMasaBH(double mBH);
    ~cAgujero(void); //Destructor
};