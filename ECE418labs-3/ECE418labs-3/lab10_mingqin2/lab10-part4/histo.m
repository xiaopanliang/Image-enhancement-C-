load histogram_4_lena.txt

B = histogram_4_lena;
B = B';
[n,p] = size (histogram_4_lena);
t = 0:510;
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
plot(t,histogram_4_lena)
%-1692.187;347671.986
title('Lab10 Part4: Histogram of lena');
