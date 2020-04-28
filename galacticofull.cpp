/*--------------------------------------------------------------
    SIMULACIÓN GALÁCTICA: programa que simula la evolución
    de una galaxia desde unas condiciones iniciales aleatorias
    haciendo uso del algoritmo de velocidades de Verlet.
    La galaxia está formada por muchos sistemas solares
    y por un agujero negro supermasivo central. 
    El objetivo será ver como giran alrededor de dicho agujero
    negro, alcanzando un estado estacionario.
----------------------------------------------------------------*/

#include <iostream>
#include <random>
#include <cmath>
#include <fstream>

#define numsist 10 //Número de sistemas solares
#define G 6.351695379e-10 //Constante de grav en kly^3/(M_sol*My^2)
//Donde G=(6.67e-11 m^3/(kgs^2))*(1.989e30 kg/M_sol)*
//*(365.25*24*60*60e6 s/My)^2/(c*365.25*24*60*60e3 m/kly)^3
#define cte_vel 148.6271359 // 1 kly/My= 148627.1359 m/s = 148.6271359 km/s
                            //porque prefiero escribir en mi fichero en km/s


/*CLASES*/


//-----------------------------


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
    double getPosX(void){return sPosicionX;}; double getPosY(void){return sPosicionY;};
    //Métodos para la muestra de los datos de velocidades
    double getVelX(void){return sVelocidadX;}; double getVelY(void){return sVelocidadY;};
    //Método para la muestra de la masa
    double masa(){return sMasa;};
    //Métodos para establecer las posiciones y las velocidades
    void setPosX(double px){sPosicionX=px;}; void setPosY(double py){sPosicionY=py;};
    void setVelX(double vx){sVelocidadX=vx;}; void setVelY(double vy){sVelocidadY=vy;};
    ~cSistema(void){}; //Destructor
};


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



//-----------------------------


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
    double getPosXBH(void){return aPosicionX;}; double getPosYBH(void){return aPosicionY;};
    //Métodos para la muestra de los datos de velocidades (BH=blackhole)
    double getVelXBH(void){return aVelocidadX;}; double getVelYBH(void){return aVelocidadY;};
    //Método para la muestra de la masa (BH=blackhole)
    double masaBH(){return aMasa;};
    //Métodos para establecer las posiciones, las velocidades y la masa
    void setPosXBH(double pxBH){aPosicionX=pxBH;}; void setPosYBH(double pyBH){aPosicionY=pyBH;};
    void setVelXBH(double vxBH){aVelocidadX=vxBH;}; void setVelYBH(double vyBH){aVelocidadY=vyBH;};
    void setMasaBH(double mBH){aMasa=mBH;};
    ~cAgujero(void){}; //Destructor
};

//MÉTODOS DE cAgujero

//Constructor del agujero negro
cAgujero::cAgujero(void)
{
    /*Valores iniciales para el agujero negro*/
    aMasa=4.154e6; //La masa del agujero negro en masas solares
    aPosicionX=0; aPosicionY=0; //Lo coloco en el centro
    aVelocidadX=0; aVelocidadY=0; //Y sin velocidad
}


//-----------------------------



//CLASE de la galaxia
class cGalaxia {
private:
    cSistema Solar[numsist];
    cAgujero Negro;

public :
   cGalaxia(){};
   ~cGalaxia(){}; //Constructor y destructor
   //Método del algoritmo de verlet
   void verlet(cSistema (&Solar)[numsist], cAgujero Negro,
   double (&ax)[numsist+1], double (&ay)[numsist+1], double h);
   //Método que inicializa las aceleraciones (verlet, pero solo un uso)
   void AceleracionesIniciales(cSistema (&Solar)[numsist], cAgujero Negro,
   double (&ax)[numsist+1], double (&ay)[numsist+1]);
   //HACER FUNCION choques(); //Método de los choques inelésticos
};

//Método de las aceleraciones iniciales
//ACELERACION AGUJERO NEGRO ("elemento 1000")
void cGalaxia::AceleracionesIniciales(cSistema (&Solar)[numsist], cAgujero Negro,
double (&ax)[numsist+1], double (&ay)[numsist+1])
 {
    int i, j;
    double xaux,yaux,denom;
    double xauxBH, yauxBH, denomBH;
    //Aceleración de cada sistema
    ax[numsist]=0; ay[numsist]=0; //Inicialización para el agujero negro
    for(i=0;i<numsist;i++) //Evalúo cada sistema
    {
        ax[i]=0;ay[i]=0; //Inicialización aceleraciones 
        for(j=0;j<numsist;j++){ //Evaluo la interacción con cada otro sistema
        if (j!=i && (abs(Solar[i].getPosX()-Solar[j].getPosX())<20) 
        && (abs(Solar[i].getPosY()-Solar[j].getPosY())<20)) //Cuadrado de 20x20 para evitar
                                                           //calculos inútiles, solo influencia cercana
        {
            //Variables auxiliares
            xaux=Solar[i].getPosX()-Solar[j].getPosX();
            yaux=Solar[i].getPosY()-Solar[j].getPosY();
            denom=pow(((xaux*xaux)+(yaux*yaux)),1.5);

            //Aceleraciones
            ax[i]-=G*Solar[j].masa()*xaux/denom;
            ay[i]-=G*Solar[j].masa()*yaux/denom;
        }}
        //Añado la contribución del agujero negro a cada sistema i
        xaux=Solar[i].getPosX()-Negro.getPosXBH();
        yaux=Solar[i].getPosY()-Negro.getPosYBH();
        denom=pow(((xaux*xaux)+(yaux*yaux)),1.5);
        ax[i]-=G*Negro.masaBH()*xaux/denom;
        ay[i]-=G*Negro.masaBH()*yaux/denom;

        //control aceleraciones por consola
        //std::cout<<"Aceleracion x"<<i<<": "<<ax[i]<<std::endl;
        //std::cout<<"Aceleracion y"<<i<<": "<<ay[i]<<std::endl;

        //Calculo la aceleración del agujero negro
        xauxBH=Negro.getPosXBH()-Solar[i].getPosX();
        yauxBH=Negro.getPosYBH()-Solar[i].getPosY();
        denomBH=pow(((xauxBH*xauxBH)+(yauxBH*yauxBH)),1.5);
        ax[numsist]-=G*Solar[i].masa()*xauxBH/denomBH;
        ay[numsist]-=G*Solar[i].masa()*yauxBH/denomBH;
    }
    //control aceleraciones por consola
    //std::cout<<"Aceleracion xBH: "<<ax[numsist]<<std::endl;
    //std::cout<<"Aceleracion yBH: "<<ay[numsist]<<std::endl;
    return; 
 }


//Método de aplicación del algoritmo de verlet a la galaxia completa
void cGalaxia::verlet(cSistema (&Solar)[numsist], cAgujero Negro,
 double (&ax)[numsist+1], double (&ay)[numsist+1], double h)
{
    //Declaración de variables
    double wx[numsist+1],wy[numsist+1],xaux,yaux,denom, xauxBH, yauxBH, denomBH;
    int i,j;

    //Posiciones, y velocidades auxiliares
    for(i=0; i<numsist; i++)
    {
        //Cambio de r(t) a r(t+h)
        Solar[i].setPosX(Solar[i].getPosX()+(h*Solar[i].getVelX())+((h*h/2.)*ax[i]));
        Solar[i].setPosY(Solar[i].getPosY()+(h*Solar[i].getVelY())+((h*h/2.)*ay[i]));
        //Velocidades auxiliares
        wx[i]=Solar[i].getVelX()+((h/2.)*ax[i]);
        wy[i]=Solar[i].getVelY()+((h/2.)*ay[i]);
    }
    //Para el agujero negro
    Negro.setPosXBH(Negro.getPosXBH()+(h*Negro.getVelXBH())+((h*h/2.)*ax[numsist]));
    Negro.setPosYBH(Negro.getPosYBH()+(h*Negro.getVelYBH())+((h*h/2.)*ay[numsist]));
    wx[numsist]=Negro.getVelXBH()+((h/2.)*ax[numsist]);
    wy[numsist]=Negro.getVelYBH()+((h/2.)*ay[numsist]);
    

    //Aceleraciones
    for(i=0; i<numsist;i++) //Evalúo cada sistema
    {
        ax[i]=0;ay[i]=0; //Inicialización aceleraciones
        for(j=0;j<numsist;j++){ //Evalúo la interacción con los demás sistemas
        if (j!=i && (abs(Solar[i].getPosX()-Solar[j].getPosX())<20) 
        && (abs(Solar[i].getPosY()-Solar[j].getPosY())<20)) //Cuadrado de 20x20 para no hacer 
                                                           //calculos inútiles, solo influencia cercana
        {
            //Variables auxiliares
            xaux=Solar[i].getPosX()-Solar[j].getPosX();
            yaux=Solar[i].getPosY()-Solar[j].getPosY();
            denom=pow(((xaux*xaux)+(yaux*yaux)),1.5);

            //Aceleraciones
            ax[i]-=G*Solar[j].masa()*xaux/denom;
            ay[i]-=G*Solar[j].masa()*yaux/denom;
        }}  
        //Añado la contribución del agujero negro a cada sistema i
        xaux=Solar[i].getPosX()-Negro.getPosXBH();
        yaux=Solar[i].getPosY()-Negro.getPosYBH();
        denom=pow(((xaux*xaux)+(yaux*yaux)),1.5);
        ax[i]-=G*Negro.masaBH()*xaux/denom;
        ay[i]-=G*Negro.masaBH()*yaux/denom;
        //Calculo la aceleración del agujero negro
        xauxBH=Negro.getPosXBH()-Solar[i].getPosX();
        yauxBH=Negro.getPosYBH()-Solar[i].getPosY();
        denomBH=pow(((xauxBH*xauxBH)+(yauxBH*yauxBH)),1.5);
        ax[numsist]-=G*Solar[i].masa()*xauxBH/denomBH;
        ay[numsist]-=G*Solar[i].masa()*yauxBH/denomBH;
    }

    //Velocidades
    for(i=0;i<numsist;i++)
    {
        Solar[i].setVelX(wx[i]+((h/2.)*ax[i]));
        Solar[i].setVelY(wy[i]+((h/2.)*ay[i]));
    }
    //Para el BH
    Negro.setVelXBH(wx[numsist]+((h/2.)*ax[numsist]));
    Negro.setVelYBH(wy[numsist]+((h/2.)*ay[numsist]));
    
    return;
}


//=====================================================





//=====================================================





/*FUNCIÓN PRINCIPAL*/

int main (void)
{
    //DECLARACIÓN DE VARIABLES PRINCIPALES
    cSistema sistemas[numsist]; //Todos los sistemas solares
    cAgujero agujero;
    cGalaxia galaxia;
    int i, j, iter, escribo;
    double aceleracionx[numsist+1], aceleraciony[numsist+1];
    double t_fin,t_aux, h;
    std::ofstream pos; //Llamo a mi fichero de posiciones "pos"
    std::ofstream vel; //Llamo a mi fichero de velocidades "vel"

    //Puesta a punto de los ficheros
    pos.open("posiciones.txt"); //Lo abro en un fichero llamado posiciones.txt
    if (!pos.is_open()) std::cout<<"Error al abrir el fichero posiciones.txt";
    vel.open("velocidades.txt"); //Lo abro en un fichero llamado velocidades.txt
    if (!vel.is_open()) std::cout<<"Error al abrir el fichero velocidades.txt";
    pos << "#PosicionX   PosicionY"<<std::endl; //Almohadilla porque es comentario en gnuplot
    vel << "#Radio    Velocidad"<<std::endl; //Almohadilla porque es comentario en gnuplot

    //Lectura de datos temporales por pantalla
    /*std::cout<<"Introduzca el paso infinitesimal temporal: ";
    std::cin>>h;std::cout<<std::endl;
    std::cout<<"Introduzca el tiempo de evolucion galactica: ";
    std::cin>>t_fin;std::cout<<std::endl;
    std::cout<<"Escribir datos en ficheros cada cuantas iteraciones: ";
    std::cin>>escribo;std::cout<<std::endl;*/

    //Por si me cansa la escritura en la consola:
    //Año galactico ~250My
    h=0.1;t_fin=2e5;
    escribo=(int) t_fin/(h*1e3);

    //Implementación del algoritmo a la función principal, INICIALIZACIÓN
    galaxia.AceleracionesIniciales(sistemas,agujero,aceleracionx,aceleraciony);

    //Para simplificar las cosas, uso numeros enteros
    //Porque me es útil para saber el porcentaje de ejecución
    //y tambien el numero de escrituras en archivo (va con division entera)
    int hmod=lround(h/h);
    int t_auxmod=lround(t_aux/h); 
    int t_finmod=lround(t_fin/h);
    int orden=floor(log10(t_fin));

    for(t_auxmod=hmod; t_auxmod<=t_finmod+hmod; t_auxmod=t_auxmod+hmod) //de h a t_fin con pasos de h
    {
        galaxia.verlet(sistemas,agujero,aceleracionx,aceleraciony,h); //ALGORITMO

        //ESCRITURA
        //El numero de iteraciones del algoritmo será:
        iter=t_auxmod/hmod;
        //Escribimos cada determinado número de iteraciones
        if((iter%escribo)==0){
        //Escritura en fichero pos y vel del agujero negro
        pos << agujero.getPosXBH() << "\t" << agujero.getPosYBH() << std::endl;
            vel << sqrt(agujero.getPosXBH()*agujero.getPosXBH()+agujero.getPosYBH()*agujero.getPosYBH())
        << "\t" <<sqrt(agujero.getVelXBH()*agujero.getVelXBH()+agujero.getVelYBH()*agujero.getVelYBH())*cte_vel<<std::endl;
        //Escritura de los sistemas solares
        for(i=0;i<numsist;i++){
        pos<<sistemas[i].getPosX() <<"\t"<< sistemas[i].getPosY() << std::endl;  
            vel<<sqrt(sistemas[i].getPosX()*sistemas[i].getPosX()+sistemas[i].getPosY()*sistemas[i].getPosY())
        <<"\t"<<sqrt(sistemas[i].getVelX()*sistemas[i].getVelX()+sistemas[i].getVelY()*sistemas[i].getVelY())*cte_vel<<std::endl;
        }//endfor
        pos<<std::endl<<std::endl;vel<<std::endl<<std::endl;//Dos espacios en blanco separan cada iteración en ambos ficheros
        }//endif


        //PORCENTAJE DE EJECUCIÓN
        int pt=10; //Paso porcentual que se muestra en pantalla
        //Exponentes
        int grande=1+orden;
        int exp1=2+grande; int exp2=1+grande; int exp3=grande;
        //Porcentaje de ejecución, cada 10%, fallos por ser numeros inexactos y redondeo de int
        unsigned long int porcentaje=100*t_aux/t_fin;
        if(porcentaje%pt==0){ //Detección paso a paso no muy precisa
        porcentaje = pow(pt,exp1)*t_auxmod/t_finmod; //Para ser más preciso en mis cálculos (division de enteros redondea)
        if(porcentaje%(int)pow(pt,exp2)==0) std::cout << "Ejecutado al "<<porcentaje/pow(pt,exp3)<<"%"<<std::endl;
        //^Detección precisa
        }//endif
    }
    
    //Cierro los ficheros
    pos.close();
    vel.close();

    return 0;
}