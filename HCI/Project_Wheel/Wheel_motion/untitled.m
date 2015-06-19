
clear all;
close all;
clc;

addpath(genpath('../Wheel_Detection/'));
addpath(genpath('/export/home/etikhonc/Documents/Tools/OpticalFlow'));

pathVideos = '/export/home/etikhonc/Documents/Work/Videos/Wheel/';
pathFrames = '/export/home/etikhonc/Documents/Work/Videos/Wheel_frames/';

maus_ID = 'Maus_1';
cal_time = 'cal_5_days';
network_ID = 'Network_1_old';

path_d2 = [pathFrames, maus_ID, filesep];
path_d3 = [path_d2, cal_time, filesep];   
path_d4 = [path_d3, network_ID, filesep];

d4 = dir(path_d4);
           
% subfolders with frames, depth=4
issub = [d4(:).isdir]; 
nameFolds_frames = {d4(issub).name}';
nameFolds_frames(ismember(nameFolds_frames,{'.','..'})) = [];

nFramesets = size(nameFolds_frames,1);

% set optical flow parameters (see Coarse2FineTwoFrames.m for the definition of the parameters)
alpha = 0.012;
ratio = 0.75;
minWidth = 20;
nOuterFPIterations = 3;
nInnerFPIterations = 1;
nSORIterations = 20;
    
OF_para = [alpha,ratio,minWidth,nOuterFPIterations,nInnerFPIterations,nSORIterations];


% for each set of frames
for j = 1:nFramesets
   videoName = nameFolds_frames{j,1};
   videoName = videoName(8:end);

   fprintf('---%s ... \n', videoName);
   
   filename_wheel_coord = [ videoName, '_wheel_param.mat'];
   load( [path_d4, filename_wheel_coord]);
   
   x0 = wheel_param(1); y0 = wheel_param(2); R = wheel_param(3);
   fprintf(' wheel center (%d,%d), radius=%d \n', x0, y0, R);
  
    
   % Get list of all jpg files in folder
   frameDir = [path_d4, 'frames_', videoName, filesep];
   frames = dir([frameDir,'*.jpg']);
   
   nFrame = size(frames,1);

   % % run through all frames and estimate velocity of the wheel
   
   frame_prev = rgb2gray(imread([frameDir, frames(1).name]));
   
   [m,n] = size(frame_prev);
   
   % select area around wheels
   margin = 40; 
   
   points = [repmat((1:m)',n,1), kron((1:n)',ones(m,1)) ];
   rad = sqrt((points(:,1)-y0).^2 + (points(:,2)-x0).^2);
   
   sel_points_ind = ( rad>=R-margin & rad<=R+margin );
   sel_points = points(sel_points_ind,1:2);
   
   mask = false(m,n);
   mask(sel_points_ind) = true;
   
   frame_prev = frame_prev.* uint8(mask);
   
   vel_wheel = [];      % vector of estimated wheel velocity between two consecutive frames

   %%
   for i=2:2
       
        frame_next = rgb2gray(imread([frameDir, frames(i).name]));
        frame_next = frame_next.* uint8(mask);
        
        [vx, vy, ~] = Coarse2FineTwoFrames(frame_prev, frame_next, OF_para);
        
        Vxy = [ vx(sel_points_ind), vy(sel_points_ind)]';
        
        theta = wheel_motion_estimation( sel_points, Vxy, wheel_param);
        
%         vel_wheel =  [vel_wheel; theta];

        frame_prev = frame_next;
        
   end

   
    
end
           



