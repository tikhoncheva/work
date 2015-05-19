
addpath(genpath('./Circle_Hough_Transformation'));
% % addpath(genpath('./opticalflow'));
addpath(genpath('/export/home/etikhonc/Documents/Tools/OpticalFlow'));
% 
% path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 4 Cal/4 weeks 0p.o/Naehe Network 1 old/';
% fileName = 'Trial_201411122034231.avi';
% 
% % path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 1 Cal/Maus 1 Cal 5d p.o/20.10.2014 Maus 1 Cal 5d p.o. Network 1 old/';
% % fileName = 'Trial_20141020191115117.avi';
% 
% % path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 1 Cal/Maus 1 Cal 4 weeks p.o. Network 1 imaging 11.11.2014/Network 3';
% % fileName = 'Trial_201411111614571.avi';
% 
% % path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 5 Cal/4 weeks p.o/Network 3 21.10.2014/';
% % fileName = 'Trial_2014102103300061.avi';
% 
% 
% % %% Section 1; read frames from video
% t1 = tic;
% nFrames = 100;
% inframe = extractFramesFromVideo(path, fileName, [1 1 nFrames]);
% display(sprintf('Extraction of %d frames from video took %.3f seconds', nFrames, toc(t1)))
% 
% [m,n] = size(inframe(:,:,:,1));
% assert( nFrames == size(inframe,4));

%% Calculate optical flow for extracted frames (Lucas-Kanade Algorithm)

windowsize = [21 21];

prev_frame = inframe(:,:,:,1);
[m,n] = size(prev_frame);

prev_frame_edges = edge(prev_frame, 'canny');
img = single(im2bw(prev_frame, 0.8));

% set optical flow parameters (see Coarse2FineTwoFrames.m for the definition of the parameters)
alpha = 0.02;          
ratio = 0.5;
minWidth = 20;
nOuterFPIterations = 10;
nInnerFPIterations = 1;
nSORIterations = 30;

para = [alpha,ratio,minWidth,nOuterFPIterations,nInnerFPIterations,nSORIterations];

flow_x = [];
flow_y = [];

figure, imshow(prev_frame), hold on;
for t = 2:5
    next_frame = inframe(:,:,:,t);
    
    [vx,vy,warpI2] = Coarse2FineTwoFrames(prev_frame,next_frame,para);
%     figure;imshow(prev_frame);figure;imshow(warpI2);
    
    prev_frame = next_frame;
    
    flow_x = cat(3, flow_x, vx);
    flow_y = cat(3, flow_y, vy);
    
    rad = sqrt(vx.^2+vy.^2);          
    a = atan2(-vy, -vx)/pi;
    
    quiver(vy, vx);
    
end


% visualize flow field
clear flow;
flow(:,:,1) = vx;
flow(:,:,2) = vy;
imflow = flowToColor(flow);

figure;imshow(imflow);
% imwrite(imflow,fullfile('output',[example '_flow.jpg']),'quality',100);
