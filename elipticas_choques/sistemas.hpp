#pragma once
#include <random>
#include <cmath>
//CLASE de los sistemas solares
class cSistema
{
private:
    //Características de los sistemas (comienza por s de Sistema Solar)
    double sPosicionX, sVelocidadX, sPosicionY;
    double  sVelocidadY, sMasa, sExcentricidad;
public:
    cSistema(void); //Constructor
    bool enAgujero(void); //Método para comprobar si ha sido absorbido
    //Métodos para la muestra de los datos de posiciones
    double getPosX(void); double getPosY(void);
    //Métodos para la muestra de los datos de velocidades
    double getVelX(void); double getVelY(void);
    //Método para la muestra de la masa
    double masa();
    //Métodos para establecer las posiciones y las velocidades
    void setPosX(double px); void setPosY(double py);
    void setVelX(double vx); void setVelY(double vy);
    //Método para cambiar la posición a la frontera, si absorbido por BH
    void cambioPos();
    //Cambios de velocidades tras el choque inelastico
    double velXChoque(double vx1, double px1, double vx2, double px2);
    double velYChoque(double vy1, double py1, double vy2, double py2);
    ~cSistema(void); //Destructor
};