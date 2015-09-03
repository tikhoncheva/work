clc; clear;

pathIn = ['.', filesep, 'signals_aligned', filesep];
sname = '2014_09_10__18_20_38h';
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


%% Remove gaussian noise

fchannel1_d_aligned = channel1_d_aligned;
fchannel2_d_aligned = channel2_d_aligned;

for i = 1:size(channel1_d_aligned,3)
    fchannel1_d_aligned(:,:,i) = wiener2(fchannel1_d_aligned(:,:,i),[5 5]);
    fchannel2_d_aligned(:,:,i) = wiener2(fchannel2_d_aligned(:,:,i),[5 5]);
end

%% create new signal s = channel1_alligned(x,y,t)/channel1_alligned(x,y,t)

s = double(channel1_d_aligned);
ind_div0 = (channel2_d_aligned ==0);
channel2_d_aligned(ind_div0) = 1;
s = s./double(channel2_d_aligned);
channel2_d_aligned(ind_div0) = 0;

% s = double(fchannel1_d_aligned);
% ind_div0 = (fchannel2_d_aligned ==0);
% fchannel2_d_aligned(ind_div0) = 1;
% s = s./double(fchannel2_d_aligned);
% fchannel2_d_aligned(ind_div0) = 0;

% normalize values of s to range [0,1]

mi = min(s(:));
ma = max(s(:));
s_norm = (s-mi)./(ma-mi);

mi = 0.2;
ma = 0.8;
s = s-mi;
s(s<0)=0;
s = s./(ma-mi);
s(s>1)=1;
    

%%
fs = s;
for i = 1:size(fs,3)
    fs(:,:,i) = wiener2(fs(:,:,i), [5,5]);
end
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

s_cf(:,:,1) = fs(:,:,1);
for i = 2:size(fs,3)
    s_cf(:,:,i) = sum(fs(:,:,1:i), 3);
end

cf_mean = mean(reshape(s_cf, size(s_cf,1)*size(s_cf,2), size(s_cf,3)));

%% normalization of the gray values
cfmean = reshape(repmat(cf_mean, 64*128,1), 64, 128, 374);

diff = s_cf - cfmean;
% diff_min = min(diff(:));
% diff_max = max(diff(:));
% 
% diff2 = (diff - diff_min)/ (diff_max-diff_min);

ns1 = s;
for i = 1:size(s,3)
   
   temp = diff(:,:,i);
   diff_min = min(temp(:));
   diff_max = max(temp(:));  
   
   ns1(:,:,i) = (s(:,:,i)-diff_min) / (diff_max-diff_min);
    
end

%%
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
mi = min(ns(:));
ma = max(ns(:));
ns_8bit = uint8(255*(ns-mi)./(ma-mi));

ns = ns_8bit;

%% assign gray values to percentiel values and save result

imwrite(ns(:,:,1), [pathOut, 'ns12_', sname, '.tif']);
for t = 2:T
    imwrite(ns(:,:,t), [pathOut, 'ns12_', sname, '.tif'], 'WriteMode','append');
end


 
