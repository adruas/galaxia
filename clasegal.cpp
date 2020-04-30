#include "clasegal.hpp"


//#define numsist 500 //Número de sistemas solares
//#define G 6.351695379e-10 //Constante de grav en kly^3/(M_sol*My^2)
//Donde G=(6.67e-11 m^3/(kgs^2))*(1.989e30 kg/M_sol)*
//*(365.25*24*60*60e6 s/My)^2/(c*365.25*24*60*60e3 m/kly)^3

//Constructor
cGalaxia::cGalaxia(){}


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



//Destructor
cGalaxia::~cGalaxia(){}