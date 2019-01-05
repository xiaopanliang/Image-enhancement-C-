load histgram_2_lenaerrdiff.txt

B = histgram_2_lenaerrdiff;
B = B';
[n,p] = size (histgram_2_lenaerrdiff);
t = 0:10383;
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
grid on;
%t = [0,64,192,205.457,255];
%A = [347671.986;239372;22772;0;0];
plot(t,histgram_2_lenaerrdiff)
%-1692.187;347671.986
title('Lab10 Part2: Histogram of lenaerrdiff');
