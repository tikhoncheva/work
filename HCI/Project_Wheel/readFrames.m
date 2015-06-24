%% Detection of wheel given video frames
% same as wheel_detection_test, but runs automatically on all detected
% video frames under given path (pathFrames)

clear all;
close all;
clc;

pathVideos = '/export/home/etikhonc/Documents/Work/Videos/Wheel/';
pathFrames = '/export/home/etikhonc/Documents/Work/Videos/Wheel_frames/';

d1 = dir(pathVideos);

% subfolders, depth=1
issub = [d1(:).isdir]; 
nameFolds_d1 = {d1(issub).name}';
nameFolds_d1(ismember(nameFolds_d1,{'.','..'})) = [];

nSub_d1 = size(nameFolds_d1,1); 

for i_d1 = 1:nSub_d1 
   maus_ID = nameFolds_d1{i_d1,1};
   fprintf('%s \n', maus_ID); 
   
   path_d2v = [pathVideos, maus_ID, filesep];
   d2 = dir(path_d2v);
   
   % subfolders, depth=2
   issub = [d2(:).isdir]; 
   nameFolds_d2 = {d2(issub).name}';
   nameFolds_d2(ismember(nameFolds_d2,{'.','..'})) = [];
   
   
   for  i_d2 = 1:size(nameFolds_d2)
       
       cal_time = nameFolds_d2{i_d2,1};
       fprintf('-%s \n', cal_time); 
       
       path_d3v = [path_d2v, cal_time, filesep];   
       d3 = dir(path_d3v);
       
       % subfolders, depth=3
       issub = [d3(:).isdir]; 
       nameFolds_d3 = {d3(issub).name}';
       nameFolds_d3(ismember(nameFolds_d3,{'.','..'})) = [];
       
       for  i_d3 = 1:size(nameFolds_d3)
       
           network_ID = nameFolds_d3{i_d3,1};
           fprintf('--%s \n', network_ID);

           path_d4v = [path_d3v, network_ID, filesep];
           d4 = dir(path_d4v);
           
           % subfolders with frames, depth=4
           nameVideos_frames = {d4.name}';
           nameVideos_frames(ismember(nameVideos_frames,{'.','..'})) = [];
           
           nVideos = size(nameVideos_frames,1);
           
           % for each video
           for j = 1:1 % nVideos
               
               videoName = nameVideos_frames{j,1};
               videoName = videoName(1:end-4);
               
               fprintf('---%s ...', videoName);
               
               tic;
               path_d4f = [pathFrames, maus_ID, filesep, cal_time, filesep, network_ID, filesep];
               nFrames = extractFramesFromVideo( path_d4v, path_d4f, videoName);
               fprintf('  %d frames in %0.3fsec ', nFrames, toc);
               
               fprintf(' finished \n');
           end
           
       end
       
   
       
   end
   
   
end



