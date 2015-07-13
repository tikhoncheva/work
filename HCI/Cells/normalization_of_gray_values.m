%% read image of two frames
fname_channel1 = '2014_09_10__17_08_13h__channel01';
fname_channel2 = '2014_09_10__17_08_13h__channel02';

%%
fName_tiff = './2014_09_10__17_08_13h__channel01.tif';
info = imfinfo(fName_tiff);
T = numel(info);

channel1_16bit = [];
for k = 1:T
    img16 = imread(fName_tiff, k, 'Info', info);
    channel1_16bit = cat(3, channel1_16bit, img16);
end

%%
fName_tiff = './2014_09_10__17_08_13h__channel02.tif';
info = imfinfo(fName_tiff);
T = numel(info);

channel2_16bit = [];
for k = 1:T
    img16 = imread(fName_tiff, k, 'Info', info);
    channel2_16bit = cat(3, channel2_16bit, img16);
end

%%
% frames of the first channel
% dir_ch1 = dir(['.', filesep, fname_channel1]);
% issub = [dir_ch1(:).isdir]; 
% nameImages1 = {dir_ch1(~issub).name}';
% 
% T = size(nameImages1,1);

channel1_8bit = [];
for t = 1:T
    img16 = channel1_16bit(:,:,t);
    img8 = uint8( (double(img16) - double(min(img16(:)))) / (double(max(img16(:))) - double(min(img16(:)))) * 255 ); 
    channel1_8bit = cat(3, channel1_8bit, img8);
end


% frames of the second channel
% dir_ch2 = dir(['.', filesep, fname_channel2]);
% issub = [dir_ch2(:).isdir]; 
% nameImages2 = {dir_ch2(~issub).name}';
% 
% assert(T == size(nameImages2,1), 'two channels have different signal length');

channel2_8bit = [];
for t = 1:T
    img16 = channel2_16bit(:,:,t);
    img8 = uint8( (double(img16) - double(min(img16(:)))) / (double(max(img16(:))) - double(min(img16(:)))) * 255 ); 
    channel2_8bit = cat(3, channel2_8bit, img8);
end

%% read offset matrices

offset = dlmread(['.', filesep, 'offset_channel02.txt'], '\t', 1, 0);

% offset_x = offset(:,1);
% offset_y = offset(:,2); 
       
%% align signals
[m,n] = size(channel1_8bit(:,:,1));

channel1_16bit_aligned = uint16(zeros(3*m, 3*n, T));
channel2_16bit_aligned = uint16(zeros(3*m, 3*n, T));

for t = 1:T
    dx = offset(t,1); dy = offset(t,2);
    channel1_16bit_aligned(m+1+dy:2*m+dy, n+1+dx:2*n+dx, t) = channel1_8bit(:,:,t);
    channel2_16bit_aligned(m+1+dy:2*m+dy, n+1+dx:2*n+dx, t) = channel2_8bit(:,:,t);
end

channel1_16bit_aligned = channel1_16bit_aligned(m+1:2*m, n+1:2*n, :);
channel2_16bit_aligned = channel2_16bit_aligned(m+1:2*m, n+1:2*n, :);
%% create new signal s = channel1_alligned(x,y,t)/channel1_alligned(x,y,t)

% s = double(channel1_16bit_aligned);
% ind_div0 = (channel2_16bit_aligned ==0);
% channel2_16bit_aligned(ind_div0) = 1;
% s = s./double(channel2_16bit_aligned);
% channel2_16bit_aligned(ind_div0) = 0;

s = double(channel1_16bit);
ind_div0 = (channel2_16bit ==0);
channel2_16bit(ind_div0) = 1;
s = s./double(channel2_16bit);
channel2_16bit(ind_div0) = 0;

% % % frames of the second channel
% % dir_s = dir(['.', filesep, 'frames_DeltaR_R']);
% % issub = [dir_s(:).isdir]; 
% % nameImages = {dir_s(~issub).name}';
% % 
% % T = size(nameImages,1);
% % 
% % s = [];
% % for t = 1:T
% %     img16 = rgb2gray(imread(['.', filesep, 'frames_DeltaR_R', filesep,  nameImages{t}]));
% %     s = cat(3, s, im2double(img16));
% % end
% % 
% % 

% contrast normalization
% s_16bit = [];
% mi = min(s(:));
% ma = max(s(:));
% for t = 1:T
%     img = s(:,:,t);
%     img = uint16(((img-mi)/(ma-mi))*255); 
%     s_16bit = cat(3, s_16bit, img);
% end
% s = s_16bit;


for t = 1:T    
    img = s(:,:,t);
    mi = min(img(:));
    ma = max(img(:));
    img = (img-mi)/(ma-mi); 
    s(:, :, t) = img;
end

sum_s = sum(s,3);
ind = (sum_s==0); sum_s(ind) = 1;
s = s./repmat(sum_s,1,1,T);


imwrite(s(:,:,1)*255, ['.', filesep, 's.tif']);
for t = 2:T
    imwrite(s(:,:,t)*255, ['.', filesep, 's.tif'], 'WriteMode','append');
end


%% Cumulative distribution over Intensity for each pixel (x,y)

s_sorted = sort(s,3);

cdf_s = s_sorted(:,:,1);
for t = 2:T
    cdf_s(:,:,t) = sum(s_sorted(:,:, 1:t), 3);
end


%% Calculate percentiles
t = [1:1:100];
kT = round(t/100*T);
% percentiels = zeros(size(cdf_s,1), size(cdf_s,2), numel(kT));
percentiels = cdf_s(:,:, kT);


%% normalization of the gray values

ns = [];
for t=1:T
   % values of the signal at the time t 
   v = double(s(:,:,t));
   % compare with the values of percentiels
   diff = abs(repmat(v, 1,1, numel(kT))- double(percentiels));
   [~, min_diff_pos] = min(diff, [], 3);
   
   ns = cat(3, ns, min_diff_pos);
    
end


%%

% mi = min(ns(:));
% ma = max(ns(:));
ns_8bit = [];
for t = 1:T
    img = ns(:,:,t);
    mi = min(img(:));
    ma = max(img(:));
    img = uint8(255*(img-mi)/(ma-mi)); 
    ns_8bit = cat(3, ns_8bit,img);
end

ns = ns_8bit;
%% assign gray values to percentiel values and save result

imwrite(ns(:,:,1), ['.', filesep, 'ns.tif']);
% imwrite(I, ['.', filesep, 'ns', filesep, sprintf('frame-%05d.jpg', 1)]);
for t = 2:T
    imwrite(ns(:,:,t), ['.', filesep, 'ns.tif'], 'WriteMode','append');
%     imwrite(I, ['.', filesep, 'ns', filesep, sprintf('frame-%05d.jpg', t)]);
end




   


 
