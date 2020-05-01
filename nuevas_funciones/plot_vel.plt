#Ploteo de la variación de la velocidad radial con el tiempo
reset
set term gif animate delay 5

stats "velocidades.txt"

set xr [0:150]
set yr [0:30]

set grid

G=6.351695379e-10
M=4.154e6
f(x)=148.6271359*sqrt(G*M/x)

set key box opaque

set ylabel "Velocidad radial (km/s)"
set xlabel "Distancia al centro (kly)"

set output "velocidades_radiales.gif"

do for [i=0:int(STATS_blocks-1)]{
plot "velocidades.txt" index i pt 7 ps 0.5 lc "red" title "Bloque ".(i+1), f(x) notitle
}

