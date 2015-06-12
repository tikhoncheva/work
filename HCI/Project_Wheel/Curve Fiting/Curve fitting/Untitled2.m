clear all;
close all;
clc;

mainDir = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus_1/cal_1_day/Network_1_neu/' ;

videoName = 'Trial_201410151540411';

framesDir = [mainDir, 'frames_', videoName, '/'];

filename = [framesDir, 'results/small_circle_centers_095.mat'];
load(filename);

I = rgb2gray(imread([framesDir,'frame-00001.jpg']));


N = size(points, 1);
nIteration = 3000;

radius_range = 1000:1100; % 800:1300;   (1)500:1250;  (2)500:1250;  (3)500:1050;
x0 = -500:500;
y0 = 1100:2500;
th = 10;

nr = size(radius_range,2);
nx = size(x0, 2);
ny = size(y0, 2);

nInliers = 0; nOutliers = N;
best = [];


for iter=1:nIteration
    ind_r = randperm(nr); ind_x = randperm(nx); ind_y = randperm(ny);
    ind_r = ind_r(1);  ind_x = ind_x(1); ind_y = ind_y(1);
    r = repmat(radius_range(ind_r),[N,1]);
    h = repmat(x0(ind_x),[N 1]);  k = repmat(y0(ind_y),[N,1]);
    
    dist = abs(((points(:,1)-h).^2 + (points(:,2)-k).^2).^(1/2) - r);
    
    nIn  = size(dist(dist<th),1);
    nOut = N- nInliers;
    
    
    if nIn>nInliers
        nInliers = nIn;   nOutliers = N - nIn;
        best = [h(1), k(1), r(1)];
    end
    
end
    
   

r = best(3); x = best(1); y = best(2);


ang=0:0.01:2*pi; 
xp=r*cos(ang);
yp=r*sin(ang);


g1= figure; 
imshow(I); hold on;
plot(x+xp,y+yp,'LineWidth',2.5,'Color','green');
set(gca, 'YDir', 'reverse');  hold on;
title(sprintf( 'Maus 1, 1 day p.o, Network 1 new, %s', videoName));
% axis equal;
hold off;
saveas(g1, [mainDir, 'result_', videoName, '.jpg'])


% g2=figure;
% imshow(I); hold on;
% scatter(points(:,1), points(:,2),'.k');
% set(gca, 'YDir', 'reverse');  hold on;
% plot(x+xp,y+yp,'LineWidth',2.0,'Color','red');
% set(gca, 'YDir', 'reverse');  hold on;


