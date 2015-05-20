
addpath(genpath('./Circle_Hough_Transformation'));
% % addpath(genpath('./opticalflow'));
addpath(genpath('/export/home/etikhonc/Documents/Tools/OpticalFlow'));


path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 4 Cal/4 weeks 0p.o/Naehe Network 1 old/';
fileName = 'Trial_201411122034231.avi';

% path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 1 Cal/Maus 1 Cal 5d p.o/20.10.2014 Maus 1 Cal 5d p.o. Network 1 old/';
% fileName = 'Trial_20141020191115117.avi';

% path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 1 Cal/Maus 1 Cal 4 weeks p.o. Network 1 imaging 11.11.2014/Network 3';
% fileName = 'Trial_201411111614571.avi';

% path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 5 Cal/4 weeks p.o/Network 3 21.10.2014/';
% fileName = 'Trial_2014102103300061.avi';


%% Section 1; read frames from video
t1 = tic;
inframe = extractFramesFromVideo(path, fileName, [1 10 6000]);
display(sprintf('Extraction of all frames from video took %.3f seconds', toc(t1)))

[m,n] = size(inframe(:,:,:,1));
nFrames = size(inframe,4);


%% Calculate optical flow for extracted frames (Lucas-Kanade Algorithm)

prev_frame = inframe(:,:,:,1);
[m,n] = size(prev_frame);

prev_frame_edges = double(edge(prev_frame, 'canny'));


% set optical flow parameters (see Coarse2FineTwoFrames.m for the definition of the parameters)
alpha = 0.012;
ratio = 0.75;
minWidth = 20;
nOuterFPIterations = 3;
nInnerFPIterations = 1;
nSORIterations = 20;

para = [alpha,ratio,minWidth,nOuterFPIterations,nInnerFPIterations,nSORIterations];

flow_x = [];
flow_y = [];

% figure, imshow(prev_frame), hold on;
% figure, hold on;

mag = zeros([m,n]);
% imflow_sum = zeros(m,n);

for t = 2:20
    
    next_frame = inframe(:,:,:,t);
    next_frame_edges = double(edge(next_frame, 'canny'));
    
    [vx,vy,warpI2] = Coarse2FineTwoFrames(prev_frame,next_frame, para);
    
    prev_frame = next_frame;
    prev_frame_edges = next_frame_edges;
    
    flow_x = cat(3, flow_x, vx);
    flow_y = cat(3, flow_y, vy);
        
    mag = mag + sqrt(vx.^2+vy.^2);          
    angle = atan2(-vy, -vx)/pi;
    
%     vx(m>3.5) = 0;
%     vy(m>3.5) = 0;

%     clear flow;
%     flow(:,:,1) = vx;
%     flow(:,:,2) = vy;
%     imflow = flowToColor(flow);

%     imflow_sum = imflow_sum + double(rgb2gray(imflow))/255;
%     quiver(vx, vy);
    
end

%%
% visualize flow field
clear flow;
flow(:,:,1) = vx;
flow(:,:,2) = vy;
imflow = flowToColor(flow);
% 
figure;imshow(imflow);
% imwrite(imflow,fullfile('output',[example '_flow.jpg']),'quality',100);
