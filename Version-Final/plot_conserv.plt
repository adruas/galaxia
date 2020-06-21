#Ploteo de la conservacion de energia y momentos, tambien de las absorciones

reset
set term pngcairo enhanced

set style line 1 \
    linecolor rgb '#0060ad' \
    linetype 1 linewidth 0.5 \
    pointtype 7 pointsize 0.8

set style fill transparent solid 0.7 noborder




#ENERGIA
set output "conserv_energia.png"
set xlabel "Número de pasos"
set ylabel "Energía/u.e."
plot "energia.txt" with linespoints linestyle 1 notitle



#INERCIA
set output "conserv_inercia.png"
set xlabel "Número de pasos"
set ylabel "Momento de inercia/10^{6}u.m.i."
#Añadir los sgtes datos segun la estadistica del problema:
set yr [1.85 : *]
min_y=1.94579026657884 #media-std_dev
mean_y=1.9627021415608
max_y=1.97961401654276 #media+std_dev

set arrow 1 from 450, min_y-0.05 to 450, min_y lw 0.5
set label 1 'Inicio del estado estacionario' at 450, min_y-0.055 centre

plot 'inercia.txt' u (($1)/1000000) with linespoints linestyle 1 notitle, -1 with points pt '' title "{/Symbol m}=1962702.14 u.m.i., {/Symbol s}=16911.88 u.m.i.",\
     [450 : 1000] min_y with filledcurves y1=mean_y lt 1 lc rgb "#bbbbdd" notitle ,\
     [450 : 1000] max_y with filledcurves y1=mean_y lt 1 lc rgb "#bbddbb" notitle





#MOMENTO ANGULAR
set output "conserv_angular.png"
set xlabel "Número de pasos"
set ylabel "Momento angular/10^{4}u.m.a."
unset label 1
unset arrow 1
set yr [0:*]
#Añadir los sgtes datos segun la estadistica del problema:
min_y=0.542524
mean_y=1.1200
max_y=1.697476
plot 'angular.txt' u (($1)/10000) with linespoints linestyle 1 notitle, -1 with points pt '' title "{/Symbol m}=11200 u.m.a., {/Symbol s}=5774.76 u.m.a.",\
     min_y with filledcurves y1=mean_y lt 1 lc rgb "#bbbbdd" notitle ,\
     max_y with filledcurves y1=mean_y lt 1 lc rgb "#bbddbb" notitle




#ABSORCIONES Y CHOQUES
set output "graf_absorciones.png"
set xlabel "Número de pasos"
set ylabel "Número de absorciones"
plot "absorciones.txt" with linespoints linestyle 1

set output "graf_choques.png"
set xlabel "Número de pasos"
set ylabel "Número de choques acumulados"
plot "choques.txt" with linespoints linestyle 1
