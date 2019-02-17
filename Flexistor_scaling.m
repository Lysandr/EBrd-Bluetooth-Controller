%% some resistor bois
clc; close all; clear all;
% R1 = 20k, R2 = 100K

r2hist = linspace(30e3,130e3, 1000);
angle = linspace(0,90,1000);
n = 150;
delta = zeros(1,n);
r_1 = zeros(1,n);
V1 = 5;

figure;

for i = 1:n
   r_1(i) = i*1E3; 
   Vjunct = V1*(r2hist ./(r_1(i)+r2hist));
   delta(i) = Vjunct(end)-Vjunct(1);
   plot(angle,Vjunct); hold on;
end
title('Voltage at ADC vs flex deflection')


figure;plot(r_1,delta);title('total change vs r_1')
[val, idx] = max(delta);
disp('Best R1 value: ')
disp(r_1(idx))


ADC_volt_opt = V1*(r2hist ./(45E3+r2hist));
figure; plot(angle, ADC_volt_opt);
title('BEST R1 plot of Voltage at ADC vs flex deflection')

minADCvalue = min(ADC_volt_opt)*(1023/V1)
maxADCvalue = max(ADC_volt_opt)*(1023/V1)

minPWM = 1000;
maxPWM = 1500;

val = linspace(minADCvalue, maxADCvalue, 1000);
outputPWM = minPWM + (val-minADCvalue).*((maxPWM - minPWM)/(maxADCvalue - minADCvalue));
figure; plot(val, outputPWM); title('outputPWM vs inputADCread')
xlabel('Input ADC value (higher the val higher the throttle)');
ylabel('Output PWM microseconds')













