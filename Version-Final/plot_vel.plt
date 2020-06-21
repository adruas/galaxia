#Ploteo de la variación de la velocidad radial con el tiempo
reset
set term gif animate delay 15

stats "velocidades.txt"

set xr [0:150]
set yr [0:7]


G=6.351695379e-10
M=4.154e6
f(x)=148.6271359*sqrt(G*M/x)





set output "velocidades_radiales.gif"



do for [i=0:int(STATS_blocks-1)]{
set multiplot
set origin 0,0
set size 1,1
set xrange [0:125]
set yrange [0:7]
set ylabel "Velocidad radial (km/s)"
set xlabel "Distancia al centro (kly)"
set key box opaque
set tics scale 0.5 front
set xtics 10 offset 0,0
set ytics 1 offset 0,0
plot "velocidades.txt" index i pt 7 ps 0.5 lc "red" title "Bloque ".(i+1), f(x) lw 3 lc "blue" notitle
set origin 0.2,0.3
set size 0.55,0.6
set xrange [5:40]
set yrange [1:3]
unset xlabel
unset ylabel
unset label
unset key
set tics scale 0.5 front
set xtics 5 offset 0,0.5
set ytics 0.5 offset 0.5,0
plot "velocidades.txt" index i pt 7 ps 0.5 lc "red" title "Bloque ".(i+1), f(x) lw 3 lc "blue" notitle
unset multiplot
}

