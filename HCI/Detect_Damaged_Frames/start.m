clc; close all; clear;
%% path to the folder with the tif videos

folder =  'new';

path_in = ['..', filesep, 'brain_data', filesep, 's_ns', filesep, folder, filesep];
path_out = ['..', filesep, 'brain_data', filesep, 's_colored', filesep, folder, filesep];

d = dir([path_in, '*.tif']);
filenames = {d.name}';
nSignals = size(filenames,1);

for i = 1: nSignals
    sname = filenames{i};
    sname = sname(1:end-4);
    
    original_signal = importdata([path_in, sname, '.mat']);
    
    [R, C, T] = size(original_signal);
    signal = original_signal;
    
    %% detect damaged frames
    ind_damaged = find_damaged_frames(signal);
    
    %% highlight damaged frames
    
    % normalize signal to save in tiff-format
    s = (signal-min(signal(:)))/(max(signal(:))-min(signal(:)));
    s_RGB = uint8(zeros(R,C,3,T));
    
    transp = 0.3;
    background_damaged = uint8(zeros(R,C,3));
    background_damaged(:,:,1) =  round(ones(R,C)*255*transp);

    for t =1:T
       img = s(:,:,t); 
       imgRGB = repmat(uint8(255.*img),[1 1 3]);
       if ismember(t, ind_damaged)
           imgRGB = imgRGB + background_damaged;
       end
       s_RGB(:,:,:,t) = imgRGB;
    end
    
    writeTiffFile(s_RGB, [path_out, sname, '.tif']);
   
end
