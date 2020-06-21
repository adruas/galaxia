#Ploteo de la variación de la masa radial con el tiempo
reset
set term gif animate delay 15

stats "masas.txt"

#n=40 #numero de intervalos
max=150 
min=0
width=5 #anchura del intervalo
set xtics min,(max-min)/5,max
set boxwidth width*0.9
set style fill solid 0.5
set tics out nomirror

set xr [min:max]
set yr [0:STATS_max_y]


set key box opaque

set ylabel "Masa (M_{sol})"
set xlabel "Distancia al centro (kly)"

set output "densidad_de_masa.gif"

do for [i=0:int(STATS_blocks-1)]{
plot "masas.txt" index i smooth freq w boxes lc "red" title "Bloque ".(i+1)
}

