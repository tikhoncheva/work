
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
   
   fprintf('   Wheel parameters: ');
   filename_wheel_coord = [ videoName, '_wheel_param.mat'];
   load( [path_d4, filename_wheel_coord]);
   
   x0 = wheel_param(1);
   y0 = wheel_param(2);
   R = wheel_param(3);
   
   fprintf('center coordinates (%d,%d), radius=%d', x0, y0, R);
  
    
   % Get list of all jpg files in ornder 
   frameDir = [path_d4, 'frames_', videoName, filesep];
   
   % read all frames 
   load([frameDir 'inframe_501.mat']);
   nFrames = size(inframe, 4);

   % then read all images from the order
   frame_prev = inframe(:,:,:,1);
   [m,n] = size(frame_prev);
   mask = zeros(m,n);
   
   for i=2:2
       
        frame_next = inframe(:,:,:,i);
        
        [vx, vy, ~] = Coarse2FineTwoFrames(frame_prev, frame_next, OF_para);
        
        figure, imshow(frame_prev), hold on;
        quiver(vx, vy);
        
        frame_prev = frame_next;
        
   end

   

end
           



