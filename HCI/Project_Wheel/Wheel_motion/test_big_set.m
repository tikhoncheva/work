
clear all;
close all;
clc;

addpath(genpath('../../Tools/kamarain_image_alignment/.'));
addpath(genpath('../../Tools/OpticalFlow'))

% 
pathFrames = '/export/home/etikhonc/Documents/Work/Videos/Wheel_frames/';
maus_ID = 'Maus_1';
cal_time = 'cal_1_day';
network_ID = 'Network_1_new';

% method
% method = 1;         % using optical flow
 method = 2;         % using cross correlation
% % method = 3;         % using gradients

% go into deep 4 inside the selected folder
path_d2 = [pathFrames, maus_ID, filesep];
path_d3 = [path_d2, cal_time, filesep];   
path_d4 = [path_d3, network_ID, filesep];

d4 = dir(path_d4);
          
% subfolders with frames, depth=4
issub = [d4(:).isdir]; 
nameFolds_frames = {d4(issub).name}';
nameFolds_frames(ismember(nameFolds_frames,{'.','..'})) = [];

filter = strfind(nameFolds_frames, '_Trial_');
emptyCells = cellfun(@isempty, filter);
nameFolds_frames(emptyCells) = [];

% consider ONLY folders with the suffix '_big_set'
filter = strfind(nameFolds_frames, '_big_set');
emptyCells = cellfun(@isempty, filter);
nameFolds_frames(emptyCells) = [];

% number of frame sets
nFramesets = size(nameFolds_frames,1);


% for each set of frames
for j = 1:nFramesets
    
   videoName = nameFolds_frames{j,1};
   videoName = videoName(8:end);
   videoName_short = videoName(1:end-8);

   fprintf('---%s ... \n', videoName);
   
   tic;         % start clock
   
   pathResults = [path_d4, 'frames_', videoName, filesep, 'results', filesep];
       
   % Get list of all jpg files in folder
   frameDir = [path_d4, nameFolds_frames{j,1}, filesep];
   frames = dir([frameDir,'*.jpg']);
   
   % estimated parameters of the wheel
   filename_wheel_coord = [ videoName_short, '_wheel_param.mat'];
   load( [pathResults, filename_wheel_coord]);
   
   % Frames to consider
   fstart = 250;
   fstop = 400; %nFrames;
   fstep = 1;
   
   
   switch method
       
       case 1  % Method 1: using optical flow
            % set optical flow parameters (see Coarse2FineTwoFrames.m for the definition of the parameters)
            alpha = 0.012;
            ratio = 0.75;
            minWidth = 40;
            nOuterFPIterations = 3;
            nInnerFPIterations = 1;
            nSORIterations = 20;

            OF_param = [alpha,ratio,minWidth,nOuterFPIterations,nInnerFPIterations,nSORIterations];

            % width of the area around circle
            margin = 5;

            % run estiomation
            vel_wheel = using_OF.motion_estimation(frames, wheel_param, margin, OF_param, ....
                                                   fstart, fstop, fstep, frameDir);            
            
            Suffix = '_wheel_motion_using_OF';
            
       case 2  % Method 2: using cross correlation
           
            % location of the initial a patch
            alpha0 = 85*pi/180;
%             rx = round(x0 + R* cos(alpha0));
%             ry = round(y0 - R* sin(alpha0));
%             rect = [rx-100, ry-50, 300, 100];
%             figure, imshow(imcrop(imread([frameDir, frames(1).name]), rect));
            
            vel_wheel = using_cross_corr.motion_estimation(frames, wheel_param, alpha0, ...
                                                                  fstart, fstop, fstep, frameDir);
            
            Suffix = '_wheel_motion_using_cross_correlation';
           
       case 3  % Method 3: using image gradient
            
            % width of the area around circle
            margin = 5;

            % run estiomation
            vel_wheel = using_img_grad.motion_estimation(frames, wheel_param, margin,  ....
                                                   fstart, fstop, fstep, frameDir);            
            
            Suffix = '_wheel_motion_using_image_gradient';
            
       otherwise
           fprintf('Error: Select one of three possible methods:\n');
           fprintf(' Method = 1: using optical flow\n');
           fprintf(' Method = 2: using cross correlation\n');
           fprintf(' Method = 3: using image gradient\n\n');
           
   end
  

   % save estimaed wheel motion (rad/frame) in a file
   fileID = fopen([pathResults, videoName, Suffix, '.txt'],'w');
   fprintf(fileID,'  frame   \t motion (rad/frame) \t similarity \n');
   fprintf(fileID,'%05d-%05d \t     %2.3f          \t %2.3f \n', vel_wheel(:,1:4)');
   fclose(fileID);
    
   fprintf('   finished in %5.2f sec\n', toc);  % stop clock
 
  %%
   % save frames with plotted wheel on them
   % create a video
   plot_wheel( frames, wheel_param, fstart, fstep, vel_wheel, frameDir, Suffix, videoName)
    

   
end
           



