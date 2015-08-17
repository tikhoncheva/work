clc; clear;

addpath(genpath('../Tools/BM3D/'));
%%
pathIn_signal = ['.', filesep, 'signals', filesep];
pathIn_signal_aligned = ['.', filesep, 'signals_aligned', filesep];
sname = '2014_09_10__18_20_38h';

pathOut = ['.', filesep, 'fs', filesep];

%% Aligned signals

fName_tiff = [pathIn_signal_aligned, 'aligned_channel01_', sname, '.tif'];

info = imfinfo(fName_tiff);
T = numel(info);

channel1_d_aligned = [];
for k = 1:T
    img = im2double(imread(fName_tiff, k, 'Info', info));
    channel1_d_aligned = cat(3, channel1_d_aligned, img);    
end


fName_tiff = [pathIn_signal_aligned, 'aligned_channel02_', sname, '.tif'];
info = imfinfo(fName_tiff);
T = numel(info);

channel2_d_aligned = [];
for k = 1:T
    img = im2double(imread(fName_tiff, k, 'Info', info));
    channel2_d_aligned = cat(3, channel2_d_aligned, img);    
end

%% Apply filter on s 

sigma             = 11;      % noise standard deviation given as percentage of the
                             % maximum intensity of the signal, must be in [0,100]
profile           = 'np';    % BM3D parameter profile
                             %  'lc' --> low complexity
                             %  'np' --> normal profile
print_to_screen = 0;         % 0 --> do not print output information (and do 
                             %                              not plot figures)
                             % 1 --> print information and plot figures


fchannel1_d_aligned = [];
for t = 1:T
    frame = channel1_d_aligned(:,:,t);
    [PSNR, frame_est] = BM3D(1, frame, sigma, profile, print_to_screen);
    fchannel1_d_aligned = cat(3, fchannel1_d_aligned, frame_est);
end

