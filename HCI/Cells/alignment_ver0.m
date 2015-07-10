%%

fName_tiff = './2014_09_10__17_08_13h__channel02.tif';

folderName = fName_tiff(3:end-4);

if ~exist(folderName, 'dir')
  mkdir(folderName);
end;

info = imfinfo(fName_tiff);
nImages = numel(info);

images = [];
for k = 1:nImages
    img1 = im2double(imread(fName_tiff, k, 'Info', info));
    images = cat(3, images, img1);
    imwrite(img1, ['.', filesep, folderName, filesep,  sprintf('frame-%05d.jpg', k)], 'jpg');
end

%%

[m,n] = size(images(:,:,1));
nImages = size(images, 3);

corr_coeff_max = zeros(nImages-1,1);

aligned_images = zeros(3*m, 3*n, nImages);



img1 = images(:,:,1);
aligned_images(m+1:2*m,n+1:2*n,1) = img1;
imwrite(aligned_images(:,:,1), ['.', filesep, 'results_normxcorr0_channel02', filesep,  sprintf('frame-%05d.jpg', 1)], 'jpg');

for k = 2:nImages
    fprintf('Iteration %d from %d \n', k-1, nImages-1);
    img2 = images(:,:,k);
    
    c = normxcorr2(img2,img1);
    c_max = max(abs(c(:)));
    [ypeak, xpeak] = find(abs(c)==c_max);
    ypeak = ypeak-size(img2,1) + 1;
    xpeak = xpeak-size(img2,2) + 1; 
    
    
    corr_coeff_max(k-1) = c_max;
    
    % plot the above result
    aligned_images(m+1+ypeak:2*m+ypeak, ...
                   n+1+xpeak:2*n+xpeak,k) = img2; 
    imwrite(aligned_images(:,:,k), ['.', filesep, 'results_normxcorr0_channel02', filesep,  sprintf('frame-%05d.jpg', k)], 'jpg');
    
    img1 = img2;
end

figure;
plot(corr_coeff_max);
xlabel('frames');
ylabel('max correlation coefficient');
title('Normalized 2-D cross-correlation between frames');

%%
imwrite(aligned_images(:,:,1), ['.', filesep, 'results_normxcorr0_channel02', filesep, 'aligned.tif']);
for k = 2:nImages
    imwrite(aligned_images(:,:,k), ['.', filesep, 'results_normxcorr0_channel02', filesep, 'aligned.tif'], 'WriteMode','append');
end