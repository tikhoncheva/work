
clear all;
close all;
clc;

addpath(genpath('../Wheel_Detection/'));
addpath(genpath('/export/home/etikhonc/Documents/Tools/OpticalFlow'));

pathVideos = '/export/home/etikhonc/Documents/Work/Videos/Wheel/';
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
ratio = 0.85;
minWidth = 20;
nOuterFPIterations = 3;
nInnerFPIterations = 1;
nSORIterations = 20;
    
OF_para = [alpha,ratio,minWidth,nOuterFPIterations,nInnerFPIterations,nSORIterations];


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
   
   ind_prev = 1;
   frame_prev = imread([frameDir, frames(ind_prev).name]);
%    frame_prev = rgb2gray(frame_prev);
   
   [m,n] = size(frame_prev);
     
   % select area around wheels
   margin = 5; 
   
   points = [repmat((1:m)',n,1), kron((1:n)',ones(m,1)) ];
   rad = sqrt((points(:,1)-y0).^2 + (points(:,2)-x0).^2);
   
   sel_points_ind = ( rad>=R-margin & rad<=R+margin );
   sel_points = points(sel_points_ind,1:2);
   sel_points(:,[1 2]) = sel_points(:,[2 1]);
   
   mask = false(m,n);
   mask(sel_points_ind) = true;
   
   mframe_prev = frame_prev.* uint8(mask);
   
   vel_wheel_l1 = zeros(nFrames,4);      % vector of estimated wheel velocity between two consecutive frames
%    vel_wheel_l2 = zeros(1,4);      % vector of estimated wheel velocity between two consecutive frames
   vel_wheel_l1_greedy = zeros(nFrames,4);      % vector of estimated wheel velocity between two consecutive frames
   
   %%
   for i=2:2%nFrames
        fprintf('   iteration %d from %d \n', i, nFrames);
        
        frame_next = imread([frameDir, frames(i).name]);
%         frame_next = rgb2gray(frame_next);
        mframe_next = frame_next.* uint8(mask);
        ind_next = i;
        
        [Vx, Vy] = Coarse2FineTwoFrames(frame_prev, frame_next, OF_para);
        Vabs = sqrt(Vx.^2 + Vy.^2);
        Vabs(Vabs<1) = 0;
        Vx(Vabs<1) = 0;
        Vy(Vabs<1) = 0;
    
        V = [ Vx(sel_points_ind), Vy(sel_points_ind)]';
        
        [theta, er, ~] = wheel_ME( sel_points, V, wheel_param, 'l1');
        vel_wheel_l1(i,:) = [ind_prev, ind_next, theta, er];
        
        [theta1, er1] = greedy_ME( sel_points, V, wheel_param, 'l1');
        vel_wheel_l1_greedy(i,:) = [ind_prev, ind_next, theta1, er1];
        
%         [theta, er, ~] = wheel_ME( sel_points, V, wheel_param, 'l2');
%         vel_wheel_l2(1,:) = [ind_prev, ind_next, theta, er];

        frame_prev = frame_next;
        mframe_prev = mframe_next;
        ind_prev = ind_next;
        
   end
   
   %%
   % save estimaed wheel motion (rad/frame) in a file
   fileID = fopen([pathResults, videoName, '_wheel_motion_l1.txt'],'w');
   
   fprintf(fileID,'  frame   \t motion (rad/frame) \t summary error in frame \n');
   fprintf(fileID,'%05d-%05d \t     %2.3f          \t %2.3f \n', vel_wheel_l1(:,1:4)');
   
   fclose(fileID);
   
%    % save estimaed wheel motion (rad/frame) in a file
%    fileID = fopen([path_d4, videoName, '_wheel_motion_l2.txt'],'w');
%    
%    fprintf(fileID,'  frame   \t motion (rad/frame) \t summary error in frame\n');
%    fprintf(fileID,'%05d-%05d \t     %2.3f          \t %2.3f \n', vel_wheel_l2(:,1:4)');
%    
%    fclose(fileID);

   % save estimaed wheel motion (rad/frame) in a file
   fileID = fopen([pathResults, videoName, '_wheel_motion_l1_greedy.txt'],'w');
   
   fprintf(fileID,'  frame   \t motion (rad/frame) \t summary error in frame\n');
   fprintf(fileID,'%05d-%05d \t     %2.3f          \t %2.3f \n', vel_wheel_l1_greedy(:,1:4)');
   
   fclose(fileID);
    
   fprints('   finished in %5.2f sec', toc);
   %%
%    for i=1:nFrames;
%        frame = imread([frameDir, frames(i).name]); frame = rgb2gray(frame);
%        
%        ang=0:0.01:2*pi;
%        x1 = R*cos(ang); y1 = R*sin(ang);
%        
%        if i==1
%            theta = pi/2;
%        else
%            theta = theta - vel_wheel_l1(i,3);
%        end
%        
%        f1 = figure('Visible', 'off'); 
%        imshow(frame), hold on;
%        plot(x0+x1,y0+y1,'LineWidth',2,'Color','green'), hold on;
%        plot([x0-R*cos(theta), x0+R*cos(theta)], ...
%             [y0+R*sin(theta), y0-R*sin(theta)], 'LineWidth',2,'Color','green'), hold on;
%        plot([x0-R*cos(theta+pi/2), x0+R*cos(theta+pi/2)], ...
%             [y0+R*sin(theta+pi/2), y0-R*sin(theta+pi/2)], 'LineWidth',2,'Color','green'), hold on;
%        plot([x0-R*cos(theta+pi/4), x0+R*cos(theta+pi/4)], ...
%             [y0+R*sin(theta+pi/4), y0-R*sin(theta+pi/4)], 'LineWidth',2,'Color','green'), hold on;
%        plot([x0-R*cos(theta-pi/4), x0+R*cos(theta-pi/4)], ...
%             [y0+R*sin(theta-pi/4), y0-R*sin(theta-pi/4)], 'LineWidth',2,'Color','green'), hold off;
% 
%         
%        saveas(f1, [path_d4, 'output_wheel_motion', filesep,  sprintf('frame-%05d.jpg', i)]);
%        
%    end
       
end
           



