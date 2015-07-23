clc; clear;

addpath('./fastBSpline');

%%
fName_tiff = './2014_09_10__17_08_13h__channel01.tif';
settings = 'Gaussian';

%% Initial frames
folderName = fName_tiff(3:end-4);
channelnumb = folderName(end);


info = imfinfo(fName_tiff);
nImages = numel(info);

images = [];
for k = 1:nImages
    img1 = im2double(imread(fName_tiff, k, 'Info', info));
    images = cat(3, images, img1');
end

%% Convert into double
ma = double(max(images(:)));
mi = double(min(images(:)));
    
images_d = [];
for k = 1:nImages
    img = images(:,:,k);
    img_d = (double(img)-mi)/(ma-mi);
    images_d = cat(3, images_d, img_d);
end

%% delete bad frames
frames2del = [1; 123; 124];
images_d(:,:, frames2del) = [];

nImages = size(images_d, 3);

%% Smooth

GaussianFilter = fspecial('gaussian',[5 5], 1.5);
DiskFilter = fspecial('disk',1);
subsampleRate = 1;


images_dfs = [];
for k = 1:nImages
    img = images_d(:,:,k);
    
    imgs = img(1:subsampleRate:end, 1:subsampleRate:end);
%     imgfs = imgs;
%     imgfs = imfilter(imgs, DiskFilter, 'replicate');
    imgfs = imgaussfilt(imgs, 1.5);
%     imgfs = medfilt2(imgs);
%     [imgfs,~] = wiener2(imgs,[3 3]);
%     imgfs = imgf(1:subsampleRate:end, 1:subsampleRate:end);
    images_dfs = cat(3, images_dfs, imgfs);
end

%% Alignment
nImages = size(images_dfs, 3);

% Template image
T = mean(images_dfs,3);


%% Alignment
aligned_images = [];

for k = 1:nImages
    I = images_dfs(:,:,k);
    [Id,dpx,dpy] = doLucasKanade(T, I);
    
    aligned_images = cat(3, aligned_images, Id');
   
end

%%
imwrite(aligned_images(:,:,1), ['.', filesep, 'aligned_channel0', channelnumb, '_', settings '.tif']);
for k = 2:nImages
    imwrite(aligned_images(:,:,k), ['.', filesep, 'aligned_channel0', channelnumb, '_', settings '.tif'],...
            'WriteMode','append');
end

