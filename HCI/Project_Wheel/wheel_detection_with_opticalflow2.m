addpath(genpath('./Circle_Hough_Transformation'));
% % addpath(genpath('./opticalflow'));
addpath(genpath('/export/home/etikhonc/Documents/Tools/OpticalFlow'));

%% Select video
path = 1;

switch path
    case 1
        path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 4 Cal/4 weeks 0p.o/Naehe Network 1 old/';
        fileName = 'Trial_201411122034231.avi';
        path_to_save = './pathmat1/';
    case 2
        path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 1 Cal/Maus 1 Cal 5d p.o/20.10.2014 Maus 1 Cal 5d p.o. Network 1 old/';
        fileName = 'Trial_20141020191115117.avi';
        path_to_save = './pathmat2/';
    case 3
        path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 1 Cal/Maus 1 Cal 4 weeks p.o. Network 1 imaging 11.11.2014/Network 3';
        fileName = 'Trial_201411111614571.avi';
        path_to_save = './pathmat3/';
    case 4
        path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 5 Cal/4 weeks p.o/Network 3 21.10.2014/';
        fileName = 'Trial_2014102103300061.avi';
        path_to_save = './pathmat4/';
end

%% Section 1; read frames from video

% t1 = tic;
% inframe = extractFramesFromVideo(path, fileName, [1 5 6000]);
% display(sprintf('Extraction of all frames from video took %.3f seconds', toc(t1)))
% 
% [m,n] = size(inframe(:,:,:,1));
% nFrames = size(inframe,4);
% 
% save([path_to_save 'inframe_1002.mat'], 'inframe');

%%

load([path_to_save 'inframe_501.mat']);
nFrames = size(inframe, 4);

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

of_para = [alpha,ratio,minWidth,nOuterFPIterations,nInnerFPIterations,nSORIterations];

flow_x = [];
flow_y = [];

mag = [];
angle = [];

for t = 2:round(nFrames/4)

    next_frame = inframe(:,:,:,t);
    next_frame_edges = double(edge(next_frame, 'canny'));
    
    [vx,vy,warpI2] = Coarse2FineTwoFrames(prev_frame_edges,next_frame_edges, of_para);
    
    prev_frame = next_frame;
    prev_frame_edges = next_frame_edges;
    
    flow_x = cat(3, flow_x, vx);
    flow_y = cat(3, flow_y, vy);
       
    mag = cat(3, mag, sqrt(vx.^2 + vy.^2));
    angle = cat(3, angle, atan2(vy, vx));
end
%%
save([path_to_save 'flow_x_1002_first250.mat'], 'flow_x');
save([path_to_save 'flow_y_1002_first250.mat'], 'flow_y');
% save([path_to_save' flow_x_500.mat'], 'flow_x');
% save([path_to_save' flow_y_500.mat'], 'flow_y');

% save([path_to_save 'flow_x_edges_250.mat'], 'flow_x');
% save([path_to_save 'flow_y_edges_250.mat'], 'flow_y');
% save([path_to_save 'flow_x_edges_500.mat'], 'flow_x');
% save([path_to_save 'flow_y_edges_500.mat'], 'flow_y');


save([path_to_save  'flowmag_1002_first250.mat'], 'mag');
save([path_to_save 'flowangl_1002_first250.mat'], 'angle');
% save([path_to_save  'flowmag_edges_500.mat'], 'mag');
% save([path_to_save 'flowangl_edges_500.mat'], 'angle');

% save([path_to_save  'flowmag_edges_250.mat'], 'mag');
% save([path_to_save 'flowangl_edges_250.mat'], 'angle');
% save([path_to_save  'flowmag_edges_500.mat'], 'mag');
% save([path_to_save 'flowangl_edges_500.mat'], 'angle');


%% Load saved data
%  
load([path_to_save 'flow_x_500_first250.mat']); 
load([path_to_save 'flow_y_500_first250.mat']); 

% % load([path_to_save 'flow_x_500.mat']); 
% % load([path_to_save 'flow_y_500.mat']); 

% % load([path_to_save 'flow_x_edges_250.mat']); 
% % load([path_to_save 'flow_y_edges_250.mat']); 
% 
% % load([path_to_save 'flow_x_edges_500.mat']); 
% % load([path_to_save 'flow_y_edges_500.mat']); 
% %%

load([path_to_save 'flowmag_500_first250.mat']);
load([path_to_save 'flowangl_500_first250.mat']);

% % load([path_to_save 'flowmag_500.mat']);
% % load([path_to_save 'flowangl_500.mat']);

% load([path_to_save 'flowmag_edges_250.mat']);
% load([path_to_save 'flowangl_edges_250.mat']);

% % load([path_to_save 'flowmag_edges_500.mat']);
% % load([path_to_save 'flowangl_500.mat']);


%%
angle = [];

for k = 1:size(flow_x,3)
   vx = flow_x(:,:,k);
   vy = flow_y(:,:,k);
    
   angle = cat(3, angle, atan2(vy, vx));
   
end
% save([path_to_save 'flowangl_500_first250.mat'], 'angle');

%%
[m,n] = size(inframe(:,:,1));

ww = 10;       % half of the window width around pixel
nf = 20;       % average over nf first frames
nbins = 20;

angle_tvotes_val = zeros(m,n);
angle_tvotes_mag = zeros(m,n);

angle_tres = angle;
angle_tres(mag<=sqrt(2)) = NaN;

for i = 1 + ww : m-ww
    for j = 1 + ww : n-ww
        
        if (mag(i,j) > sqrt(2))

            flow_angle_w = angle_tres(i-ww:i+ww, j-ww:j+ww, 1:nf);
        
            [h_counts, h_edges] = histcounts(flow_angle_w(:), nbins);
            h_binwidth = h_edges(2)-h_edges(1);
            
            [sort_bincounts, sortind] = sort(h_counts, 'descend');
            
            maxangle = h_edges(1) + (sortind(1) - 1)*h_binwidth + h_binwidth/2;
            
            angle_tvotes_val(i,j) = maxangle;
            
            angle_tvotes_mag(i,j) = sort_bincounts(1) + sort_bincounts(2);
        
        end % if there is motion
        
    end % j    
end % i


%%
% avg_angle1 = atan2(-avg_angle_y, -avg_angle_x) + pi;

U1 = angle_tvotes_mag.* cos(angle_tvotes_val);
V1 = angle_tvotes_mag.* sin(angle_tvotes_val);

figure, imshow(inframe(:,:,1)), hold on;
quiver(U1,V1), 
hold off;

figure, imagesc(angle_tvotes_mag);

%%



angles_per1 = angle_tvotes_val + pi/2;
angles_per2 = angle_tvotes_val - pi/2;

mask1 = (angles_per1>0 & angles_per1<pi);
mask2 = (angles_per2>0 & angles_per2<pi);

angles_per = angles_per1.*mask1 + angles_per2.*mask2;


U2 = angle_tvotes_mag.* cos(angles_per);
V2 = angle_tvotes_mag.* sin(angles_per);

figure, imshow(inframe(:,:,1)), hold on;
quiver(U2,V2);
title('angles perpendicular');
hold off;


%% look for a big circles (wheel)

Rmin = round(3*n/4)-20;
Rmax = round(3*n/4)+20;

cy_range = [round(5*m/4), round(8*m/4)];  % [1 m]
cx_range = [-round(n/3), round(3*n/4)]; % [1 n]

angle2 = angle;
angle2 = angle2 + pi + pi/2;

circles = circle_detection(angle_tvotes_mag, angles_per, cy_range, cx_range, [Rmin Rmax]);

[x,y] = get_circle_points(inframe(:,:,1), circles);

f1 = figure;
  imshow(inframe(:,:,1)), hold on;
  for i=1:size(x,1)
      plot(x(i,:), y(i,:), 'r--'), hold on;
  end
hold off;

saveas(f1, [path_to_save 'result_voting_edges'], 'jpg');

%% 3 best votes

nbest = 3;

[~, ind] = sort(circles(:,4),'descend');
ind = ind(1:min(nbest, length(ind)) );
circles = circles(ind,:);

[x,y] = get_circle_points(inframe(:,:,1), circles);

f2 = figure ;
  imshow(inframe(:,:,1)), hold on;
  for i=1:size(x,1)
      plot(x(i,:), y(i,:), 'r--'), hold on;
  end
hold off;
saveas(f2, [path_to_save 'result_voting_edges_3best'], 'jpg');