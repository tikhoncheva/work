% 
% addpath(genpath('./Circle_Hough_Transformation'));
% % % addpath(genpath('./opticalflow'));
% addpath(genpath('/export/home/etikhonc/Documents/Tools/OpticalFlow'));
% 
% % path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 4 Cal/4 weeks 0p.o/Naehe Network 1 old/';
% % fileName = 'Trial_201411122034231.avi';
% % path_to_save = './pathmat1/';

path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 1 Cal/Maus 1 Cal 5d p.o/20.10.2014 Maus 1 Cal 5d p.o. Network 1 old/';
fileName = 'Trial_20141020191115117.avi';
path_to_save = './pathmat2/';

% % path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 1 Cal/Maus 1 Cal 4 weeks p.o. Network 1 imaging 11.11.2014/Network 3';
% % fileName = 'Trial_201411111614571.avi';
% % path_to_save = './pathmat3/';
% 
% % path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 5 Cal/4 weeks p.o/Network 3 21.10.2014/';
% % fileName = 'Trial_2014102103300061.avi';
% % path_to_save = './pathmat4/';
% 
% %% Section 1; read frames from video
% t1 = tic;
% inframe = extractFramesFromVideo(path, fileName, [1 10 6000]);
% display(sprintf('Extraction of all frames from video took %.3f seconds', toc(t1)))
% 
% [m,n] = size(inframe(:,:,:,1));
% nFrames = size(inframe,4);
% 
% save([path_to_save 'inframe.mat'], 'inframe');

%% Calculate optical flow for extracted frames (Lucas-Kanade Algorithm)

load([path_to_save 'inframe.mat']);
nFrames = size(inframe, 4);

%%

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

for t = 2:round(nFrames/2)

    next_frame = inframe(:,:,:,t);
    next_frame_edges = double(edge(next_frame, 'canny'));
    
    [vx,vy,warpI2] = Coarse2FineTwoFrames(prev_frame_edges,next_frame_edges, of_para);
    
    prev_frame = next_frame;
    prev_frame_edges = next_frame_edges;
    
    flow_x = cat(3, flow_x, vx);
    flow_y = cat(3, flow_y, vy);
       
    mag = cat(3, mag, sqrt(vx.^2 + vy.^2));
    angle = cat(3, angle, atan2(-vy, -vx));
end

% save('flow_x2.mat', 'flow_x');
% save('flow_y2.mat', 'flow_y');

save([path_to_save 'flow_x_edges.mat'], 'flow_x');
save([path_to_save 'flow_y_edges.mat'], 'flow_y');

save([path_to_save  'flowmag.mat'], 'mag');
save([path_to_save 'flowangl.mat'], 'angle');


%% Summary flow magnitude

% load([path_to_save 'flowmag.mat']);
% load([path_to_save 'flowangl.mat']);

mag_sum = sum(mag, 3);

%%
mag_sum_edges = edge(mag_sum);
mag_sum_edges = bwmorph(mag_sum_edges, 'close');


%% look for a big circles (wheel)

N = size(mag, 3);

Rmin = round(3*n/4)-20;
Rmax = round(3*n/4)+20;

cy_range = [round(5*m/4), round(8*m/4)];  % [1 m]
cx_range = [-round(n/3), round(3*n/4)]; % [1 n]

angle2 = angle;
angle2 = angle2 + pi + pi/2;

H = zeros (cy_range(2)-cy_range(1)+1, cx_range(2)-cx_range(1)+1, Rmax - Rmin + 1);

for i=1:N;
    
    H_i = HoughTr_circles(mag_sum_edges, angle2(:,:,i), cy_range, cx_range, [Rmin Rmax]);
    
    H = H + H_i;
end

% search for picks in the voting matrix
circles = [];
threshold = 0.55 * max(H(:));
% select 5 most prominent picks for each radius
npicks_per_r = 5;

for r = Rmin:Rmax
   H_r = H(:, :, r-Rmin+1);
   tmp =  H_r - threshold;
   tmp(tmp<0) = 0;
   
   [picks, pos] = findpeaks(tmp(:));
   
   [I,J] = ind2sub(size(H_r), pos);
   
   [picks_sort, pos_sort] = sort(picks, 'descend');
   
   cy = I(pos_sort(1:min(npicks_per_r, numel(picks_sort)) ));
   cx = J(pos_sort(1:min(npicks_per_r, numel(picks_sort)) ));
   
   tmp = picks_sort(1:min(npicks_per_r, numel(picks_sort)) );

   cy = cy + repmat(cy_range(1) - 1, size(cy));
   cx = cx + repmat(cx_range(1) - 1, size(cx));
   
   circles_local = [[cy, cx], repmat(r, size(cy)), tmp];
   
   circles = [circles; circles_local ];
end


[x,y] = get_circle_points(mag_sum_edges, circles);

f1 = figure;
  imshow(inframe(:,:,1)), hold on;
  for i=1:size(x,1)
      plot(x(i,:), y(i,:), 'r--'), hold on;
  end
hold off;
saveas(f1, [path_to_save 'result_voting_edges'], 'jpg');

%% 3 best votes

ncircles = 3;

[~, ind] = sort(circles(:,4),'descend');
ind = ind(1:min(ncircles, length(ind)) );
circles = circles(ind,:);

[x,y] = get_circle_points(mag_sum_edges, circles);

f2 = figure ;
  imshow(inframe(:,:,1)), hold on;
  for i=1:size(x,1)
      plot(x(i,:), y(i,:), 'r--'), hold on;
  end
hold off;
saveas(f2, [path_to_save 'result_voting_edges_3best'], 'jpg');

