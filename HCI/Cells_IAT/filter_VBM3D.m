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
NFrames = T;

PrintInfo = 0;               % If non-zero, then print to screen and save 
%                                          the denoised video in .AVI
%                                          format. (default: 1)


[~, fchannel1_d_aligned] = VBM3D(channel1_d_aligned, sigma, NFrames, PrintInfo);
[~, fchannel2_d_aligned] = VBM3D(channel2_d_aligned, sigma, NFrames, PrintInfo);

%% create new signal s = channel1_alligned(x,y,t)/channel1_alligned(x,y,t)

fs = double(fchannel1_d_aligned);
ind_div0 = (fchannel2_d_aligned ==0);
fchannel2_d_aligned(ind_div0) = 1;
fs = fs./double(fchannel2_d_aligned);
fchannel2_d_aligned(ind_div0) = 0;


% normalize values of s to range [0,1]
mi = min(fs(:));
ma = max(fs(:));
fs_norm = (fs-mi)./(ma-mi);

% s = s_norm;

imwrite(fs(:,:,1), [pathOut, 'fs_', sname, '.tif']);
for k = 2:T
    imwrite(fs(:,:,k), [pathOut, 'fs_', sname, '.tif'], 'WriteMode','append');
end