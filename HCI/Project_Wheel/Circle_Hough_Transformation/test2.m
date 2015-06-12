%% Detection of circle objects on an image using Hough Transformation
%
% Input: img        image

% Output: img2      image with detected circle structures

function [ img2 ] = circle_detection( datapath, imgName )

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


%%
img1 = img;
[m1, n1] = size(img1);

Rmin = 18;
Rmax = 32;

cy_range = [1 m1];
cx_range = [1 n1];

% Edge Detection
imgEdges = edge(img1);               

%%
[Dx, Dy] = imgradientxy(img);

Gmag = sqrt(Dx.^2 + Dy.^2);
Gdir = atan2(Dy,Dx) + pi;

% U = Gmag.* cos(Gdir);
% V = Gmag.* sin(Gdir);
% 
% figure; imshow(img1), hold on;
% quiver(U,V);
% hold off;


list_of_circles = circle_detection(single(imgEdges), Gdir, cy_range, cx_range, [Rmin Rmax], 10);

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

