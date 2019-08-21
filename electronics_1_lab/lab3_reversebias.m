figure(1);
Vs = 0.7; %voltage setting
R = 1000000; %resistance value
load = Vs*1000000/R;
RBV = [ 0.021 1.64 3.25 4.84 6.51 8.157 9.770]; %reverse bias voltage
RBI = [ 0.002008 0.3775 0.75 1.129 1.501 1.880 2.252 ]; %forward bias current
plot ([0 0.7], [load, 0], 'k-',RBV, RBI, 'b--');
xlabel ('Reverse Bias Voltage, V'); ylabel ('Bias current, uA (microAmps)');
grid on;
legend ('Load line', 'I vs. V');
title('Eric Swanson, EECE 3110 - 803 Reverse Bias Plot','Fontsize', 12)
