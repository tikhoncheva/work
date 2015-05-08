%% Detection of circle objects on an image using Hough Transformation
%
% Input: img        image

% Output: img2      image with detected circle structures

function [ img2 ] = circle_detection( datapath, imgName )

datapath = './';
imgName = 'img3.jpg';

% img = imread ([datapath filesep imgName]);      % should be already be in gray scale  

% % apply filter
% h = fspecial('average');
% img = imfilter(img,h,'symmetric');

% figure, imshow(edge(img));

img = imread('coins.png');
[m,n] = size(img);


crop_rect3 = [0, 100, 170, 120];
% crop_rect3 = [0, 100, 170, 183];



% crop_rect = crop_rect3;
% img1 = imcrop(img, crop_rect);

crop_rect = [175, 0, 125, 120];
img1 = imcrop(img, crop_rect);
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
img1 = img;
[m1, n1] = size(img1);

Rmin = 25;
Rmax = 30;

cy_range = [1 m1];
cx_range = [1 n1];

% Edge Detection
imgEdges = edge(img1);                  % extract edges
% figure, imshow(imgEdges);


% list_of_circles = circles_hough(imgEdges, cy_range, cx_range, [Rmin Rmax],10);
list_of_circles = circles_hough_polar(imgEdges, cy_range, cx_range, [Rmin Rmax],10);
% 
% list_of_circles(:,1) = list_of_circles(:,1) + crop_rect(2);
% list_of_circles(:,2) = list_of_circles(:,2) + crop_rect(1);

fprintf('Number of detected circles: %d \n', size(list_of_circles,1));

[~, indmin] = min(list_of_circles(:,3));

% add_circle1 = list_of_circles(indmin,:);
% add_circle2 = list_of_circles(indmin,:);
% add_circle1(3) = add_circle1(3) + 270;
% add_circle2(3) = add_circle2(3) + 80;

% list_of_circles = [list_of_circles; add_circle1];
% list_of_circles = [list_of_circles; add_circle2];


figure 
  imshow(img1), hold on;
%   plot(list_of_circles(:,1), list_of_circles(:,2), 'r*');
  [x,y] = get_circle_points(img, list_of_circles);
  for i=1:size(x,1)
      plot(x(i,:), y(i,:), 'r--'), hold on;
  end
hold off  

end

