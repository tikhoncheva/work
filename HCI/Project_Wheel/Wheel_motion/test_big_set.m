
clear all;
close all;
clc;

addpath(genpath('../'));
addpath(genpath('../../Tools/OpticalFlow'));

pathFrames = '/export/home/etikhonc/Documents/Work/Videos/Wheel_frames/';

maus_ID = 'Maus_1';
cal_time = 'cal_1_day';
network_ID = 'Network_1_new';

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


filter = strfind(nameFolds_frames, '_big_set');
emptyCells = cellfun(@isempty, filter);
nameFolds_frames(emptyCells) = [];

nFramesets = size(nameFolds_frames,1);

% set optical flow parameters (see Coarse2FineTwoFrames.m for the definition of the parameters)
alpha = 0.012;
ratio = 0.75;
minWidth = 40;
nOuterFPIterations = 3;
nInnerFPIterations = 1;
nSORIterations = 20;
    
OF_para = [alpha,ratio,minWidth,nOuterFPIterations,nInnerFPIterations,nSORIterations];

%%
% for each set of frames
for j = 1:nFramesets
   videoName = nameFolds_frames{j,1};
   videoName = videoName(8:end);
   videoName_short = videoName(1:end-8);
%     videoName = videoName(8:end);

   fprintf('---%s ... \n', videoName);
   tic;
   
   pathResults = [path_d4, 'frames_', videoName, filesep, 'results', filesep];
   
   % estimated parameters of the wheel
   filename_wheel_coord = [ videoName_short, '_wheel_param.mat'];
   load( [pathResults, filename_wheel_coord]);
   
   x0 = wheel_param(1); y0 = wheel_param(2); R = wheel_param(3);
   fprintf('   wheel center (%d,%d), radius=%d \n', x0, y0, R);
  
    
   % Get list of all jpg files in folder
   frameDir = [path_d4, nameFolds_frames{j,1}, filesep];
   frames = dir([frameDir,'*.jpg']);
   
   nFrames = size(frames,1);

   % % run through all frames and estimate velocity of the wheel
   
   ind_prev = 700;
   frame_prev = im2double(imread([frameDir, frames(ind_prev).name]));  
   [m,n] = size(frame_prev);
     
   % select area around wheels
   margin = 5; 
   
   points = [repmat((1:m)',n,1), kron((1:n)',ones(m,1)) ];
   rad = sqrt((points(:,1)-y0).^2 + (points(:,2)-x0).^2);
   
   sel_points_ind = ( rad>=R-2*margin & rad<=R-margin);%+margin );
   sel_points = points(sel_points_ind,1:2);
   sel_points(:,[1 2]) = sel_points(:,[2 1]);
   
%    mask = false(m,n);
%    mask(sel_points_ind) = true;
%    mframe_prev = frame_prev.* mask;
   
   
   %%
   nStart = ind_prev; nStop = 750; %nFrames;
   step = 1;
   nIterations = floor((nStop-nStart)/step)+1;
   
   vel_wheel_l1_greedy = zeros(nIterations,4);  % vector of estimated wheel velocity between two consecutive frames   
   
   eps = 0.001;
   
   for i= nStart+step:step:nStop
        it = (i-nStart)/step; 
        fprintf('   iteration %d from %d \n', (i-nStart)/step, nIterations);
        
        frame_next = im2double(imread([frameDir, frames(i).name]));
        ind_next = i;
        
        t1 = tic;
        [Vx, Vy] = Coarse2FineTwoFrames(frame_prev, frame_next, OF_para);
        fprintf('     Optical flow: %.03f \n', toc(t1));
        
        Vabs = sqrt(Vx.^2 + Vy.^2);
        Vabs(Vabs<1) = 0; Vx(Vabs<1) = 0; Vy(Vabs<1) = 0;
    
        V = [ Vx(sel_points_ind), Vy(sel_points_ind)]';
   
%         t2 = tic;
%         [theta, er, ~] = wheel_ME( sel_points, V, wheel_param, 'l1');
%         vel_wheel_l1(it+1,:) = [ind_prev, ind_next, theta, er];
%         fprintf('     Velocity estimation: %.03f \n', toc(t2));
        
        t3 = tic;
        [theta1, er1] = greedy_ME( sel_points, V, wheel_param, 'l1');
        fprintf('     Velocity estimation: %.03f \n', toc(t3));

        if abs(theta1)>eps
      
           frame_prev = frame_next;    
           ind_prev = ind_next;
        end
        
        vel_wheel_l1_greedy(it+1,:) = [ind_prev, ind_next, theta1, er1];
        
   end
   
   %%
%    % save estimaed wheel motion (rad/frame) in a file
%    fileID = fopen([pathResults, videoName, '_wheel_motion_l1.txt'],'w'); 
%    fprintf(fileID,'  frame   \t motion (rad/frame) \t summary error in frame \n');
%    fprintf(fileID,'%05d-%05d \t     %2.3f          \t %2.3f \n', vel_wheel_l1(:,1:4)');
%    fclose(fileID);

   % save estimaed wheel motion (rad/frame) in a file
   fileID = fopen([pathResults, videoName, '_wheel_motion_l1_greedy.txt'],'w');
   fprintf(fileID,'  frame   \t motion (rad/frame) \t summary error in frame\n');
   fprintf(fileID,'%05d-%05d \t     %2.3f          \t %2.3f \n', vel_wheel_l1_greedy(:,1:4)');
   fclose(fileID);
    
   fprintf('   finished in %5.2f sec\n', toc);
   
   %%
  
   plot_wheel(frameDir, videoName, frames, nStart, step, wheel_param, vel_wheel_l1_greedy)
    

   
end
           



