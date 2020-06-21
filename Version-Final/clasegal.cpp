#include "clasegal.hpp"


const double radio_sistsol=1.95e-6; //120AU = 0.00195 ly (radio de la heliopausa)

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
        if (j!=i && (abs(Solar[i].getPosX()-Solar[j].getPosX())<1) 
        && (abs(Solar[i].getPosY()-Solar[j].getPosY())<1)) //Cuadrado de 1000x1000 años luz para evitar
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
        if (j!=i && (abs(Solar[i].getPosX()-Solar[j].getPosX())<1) 
        && (abs(Solar[i].getPosY()-Solar[j].getPosY())<1)) //Cuadrado de 1000x1000 años luz para no hacer 
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


//Método que absorbe a un sistema, añade su masa al BH y lo coloca en la frontera
int cGalaxia::absorber(cSistema (&Solar)[numsist], cAgujero Negro)
{
    int contador=0;
    //Compruebo para todos los sistemas
    for(int i=0; i<numsist; i++)
    {
        //Si cae dentro del agujero negro
        if(Solar[i].enAgujero()==true)
        {
            //Aumento la masa del BH 
            Negro.setMasaBH(Negro.masaBH()+Solar[i].masa());
            //Coloco al sistema en la frontera
            Solar[i].cambioPos();
            contador+=1;
        }
    }
    return contador;
}


//Método del choque inelástico
int cGalaxia::choqueInelastico(cSistema (&Solar)[numsist])
{
    int contadordechoques=0;
    double x1_menos_x2, y1_menos_y2, distancia, vxi, vyi, vxj, vyj;


    //Evaluo para cada sistema
    for(int i=0; i<numsist; i++)
    {
        //Evaluo con cada otro sistema
        for(int j=0; j<numsist; j++)
        {
            if(j!=i){


            //Variables que me facilitan los calculos
            x1_menos_x2=Solar[i].getPosX()-Solar[j].getPosX();
            y1_menos_y2=Solar[i].getPosY()-Solar[j].getPosY();
            distancia=sqrt((x1_menos_x2*x1_menos_x2)+(y1_menos_y2*y1_menos_y2));
            //Si la distancia entre ambos es menor o igual al radio solar
            //cambio sus velocidades según el choque inelástico
            if (distancia<=radio_sistsol)
            {
                //Aumento en 1 el contador de choques
                contadordechoques+=1;

                //Cambio la velocidad de i en x con VelXChoque
                vxi=Solar[i].velXChoque(Solar[i].getVelX()
                ,Solar[i].getPosX(),Solar[j].getVelX(),Solar[j].getPosX());

                Solar[i].setVelX(vxi);


                //Cambio la velocidad de i en y con VelYChoque
                vyi=Solar[i].velYChoque(Solar[i].getVelY()
                ,Solar[i].getPosY(),Solar[j].getVelY(),Solar[j].getPosY());

                Solar[i].setVelY(vyi);


                //Cambio la velocidad de j en x con VelXChoque
                vxj=Solar[j].velXChoque(Solar[j].getVelX()
                ,Solar[j].getPosX(),Solar[i].getVelX(),Solar[i].getPosX());

                Solar[j].setVelX(vxj);


                //Cambio la velocidad de j en y con VelYChoque
                vyj=Solar[j].velYChoque(Solar[j].getVelY()
                ,Solar[j].getPosY(),Solar[i].getVelY(),Solar[i].getPosY());
                
                Solar[j].setVelY(vyj);                
            }//endif distancia<=radiosolar
            
            
            }//endif j!=i
        }//endfor j

    }//endfor i

    return contadordechoques;
}


void cGalaxia::momentos(cSistema (&Solar)[numsist], cAgujero Negro,
double& inercia, double& angular)
{
    //Variables
    int i; double pos, vel, wsuma;
    //Para el momento de inercia y el momento angular L=Iw, w=v/r:
    pos=Negro.getPosXBH()*Negro.getPosXBH()+Negro.getPosYBH()*Negro.getPosYBH();
    vel=Negro.getVelXBH()*Negro.getVelXBH()+Negro.getVelYBH()*Negro.getVelYBH();
    inercia = Negro.masaBH()*pos; //I del BH
    

    //Para prevenir infinitos:
    if (pos!=0) wsuma=vel/sqrt(pos);
    else wsuma=0;

    for(i=0; i<numsist; i++) //I y w del resto de sistemas
    {
        pos=Solar[i].getPosX()*Solar[i].getPosX()+Solar[i].getPosY()*Solar[i].getPosY();
        vel=Solar[i].getVelX()*Solar[i].getVelX()+Solar[i].getVelY()*Solar[i].getVelY();
        inercia+=Solar[i].masa()*pos;
        if (pos!=0) wsuma+=vel/sqrt(pos);
        else wsuma+=0;
    }
    //L=Iw
    angular=inercia*wsuma;
}



double cGalaxia::energia(cSistema(&Solar)[numsist], cAgujero Negro)
{
    double cinetica, potencial, total;
    int i, j;
    //la velocidad al cuadrado del BH
    double velocidad=Negro.getVelXBH()*Negro.getVelXBH()+Negro.getVelYBH()*Negro.getVelYBH();
    //la energia cinética del BH
    cinetica = 0.5*Negro.masaBH()*velocidad;
    //la energía cinética del resto
    for(i=0; i<numsist; i++)
    {
        //la velocidad al cuadrado
        double velocidad=Solar[i].getVelX()*Solar[i].getVelX()+Solar[i].getVelY()*Solar[i].getVelY();
        cinetica += 0.5 * Solar[i].masa()*velocidad;
    }

    //para la energia potencial
    potencial=0;
    for (i =0; i<numsist; i++)
    {
        //AGUJERO NEGRO
        //la distancia al cuadrado
        double distancia=(Negro.getPosXBH()-Solar[i].getPosX())*(Negro.getPosXBH()-Solar[i].getPosX())+
        (Negro.getPosYBH()-Solar[i].getPosY())*(Negro.getPosYBH()-Solar[i].getPosY());
        //la energia potencial del agujero negro
        potencial += -G*Negro.masaBH()*Solar[i].masa()/sqrt(distancia);
        //SISTEMAS SOLARES
        for(j = 0; j<numsist; j++)
        {
            if(j!=i) //Para no contabilizar el sistema consigo mismo
            {
                //la distancia al cuadrado
                distancia=(Solar[i].getPosX()-Solar[j].getPosX())*(Solar[i].getPosX()-Solar[j].getPosX())+
                (Solar[i].getPosY()-Solar[j].getPosY())*(Solar[i].getPosY()-Solar[j].getPosY());
                //la energia potencial
                potencial += -0.5*G*Solar[i].masa()*Solar[j].masa()/sqrt(distancia);
                //El 0.5 está para contar solo una vez la accion de i sobre j y de j sobre i
            }

            
        }
    }

    //LA ENERGIA TOTAL
    total=potencial+cinetica;
    return total;
}



//Destructor
cGalaxia::~cGalaxia(){}