
clear all;
close all;
clc;

pathVideos = '/export/home/etikhonc/Documents/Work/Videos/Wheel/';
pathFrames = '/export/home/etikhonc/Documents/Work/Videos/Wheel_frames/';
filename_scircle_centers = 'small_circle_centers_095.mat';

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

% for each set of frames
for j = 1:nFramesets
   videoName = nameFolds_frames{j,1};
   videoName = videoName(8:end);

   fprintf('---%s ...', videoName);
   filename = [path_d4, filename_scircle_centers];
   load(filename);

%    t1 = tic;
%    detection_small_circle_centers(path_d4, videoName, filename_scircle_centers);
%    fprintf(' %0.3fsec ', toc(t1));

   t2 = tic;
   fitting_big_circle(path_d4, maus_ID, cal_time, network_ID, videoName, filename_scircle_centers);
   fprintf('  %0.3fsec ', toc(t2));

   fprintf(' finished \n');
end
           



