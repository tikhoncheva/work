clc; clear;

%% Channel 1
fName_tiff = './2014_09_10__17_08_13h__channel01.tif';

info = imfinfo(fName_tiff);
nImages = numel(info);

channel1 = [];
for k = 1:nImages
    img1 = im2double(imread(fName_tiff, k, 'Info', info));
    channel1 = cat(3, channel1, img1);
end


%% Channel 2
fName_tiff = './2014_09_10__17_08_13h__channel02.tif';

info = imfinfo(fName_tiff);
nImages = numel(info);

channel2 = [];
for k = 1:nImages
    img2 = im2double(imread(fName_tiff, k, 'Info', info));
    channel2 = cat(3, channel2, img2);
end

%% Smooth images

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


frames2del = [1; 123; 124];
channel1_d(:,:, frames2del) = [];
channel2_d(:,:, frames2del) = [];

nImages = size(channel1_d, 3);

%%

% Template image
% T = channel1_d(:,:,5);
% T = channel1_dfs(:,:,5);
T1 = mean(channel1_d,3);
T2 = mean(channel2_d,3);
% T = (T-min(T(:))) / (max(T(:)) - min(T(:)));


%% Alignment

[p, ~, ~, ~, ~, ~, ~, channels_aligned] = correct_scanned_imaging(channel1_d, T1, [], channel2_d);
channel1_aligned = channels_aligned(:,:,:,1);
channel2_aligned = channels_aligned(:,:,:,2);

% [p1, ~, ~, ~, ~, ~, ~, channel1_aligned] = correct_scanned_imaging(channel1_d, T1);
% [p2, ~, ~, ~, ~, ~, ~, channel2_aligned] = correct_scanned_imaging(channel2_d, T2);


% [p, ~, ~, ~, ~, ~, ~, channels_aligned] = correct_scanned_imaging(channel1_d, T1, [], channel2_d);
% channel1_aligned = channels_aligned(:,:,:,1);
% channel2_aligned = channels_aligned(:,:,:,2);


%%

ma1 = max(channel1_aligned(:));
mi1 = min(channel1_aligned(:));

ma2 = max(channel2_aligned(:));
mi2 = min(channel2_aligned(:));
    
channel1_aligned_8bit = []; channel2_aligned_8bit= [];
for k = 1:nImages
    img1 = channel1_aligned(:,:,k);
    img1_8bit = uint8(255*(img1-mi1)/(ma1-mi1));
    channel1_aligned_8bit = cat(3, channel1_aligned_8bit, img1_8bit);

    img2 = channel2_aligned(:,:,k);
    img2_8bit = uint8(255*(img2-mi2)/(ma2-mi2));
    channel2_aligned_8bit = cat(3, channel2_aligned_8bit, img2_8bit);
end


%%
imwrite(channel1_aligned_8bit(:,:,1), ['.', filesep, 'aligned_channel01.tif']);
for k = 2:nImages
    imwrite(channel1_aligned_8bit(:,:,k), ['.', filesep, 'aligned_channel01.tif'], 'WriteMode','append');
end

% %%
imwrite(channel2_aligned_8bit(:,:,1), ['.', filesep, 'aligned_channel02.tif']);
for k = 2:nImages
    imwrite(channel2_aligned_8bit(:,:,k), ['.', filesep, 'aligned_channel02.tif'], 'WriteMode','append');
end