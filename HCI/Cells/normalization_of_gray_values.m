%% read image of two frames
fname_channel1 = '2014_09_10__17_08_13h__channel01';
fname_channel2 = '2014_09_10__17_08_13h__channel02';

%%
fName_tiff = './2014_09_10__17_08_13h__channel01.tif';
info = imfinfo(fName_tiff);
T = numel(info);

GaussianFilter = fspecial('gaussian',[5 5], 0.5);


channel1_16bit = [];
for k = 1:T
    img16 = imread(fName_tiff, k, 'Info', info);
    img16f = imfilter(img16, GaussianFilter, 'replicate');
    channel1_16bit = cat(3, channel1_16bit, img16f);
end


fName_tiff = './2014_09_10__17_08_13h__channel02.tif';
info = imfinfo(fName_tiff);
T = numel(info);

channel2_16bit = [];
for k = 1:T
    img16 = imread(fName_tiff, k, 'Info', info);
    img16f = imfilter(img16, GaussianFilter, 'replicate');
    channel2_16bit = cat(3, channel2_16bit, img16f);
end


%% delete some bad frames

frames2del = [1; 123; 124];
channel1_16bit(:,:, frames2del) = [];
channel2_16bit(:,:, frames2del) = [];

T = T - numel(frames2del);

%%
channel1_8bit = [];
for k = 1:T
    img16 = channel1_16bit(:,:,k);
    img8 = uint8( (double(img16) - double(min(img16(:)))) / (double(max(img16(:))) - double(min(img16(:)))) * 255 ); 
    channel1_8bit = cat(3, channel1_8bit, img8);
end

channel2_8bit = [];
for k = 1:T
    img16 = channel2_16bit(:,:,k);
    img8 = uint8( (double(img16) - double(min(img16(:)))) / (double(max(img16(:))) - double(min(img16(:)))) * 255 ); 
    channel2_8bit = cat(3, channel2_8bit, img8);
end

%% read offset matrices

offset = dlmread(['.', filesep, 'offset_channel01_1.txt'], '\t', 1, 0);
offset(frames2del,:) = [];

% offset_x = offset(:,1);
% offset_y = offset(:,2); 

%% align signals
% [m,n] = size(channel1_8bit(:,:,1));
% 
% channel1_8bit_aligned = uint8(zeros(3*m, 3*n, T));
% channel2_8bit_aligned = uint8(zeros(3*m, 3*n, T));
% 
% for k = 1:T
%     dx = offset(k,1); dy = offset(k,2);
%     
%     img1 = channel1_8bit(:,:,k);
%     img1 = padarray(img1, [m+dy n+dx], 'replicate', 'pre');
%     img1 = padarray(img1, [3*m - (2*m+dy) 3*n - (2*n+dx)], 'replicate', 'post');
%     channel1_8bit_aligned(:, :, k) = img1;
% %     channel1_8bit_aligned(m+1+dy:2*m+dy, n+1+dx:2*n+dx, k) = img1;    
%     
%     img2 = channel2_8bit(:,:,k);
%     img2 = padarray(img2, [m+dy n+dx], 'replicate', 'pre');
%     img2 = padarray(img2, [3*m - (2*m+dy) 3*n - (2*n+dx)], 'replicate', 'post');    
%     channel2_8bit_aligned(:, :, k) = img2;
% %     channel2_8bit_aligned(m+1+dy:2*m+dy, n+1+dx:2*n+dx, k) = img2;
% end
% 
% channel1_8bit_aligned = channel1_8bit_aligned(m+1:2*m, n+1:2*n, :);
% channel2_8bit_aligned = channel2_8bit_aligned(m+1:2*m, n+1:2*n, :);

%% align signals
[m,n] = size(channel1_8bit(:,:,1));

channel1_16bit_aligned = uint16(zeros(3*m, 3*n, T));
channel2_16bit_aligned = uint16(zeros(3*m, 3*n, T));

for k = 1:T
    dx = offset(k,1); dy = offset(k,2);
    
    img1 = channel1_16bit(:,:,k);
    img1 = padarray(img1, [m+dy n+dx], 'replicate', 'pre');
    img1 = padarray(img1, [3*m - (2*m+dy) 3*n - (2*n+dx)], 'replicate', 'post');
    channel1_16bit_aligned(:, :, k) = img1;
%     channel1_16bit_aligned(m+1+dy:2*m+dy, n+1+dx:2*n+dx, k) = img1;    
    
    img2 = channel2_16bit(:,:,k);
    img2 = padarray(img2, [m+dy n+dx], 'replicate', 'pre');
    img2 = padarray(img2, [3*m - (2*m+dy) 3*n - (2*n+dx)], 'replicate', 'post');    
    channel2_16bit_aligned(:, :, k) = img2;
%     channel2_16bit_aligned(m+1+dy:2*m+dy, n+1+dx:2*n+dx, k) = img2;
end

channel1_16bit_aligned = channel1_16bit_aligned(m+1:2*m, n+1:2*n, :);
channel2_16bit_aligned = channel2_16bit_aligned(m+1:2*m, n+1:2*n, :);

%% create new signal s = channel1_alligned(x,y,t)/channel1_alligned(x,y,t)

s = double(channel1_16bit_aligned);
ind_div0 = (channel2_16bit_aligned ==0);
channel2_16bit_aligned(ind_div0) = 1;
s = s./double(channel2_16bit_aligned);
channel2_16bit_aligned(ind_div0) = 0;

s_8 = [];
for k = 1:T    
    img = s(:,:,k);
    mi = min(img(:));
    ma = max(img(:));
    img = uint8(255*(img-mi)/(ma-mi)); 
    s_8 = cat(3, s_8, img);
end

% sum_s = sum(s_16,3);
% ind = (sum_s==0); sum_s(ind) = 1;
% s_16 = s_16./repmat(sum_s,1,1,T);


imwrite(s_8(:,:,1), ['.', filesep, 's_smoothed.tif']);
for k = 2:T
    imwrite(s_8(:,:,k), ['.', filesep, 's_smoothed.tif'], 'WriteMode','append');
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

imwrite(ns(:,:,1), ['.', filesep, 'ns_smoothed.tif']);
% imwrite(I, ['.', filesep, 'ns', filesep, sprintf('frame-%05d.jpg', 1)]);
for t = 2:T
    imwrite(ns(:,:,t), ['.', filesep, 'ns_smoothed.tif'], 'WriteMode','append');
%     imwrite(I, ['.', filesep, 'ns', filesep, sprintf('frame-%05d.jpg', t)]);
end




   


 
