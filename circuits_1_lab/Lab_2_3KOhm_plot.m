%Lab 2 plotting I vs Vr
%Eric Swanson EECE 2070 Lab 2 Plot 2 (3.3 KOhm resistor)
figure (1);
IDMM = [ 0 0.640 1.250 1.863 2.478 3.092 ];
V = [ 0 2 4 6 8 10 ];
plot (V,IDMM,'k-');
xlabel ('Volts, V'); ylabel ('Current, mA');
grid on;
legend ('I from DMM')
title ('Eric Swanson- EECE 2070 Lab 2 I vs Vr (3.3 KOhm Resistor)');