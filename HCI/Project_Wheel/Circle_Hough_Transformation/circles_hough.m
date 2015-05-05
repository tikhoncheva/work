function [ H ] = circles_hough( img )
% size of the image:
[m,n] = size(img);

% calculate the maximal radius R, i.e. length of the diagonal of the image:
% R = round(sqrt(m^2 + n^2));
R = max(n,m)/2;
% quantization of theta = [-90, 90]:
dtheta = 1;

% [yIndex xIndex] = find(img); % find x,y of edge pixels

% initialise voting matrix H with zeros
a_width = round(m/2+R);
b_width = round(n/2+R);
H = zeros (a_width, b_width, R);

% for each pixal of the image bwImage
for x = 1:m
    for y = 1:n; 
        if img(x,y)~= 0
            % coordinate systems starts in the upper left corner
            for r = 1:R
                for t = 0:180/dtheta; % quantization of pi:
                    theta = t*dtheta*pi/180;
                    
                    a = x - r * cos(theta);
                    b = y + r * sin(theta);
                    
                    H(a,b,r) = H(a,b,r) + 1;
                end
            end
        end
    end
end

end

function [x,y] = lines(img,p)
% size of the image and maximal value of d
[m,n] = size(img);
d_max = 2 * round(sqrt(m^2 + n^2));
% number of detected lines
k = size(p,1);
x = [1,m];
y = zeros(k, 2);
%
for i =1:k
    theta = p(i,2)*pi/180;
    d = (d_max)/2 - p(i,1);
    y(i,:) = x*cos(theta)/sin(theta) + d /sin(theta);
end

end

