function signal = denoiseAlignMerge(signal_channel1, signal_channel2)

addpath(genpath('~/Desktop/IAT_v0.9.1/'));
addpath(genpath('~/Desktop/BM3D/'));

[R C T] = size(signal_channel1);


%% Convert to  double

ma1 = double(max(signal_channel1(:)));
mi1 = double(min(signal_channel1(:)));

ma2 = double(max(signal_channel2(:)));
mi2 = double(min(signal_channel2(:)));

channel1_d = []; channel2_d = [];

for k = 1:T
    img1 = signal_channel1(:,:,k);
    img1_d = (double(img1)-mi1)/(ma1-mi1);
    channel1_d = cat(3, channel1_d, img1_d);

    img2 = signal_channel2(:,:,k);
    img2_d = (double(img2)-mi2)/(ma2-mi2);
    channel2_d = cat(3, channel2_d, img2_d);
end

% Template image
T1 = mean(channel1_d(:, :, 2:end),3);
T2 = mean(channel2_d(:, :, 2:end),3);

%% Align
channel1_aligned = [];
channel2_aligned = [];

% parameters for ECC (Enhanced Correlation Coefficient) and Lucas-Kanade 
% possible transformation types:  {'translation', 'euclidean','similarity','affine','homography'}
transform = 'euclidean';

par = [];
par.levels =    3;
par.iterations = 15;
par.transform = transform;

for i = 1:T
    img1 = channel1_d(:,:,i);
    img2 = channel2_d(:,:,i);

    % Lucas-Kanade algorithm
    [LKWarp1]=iat_LucasKanade(img1, T1, par);
    [LKWarp2]=iat_LucasKanade(img2, T2, par);

    % Compute the warped image and visualize the error
    [img1aligned, ~] = iat_inverse_warping(img1, LKWarp1, par.transform, 1:size(T1,2),1:size(T1,1));
    [img2aligned, ~] = iat_inverse_warping(img2, LKWarp2, par.transform, 1:size(T2,2),1:size(T2,1));

    ind1 = (img1aligned==0); 
    ind2 = (img2aligned==0); 

    img1aligned(ind1) = T1(ind1);
    img2aligned(ind2) = T2(ind2);

    channel1_aligned = cat(3, channel1_aligned, img1aligned);
    channel2_aligned = cat(3, channel2_aligned, img2aligned);
end

%% Denoising
 sigma = 10;
[PSNR, channel1_d_aligned] = VBM3D(channel1_aligned, sigma, 0, 0);
[PSNR, channel2_d_aligned] = VBM3D(channel2_aligned, sigma, 0, 0);

%% create new signal s = channel1_alligned(x,y,t)/channel1_alligned(x,y,t)

s = double(channel1_d_aligned);
ind_div0 = (channel2_d_aligned ==0);
channel2_d_aligned(ind_div0) = 1;
s = s./double(channel2_d_aligned);
% channel2_d_aligned(ind_div0) = 0;


mi = min(s(:));
ma = max(s(:));
s = (s-mi)./(ma-mi);

mi = 0.05;
ma = 0.8;
s = s-mi;
s(s<0)=0;
s = s./(ma-mi);
s(s>1)=1;

signal = s;