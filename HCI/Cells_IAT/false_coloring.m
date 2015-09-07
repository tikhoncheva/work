% clc; clear;
%%
pathData = ['..', filesep, 'brain_data', filesep];

pathS = [pathData, 's_ns', filesep];
path_mouse_no_motion_frames = [pathData, 'mouse_no_motion', filesep];

% sname = '2014_09_10__18_20_38h';
sname = '2014_09_10__18_24_12h';
pathOut = [pathData, filesep, 'fcol_s', filesep];
%%
fName_tiff = [pathS, 's_', sname, '.tif'];
info = imfinfo(fName_tiff);
T = numel(info);

s = [];
for k = 1:T
    img = im2double(imread(fName_tiff, k, 'Info', info));
    s = cat(3, s, img);    
end
    
%% no motion frames (mouse video)
if exist([path_mouse_no_motion_frames, 'mouse_no_motion_frames_', sname, '.mat'], 'file')
    load([path_mouse_no_motion_frames, 'mouse_no_motion_frames_', sname, '.mat']); % load no_motion_movement

    s_mouse_no_motion = [];
    for i = 1:size(no_motion_movement,2)
        s_mouse_no_motion = [s_mouse_no_motion, no_motion_movement{1,i}'];  
    end
    % no motion frames (brain video)
    r = 5000/375;
    ind_s_no_motion = unique(ceil(s_mouse_no_motion/r));
else
    ind_s_no_motion = (1:T);
end



%% Cut frames without motion
s_no_motion = s(:,:, ind_s_no_motion);

%% transform s_no_motion into Fourier space

%% Normalize signal
s_no_motion_med = median(s_no_motion,3);

s_norm = s - repmat(s_no_motion_med,1,1,T);

mi = min(s_norm(:));
ma = max(s_norm(:));
s_norm = (s_norm-mi)/(ma-mi);

%% Color normalized signal in false-colors
S = 0.55;  % shift (-1.<= S <=1.)
F1 = 10;  % spread the image gray values
F2 = 1.; % move the result in the blue or red direction (0.0<F2<=1)
s_col = [];
for t = 1:T
    img = s_norm(:,:,t);    
    img_pr = F1*(img-S);
    img_exp = F2./(1+exp(-img_pr));
    img_rgb = ind2rgb(round(img_exp*512), jet(512));
    s_col = cat(4, s_col, img_rgb );
end

writeTiffFile(s_col, [pathOut, 's_', sname, '.tif']);


