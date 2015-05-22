
addpath(genpath('./Circle_Hough_Transformation'));
% addpath(genpath('./opticalflow'));
addpath(genpath('/export/home/etikhonc/Documents/Tools/opticalFlow'));

path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 4 Cal/4 weeks 0p.o/Naehe Network 1 old/';
fileName = 'Trial_201411122034231.avi';

% path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 1 Cal/Maus 1 Cal 5d p.o/20.10.2014 Maus 1 Cal 5d p.o. Network 1 old/';
% fileName = 'Trial_20141020191115117.avi';

% path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 1 Cal/Maus 1 Cal 4 weeks p.o. Network 1 imaging 11.11.2014/Network 3';
% fileName = 'Trial_201411111614571.avi';

% path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 5 Cal/4 weeks p.o/Network 3 21.10.2014/';
% fileName = 'Trial_2014102103300061.avi';


% %% Section 1; read frames from video
t1 = tic;
nFrames = 100;
inframe = extractFramesFromVideo(path, fileName, [1 1 nFrames]);
display(sprintf('Extraction of %d frames from video took %.3f seconds', nFrames, toc(t1)))

[m,n] = size(inframe(:,:,:,1));
assert( nFrames == size(inframe,4));

%% Calculate optical flow for extracted frames (Lucas-Kanade Algorithm)

windowsize = [21 21];

prev_frame = inframe(:,:,:,1);
[m,n] = size(prev_frame);

prev_frame_edges = edge(prev_frame, 'canny');
img = single(im2bw(prev_frame, 0.8));

[Ix, Iy] = imgradientxy(prev_frame);
% [I,J] = find(prev_frame_edges);

M = sqrt(Ix.^2 + Iy.^2);
M(M<60) = 0;
M = bwmorph(M, 'clean');

[I,J] = find(M);



points2track = [I,J];
nans = NaN * ones(size(points2track,1),1) ;
 
U = [];
V = [];

figure, imshow(prev_frame), hold on;

for t = 2:5
    next_frame = inframe(:,:,:,t);
     
    [locU,locV] = Lucas_Kanade(prev_frame, next_frame, windowsize, points2track);
    
    ind = sub2ind([m,n], points2track(:,1), points2track(:,2));
    
    new_points2track(:,1) = points2track(:,1) + locU(ind);
    new_points2track(:,2) = points2track(:,2) + locV(ind);
    new_points2track = round(new_points2track);
    
    U = cat(3, U, locU);
    V = cat(3, V, locV);
    
    x = [ points2track(:,1) , new_points2track(:,1) , nans ] ;
    y = [ points2track(:,2) , new_points2track(:,2) , nans ] ;
    
    line(y', x', 'Color', 'g'); 
    
    prev_frame = next_frame;
    points2track = new_points2track;
end


% %% Display optical flow
% 
% img = inframe(:,:,:,1);
% 
% M = sqrt(dIx.^2 + dIy.^2);
% 
% tM = M;
% tM(tM<400) = 0;
% 
% tM2 = tM;
% % tM2 = medfilt2(tM, [5, 5]);
% % tM2 = medfilt2(tM, [10,10]);
% 
% img2 = tM2;
% 
% figure, imshow(img2);
% 
% %%
% img2 = tM2;
% 
% img2 = bwmorph(img2,'clean',Inf);
% img2 = bwmorph(img2,'open',Inf);
% img2 = bwmorph(img2,'close',Inf);
% 
% F5 = ones(5);
% % F5(2:4,2:4) = 1;
% img2 = imdilate(img2, F5);
% 
% figure, imshow(img2);
% 
% %%
% labels = bwlabel(img2, 4);
% 
% labels_unique = unique(labels);
% nlabels = numel(labels_unique);
% 
% component_numel = zeros(nlabels,1);
% for l=1:nlabels
%    component_l = (labels== l);
%    
%    component_numel(l) = sum(component_l(:));    
% end
% 
% [~, label_max_cc] = max (component_numel);
% 
% max_cc = (labels== label_max_cc);
% 
% img3 = max_cc;
% figure, imshow(img3);
% 
% img2 = img3;
% 
% 
% %% look for a big circles (wheel)
% % Rmin = round(3*n/4);
% % Rmax = round(3*n/4);
% 
% Rmin = round(3*n/4)-10;
% Rmax = round(3*n/4)+10;
% 
% cy_range = [round(5*m/4), round(8*m/4)];  % [1 m]
% cx_range = [-round(n/3), round(3*n/4)]; % [1 n]
% 
% list_of_circles2 = circles_hough_polar(img2, cy_range, cx_range, [Rmin Rmax],5);
% [x,y] = get_circle_points(img2, list_of_circles2);
% 
% figure 
%   imshow(img2), hold on;
%   for i=1:size(x,1)
%       plot(x(i,:), y(i,:), 'r--'), hold on;
%   end
% hold off;
% %%
% 
% middle = mean(list_of_circles2);
% up = middle; up(3) = up(3) + 180;
% down = middle; down(3) = down(3) - 100;
% 
% list = [down; middle; up];
% [x,y] = get_circle_points(img2, list);
% 
% figure
%   imshow(img), hold on;
%   for i=1:size(x,1)
%       plot(x(i,:), y(i,:), 'b-'), hold on;
%   end
% hold off  

