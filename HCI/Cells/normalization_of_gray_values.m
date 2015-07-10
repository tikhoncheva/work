%% read image of two frames
fname_channel1 = '2014_09_10__17_08_13h__channel01';
fname_channel2 = '2014_09_10__17_08_13h__channel02';

% frames of the first channel
dir_ch1 = dir(['.', filesep, fname_channel1]);
issub = [dir_ch1(:).isdir]; 
nameImages1 = {dir_ch1(~issub).name}';

T = size(nameImages1,1);

channel1 = [];
for t = 1:T
    img = im2double(imread(['.', filesep, fname_channel1, filesep, nameImages1{t}]));
    channel1 = cat(3, channel1, img);
end


% frames of the second channel
dir_ch2 = dir(['.', filesep, fname_channel2]);
issub = [dir_ch2(:).isdir]; 
nameImages2 = {dir_ch2(~issub).name}';

assert(T == size(nameImages2,1), 'two channels have different signal length');

channel2 = [];
for t = 1:T
    img = im2double(imread(['.', filesep, fname_channel2, filesep,  nameImages2{t}]));
    channel2 = cat(3, channel2, img);
end

%% read offset matrices

offset = dlmread(['.', filesep, 'offset_channel02.txt'], '\t', 1, 0);

% offset_x = offset(:,1);
% offset_y = offset(:,2); 
       
%% align signals
[m,n] = size(channel1(:,:,1));

channel1_aligned = zeros(3*m, 3*n, T);
channel2_aligned = zeros(3*m, 3*n, T);

for t = 1:T
    dx = offset(t,1); dy = offset(t,2);
    channel1_aligned(m+1+dy:2*m+dy, n+1+dx:2*n+dx, t) = channel1(:,:,t);
    channel2_aligned(m+1+dy:2*m+dy, n+1+dx:2*n+dx, t) = channel2(:,:,t);
end

channel1_aligned = channel1_aligned(m+1:2*m, n+1:2*n, :);
channel2_aligned = channel2_aligned(m+1:2*m, n+1:2*n, :);
%% create new signal s = channel1_alligned(x,y,t)/channel1_alligned(x,y,t)

% % % s = channel1_aligned;
% % % channel2_aligned(channel2_aligned ==0) = 0.0001;
% % % s = s./channel2_aligned;
% % 
% % s = channel1;
% % channel2(channel2 ==0) = 0.0001;
% % s = s./channel2;
% % 

% frames of the second channel
dir_s = dir(['.', filesep, 'frames_DeltaR_R']);
issub = [dir_s(:).isdir]; 
nameImages = {dir_s(~issub).name}';

T = size(nameImages,1);

s = [];
for t = 1:T
    img = rgb2gray(imread(['.', filesep, 'frames_DeltaR_R', filesep,  nameImages{t}]));
    s = cat(3, s, im2double(img));
end


imwrite(s(:,:,1), ['.', filesep, 's.tif']);
for t = 2:T
    imwrite(s(:,:,t), ['.', filesep, 's.tif'], 'WriteMode','append');
end


%% normalize s to probabilities
sum_s = sum(s,3);
ind = (sum_s==0); sum_s(ind) = 1;
s = s./repmat(sum_s,1,1,T);

%% Cumulative distribution over Intensity for each pixel (x,y)

s_sorted = sort(s,3);

cdf_s = s_sorted(:,:,1);
for t = 2:T
    cdf_s(:,:,t) = sum(s_sorted(:,:, 1:t), 3);
end


%% Calculate percentiles
t = [1:1:100];
kT = round(t/100*T);
precentiels = zeros(size(cdf_s,1), size(cdf_s,2), numel(kT));
percentiels = cdf_s(:,:, kT);


%% normalization of the gray values

ns = double(zeros(size(s)));
for t=1:T
   % values of the signal at the time t 
   v = s(:,:,t);
   % compare with the values of percentiels
   diff = abs(repmat(v, 1,1, numel(kT))- percentiels);
   [~, min_diff_pos] = min(diff, [], 3);
   
   ns(:,:,t) = min_diff_pos;
    
end

%% assign gray values to percentiel values and save result

I = mat2gray(ns(:,:,1));

imwrite(I, ['.', filesep, 'ns.tif']);
% imwrite(I, ['.', filesep, 'ns', filesep, sprintf('frame-%05d.jpg', 1)]);
for t = 2:T
    I = mat2gray(   ns(:,:,t));
    imwrite(I, ['.', filesep, 'ns.tif'], 'WriteMode','append');
%     imwrite(I, ['.', filesep, 'ns', filesep, sprintf('frame-%05d.jpg', t)]);
end




   


 
