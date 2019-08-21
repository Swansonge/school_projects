%Plot for BJT characteristic curve
figure(1);
Vce = [2.0 4.0 6.0 8.0]; 
Ic1 = [7.98 6.84 6.77 7.40]; %Ib = 50 uA
Ic2 = [19.00 12.45 12.45 13.2]; %Ib = 100 uA
Ic3 = [28.66 20.09 21.15 25.08]; %Ib = 150 uA
plot (Vce, Ic1, 'k-', Vce, Ic2, 'b--', Vce, Ic3, 'r-.');
xlabel('Vce, V'); ylabel('Ic, mA');
grid on;
legend ('Ib = 50 uA', 'Ib = 100 uA', 'Ib =150 uA');
title('Eric Swanson - EECE 3110 Lab 6 Ic - Vce Characteristic Curves', 'Fontsize', 12)