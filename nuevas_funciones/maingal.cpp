/*--------------------------------------------------------------
    SIMULACIÓN GALÁCTICA: programa que simula la evolución
    de una galaxia desde unas condiciones iniciales aleatorias
    haciendo uso del algoritmo de velocidades de Verlet.
    La galaxia está formada por muchos sistemas solares
    y por un agujero negro supermasivo central. 
    El objetivo será ver como giran alrededor de dicho agujero
    negro, alcanzando un estado estacionario.
----------------------------------------------------------------*/

//Para compilar: g++ maingal.cpp sistemas.cpp agujero.cpp clasegal.cpp -o maingal.exe

#include <iostream>
#include <random>
#include <cmath>
#include <fstream>

#include "sistemas.hpp"
#include "agujero.hpp"
#include "clasegal.hpp"

#define numsist 50 //Número de sistemas solares
#define cte_vel 148.6271359 // 1 kly/My= 148627.1359 m/s = 148.6271359 km/s
                            //porque prefiero escribir en mi fichero en km/s

                            

/*FUNCIÓN PRINCIPAL*/

int main (void)
{
    //DECLARACIÓN DE VARIABLES PRINCIPALES
    cSistema sistemas[numsist]; //Todos los sistemas solares
    cAgujero agujero;
    cGalaxia galaxia;
    int i, j, iter, escribo, contador=0, cuentahisto[200], ancho=5;
    double aceleracionx[numsist+1], aceleraciony[numsist+1], momento_inercia, momento_angular;
    double t_fin,t_aux, h;
    int deltar; //El ancho de los histogramas en kly
    std::ofstream pos; //Llamo a mi fichero de posiciones "pos"
    std::ofstream vel; //Llamo a mi fichero de velocidades "vel"
    std::ofstream mas; //Llamo a mi fichero de masas "mas"
    std::ofstream inr; //Fichero de momentos de inercia
    std::ofstream ang; //Fichero de momentos angulares
    std::ofstream eng; //Fichero de energias
    std::ofstream abs; //Fichero de absorción

    //Puesta a punto de los ficheros
    pos.open("posiciones.txt"); //Lo abro en un fichero llamado posiciones.txt
    if (!pos.is_open()) std::cout<<"Error al abrir el fichero posiciones.txt";
    vel.open("velocidades.txt"); //Lo abro en un fichero llamado velocidades.txt
    if (!vel.is_open()) std::cout<<"Error al abrir el fichero velocidades.txt";
    pos << "#PosicionX   PosicionY"<<std::endl; //Almohadilla porque es comentario en gnuplot
    vel << "#Radio    Velocidad"<<std::endl; //Almohadilla porque es comentario en gnuplot
    mas.open("masas.txt"); //Lo abro en un fichero llamado masas.txt
    if (!mas.is_open()) std::cout<<"Error al abrir el fichero masas.txt";
    mas << "#Radio    Masa"<<std::endl; //Almohadilla porque es comentario en gnuplot
    inr.open("inercia.txt"); //Lo abro en un fichero llamado inercia.txt
    if (!inr.is_open()) std::cout<<"Error al abrir el fichero inercia.txt";
    inr << "#Momento de inercia total"<<std::endl; //Almohadilla porque es comentario en gnuplot
    ang.open("angular.txt"); //Lo abro en un fichero llamado angular.txt
    if (!ang.is_open()) std::cout<<"Error al abrir el fichero angular.txt";
    ang << "#Momento angular total"<<std::endl; //Almohadilla porque es comentario en gnuplot
    eng.open("energia.txt"); //Lo abro en un fichero llamado energia.txt
    if (!eng.is_open()) std::cout<<"Error al abrir el fichero energia.txt";
    eng << "#Energia total"<<std::endl; //Almohadilla porque es comentario en gnuplot
    abs.open("absorciones.txt"); //Lo abro en un fichero llamado absorciones.txt
    if (!abs.is_open()) std::cout<<"Error al abrir el fichero absorciones.txt";
    abs<<"#Absorciones\tTiempo\n";

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

        //Absorbe a un sistema solar si se acerca demasiado y cuenta cuantos absorbe
        contador+=galaxia.absorber(sistemas,agujero);

        //Efectua un choque inelástico si dos sistemas se acercan demasiado
        galaxia.choqueInelastico(sistemas);


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
            //Posicion
            pos<<sistemas[i].getPosX() <<"\t"<< sistemas[i].getPosY() << std::endl;
            //Variable auxiliar
            double dist=sistemas[i].getPosX()*sistemas[i].getPosX()+sistemas[i].getPosY()*sistemas[i].getPosY();
            //Velocidad  
            vel<<sqrt(dist)<<"\t"<<sqrt(sistemas[i].getVelX()*sistemas[i].getVelX()+sistemas[i].getVelY()*sistemas[i].getVelY())*cte_vel<<std::endl;
            //Masa
            for(deltar=ancho;deltar<200;deltar+=ancho)//El ancho de los histogramas en kly
            {
                if(sqrt(dist)<deltar && sqrt(dist)>=deltar-ancho) cuentahisto[deltar]+=1;
                //A una anchura menor que deltar, hay cuentahisto masas solares
            }
        }//endfor
        pos<<std::endl<<std::endl;vel<<std::endl<<std::endl;mas<<std::endl<<std::endl;
        //Dos espacios en blanco separan cada iteración en ambos ficheros

        //Cálculo de los momentos de inercia y angular totales de la galaxia
        galaxia.momentos(sistemas, agujero, momento_inercia, momento_angular);
        inr<<momento_inercia<<std::endl;
        ang<<momento_angular<<std::endl;
        //Energia
        eng<<galaxia.energia(sistemas,agujero)<<std::endl;
        //Absorciones (nº frente a tiempo)
        abs<<contador<<std::endl;
        //Densidad de masa (histograma)
        for(deltar=ancho; deltar<200;deltar+=ancho)
        {
            mas<<deltar-0.5*ancho<<"\t"<<cuentahisto[deltar]<<std::endl;
        }
        }//endif
        else
        {
            //Inicializo el contador
            for(i=0;i<200;i++)  cuentahisto[i]=0;
        }

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
    
    //abs<<"\n\n\n#En un tiempo t="<<t_fin<<" se han realizado "<<contador<< "absorciones, así que "
    //"la absorción media en el tiempo es de: "<<std::endl<<contador/int(t_fin)<< "#M_sol/My";


    //Cierro los ficheros
    pos.close();
    vel.close();
    mas.close();
    inr.close();
    ang.close();
    eng.close();
    abs.close();

    return 0;
}