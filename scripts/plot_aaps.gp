set logscale y
set logscale x
set xlabel "Radial Wavenumber (kr)"
set ylabel "c(kr)"
set title "Azimuthal Average Power Spectrum - Real vs AI"
set grid
plot 'real_aaps.dat' using 1:2 with lines title 'real', \
     'ai_aaps.dat'   using 1:2 with lines title 'ai'
