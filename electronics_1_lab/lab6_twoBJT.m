%Two BJT curve
figure(1);
Vin = [0.000054 0.565 0.934 1.67 2.37 5.00 11.99];
Vout = [0.03212 0.03215 10.05 10.00 9.99 9.99 9.99];
plot(Vin, Vout, 'k-');
xlabel('Vin, V'); ylabel('Vout, V');
grid on;
title('Eric Swanson - EECE 3110 Lab 6 Hysterisis Curve', 'Fontsize', 12)