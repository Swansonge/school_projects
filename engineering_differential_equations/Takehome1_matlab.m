%Eric Swanson
%Sec 208/ 3 PM
%Exam 1 Take Home
x=linspace(0,4);
y1=exp(-2*x);
y2=-1*exp(-2*x);
y3=(exp(-2*x).*cos(2*pi*x));
plot(x,y1,'-g',x,y2,'--b',x,y3,'--r')
title('Eric Swanson- Graph of e^-^2^x, -e^-^2^x, (e^-^2^x)*cos(2*pi*x)')
legend('y = e^-^2^x','y = -e^-^2^x','y = (e^-^2^x)*cos(2*pi*x)')
xlabel('x')
ylabel('y')
t=datestr(now);
text(2,-0.6,t)