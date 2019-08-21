function dirfield(f, xmin, xmax, ymin, ymax)
%The function dirfield draws the direction field for the first-order
%differential equation dy/dx = f(x, y). The function f must be defined in a
%separate function file. dirfield will also generate numerical solutions to
%the differential equation dy/dx = f(x, y) passing through user-defined
%data points.
%Inputs:
%The name of the file defining the right-hand side of the differential
%equation
%Minimum value of independent variable x
%Maximum value of independent variable x
%Minimum value of dependent variable y
%Maximum value of dependent variable y
figure
delta_x = (xmax - xmin)/20;
delta_y = (ymax - ymin)/20;
x = xmin:delta_x:xmax;
y = ymin:delta_y:ymax;
[xm, ym] = meshgrid(x, y);
yp = feval(f, xm, ym);
quiver(xm, ym, ones(size(yp)), yp, '.k')
axis([xmin xmax ymin ymax])
xlabel('x')
ylabel('y')
hold on
xy0 = input('Enter initial data point [x0,y0]. Just hit the Enter key with no input to stop the program. ');
while length(xy0)~=0
    [xc1, yc1] = ode45(f, [xy0(1), xmin], xy0(2));
    [xc2, yc2] = ode45(f, [xy0(1), xmax], xy0(2));
    plot(xc1,yc1,'-b',xc2,yc2,'-b')
    axis([xmin xmax ymin ymax])
    xy0 = input('Enter initial data point [x0,y0]. Just hit the Enter key with no input to stop the program. ');
end 
end