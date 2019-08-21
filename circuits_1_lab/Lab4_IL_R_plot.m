%Eric Swanson
%EECE 2070 - 803
%Plot 1, figure 1
% IL vs. R plot

figure (1);
IL = [ 0.0219 0.0182 0.0142 0.0102 0.00804  ];
R = [ 0 85.56 244.29 530 780 ];
plot (R, IL, 'k-');
ylabel ('Load Current, A'); xlabel ('Rheostat Resistance, R');
grid on;
title ('Eric Swanson - EECE 2070 Lab 4 Plot 1', 'Fontsize', 12);
