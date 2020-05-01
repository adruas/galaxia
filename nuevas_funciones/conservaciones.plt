#Ploteo de la conservacion de energia y momentos, tambien de las absorciones

reset
set term pngcairo enhanced

set style line 1 \
    linecolor rgb '#0060ad' \
    linetype 1 linewidth 0.5 \
    pointtype 7 pointsize 0.8

set output "conserv_energia.png"
set xlabel "Número de pasos"
set ylabel "Energía/u.e."
plot "energia.txt" with linespoints linestyle 1

set output "conserv_inercia.png"
set xlabel "Número de pasos"
set ylabel "Momento de inercia/u.m.i."
plot "inercia.txt" with linespoints linestyle 1

set output "conserv_angular.png"
set xlabel "Número de pasos"
set ylabel "Momento angular/u.m.a."
plot "angular.txt" with linespoints linestyle 1

set output "graf_absorciones.png"
set xlabel "Número de pasos"
set ylabel "Número de absorciones"
plot "absorciones.txt" with linespoints linestyle 1
