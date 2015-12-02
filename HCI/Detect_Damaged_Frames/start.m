clc; clear;
%% path to the folder with the tif videos
folder =  'new';
% folder =  'new_03092015/15_09_03_animal1_CaI_Base_Net1_14_09_11/';

path_in = ['..', filesep, 'brain_data', filesep, 's_ns', filesep, folder, filesep];
path_out = ['..', filesep, 'brain_data', filesep, 'damaged', filesep, folder, filesep];

d = dir([path_in, '*.tif']);
filenames = {d.name}';
nSignals = size(filenames,1);


for i = 1:nSignals
    sname = filenames{i};
    sname = sname(1:end-4);
    
    fprintf('%s\n', sname);
    
    original_signal = importdata([path_in, sname, '.mat']);
    [R, C, T] = size(original_signal);
    signal = original_signal;
    
    %% use linear interpolation to replace NaN
    NaNval= isnan(original_signal); 
    if sum(NaNval(:))>0
       signal = interpolate(original_signal, NaNval); 
    end
    
    %%
    s_std = sum(signal.^2,3);
    neuron_map = (s_std-min(s_std(:)))./(max(s_std(:))-min(s_std(:)));
    
    %% Neuron segmentation
    [regions, mask] = segmentationLoG(neuron_map);

    %% detect damaged frames
    mask_background = 1 - logical(mask);
    ind_damaged = find_damaged_frames(signal, mask_background);
    
    %% highlight damaged frames
%     
%     % normalize signal to save in tiff-format
%     s = (signal-min(signal(:)))/(max(signal(:))-min(signal(:)));
%     s_RGB = uint8(zeros(R,C,3,T));
%     
%     transp = 0.3;
%     background_damaged = uint8(zeros(R,C,3));
%     background_damaged(:,:,1) =  round(ones(R,C)*255*transp);
% 
%     for t =1:T
%        img = s(:,:,t); 
%        imgRGB = repmat(uint8(255.*img),[1 1 3]);
%        if ismember(t, ind_damaged)
%            imgRGB = imgRGB + background_damaged;
%        end
%        s_RGB(:,:,:,t) = imgRGB;
%     end
    %% cut damaged frames and create savee them separately with few frames around
    
    % normalize signal to save in tiff-format
    mi = min(signal(:));
    ma = max(signal(:));
    s = (signal-mi)/(ma - mi);
    
    Tdamaged = numel(ind_damaged);
    s_damaged = [];
    
    for k = 1:Tdamaged
       t = ind_damaged(k);
       s_cut = s(:,:,max(1,t-3):min(T,t+3)); 
       s_damaged = cat(3, s_damaged, s_cut);
       s_damaged = cat(3, s_damaged, false(R,C,3));
    end
    
    save([path_out, sname, '_ind_damaged.mat'], 'ind_damaged');
    writeTiffFile(s_damaged, [path_out, sname, '.tif']);
   
end
