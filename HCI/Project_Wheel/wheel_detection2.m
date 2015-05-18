
addpath(genpath('./Circle_Hough_Transformation'));
addpath(genpath('./opticalflow'));

path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 4 Cal/4 weeks 0p.o/Network 3 ganz neu/';
fileName = 'Trial_201411122114071.avi';


t1 = tic;
inframe = extractFramesFromVideo(path, fileName, [1 10 6000]);
display(sprintf('Extraction all frames from video took %.3f seconds', toc(t1)))

[m,n] = size(inframe(:,:,:,1));
nFrames = size(inframe,4);

%%
[Ix,Iy] = imgradientxy(inframe(:,:,:,1));

dIx = zeros([m,n]);
dIy = zeros([m,n]);

% img_summary_flow = zeros(m,n);
U = zeros([m,n]);
V = zeros([m,n]);

for t = 2:round(nFrames/2)
    [Ix_t,Iy_t] = imgradientxy(inframe(:,:,:,t) -  inframe(:,:,:,t-1));  

    dIx = dIx +  Ix_t;
    dIy = dIy + Iy_t; 

end


%%
img = inframe(:,:,:,1);

M = sqrt(dIx.^2 + dIy.^2);

tM = M;
tM(tM<300) = 0;

tM2 = medfilt2(tM, [5, 5]);
% tM2 = medfilt2(tM, [10,10]);

img2 = tM2;

figure, imshow(img2);
%%
img2 = bwmorph(img2,'remove');
% img2 = bwmorph(img2,'skel',Inf);

figure, imshow(img2);
%% search for the small circles (rods)
% [m,n] = size(img2);
% Rmin = 10;
% Rmax = 15;
% 
% cy_range = [1 m];
% cx_range = [1 n];
% 
% 
% % list_of_circles = circles_hough(imgEdges, cy_range, cx_range, [Rmin Rmax],10);
% list_of_circles = circles_hough_polar(img2, cy_range, cx_range, [Rmin Rmax]);
% fprintf('Number of detected circles: %d \n', size(list_of_circles,1));
% 
% img_circles_centers = zeros(m,n);
% ind = sub2ind([m,n], list_of_circles(:,1), list_of_circles(:,2));
% img_circles_centers(ind) = 1;
% 
% 
% figure 
%   imshow(img2), hold on;
%   [x,y] = get_circle_points(img2, list_of_circles);
%   for i=1:size(x,1)
%       plot(x(i,:), y(i,:), 'r--'), hold on;
%   end
% hold off  

%% look for a big circles (wheel)
% Rmin = round(3*n/4);
% Rmax = round(3*n/4);

Rmin = round(3*n/4)-10;
Rmax = round(3*n/4);

cy_range = [round(6*m/4), round(7*m/4)];  % [1 m]
cx_range = [-round(n/3) 0]; % [1 n]

list_of_circles2 = circles_hough_polar(img2, cy_range, cx_range, [Rmin Rmax],10);

figure 
  imshow(img2), hold on;
  [x,y] = get_circle_points(img2, list_of_circles2);
  for i=1:size(x,1)
      plot(x(i,:), y(i,:), 'r--'), hold on;
  end
hold off;

%% 
list = mean(list_of_circles2);
figure
  imshow(img), hold on;
  [x,y] = get_circle_points(img2, list);
  for i=1:size(x,1)
      plot(x(i,:), y(i,:), 'b-'), hold on;
  end
hold off  

