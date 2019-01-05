load histogram_1_baboon.txt

B = histogram_1_baboon;
B = B';
[n,p] = size (histogram_1_baboon);
t = 0:255;
% 
% for i = 1:128
%     A(2,i*2) = B(1,i);
% end
% for i = 1:256
%     A(1,i) = i-1;
% end
% idx = A>0;
% plot(A(idx));
%set(gcf, 'PaperPosition', 'default')
grid on
%t = [0,64,192,205.457,255];
%A = [347671.986;239372;22772;0;0];
plot(t,histogram_1_baboon)
%-1692.187;347671.986
title('Lab10 Part1: Histogram of baboon');
