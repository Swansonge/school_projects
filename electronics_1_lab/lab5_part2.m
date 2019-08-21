%Plots for data in part 2 of lab 5, Eric Swanson
figure(1);
V = [ 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1]; %voltage  values Vds
Id1 = [0.51 03.99 5.80 6.30 6.42 6.47 6.50 6.52 6.54 6.55 6.56]; %current for Vgs=1.5
Id2 = [-0.98 9.92 19.79 28.51 35.76 41.40 45.42 47.92 49.52 50.36 51.08]; %current for Vgs = 1.8
Id3 = [-1.18 12.22 25.32 37.92 50.08 54.24 54.13 54.06 54.01 48.22 48.15]; %current for Vgs = 2.1
Id4 = [-1.23 12.84 26.72 40.10 53.10 54.07 53.93 53.83 53.70 53.63 53.55]; %current for Vgs = 2.2
plot(V, Id1, 'k-', V, Id2, 'k*', V, Id3, 'k-.', V, Id4, 'k--');
xlabel('Vds, V'); ylabel('Id, mA');
grid on;
legend('Vgs = 1.5 V','Vgs = 1.8 V', 'Vgs = 2.1 V', 'Vgs = 2.4 V');
title('Eric Swanson - EECE 3110 lab #5 Characteristic Curve of 2N7000', 'FontSize', 10)