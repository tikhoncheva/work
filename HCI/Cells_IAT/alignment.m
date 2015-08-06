clc; clear;

addpath(genpath('../Tools/IAT_v0.9.1/'))

%%
pathIn = ['.', filesep, 'signals', filesep];
sname = '2014_09_13__21_58_05h';
pathOut = ['.', filesep, 'signals_aligned', filesep];

%% Channel 1
fName_tiff = [pathIn, sname, '__channel01.tif'];

info = imfinfo(fName_tiff);
nImages = numel(info);

channel1 = [];
for k = 1:nImages
    img1 = im2double(imread(fName_tiff, k, 'Info', info));
    channel1 = cat(3, channel1, img1);
end


%% Channel 2
fName_tiff = [pathIn, sname, '__channel02.tif'];

info = imfinfo(fName_tiff);
nImages = numel(info);

channel2 = [];
for k = 1:nImages
    img2 = im2double(imread(fName_tiff, k, 'Info', info));
    channel2 = cat(3, channel2, img2);
end

%% Conver to  double

ma1 = double(max(channel1(:)));
mi1 = double(min(channel1(:)));

ma2 = double(max(channel2(:)));
mi2 = double(min(channel2(:)));
    
channel1_d = []; channel2_d = [];

for k = 1:nImages
    img1 = channel1(:,:,k);
    img1_d = (double(img1)-mi1)/(ma1-mi1);
    channel1_d = cat(3, channel1_d, img1_d);

    img2 = channel2(:,:,k);
    img2_d = (double(img2)-mi2)/(ma2-mi2);
    channel2_d = cat(3, channel2_d, img2_d);
end

%%

nImages = size(channel1_d, 3);

frame_list = [1:nImages];
% frames2del = [1; 47; 123; 124];
frames2del = [1];

frame_list(frames2del) = [];

% channel1_d(:,:, frames2del) = [];
% channel2_d(:,:, frames2del) = [];

nImages = numel(frame_list);

%% Template image
T1 = mean(channel1_d(:,:, frame_list),3);
T2 = mean(channel2_d(:,:, frame_list),3);

%% Alignment

channel1_aligned = [];
channel2_aligned = [];

% parameters for ECC (Enhanced Correlation Coefficient) and Lucas-Kanade 

% possible transformation types:  {'translation', 'euclidean','similarity','affine','homography'}
transform = 'euclidean';

par = [];
par.levels =    3;
par.iterations = 15;
par.transform = transform;

tic;

for k = 1:nImages
i = frame_list(k);    

img1 = channel1_d(:,:,i);
img2 = channel2_d(:,:,i);

% Lucas-Kanade algorithm
[LKWarp1]=iat_LucasKanade(img1, T1, par);
[LKWarp2]=iat_LucasKanade(img2, T2, par);

% Compute the warped image and visualize the error
[img1aligned, ~] = iat_inverse_warping(img1, LKWarp1, par.transform, 1:size(T1,2),1:size(T1,1));
[img2aligned, ~] = iat_inverse_warping(img2, LKWarp2, par.transform, 1:size(T2,2),1:size(T2,1));

ind1 = (img1aligned==0); 
ind2 = (img2aligned==0); 

img1aligned(ind1) = T1(ind1);
img2aligned(ind2) = T2(ind2);

channel1_aligned = cat(3, channel1_aligned, img1aligned);
channel2_aligned = cat(3, channel2_aligned, img2aligned);
end

display(sprintf('alignment done in %0.3f sec', toc));


%%

% ma1 = max(channel1_aligned(:));
% mi1 = min(channel1_aligned(:));
% 
% ma2 = max(channel2_aligned(:));
% mi2 = min(channel2_aligned(:));
%     
% channel1_aligned_8bit = []; channel2_aligned_8bit= [];
% for k = 1:nImages
%     img1 = channel1_aligned(:,:,k);
%     img1_8bit = uint8(255*(img1-mi1)/(ma1-mi1));
%     channel1_aligned_8bit = cat(3, channel1_aligned_8bit, img1_8bit);
% 
%     img2 = channel2_aligned(:,:,k);
%     img2_8bit = uint8(255*(img2-mi2)/(ma2-mi2));
%     channel2_aligned_8bit = cat(3, channel2_aligned_8bit, img2_8bit);
% end


%%
imwrite(channel1_aligned(:,:,1), [pathOut, 'aligned_channel01_', sname, '.tif']);
for k = 2:nImages
    imwrite(channel1_aligned(:,:,k), [pathOut,  'aligned_channel01_', sname, '.tif'], 'WriteMode','append');
end

%%
imwrite(channel2_aligned(:,:,1), [pathOut,  'aligned_channel02_', sname, '.tif']);
for k = 2:nImages
    imwrite(channel2_aligned(:,:,k), [pathOut,  'aligned_channel02_', sname, '.tif'], 'WriteMode','append');
end