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
