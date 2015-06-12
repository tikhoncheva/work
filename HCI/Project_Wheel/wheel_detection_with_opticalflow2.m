addpath(genpath('./Circle_Hough_Transformation'));
% % addpath(genpath('./opticalflow'));
addpath(genpath('/export/home/etikhonc/Documents/Tools/OpticalFlow'));


addpath('/export/home/etikhonc/Documents/Tools/edges-master/');
addpath(genpath('/export/home/etikhonc/Documents/Tools/piotr_toolbox_V3.26/'));


%% Select video
path = 1;

load_frames = true;
load_opticalflow = false;

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

%% Section 1: read frames from video

if (~load_frames)
    t1 = tic;
    inframe = extractFramesFromVideo(path, fileName, [1 5 6000]);
    display(sprintf('Extraction of all frames from video took %.3f seconds', toc(t1)))

    nFrames = size(inframe,4);

    save([path_to_save 'inframe_1002.mat'], 'inframe');

else
    load([path_to_save 'inframe_501.mat']);
    nFrames = size(inframe, 4);

end

%% Section 2: Calculate optical flow for extracted frames (Lucas-Kanade Algorithm)

if (~load_opticalflow)
    
    prev_frame = inframe(:,:,:,1);
    prev_frame_edges = double(edge(prev_frame, 'canny'));

    nf = 51;
    
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

    for t = 2:nf %round(nFrames/2)

        next_frame = inframe(:,:,:,t);
        next_frame_edges = double(edge(next_frame, 'canny'));

        [vx,vy,warpI2] = Coarse2FineTwoFrames(prev_frame_edges,next_frame_edges, of_para);
%         [vx,vy,warpI2] = Coarse2FineTwoFrames(prev_frame, next_frame, of_para);

        prev_frame = next_frame;
        prev_frame_edges = next_frame_edges;

        flow_x = cat(3, flow_x, vx);
        flow_y = cat(3, flow_y, vy);

        mag = cat(3, mag, sqrt(vx.^2 + vy.^2));
        angle = cat(3, angle, atan2(vy, vx));
    end

    save([path_to_save 'flow_x_500_edges_first50.mat'], 'flow_x');
    save([path_to_save 'flow_y_500_edges_first50.mat'], 'flow_y');

    save([path_to_save  'flowmag_500_edges_first50.mat'], 'mag');
    save([path_to_save 'flowangl_500_edges_first50.mat'], 'angle');

else % Load already saved datas


    load([path_to_save 'flow_x_500_edges_first250.mat']); 
    load([path_to_save 'flow_y_500_edges_first250.mat']); 

    load([path_to_save 'flowmag_500_edges_first250.mat']);
    load([path_to_save 'flowangl_500_edges_first250.mat']);

end


%% Section 3: Direction of the flow
[m,n] = size(inframe(:,:,1));

ww = 10;       % half of the window width around pixel
nf = 50;       % consider nf first frames
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
            val1 = sort_bincounts(1);
            
            
            maxangle_oppositedir = mod(maxangle + pi, 2*pi);
            val2 = 0;
            if (maxangle_oppositedir>h_edges(1) && maxangle_oppositedir<h_edges(end))
                binnum = floor( (maxangle_oppositedir-h_edges(1))/h_binwidth) + 1;
                val2 = h_counts(binnum) ;
            end
            
            angle_tvotes_val(i,j) = maxangle;
            angle_tvotes_mag(i,j) = val1 + val2;
        
        end % if there is motion
        
    end % j    
end % i


%%

U1 = angle_tvotes_mag.* cos(angle_tvotes_val);
V1 = angle_tvotes_mag.* sin(angle_tvotes_val);

f1 = figure; imshow(inframe(:,:,1)), hold on;
quiver(U1,V1), 
hold off;
% saveas(f1, [path_to_save 'angle_tvotes_values'], 'jpg');

f2 = figure; imagesc(angle_tvotes_mag);
saveas(f2, [path_to_save 'angle_tvotes_mag'], 'jpg');


%% Section 4: Direction perpendicular to flow direction

angles_per1 = angle_tvotes_val + pi/2;
angles_per2 = angle_tvotes_val - pi/2;

mask1 = (angles_per1>0 & angles_per1<pi);
mask2 = (angles_per2>0 & angles_per2<pi);

angles_per = angles_per1.*mask1 + angles_per2.*mask2;


U2 = angle_tvotes_mag.* cos(angles_per);
V2 = angle_tvotes_mag.* sin(angles_per);

f3 = figure; imshow(inframe(:,:,1)), hold on;
quiver(U2,V2);
title('angles perpendicular');
hold off;
% saveas(f3, [path_to_save 'voting_directions'], 'jpg');

%% Section 5: Circle detection (Hough Transformation for circles)

Rmin = round(3*n/4-10);
Rmax = round(3*n/4+10);

cy_range = [round(5*m/4), round(8*m/4)];  % [1 m]
cx_range = [-round(n/3), round(3*n/4)]; % [1 n]

angle2 = angle;
angle2 = angle2 + pi + pi/2;

circles = circle_detection(angle_tvotes_mag, angles_per, cy_range, cx_range, [Rmin Rmax]);

[x,y] = get_circle_points(inframe(:,:,1), circles);

f4 = figure;
  imshow(inframe(:,:,1)), hold on;
  for i=1:size(x,1)
      plot(x(i,:), y(i,:), 'r--'), hold on;
  end
hold off;
% saveas(f4, [path_to_save 'detection_wheel_3dclustering'], 'jpg');

%% Section 6: best votes

nbest = 2;

[~, ind] = sort(circles(:,4),'descend');
ind = ind(1:min(nbest, length(ind)) );
circles_best = circles(ind,:);

[x_best,y_best] = get_circle_points(inframe(:,:,1), circles_best);

f5 = figure ;
  imshow(inframe(:,:,1)), hold on;
  for i=1:size(x_best,1)
      plot(x_best(i,:), y_best(i,:), 'r--'), hold on;
  end
hold off;
% saveas(f5, [path_to_save 'detection_wheel_best_3dclustering'], 'jpg');