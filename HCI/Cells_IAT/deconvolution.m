% clc; clear;
%%
pathData = ['..', filesep, 'brain_data', filesep];

pathS = [pathData, 's_ns', filesep];
% pathS = [pathData, 'signals', filesep];
path_mouse_no_motion_frames = [pathData, 'mouse_no_motion', filesep];

sname = '2014_09_10__18_20_38h';
% sname = '2014_09_10__18_24_12h';
pathOut = ['.', filesep, 'deconv', filesep];
%%
fName_tiff = [pathS, 's_', sname, '.tif'];
% fName_tiff = [pathS, sname, '__channel01.tif'];
info = imfinfo(fName_tiff);
T = numel(info);

s = [];
for k = 1:T
    img = im2double(imread(fName_tiff, k, 'Info', info));
    s = cat(3, s, img);    
end
    
%% no motion frames (mouse video)
load([path_mouse_no_motion_frames, 'mouse_no_motion_frames_', sname, '.mat']); % load no_motion_movement

s_mouse_no_motion = [];
for i = 1:size(no_motion_movement,2)
    s_mouse_no_motion = [s_mouse_no_motion, no_motion_movement{1,i}'];  
end

%% no motion frames (brain video)
r = 5000/375;
ind_s_no_motion = unique(ceil(s_mouse_no_motion/r));
ind_s_motion = (1:T); ind_s_motion(ind_s_no_motion) = [];

%% Cut frames without motion
s_no_motion = s(:,:, ind_s_no_motion);



%% Fourier spectrum of the noise (of each frame separately, noise = frames without motion)
T_prime = size(s_no_motion,3);
fft_size = 128;
fft_s_no_motion = [];
Fnoise = [];
for t = 1:T_prime
    img = s_no_motion(:,:,t);
    fft_img = fft2(img, fft_size, fft_size);
%     fft_img = fftshift(fft_img);
    I = imag(fft_img);
    R = real(fft_img);
    
    Amp = sqrt(I.^2 + R.^2);
    Fnoise = cat(1,Fnoise, Amp(1:size(Amp)+1:end));
    
    fft_s_no_motion = cat(3, fft_s_no_motion, Amp);
    
end

Fnoise_max = max(Fnoise, [], 1);
Fnoise_median = median(Fnoise, 1);

f = 2*pi*(1:fft_size)/fft_size;
figure;
for i = 1: size(s_no_motion,3)
    plot(f, Fnoise(i,:)), hold on;
end
plot(f, Fnoise_max, 'LineWidth',2, 'Color', 'r', 'LineStyle', '--'); hold off;
title('FFT of the noise + max power')

f = 2*pi*(1:fft_size)/fft_size;
figure;
for i = 1: size(s_no_motion,3)
    plot(f, Fnoise(i,:)), hold on;
end
plot(f, Fnoise_median, 'LineWidth',2, 'Color', 'r', 'LineStyle', '--'); hold off;
title('FFT of the noise + median power')

%% Fourier spectrum of the signal (of each frame separately)
Fs = [];

for i = 1:numel(ind_s_motion)
    img = s(:,:,ind_s_motion(i));
    fft_img = fft2(img, fft_size, fft_size);
    
    I = imag(fft_img);
    R = real(fft_img);
    Amp = sqrt(I.^2 + R.^2);
    Fs = cat(1,Fs, Amp(1:size(Amp)+1:end));
end

Fs_max = max(Fs, [], 1);
Fs_median = median(Fs, 1);

%%
f = 2*pi*(1:fft_size)/fft_size;
figure;
for i = 1:numel(ind_s_motion)
    plot(f, Fs(i,:), 'Marker', '.'), hold on;
end
plot(f, Fs_max, 'LineWidth',2, 'Color', 'r', 'LineStyle', '--', 'Marker', '.'); hold on;
title('FFT of signal frames + max power')
hold off;

f = 2*pi*(1:fft_size)/fft_size;
figure;
for i = 1:numel(ind_s_motion)
    plot(f, Fs(i,:), 'Marker', '.'), hold on;
end
plot(f, Fs_median, 'LineWidth',2, 'Color', 'r', 'LineStyle', '--', 'Marker', '.'); hold on;
title('FFT of signal frames + median power')
hold off;


figure;
plot(f, Fnoise_median, 'LineWidth',2, 'Color', 'r', 'LineStyle', '--'); hold on;
plot(f, Fs_median, 'LineWidth',2, 'Color', 'b', 'LineStyle', '--'); hold off;
title('Median power of the signal (blau) and noise (red)');

%% detect frequencies, where the signal power is smaller than the noise power
% ind_f_to_suppress = Fs_max < Fnoise_max;
ind_f_to_suppress = Fs_median < Fnoise_median;

%% filter s in Fourier space
 
% Gaussian Filter
hsize = 50; % half size of the filter
sigma_sq = 10; 
filter = fspecial('gaussian', hsize*2+1, sigma_sq);
fft_filter = fft2(filter, fft_size, fft_size); % fft of the filter

s_deconv = [];
for t = 1:T
    img = s(:,:,t);
    fft_img = fft2(img, fft_size, fft_size);
    fft_img(ind_f_to_suppress, ind_f_to_suppress) = 0;
%     fft_img = fftshift(fft_img);
    
    fil_fft_img = fft_img; % .* fft_filter; 
        
    iimg = ifft2(fil_fft_img);
    iimg = iimg (1:size(img,1), 1:size(img,2)); % 5) remove padding
    s_deconv = cat(3, s_deconv, iimg);
end

%% normalize the deconvolved signal
mi = min(s_deconv(:));
ma = max(s_deconv(:));

s_deconv = (s_deconv - mi) / (ma - mi);

%% deconvolve using wiener filter
% neighborhood_size = [3, 3];
% NSR = 0;   % the noise-to-signal power ratio of the additive noise.
% 
% s_deconv = [];
% for t = 1:T
%     img = s(:,:,t);
%     uniform_quantization_var = (1/256)^2 / 12;
%     signal_var = var(img(:));
%     SR = uniform_quantization_var / signal_var;
% %     wnr2 = deconvwnr(img, PSF, NSR);
%     
%     J = wiener2(img, neighborhood_size, NSR);
%     s_deconv = cat(3, s_deconv, J);
% end
%%
writeTiffFile(s_deconv, [pathOut, 's_deconv', sname, '.tif']);