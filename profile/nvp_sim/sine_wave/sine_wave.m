fs = 10000;
f = 1;
t = 0 : 1/fs : 10 - 1/fs;
x = sin(pi*f/10*t);
y(1:1000) = 0;
z = [x y];
plot(z);
z = z.';
save high_res_sine_10K z;
