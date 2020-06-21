#Ploteo de la de las posiciones con el tiempo
reset
set term gif enhanced background rgb 'black' size 800,800 font "Helvetica,25" animate delay 3

set size ratio -1

stats "posiciones.txt"

set xr [-150:150]
set yr [-150:150]

set key tc rgb 'white'
set key box opaque top center



set ylabel "Distancia al centro (kly)" tc rgb 'white'
set xlabel "Distancia al centro (kly)" tc rgb 'white'
set border lc rgb 'white'

set output "trayectorias.gif"

set linetype 11 lc "orange"
set linetype 12 lc "white"
set style data points

do for [i=0:int(STATS_blocks-1)]{
plot "posiciones.txt" index i using 1:2:(column(0)>0 ? 0.5 : 1.5):(column(0)>0 ? 12 : 11) pt 7 ps variable lc variable notitle, -1 with points pt '' title "t = ".((i+1)*600)." Myr"
}

