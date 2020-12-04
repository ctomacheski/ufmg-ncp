set key right bottom
set yrange [0:110]
set xrange [-5:50]
set xlabel 'Tempo de Execução (s)' 
set ylabel 'Valor da Função Objetivo'
set encoding utf8
set term postscript eps color blacktext 'Helvetica' 22 lw 3
set output 'r_80_8_60_8.eps'
plot "r_80_8_60_8-cplex.dat" using 1:2 with linespoints title 'CPLEX' lt 1 dt 2 pt 4 ps 2 lc '#d32f2f' , \
     "r_80_8_60_8-cplex-lb.dat" using 1:2 with linespoints title 'CPLEX + LocB' lt 1 dt 2 pt 8 ps 2 lc '#1976d2', \
     "r_80_8_60_8-lb.dat" using 1:2 with linespoints title 'LocB' lt 1 dt 2 pt 6 ps 2 lc '#4caf50', \
     91 title "Solução ótima" lt 1 dt 2 lc '#616161'