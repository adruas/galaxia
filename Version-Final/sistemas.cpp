#include "sistemas.hpp"

const double radio_sistsol=1.95e-6; //120AU = 0.00195 ly (radio de la heliopausa)

#define G 6.351695379e-10 //Constante de grav en kly^3/(M_sol*My^2)
//Donde G=(6.67e-11 m^3/(kgs^2))*(1.989e30 kg/M_sol)*
//*(365.25*24*60*60e6 s/My)^2/(c*365.25*24*60*60e3 m/kly)^3

//Función global que me genera un número aleatorio con una semilla diferente
//cada vez que el programa se inicia
std::mt19937& prng() {
    static thread_local std::mt19937 gen(std::random_device{}());
    return gen;
}

//Variables globales
//La galaxia mide unos 200 kly, distibuyo los
//sistemas en un circulo de radio 100 kly aleatoriamente
double a=100.0; double pi=3.14159265;
double rotacion=-1; // 1: clockwise   -1: counterclockwise 
double masaAguj=4.154e6;

//MÉTODOS DE cSistema


//Constructor de los sistemas solares
cSistema::cSistema(void)
{
    //Distribución de radios desde a/8 hasta a
    std::uniform_real_distribution<double> distribucion(0,a);
    double r = distribucion(prng());
    //Distribución de ángulos de 0 a 2pi (circulo completo)
    std::uniform_real_distribution<double> distribucion_phi(0.0,2.0*pi);
    double phi=distribucion_phi(prng());
    //Distribución de los semiejes mayores (para darle excentricidad)
    //es una pequeña perturbación sobre la distancia r
    semieje_mayor= r + 0.07*distribucion(prng());

    /*Inicialización de parámetros de los sistemas*/

    //La masa de cada sistema solar en masas solares
    sMasa=1.0014;

    //Posiciones aleatorias entre -100 y 100 kly
    sPosicionX= r*cos(phi);
    sPosicionY= r*sin(phi);
    //Semieje menor con la ecuacion de la elipse

    //Velocidades aleatorias entre 0 y cte_vel=0.667 kly/My (=200km/s)
    //Ecuación de vis-viva
    double v=sqrt(G*masaAguj*((2.0/r)-(1.0/semieje_mayor)));
    sVelocidadX=rotacion*v*sin(phi);
    sVelocidadY=-rotacion*v*cos(phi);
}

//Método para saber si el sistema solar ha sido
//absorbido por el agujero negro
bool cSistema::enAgujero(void)
{
    if (sqrt(sPosicionX*sPosicionX+sPosicionY*sPosicionY)<=2e-5) //R_aguj=17R_sol=2e-5 klyr
        return true; //Devuelve verdad si está dentro
    else return false; //Falso si no
}

//Métodos para la muestra de los datos de posiciones
double cSistema::getPosX(void){return sPosicionX;}
double cSistema::getPosY(void){return sPosicionY;}
//Métodos para la muestra de los datos de velocidades
double cSistema::getVelX(void){return sVelocidadX;}
double cSistema::getVelY(void){return sVelocidadY;}
//Método para la muestra de la masa
double cSistema::masa(){return sMasa;}
//Métodos para establecer las posiciones y las velocidades
void cSistema::setPosX(double px){sPosicionX=px;}
void cSistema::setPosY(double py){sPosicionY=py;}
void cSistema::setVelX(double vx){sVelocidadX=vx;}
void cSistema::setVelY(double vy){sVelocidadY=vy;}


double cSistema::semimayoraxis(void){return semieje_mayor;}

double cSistema::ang(void)
{
    //l=mrv
    double r=sqrt(sPosicionX*sPosicionX+sPosicionY*sPosicionY);
    double v=sqrt(sVelocidadX*sVelocidadX+sVelocidadY*sVelocidadY);
    return sMasa*r*v;
}

//Método para cambiar la posición a la frontera, si absorbido por BH
void cSistema::cambioPos()
{
    //Distribución de ángulos de 0 a 2pi (circulo completo)
    std::uniform_real_distribution<double> distribucion_phi2(0.0,2.0*pi);
    double phi2=distribucion_phi2(prng());
    //Posiciones aleatorias en la frontera
    sPosicionX=a*cos(phi2);
    sPosicionY=a*sin(phi2);
    //Velocidad orbital en frontera
    double v=sqrt(G*masaAguj/a);
    sVelocidadX=rotacion*v*sin(phi2);
    sVelocidadY=-rotacion*v*cos(phi2);
}

//Cambios de velocidades tras el choque inelastico
double cSistema::velXChoque(double vx1, double px1, double vx2, double px2)
{
    double auxPx=px1-px2;
    double auxVx=vx1-vx2;
    double vxnuevo=vx1-((auxVx*auxPx)*auxPx/(radio_sistsol*radio_sistsol));
    return vxnuevo;
}
double cSistema::velYChoque(double vy1, double py1, double vy2, double py2)
{
    double auxPy=py1-py2;
    double auxVy=vy1-vy2;
    double vynuevo=vy1-((auxVy*auxPy)*auxPy/(radio_sistsol*radio_sistsol));
    return vynuevo;
}



cSistema::~cSistema(void){} //Destructor