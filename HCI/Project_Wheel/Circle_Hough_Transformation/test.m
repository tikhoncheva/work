%% Detection of circle objects on an image using Hough Transformation
%
% Input: img        image

% Output: img2      image with detected circle structures

function [ img2 ] = circle_detection( datapath, imgName )

datapath = './';
imgName = 'img3.jpg';

% img = imread ([datapath filesep imgName]);      % should be already be in gray scale  


%%
img = imread('coins.png');
% img = imread('coins2.jpg');

N = ndims(img);
if (N == 3) % RGB Image
    img = rgb2gray(img);
    if (isinteger(img))
        img = im2single(img); % If A is an integer, cast it to floating-point
    end
end    
if (N == 1)
    img = im2single(img); % If A is an integer, cast it to floating-point
end

% img = im2single(rgb2gray(img));


% [m,n] = size(img);


% crop_rect3 = [0, 100, 170, 120];
% crop_rect3 = [80, 170, 100, 200];



% crop_rect = crop_rect3;
% img1 = imcrop(img, crop_rect);

% crop_rect = [175, 0, 125, 120];
% img1 = imcrop(img, crop_rect);

%%
img1 = img;
[m1, n1] = size(img1);

Rmin = 18;
Rmax = 30;

cy_range = [1 m1];
cx_range = [1 n1];

% Edge Detection
imgEdges = edge(img1);               

imgEdges1= bwmorph(imgEdges,'bridge',Inf);
% imgEdges1= bwmorph(imgEdges,'clean',Inf);
imgEdges12= bwmorph(imgEdges1,'dilate');
% imgEdges2 = bwmorph(imgEdges,'fill',Inf);


imgEdges = imgEdges12;
% imgEdges = bwmorph(imgEdges12,'thin');
% figure, imshow(imgEdges);

%%
list_of_circles = circles_hough_polar(single(imgEdges), cy_range, cx_range, [Rmin Rmax],10);

fprintf('Number of detected circles: %d \n', size(list_of_circles,1));

%%

figure 
  imshow(img1), hold on;
%   plot(list_of_circles(:,1), list_of_circles(:,2), 'r*');
  [x,y] = get_circle_points(img, list_of_circles);
  for i=1:size(x,1)
      plot(x(i,:), y(i,:), 'r--'), hold on;
  end
hold off  



end

