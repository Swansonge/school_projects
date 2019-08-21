figure(1);
Vs = 0.7; %voltage setting
R = 330; %resistance value
load = 1000*Vs/R;
loadPointsX = [0 0.7];
loadPointsY = [load, 0];
FBV = [ 0.45 0.50 0.55 0.6 0.65 0.7 0.73 ]; %reverse bias voltage
FBI = [ 0.0152 0.109 0.324 0.885 2.542 6.921 12.345 ]; %forward bias current
semilogy (FBV,FBI,'b--');
xlabel ('Forward Bias Voltage, V'); ylabel ('Bias current, mA');
grid on;
legend ('I vs. V');
title('Eric Swanson, EECE 3110 - 803 Forward Bias Semi Log Plot','Fontsize', 12)
