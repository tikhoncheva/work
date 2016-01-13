pathIn_signal = ['.', filesep, 'signals', filesep];
pathIn_signal_aligned = ['.', filesep, 'signals_aligned', filesep];
sname = '2014_09_10__18_20_38h';

%% Signals
fName_tiff = [pathIn_signal, sname, '__channel01.tif'];

info = imfinfo(fName_tiff);
nImages = numel(info);

channel1 = [];
for k = 1:nImages
    img1 = im2double(imread(fName_tiff, k, 'Info', info));
    channel1 = cat(3, channel1, img1);
end

fName_tiff = [pathIn_signal, sname, '__channel02.tif'];

info = imfinfo(fName_tiff);
nImages = numel(info);

channel2 = [];
for k = 1:nImages
    img2 = im2double(imread(fName_tiff, k, 'Info', info));
    channel2 = cat(3, channel2, img2);
end

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

%% Time gradient

Gt_channel1 = channel1(:,:,2:end);
Gt_channel1 = Gt_channel1 - channel1(:,:,1:end-1);
for i = 1:size(Gt_channel1,3)
    Gt_channel1(:,:,i) = imadjust(Gt_channel1(:,:,i));
end

%% Remove gaussian noise

fchannel1_d_aligned = channel1_d_aligned;
fchannel2_d_aligned = channel2_d_aligned;

for i = 1:size(channel1_d_aligned,3)
    fchannel1_d_aligned(:,:,i) = wiener2(fchannel1_d_aligned(:,:,i),[3 3]);
end
