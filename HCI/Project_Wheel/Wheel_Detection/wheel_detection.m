%% Detection of wheel given video frames
% same as wheel_detection_test, but runs automatically on all detected
% video frames under given path (pathFrames)

clear all;
close all;
clc;

pathFrames = '/export/home/etikhonc/Documents/Work/Videos/Wheel_frames/';
filename_scircle_centers = 'small_circle_centers_095.mat';

d1 = dir(pathFrames);

% subfolders, depth=1
issub = [d1(:).isdir]; 
nameFolds_d1 = {d1(issub).name}';
nameFolds_d1(ismember(nameFolds_d1,{'.','..'})) = [];

nSub_d1 = size(nameFolds_d1,1); 

for i_d1 = 1: nSub_d1 
   maus_ID = nameFolds_d1{i_d1,1};
   fprintf('%s \n', maus_ID); 
   
   path_d2 = [pathFrames, maus_ID, filesep];
   d2 = dir(path_d2);
   
   % subfolders, depth=2
   issub = [d2(:).isdir]; 
   nameFolds_d2 = {d2(issub).name}';
   nameFolds_d2(ismember(nameFolds_d2,{'.','..'})) = [];
   
   
   for  i_d2 = 1:size(nameFolds_d2)
       
       cal_time = nameFolds_d2{i_d2,1};
       fprintf('-%s \n', cal_time); 
       
       path_d3 = [path_d2, cal_time, filesep];   
       d3 = dir(path_d3);
       
       % subfolders, depth=3
       issub = [d3(:).isdir]; 
       nameFolds_d3 = {d3(issub).name}';
       nameFolds_d3(ismember(nameFolds_d3,{'.','..'})) = [];
       
       for  i_d3 = 1:size(nameFolds_d3)
       
           network_ID = nameFolds_d3{i_d3,1};
           fprintf('--%s \n', network_ID);

           path_d4 = [path_d3, network_ID, filesep];
           d4 = dir(path_d4);
           
           % subfolders with frames, depth=4
           issub = [d4(:).isdir]; 
           nameFolds_frames = {d4(issub).name}';
           nameFolds_frames(ismember(nameFolds_frames,{'.','..'})) = [];
           
           nFramesets = size(nameFolds_frames,1);
           
           % for each set of frames
           for j = 1:nFramesets
               videoName = nameFolds_frames{j,1};
               videoName = videoName(8:end);
               
               fprintf('---%s ...', videoName);
                              
               % Create new folder 'results' if it doesn't exist already.
               if ~exist([path_d4, 'frames_', videoName, filesep, 'results'], 'dir')
                   mkdir([path_d4, 'frames_', videoName, filesep, 'results']);
               end
              
               filename_scircle_centers = [ videoName, '_small_circle_centers_095.mat'];
                
%                t1 = tic;
%                detection_small_circle_centers(path_d4, videoName, filename_scircle_centers);
%                fprintf(' %0.3fsec ', toc(t1));
               
               t2 = tic;
               fitting_big_circle(path_d4, maus_ID, cal_time, network_ID, videoName, filename_scircle_centers);
               fprintf('  %0.3fsec ', toc(t2));
               
               fprintf(' finished \n');
           end
           
       end
       
   
       
   end
   
   
end



