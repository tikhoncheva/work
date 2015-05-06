%% Detection of circle objects on an image using Hough Transformation
%
% Input: img        image

% Output: img2      image with detected circle structures

function [ img2 ] = circle_detection( datapath, imgName )

datapath = './';
imgName = 'img1.jpg';

% Piotr Dollar toolbox
addpath(genpath('/export/home/etikhonc/Documents/Tools/piotr_toolbox_V3.26/'));

% Edge extraction
addpath(genpath('/export/home/etikhonc/Documents/Tools/edges-master/'));
load /export/home/etikhonc/Documents/Tools/edges-master/edgesModel.mat   % model

% img = imread ([datapath filesep imgName]);      % should be already be in gray scale    
img = imread('coins.png');

% Edge Detection
imgEdges = edge(img ,'canny');                  % extract edges
% figure
%     imshow(imgEdges),colormap(gray), hold off;


% % Piotr Dollar Edge Detector
% rgbImg = repmat(img,[1 1 3]);
% % find edge points
% imgEdges = imresize(edgesDetect(imresize(rgbImg,2), model),0.5);
% 
% highthr = 0.1;
% lowthr = 0.025;
% imgEdges(imgEdges>highthr) = highthr;
% imgEdges(imgEdges<lowthr) = lowthr;
% imgEdges = 1/highthr * imgEdges;


% range of the radius R of circles we are looking for
[m,n] = size(img);
Rmin = max(m,n)/2;
Rmax = max(m,n);

list_of_circles = circles_hough(imgEdges, [1 70]);

fprintf('Number of detected circles: %d \n', size(list_of_circles,1));


figure 
  imshow(imgEdges), hold on;
%   plot(list_of_circles(:,1), list_of_circles(:,2), 'r*');
  [x,y] = circles(img, list_of_circles);
  for i=1:size(x,1)
      plot(x(i,:), y(i,:)), hold on;
  end
hold off  

end

function [x,y] = circles(img, circles)
    [m,n] = size(img);
    k = size(circles,1);    % number of detected circles
    D = 60;
    x = zeros(k,D);
    y = zeros(k,D);
    
    for i=1:k
       cy = circles(i,1);
       cx = circles(i,2);
       r = circles(i,3);
       
       for d=1:D
           theta = d*2*pi/D;
           y(i,d) = min(m, max(1, cx + r * sin(theta) ));
           x(i,d) = min(n, max(1, cy - r * cos(theta) ));
       end
       
    end   
    
end

