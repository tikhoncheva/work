
addpath(genpath('./Circle_Hough_Transformation'));
addpath(genpath('./opticalflow'));

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

%% Calculate time gradient in x and y direction
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
    
%     img1eges = edge(inframe(:,:,:,t-1), 'canny');
%     [I,J] = find(img1eges);
%     
%     [lU,lV] = Lucas_Kanade(inframe(:,:,:,t-1), inframe(:,:,:,t), [21,21], [I,J]);
%     
%     U = U + lU;
%     V = V + lV;
    
   

end


%% 
img = inframe(:,:,:,1);

M = sqrt(dIx.^2 + dIy.^2);

tM = M;
tM(tM<300) = 0;

tM2 = tM;
% tM2 = medfilt2(tM, [5, 5]);
% tM2 = medfilt2(tM, [10,10]);

img2 = tM2;

figure, imshow(img2);

%%

img2 = bwmorph(img2,'clean',Inf);

img2 = bwmorph(img2,'open',Inf);

img2 = bwmorph(img2,'close',Inf);


figure, imshow(img2);

%%
% labels = bwlabel(img2, 4);
% 
% labels_unique = unique(labels);
% 
% % for l=1:numel(labels_unique)
% %    component_l = (labels== l);
% %    figure, imshow(component_l);
% %     
% % end



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

Rmin = round(3*n/4)-25;
Rmax = round(3*n/4);

cy_range = [round(6*m/4), round(7*m/4)];  % [1 m]
cx_range = [-round(n/3) 0]; % [1 n]

list_of_circles2 = circles_hough_polar(img2, cy_range, cx_range, [Rmin Rmax],10);
[x,y] = get_circle_points(img2, list_of_circles2);

figure 
  imshow(img2), hold on;
  for i=1:size(x,1)
      plot(x(i,:), y(i,:), 'r--'), hold on;
  end
hold off;
%%

middle = mean(list_of_circles2);
up = middle; up(3) = up(3) + 180;
down = middle; down(3) = down(3) - 100;

list = [down; middle; up];
[x,y] = get_circle_points(img2, list);

figure
  imshow(img), hold on;
  for i=1:size(x,1)
      plot(x(i,:), y(i,:), 'b-'), hold on;
  end
hold off  

