#include "sistemas.hpp"



#define G 6.351695379e-10 //Constante de grav en kly^3/(M_sol*My^2)
//Donde G=(6.67e-11 m^3/(kgs^2))*(1.989e30 kg/M_sol)*
//*(365.25*24*60*60e6 s/My)^2/(c*365.25*24*60*60e3 m/kly)^3

//Función global que me genera un número aleatorio con una semilla diferente
//cada vez que el programa se inicia
std::mt19937& prng() {
    // seed only one PRNG per thread that needs is:
    static thread_local std::mt19937 gen(std::random_device{}());
    return gen;
}



//MÉTODOS DE cSistema


//Constructor de los sistemas solares
cSistema::cSistema(void)
{
    //La galaxia mide unos 200 kly, distibuyo los
    //sistemas en un circulo de radio 100 kly aleatoriamente
    double a=100.0; double pi=3.14159265;

    //Distribución de radios desde a/8 hasta a
    std::uniform_real_distribution<double> distribucion(a/8,a);
    double r = distribucion(prng());
    //Distribución de ángulos de 0 a 2pi (circulo completo)
    std::uniform_real_distribution<double> distribucion_phi(0.0,2.0*pi);
    double phi=distribucion_phi(prng());


    /*Inicialización de parámetros de los sistemas*/
    //La masa de cada sistema solar en masas solares
    sMasa=1.0014;
    //Posiciones aleatorias entre -100 y 100 kly
    sPosicionX= r*cos(phi);
    sPosicionY= r*sin(phi);
    //Velocidades aleatorias entre 0 y cte_vel=0.667 kly/My (=200km/s)
    double rotacion=-1; // 1: clockwise   -1: counter-clockwise 
    double v=sqrt(G*4.154e6/r); //Le doy justo la velocidad orbital, para que trace orbitas círculares
    sVelocidadX=rotacion*v*sin(phi);
    sVelocidadY=-rotacion*v*cos(phi);

    //Control posiciones y velocidades por consola
    //std::cout << "Posicion ("<<sPosicionX<<", "<<sPosicionY<<"), velocidad ("<<sVelocidadX<<", "
    //<<sVelocidadY<<")"<<std::endl;
    //La excentricidad se obtiene a partir de las velocidades
    //ver "Vector de excentricidad"
    }

//Método para saber si el sistema solar ha sido
//absorbido por el agujero negro (SIN USO, aún)
bool cSistema::enAgujero(void)
{
    if (sPosicionX<2e-5 && sPosicionY<2e-5) //R_aguj=17R_sol=2e-5 klyr
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
    cSistema::~cSistema(void){} //Destructor
