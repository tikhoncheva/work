
addpath(genpath('./Circle_Hough_Transformation'));
addpath(genpath('./opticalflow'));

path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 4 Cal/4 weeks 0p.o/Naehe Network 1 old/';
fileName = 'Trial_201411122034231.avi';

inframe = extractFramesFromVideo(path, fileName, [1 1 1000]);

[m,n] = size(inframe{1});
nFrames = size(inframe);

%%
[Ix,Iy] = imgradientxy(inframe{1});

dIx = zeros([m,n]);
dIy = zeros([m,n]);

% img_summary_flow = zeros(m,n);
for t = 2:nFrames
    [Ix_t,Iy_t] = imgradientxy(inframe{t} -  inframe{t-1});
    
%     dIx = dIx + (Ix_t - Ix);
%     dIy = dIy + (Iy_t - Iy);
%      

    dIx = dIx +  Ix_t;
    dIy = dIy + Iy_t;
    
%     Ix = Ix_t;
%     Iy = Iy_t;
%     
%     [U, V, I, J] = opticalflow_edgepoints(inframe{t-1}, inframe{t}, [21 21]);
%     for k=1:size(I,1)
%        y = I(k);
%        x = J(k);
%        
%        u = U(y,x);
%        v = V(y,x);
%        
%        img_summary_flow(y + u, x + v) = 1;
%     end
end

% %%
% img2 = dIx+dIy;
% 
% % search for the circles
% [m,n] = size(img2);
% 
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
% %%
% img_circles_centers = zeros(m,n);
% ind = sub2ind([m,n], list_of_circles(:,1), list_of_circles(:,2));
% img_circles_centers(ind) = 1;
% 
% Rmin = round(3*n/4);
% Rmax = round(3*n/4);
% 
% cy_range = [round(3*m/2), round(2*m)];  % [1 m]
% cx_range = [-round(n/3) round(n/3)]; % [1 n]
% 
% list_of_circles2 = circles_hough_polar(img_circles_centers, cy_range, cx_range, [Rmin Rmax], 5);
% 
% figure 
%   imshow(img2), hold on;
%   [x,y] = get_circle_points(img2, list_of_circles2);
%   for i=1:size(x,1)
%       plot(x(i,:), y(i,:), 'r--'), hold on;
%   end
% hold off  
% 
% %%
% 
% figure 
%   imshow(inframe{1}), hold on;
%   [x,y] = get_circle_points(inframe{1}, list_of_circles2);
%   for i=1:size(x,1)
%       plot(x(i,:), y(i,:), 'r--'), hold on;
%   end
% hold off  

