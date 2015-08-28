% clc; clear;
%%
pathIn = ['.', filesep, 's_ns', filesep];
sname = '2014_09_10__18_20_38h';
pathOut = ['.', filesep, 'fcol_s', filesep];
%%
fName_tiff = [pathIn, 's_', sname, '.tif'];
info = imfinfo(fName_tiff);
T = numel(info);

s = [];
for k = 1:T
    img = im2double(imread(fName_tiff, k, 'Info', info));
    s = cat(3, s, img);    
end
    
%% no motion frames (mouse video)
load(['mouse_no_motion_frames_', sname, '.mat']); % load no_motion_movement
fr_m_no_motion = [];
for i = 1:size(no_motion_movement,2)
    fr_m_no_motion = [fr_m_no_motion, no_motion_movement{1,i}'];  
end

%% no motion frames (brain video)
r = 5000/375;
fr_b_no_motion = unique(ceil(fr_m_no_motion/r));

%% Cut frames without motion
s_cut = s(:,:, fr_b_no_motion);

%% Normalize signal
% s_mean = mean(s_cut,3);
s_med = median(s_cut,3);

s_norm = s - repmat(s_med,1,1,T);

mi = min(s_norm(:));
ma = max(s_norm(:));
s_norm = (s_norm-mi)/(ma-mi);

%% Color normalized signal in false-colors

s_col = [];
for t = 1:T
    im = s_norm(:,:,t);    
    im_rgb = ind2rgb(round(im*512), jet(512));
    s_col = cat(4, s_col, im_rgb );
end

%%
writeTiffFile(s_col, [pathOut, 's_', sname, '.tif']);
 
%%
% % [m,n] = size(s(:,:,1));    % size of the images
% % blockSize = [5 5];         % block size
% % borderSize = [2 2];        % border size
% %                                      
% % H = []; % histogram of max values in time 
% % for t = 1:T
% %     img = s(:,:,t);
% %     H_t = blockproc(img, blockSize, @(x) mean(x.data(:)), 'BorderSize', borderSize, ...
% %                                          'TrimBorder', false, 'PadPartialBlocks', true);
% %     H = cat(3, H, H_t);                                     
% % end
% % 
% % [M, N] = size(H(:,:,1));
% % nP = M*N;       % number of patches

%% try to find a period of each path
% % E = ones(size(H,1), size(H,2));
% % [I,J] = find(E);
% % 
% % period = zeros(M,N);
% % 
% % for p = 1:nP
% %    i = I(p);
% %    j = J(p);
% %    Pij = reshape(H(i,j,:), 1, T);
% %    ac=xcorr(Pij,Pij);
% %    
% %    [~,locs]=findpeaks(ac);
% %    period(i,j) = mean(diff(locs)*0.1);
% %   
% % end

