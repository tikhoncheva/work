%%
clc;
clear;

fName_tiff = './2014_09_10__17_08_13h__channel01.tif';

folderName = fName_tiff(3:end-4);
channelnumb = folderName(end);

if ~exist(folderName, 'dir')
  mkdir(folderName);
end;

settings = 'test';

%%
GaussianFilter = fspecial('gaussian',[5 5], 0.5);
DiskFilter = fspecial('disk',1);

subsampleRate = 2;

info = imfinfo(fName_tiff);
nImages = numel(info);

% Original frames
images = [];
for k = 1:nImages
    img = imread(fName_tiff, k, 'Info', info);
    images = cat(3, images, img);
end

ma = double(max(images(:)));
mi = double(min(images(:)));
    
% Blured Frames
images_fs = [];
for k = 1:nImages
    img = images(:,:,k);
    img8 = uint8((double(img)-mi)/(ma-mi)*255);
    
    imgs = img8(1:subsampleRate:end, 1:subsampleRate:end);
    imgfs = imfilter(imgs, GaussianFilter, 'replicate');
%     imgfs = imfilter(imgs, DiskFilter, 'replicate');    
%     imgfs = medfilt2(img);
%     [imgf,~] = wiener2(img,[3 3]);
%     imgfs = imgf(1:subsampleRate:end, 1:subsampleRate:end);
    images_fs = cat(3, images_fs, imgfs);
end

frames2del = [1; 123; 124];
images_fs(:,:, frames2del) = [];

%%
[m,n] = size(images_fs(:,:,1));
xshift = [-20:20];
yshift = [-10:10];

%%

nImages = size(images_fs, 3);

corr_coeff_max = zeros(nImages-1,1);

aligned_images = uint8(zeros(m, n, nImages));

img = images_fs(:,:,1);
aligned_images(:,:,1) = img;

offset_x = zeros(nImages,1);    offset_x(1) = 0;
offset_y = zeros(nImages,1);    offset_y(1) = 0;

for k = 2:nImages
    
    fprintf('Iteration %d from %d \n', k-1, nImages-1);
    img2 = images_fs(:,:,k);
    
    corr_coeff = zeros(numel(xshift)*numel(yshift), 7);
    
    for i = 1:numel(yshift)
        
        p = ceil((sign(yshift(i)) + 1)/2);
        ystart = 1 - (1-p)*yshift(i);
        yend   = m - p*yshift(i);
            
        for j = 1:numel(xshift)
            
            q = ceil((sign(xshift(j)) + 1)/2);
            xstart = 1 - (1-q)*xshift(j);
            xend   = n - q*xshift(j);
            
            img2_cut = img2(ystart:yend, xstart:xend);
    
            c = normxcorr2(img2_cut,img);
            [c_max,c_max_pos] = max(abs(c(:)));
            [ypeak, xpeak] = ind2sub(size(c), c_max_pos);           % position of path in img
            
            corr_coeff( (i-1)*numel(xshift)+j,1) = c_max;                   % row-wise vectorization
            corr_coeff( (i-1)*numel(xshift)+j,2) = ypeak-size(img2_cut,1);
            corr_coeff( (i-1)*numel(xshift)+j,3) = xpeak-size(img2_cut,2);
            
            % parameters of croped rectangle from the image2
            corr_coeff( (i-1)*numel(xshift)+j,4) = ystart;
            corr_coeff( (i-1)*numel(xshift)+j,5) = xstart;
            corr_coeff( (i-1)*numel(xshift)+j,6) = yend - ystart + 1;
            corr_coeff( (i-1)*numel(xshift)+j,7) = xend - xstart + 1;
    
        end
    end
    
    [c_max, maxpos] = max(corr_coeff(:,1));
    
    % offset of the matched rectangle wrt img1
    corr_coeff_max(k-1) = c_max;
    y0 = corr_coeff(maxpos,2);
    x0 = corr_coeff(maxpos,3);
    
    % offset of the matched rectangle wrt img2
    y = corr_coeff( maxpos,4);
    x = corr_coeff( maxpos,5);
        
    dx = x0 - x;
    dy = y0 - y;
    
    offset_x(k) = dx;
    offset_y(k) = dy;     
    
    img2_aligned = img2;
    img2_aligned = padarray(img2_aligned, [m+dy n+dx], 'replicate', 'pre');
    img2_aligned = padarray(img2_aligned, [3*m - (2*m+dy) 3*n - (2*n+dx)], 'replicate', 'post');
    aligned_images(:, :, k) = img2_aligned(m+1:2*m, n+1:2*n); 
                             
%     imwrite(aligned_images(:,:,k), ['.', filesep, 'results_normxcorr', filesep, 'channel0', channelnumb, filsep,  sprintf('frame-%05d.jpg', k)], 'jpg');           
    
%     img1 = img2;
%     img = aligned_images(:, :, k);
end

figure;
plot(corr_coeff_max);
xlabel('frames');
ylabel('max correlation coefficient');
title('Normalized 2-D cross-correlation between frames');

%%
% save alignment shifts in a file
fileID = fopen(['.', filesep, 'offset_channel0', channelnumb, '_', settings, '.txt'],'w');
fprintf(fileID,'  offset x   \t offset y \n');
fprintf(fileID,'  %5d        \t     %5d  \n', [offset_x, offset_y]');
fclose(fileID);
   
%%
% imwrite(aligned_images(:,:,1), ['.', filesep, 'results_normxcorr', filesep, ...
%                                               'channel0', channelnumb, filesep, ...
%                                               'aligned_channel0', channelnumb, '_ver1.tif']);
% for k = 2:nImages
%     imwrite(aligned_images(:,:,k), ['.', filesep, 'results_normxcorr', filesep, ...
%                                               'channel0', channelnumb, filesep, ...
%                                               'aligned_channel0', channelnumb, '_ver1.tif'], 'WriteMode','append');
% end

%%
[m,n] = size(images(:,:,1));

channel_aligned = uint16(zeros(3*m, 3*n, nImages));
channel_aligned1 = uint16(zeros(3*m, 3*n, nImages));

for k = 1:50
    dx = offset_x(k); dy = offset_y(k);
    img = images(:,:,k);    
    
    img = padarray(img, [m+dy n+dx], 'replicate', 'pre');
    img = padarray(img, [3*m - (2*m+dy) 3*n - (2*n+dx)], 'replicate', 'post');
    channel_aligned(:, :, k) = img;
    
end

channel_aligned = channel_aligned(m+1:2*m, n+1:2*n, :);