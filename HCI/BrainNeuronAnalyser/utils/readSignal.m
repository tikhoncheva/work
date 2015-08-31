function signal = readSignal(file_name)

info = imfinfo(file_name);
nImages = numel(info);

signal = [];
for k = 1:nImages
    img1 = im2double(imread(file_name, k, 'Info', info));
    signal = cat(3, signal, img1);
end