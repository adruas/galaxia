#Ploteo de la distribución de excentricidades con el radio en el tiempo
reset
set term gif animate delay 15

stats "excentricidad.txt"

max=150 
min=0

set grid

set boxwidth 1 absolute
set key fixed right center vertical enhanced autotitle box lt black linewidth 1.000 dashtype solid
set style increment default
set xzeroaxis
set yzeroaxis
set zzeroaxis 

set ytics border in scale 1,0.5 nomirror norotate  autojustify
set ytics  norangelimit #0.00000,0.1
set y2tics border in scale 1,0.5 nomirror norotate  autojustify
set y2tics  norangelimit textcolor rgb "blue"

set xrange [ min : max ] noreverse writeback
set autoscale xfixmin
set autoscale xfixmax
set x2range [ * : * ] noreverse writeback
set yrange [ 0 : 1.5 ] noreverse nowriteback
set y2range [ * : * ] noreverse writeback
set zrange [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback

set ylabel "Excentricidad"
set xlabel "Distancia al centro (kly)"

set output "distribucion_de_excentricidad.gif"

do for [i=0:int(STATS_blocks-1)]{
plot "excentricidad.txt" index i u 1:2 with points pt '' lc "red" t 'Bloque '.(i+1),\
"" index i u 1:2 smooth freq t 'Excentricidad' w points pt 7 lc "red", \
""  index i u 1:2 axes x1y2 smooth cumulative lw 1.5 lc "blue" t 'Frecuencia acumulada'
}


