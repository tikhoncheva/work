clc; clear;

%%
pathIn = ['.', filesep, 'signals', filesep];
sname = '2014_09_10__17_08_13h';
pathOut = ['.', filesep, 'signals_aligned_ver2', filesep];

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
channel1_tr_d = []; channel2_tr_d = [];

for k = 1:nImages
    img1 = channel1(:,:,k);
    img1_d = (double(img1)-mi1)/(ma1-mi1);
    channel1_d = cat(3, channel1_d, img1_d);
    channel1_tr_d = cat(3, channel1_tr_d, img1_d');

    img2 = channel2(:,:,k);
    img2_d = (double(img2)-mi2)/(ma2-mi2);
    channel2_d = cat(3, channel2_d, img2_d);
    channel2_tr_d = cat(3, channel2_tr_d, img2_d');
end

%%

nImages = size(channel1_d, 3);

frame_list = [1:nImages];
frames2del = [1; 123; 124];

frame_list(frames2del) = [];

% channel1_d(:,:, frames2del) = [];
% channel2_d(:,:, frames2del) = [];
% 
nImages = numel(frame_list);

%% Template image
T1 = mean(channel1_d(:,:, frame_list),3);
T2 = mean(channel2_d(:,:, frame_list),3);

%% Alignment
subrec = [];

settings = struct('move_thresh',0.010,'corr_thresh',0.75, ...
                  'max_iter', 120,'scanlinesperparameter',4, ...
                  'pregauss', 1.0 ,'haltcorr',0.995, ...
                  'bidirec', false,'leftexclude', 0);

% align initial frames

[~, ~, corr1, failed1, ~, ~, ~, channels_aligned] = correct_scanned_imaging(channel1_d, T1, subrec, ...
                                                                        channel2_d, frame_list, settings);
channels_aligned(isnan(channels_aligned)) = 0;
channel1_aligned = channels_aligned(:,:,:,1);
channel2_aligned = channels_aligned(:,:,:,2);



settings.scanlinesperparameter = 8;
[~, ~, corr2, failed2, ~, ~, ~, channels_aligned2] = correct_scanned_imaging(channel1_d, T1, subrec, ...
                                                                        channel2_d, frame_list, settings);
channels_aligned2(isnan(channels_aligned2)) = 0;
channel1_aligned2 = channels_aligned2(:,:,:,1);
channel2_aligned2 = channels_aligned2(:,:,:,2);

% ch1_d = channel1_d(4:end,:,:);
% ch2_d = channel2_d(4:end,:,:);
% 
% [p, ~, ~, ~, ~, ~, ~, ch_aligned] = correct_scanned_imaging(ch1_d, T1, [], ch2_d);
% ch1_aligned = ch_aligned(:,:,:,1);
% ch2_aligned = ch_aligned(:,:,:,2);
% 
% ch1_aligned(1:3,:,:) = NaN;
% ch2_aligned(1:3,:,:) = NaN;
% 
% 
% channel1_aligned = bsxfun(@min, channel1_aligned, ch1_aligned);
% channel2_aligned = bsxfun(@min, channel2_aligned, ch2_aligned);

%%
% align transposed frames
settings.scanlinesperparameter = 16;
subrec = [];


[~, ~, ~, failed2, ~, ~, ~, channels_tr_aligned] = correct_scanned_imaging(channel1_tr_d, T1', subrec,...
                                                                           channel2_tr_d, frame_list, settings);
channel1_tr_aligned = channels_tr_aligned(:,:,:,1);
channel2_tr_aligned = channels_tr_aligned(:,:,:,2);

%% Combine all alignments
channel1_aligned_n = channel1_aligned;
channel2_aligned_n = channel2_aligned;
for k = 1:nImages
    img1 = channel1_aligned(:,:,k); img2 = channel1_tr_aligned(:,:,k)';
    ind = isnan(img1);
    img1(ind) = img2(ind);
    channel1_aligned_n(:,:,k) = img1;
    
    img1 = channel2_aligned(:,:,k); img2 = channel2_tr_aligned(:,:,k)';
    ind = isnan(img1);
    img1(ind) = img2(ind);
    channel2_aligned_n(:,:,k) = img1;
end

%%
% channel1_aligned = channel1_aligned_n;
% channel2_aligned = channel2_aligned_n;


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