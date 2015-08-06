clc; clear;

pathIn = ['.', filesep, 'signals_aligned', filesep];
sname = '2014_09_10__17_08_13h';
pathOut = ['.', filesep, 's_ns', filesep];
% suffix = '';

%%

fName_tiff = [pathIn, 'aligned_channel01_', sname, '.tif'];

info = imfinfo(fName_tiff);
T = numel(info);

channel1_d_aligned = [];
for k = 1:T
    img = im2double(imread(fName_tiff, k, 'Info', info));
    channel1_d_aligned = cat(3, channel1_d_aligned, img);    
end


fName_tiff = [pathIn, 'aligned_channel02_', sname, '.tif'];
info = imfinfo(fName_tiff);
T = numel(info);

channel2_d_aligned = [];
for k = 1:T
    img = im2double(imread(fName_tiff, k, 'Info', info));
    channel2_d_aligned = cat(3, channel2_d_aligned, img);    
end


%% create new signal s = channel1_alligned(x,y,t)/channel1_alligned(x,y,t)

s = double(channel1_d_aligned);
ind_div0 = (channel2_d_aligned ==0);
channel2_d_aligned(ind_div0) = 1;
s = s./double(channel2_d_aligned);
channel2_d_aligned(ind_div0) = 0;

% normalize values of s to range [0,1]
mi = min(s(:));
ma = max(s(:));
s_norm = (s-mi)./(ma-mi);

s = s_norm;

%%
imwrite(s(:,:,1), [pathOut, 's_', sname, '.tif']);
for k = 2:T
    imwrite(s(:,:,k), [pathOut, 's_', sname, '.tif'], 'WriteMode','append');
end

%% Cumulative distribution over Intensity for each pixel (x,y)
% x = [0:0.005:1];
% cdf_s = zeros(size(s,1), size(s,2), numel(x));
% for k = 2:numel(x)
%     B = s<=x(k);
%     cdf_s(:,:,k) = sum(B,3);
% end
% cdf_s = cdf_s/T;

%% Calculate percentiles
s_sorted = sort(s,3);

p = [1:0.5:100];
kT = round(p/100*T);
percentiels = s_sorted(:,:, kT);

%% normalization of the gray values
ns = [];
for t=1:T
   % values of the signal at the time t 
   v = s(:,:,t);
   % compare with the values of percentiels
   diff = abs(repmat(v, 1,1, numel(kT))- double(percentiels));
   [~, min_diff_pos] = min(diff, [], 3);
   
   ns = cat(3, ns, p(min_diff_pos));
    
end
ns = ns/100;

%% 
% mi = min(ns(:));
% ma = max(ns(:));
% ns_8bit = uint8(255*(ns-mi)./(ma-mi));
% 
% ns = ns_8bit;

%% assign gray values to percentiel values and save result

imwrite(ns(:,:,1), [pathOut, 'ns_', sname, '.tif']);
for t = 2:T
    imwrite(ns(:,:,t), [pathOut, 'ns_', sname, '.tif'], 'WriteMode','append');
end


 
