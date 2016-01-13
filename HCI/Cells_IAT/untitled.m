clc; clear;

addpath(genpath('../Tools/IAT_v0.9.1/'))

%%
pathData = ['..', filesep, 'brain_data', filesep];

pathS = [pathData, 'signals', filesep];

sname = '2014_09_10__18_20_38h';
pathOut = ['.', filesep, 'signals_aligned', filesep];

%% Channel 1
fName_tiff = [pathS, sname, '__channel01.tif'];

info = imfinfo(fName_tiff);
nImages = numel(info);

channel1 = [];
for k = 1:nImages
    img1 = im2double(imread(fName_tiff, k, 'Info', info));
    channel1 = cat(3, channel1, img1);
end


%% Channel 2
fName_tiff = [pathS, sname, '__channel02.tif'];

info = imfinfo(fName_tiff);
nImages = numel(info);

channel2 = [];
for k = 1:nImages
    img2 = im2double(imread(fName_tiff, k, 'Info', info));
    channel2 = cat(3, channel2, img2);
end