%Lab 2 plotting I vs Vr
%Eric Swanson EECE 2070 Lab 2 Plot 1 (1k Ohm resistor)
figure (1);
IDMM = [ 0 2.101 4.097 6.108 8.128 10.418 ];
V = [ 0 2 4 6 8 10 ];
plot (V,IDMM,'k-');
xlabel ('Volts, V'); ylabel ('Current, mA');
grid on;
legend ('I from DMM')
title ('Eric Swanson- EECE 2070 Lab 2 I vs Vr (1 KOhm Resistor)');