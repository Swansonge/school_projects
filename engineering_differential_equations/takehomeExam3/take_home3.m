%Eric Swanson
%Sec. 208/3 PM
%Exam 3
a =0;
b = 6*pi;
x1i1 = 0.5;
x2i1 = 0;
x1i2 = 1;
x2i2 = 0;
[t1, z1] = ode45('f', [a, b], [x1i1 ; x2i1]);
[t2, z2] = ode45('f', [a, b], [x1i2 ; x2i2]); 
plot(t1, z1(:,1), 'r')
hold
plot(t2, z2(:,1),':k')
title('Numerical Solutions of x'''' + sinx(x)=0 by Eric Swanson')
xlabel('t')
ylabel('x')
legend('x(0)=0.5', 'x(0)=1')
t=datestr(now);
text(0.5,0.5,t)

