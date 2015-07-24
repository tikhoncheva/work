clc; clear;

pathIn = ['.', filesep, 'signals_aligned_ver2', filesep];
sname = '2014_09_10__17_08_13h';
pathOut = ['.', filesep, 's_ns_ver2', filesep];
% suffix = '';

%%

fName_tiff = [pathIn, 'aligned_channel01_', sname, '.tif'];

info = imfinfo(fName_tiff);
T = numel(info);

channel1_d_aligned = [];
for k = 1:T
%     img8 = imread(fName_tiff, k, 'Info', info);    
%     channel1_8bit_aligned = cat(3, channel1_8bit_aligned, img8);
    img = im2double(imread(fName_tiff, k, 'Info', info));
    channel1_d_aligned = cat(3, channel1_d_aligned, img);    
end


fName_tiff = [pathIn, 'aligned_channel02_', sname, '.tif'];
info = imfinfo(fName_tiff);
T = numel(info);

channel2_d_aligned = [];
for k = 1:T
%     img8 = imread(fName_tiff, k, 'Info', info);
%     channel2_8bit_aligned = cat(3, channel2_8bit_aligned, img8);
    img = im2double(imread(fName_tiff, k, 'Info', info));
    channel2_d_aligned = cat(3, channel2_d_aligned, img);    
end


%% create new signal s = channel1_alligned(x,y,t)/channel1_alligned(x,y,t)

s = double(channel1_d_aligned);
ind_div0 = (channel2_d_aligned ==0);
channel2_d_aligned(ind_div0) = 1;
s = s./double(channel2_d_aligned);
channel2_d_aligned(ind_div0) = 0;

% s_8 = [];
% mi = min(s(:));
% ma = max(s(:));
% 
% for k = 1:T    
%     img = s(:,:,k);
%     img = uint8(255*(img-mi)/(ma-mi)); 
%     s_8 = cat(3, s_8, img);
% end

% sum_s = sum(s_16,3);
% ind = (sum_s==0); sum_s(ind) = 1;
% s_16 = s_16./repmat(sum_s,1,1,T);

%%
imwrite(s(:,:,1), [pathOut, 's_', sname, '.tif']);
for k = 2:T
    imwrite(s(:,:,k), [pathOut, 's_', sname, '.tif'], 'WriteMode','append');
end


%% Cumulative distribution over Intensity for each pixel (x,y)

s_sorted = sort(s,3);

cdf_s = s_sorted(:,:,1);
for t = 2:T
    cdf_s(:,:,t) = sum(s_sorted(:,:, 1:t), 3);
end


%% Calculate percentiles
t = [1:0.5:100];
kT = round(t/100*T);

% percentiels = cdf_s(:,:, kT);
percentiels = s_sorted(:,:, kT);


%% normalization of the gray values

ns = [];
for t=1:T
   % values of the signal at the time t 
   v = double(s(:,:,t));
   % compare with the values of percentiels
%    diff = abs(repmat(v, 1,1, numel(kT))- double(percentiels));
   diff = abs(repmat(v, 1,1, numel(kT))- double(percentiels));
%    diff = abs(repmat(v, 1,1, T)- cdf_s );
   [~, min_diff_pos] = min(diff, [], 3);
   
   ns = cat(3, ns, min_diff_pos);
    
end


%%

mi = min(ns(:));
ma = max(ns(:));
ns_8bit = [];
for t = 1:T
    img = ns(:,:,t);
    img = uint8(255*(img-mi)/(ma-mi)); 
    ns_8bit = cat(3, ns_8bit,img);
end

ns = ns_8bit;
%% assign gray values to percentiel values and save result

imwrite(ns(:,:,1), [pathOut, 'ns_', sname, '.tif']);
for t = 2:T
    imwrite(ns(:,:,t), [pathOut, 'ns_', sname, '.tif'], 'WriteMode','append');
end




   


 
