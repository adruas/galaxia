#pragma once
#include "sistemas.hpp"
#include "agujero.hpp"
#include <cmath>
#define numsist 50 //Número de sistemas solares
#define G 6.351695379e-10 //Constante de grav en kly^3/(M_sol*My^2)
//Donde G=(6.67e-11 m^3/(kgs^2))*(1.989e30 kg/M_sol)*
//*(365.25*24*60*60e6 s/My)^2/(c*365.25*24*60*60e3 m/kly)^3


//CLASE de la galaxia
class cGalaxia {
private:
    cSistema Solar[numsist];
    cAgujero Negro;
    double inercia, angular;

public :
   cGalaxia();
   ~cGalaxia(); //Constructor y destructor
   //Método del algoritmo de verlet
   void verlet(cSistema (&Solar)[numsist], cAgujero Negro,
   double (&ax)[numsist+1], double (&ay)[numsist+1], double h);
   //Método que inicializa las aceleraciones (verlet, pero solo un uso)
   void AceleracionesIniciales(cSistema (&Solar)[numsist], cAgujero Negro,
   double (&ax)[numsist+1], double (&ay)[numsist+1]);
   //Método que absorbe a un sistema, añade su masa al BH y lo coloca en la frontera
   int absorber(cSistema (&Solar)[numsist], cAgujero Negro);
   //Método de los choques inelésticos
   void choqueInelastico(cSistema (&Solar)[numsist]);

   void momentos(cSistema (&Solar)[numsist], cAgujero Negro,
   double& inercia, double& angular);

   double energia(cSistema(&Solar)[numsist], cAgujero Negro);
};