set key right bottom
set yrange [0:160]
set xrange [-50:3650]
set xlabel 'Tempo de Execução (s)' 
set ylabel 'Valor da Função Objetivo'
set encoding utf8
set term postscript eps color blacktext 'Helvetica' 22 lw 3
set output 'r_200_20_80_40.eps'
plot "r_200_20_80_40-cplex.dat" using 1:2 with linespoints title 'CPLEX' lt 1 dt 2 pt 4 ps 2 lc '#d32f2f', \
     "r_200_20_80_40-cplex-lb.dat" using 1:2 with linespoints title 'CPLEX + LB' lt 1 dt 2 pt 8 ps 2 lc '#1976d2', \
     "r_200_20_80_40-lb.dat" using 1:2 with linespoints title 'LocB' lt 1 dt 2 pt 6 ps 2 lc '#4caf50', \
     130 title "Melhor solução" lt 1 dt 2 lc '#616161'  