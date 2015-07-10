fName_tiff = './2014_09_10__17_08_13h__channel02.tif';
folderName = fName_tiff(3:end-4);
channelnumb = folderName(end);

%% get average image of the unaligned frames


files = dir(['.', filesep, folderName]);
          
issub = [files(:).isdir]; 
nameImages = {files(~issub).name}';

unaligned_images = [];
for k = 1:size(nameImages)
    img1 = im2double(imread(['.', filesep, folderName, filesep,  nameImages{k}]));
    [m,n] = size(img1);
    img2 = zeros(3*m, 3*n);
    img2(m+1:2*m,n+1:2*n) = img1;
    unaligned_images = cat(3, unaligned_images, img2);
end


average_unaligned_image = mean(unaligned_images, 3);
imwrite(average_unaligned_image, ['.', filesep, 'unaligned_avg_image_channel0', channelnumb, '.jpg']);


%%

% files = dir(['.', filesep, 'results_normxcorr', filesep, 'channel0', channelnumb, filesep]);
%           
% issub = [files(:).isdir]; 
% nameImages = {files(~issub).name}';
% 
% % consider ONLY images with the string 'frame' in the name
% filter = strfind(nameImages, 'frame');
% emptyCells = cellfun(@isempty, filter);
% nameImages(emptyCells) = [];
% 
% 
% img2 = [];
% for k = 1:size(nameImages)
%     img1 = im2double(imread(['.', filesep, 'results_normxcorr', filesep, 'channel0', channelnumb, filesep,  nameImages{k}]));
%     img2 = cat(3, img2, img1);
% end
% 
% 
% average_aligned_image = mean(img2, 3);
% imwrite(average_aligned_image, ['.', filesep, 'results_normxcorr', filesep, 'channel0', channelnumb, filesep, 'aligned_avg_image.jpg']);
