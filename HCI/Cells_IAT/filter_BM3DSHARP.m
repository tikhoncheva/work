clc; clear;


addpath(genpath('../Tools/VBM3D/'));

%%
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
alpha_sharp  = 1.1;          % Sharpening parameter (default: 1.5):
                             % (1,inf) -> sharpen
                             % 1      -> no sharpening
                             % (0,1)   -> de-sharpen                             
print_to_screen = 0;         % 0 --> do not print output information (and do 
                             %                              not plot figures)
                             % 1 --> print information and plot figures


fchannel1_d_aligned = [];
fchannel2_d_aligned = [];

for t = 1:T
    frame_ch1 = channel1_d_aligned(:,:,t);
    [frame_ch1_est] = BM3DSHARP(frame_ch1, sigma, alpha_sharp, profile, print_to_screen);
    fchannel1_d_aligned = cat(3, fchannel1_d_aligned, frame_ch1_est);
    
    frame_ch2 = channel2_d_aligned(:,:,t);
    [frame_ch2_est] = BM3DSHARP(frame_ch2,  sigma, alpha_sharp, profile, print_to_screen);
    fchannel2_d_aligned = cat(3, fchannel2_d_aligned, frame_ch2_est);    
end


%% create new signal s = channel1_alligned(x,y,t)/channel1_alligned(x,y,t)

fsharp_s = double(fchannel1_d_aligned);
ind_div0 = (fchannel2_d_aligned ==0);
fchannel2_d_aligned(ind_div0) = 1;
fsharp_s = fsharp_s./double(fchannel2_d_aligned);
fchannel2_d_aligned(ind_div0) = 0;


%%
imwrite(fsharp_s(:,:,1), [pathOut, 'fsharp_sigma11_alpha1.1_s_', sname, '.tif']);
for k = 2:T
    imwrite(fsharp_s(:,:,k), [pathOut, 'fsharp_sigma11_alpha1.1_s_', sname, '.tif'], 'WriteMode','append');
end