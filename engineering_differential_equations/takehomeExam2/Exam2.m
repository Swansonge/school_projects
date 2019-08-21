%Eric Swanson
%section 208/3 PM
%Exam 2 
yi=0; %y(-1)=0
yii=1; %y(-1)=1
a=-1;
b=3;
[x,y] = ode45('f', [a,b],yi); %solving for y(-1)=0
[x1,y1]=ode45('f', [a,b],yii); %solving for y(-1)=1
plot(x,y,'r')
hold
plot(x1,y1,':k')
title('Numerical Solutions of dy/dx = sin(xy^2) by Eric Swanson')
ylabel('y(x)')
xlabel('x')
legend('y(-1)=0','y(-1)=1')
t=datestr(now);
text(0.5,0.5,t)