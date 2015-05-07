%% Detection of circle objects on an image using Hough Transformation
%
% Input: img        image

% Output: img2      image with detected circle structures

function [ img2 ] = circle_detection( datapath, imgName )

datapath = './';
imgName = 'img3.jpg';

img = imread ([datapath filesep imgName]);      % should be already be in gray scale  

% % apply filter
% h = fspecial('average');
% img = imfilter(img,h,'symmetric');

figure, imshow(edge(img));
% img = imread('coins.png');
[m,n] = size(img);


crop_rect3 = [0, 100, 170, 120];
% crop_rect3 = [0, 100, 170, 183];



crop_rect = crop_rect3;
img1 = imcrop(img, crop_rect);


% img1 = imcrop(img, [175, 0, 125, 120]);
% img = img1;
% 
% Rmin = 18;
% Rmax = 40;

% cy_range = [m, 2*m];  % [1 m]
% cx_range = [-round(n/2) round(n/2)]; % [1 n]

% range of the radius R of circles we are looking for

% R = round(3*n/4);
% 
% Rmin = R;
% Rmax = R;
% 
% cy_range = [m, m + Rmax];
% cx_range = [-round(n/4), 1];

[m1, n1] = size(img1);

Rmin = 3;
Rmax = 5;

cy_range = [1 m1];
cx_range = [1 n1];

% Edge Detection
imgEdges = edge(img1);                  % extract edges
figure, imshow(imgEdges);


% list_of_circles = circles_hough(imgEdges, cy_range, cx_range, [R R]);
list_of_circles = circles_hough_polar(imgEdges, cy_range, cx_range, [Rmin Rmax]);

list_of_circles(:,1) = list_of_circles(:,1) + crop_rect(2);
list_of_circles(:,2) = list_of_circles(:,2) + crop_rect(1);

fprintf('Number of detected circles: %d \n', size(list_of_circles,1));

[~, indmin] = min(list_of_circles(:,3));

% add_circle1 = list_of_circles(indmin,:);
% add_circle2 = list_of_circles(indmin,:);
% add_circle1(3) = add_circle1(3) + 270;
% add_circle2(3) = add_circle2(3) + 80;

% list_of_circles = [list_of_circles; add_circle1];
% list_of_circles = [list_of_circles; add_circle2];


figure 
  imshow(img), hold on;
%   plot(list_of_circles(:,1), list_of_circles(:,2), 'r*');
  [x,y] = circles(img, list_of_circles);
  for i=1:size(x,1)-1
      plot(x(i,:), y(i,:), 'r--'), hold on;
  end
  plot(x(end,:), y(end,:), 'b-');
hold off  

end

function [x,y] = circles(img, circles)
    [m,n] = size(img);
    k = size(circles,1);    % number of detected circles
    D = 90;
    x = zeros(k,D);
    y = zeros(k,D);
    
    for i=1:k
       cy = circles(i,1);
       cx = circles(i,2);
       r = circles(i,3);
       
       for d=1:D
           theta = d*pi/D;
           y(i,d) = min(m, max(1, cy + r * sin(-theta) ));
           x(i,d) = min(n, max(1, cx - r * cos(-theta) ));
       end
       
    end   
    
end

