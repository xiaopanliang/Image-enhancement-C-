clear;clc;

%read input image
I1=imread('ca_coast.png','PNG');
I1=double(I1);

%read the result after 1x1 dithering
I2=imread('ca_coast_dither1x1','PNG');
I2=double(I2);

%read the result of error diffusion
I3=imread('ca_coast_errdiff','PNG');
I3=double(I3);

%plot the energy of errors in the frequency domain
dither_freq=fftshift(fft2(I2-I1));
errdiff_freq=fftshift(fft2(I3-I1));
figure;imagesc(log10(abs(dither_freq)));title('1x1 dithering');colormap(gray);colorbar;
figure;imagesc(log10(abs(errdiff_freq)));title('error diffusion');colormap(gray);colorbar;