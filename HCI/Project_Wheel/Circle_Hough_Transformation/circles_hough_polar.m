function [ circles ] = circles_hough_polar( img, cyRange, cxRange, rRange, varargin )
tic
% size of the image:
[m,n] = size(img);

img = single(im2bw(img, 0.8));

[I, J] = find(img); % find x,y of edge pixels

r_min = rRange(1);
r_max = rRange(2);

cx_min = cxRange(1);
cx_max = cxRange(2);

cy_min = cyRange(1);
cy_max = cyRange(2);

H = zeros (cy_max-cy_min+1, cx_max-cx_min+1, r_max - r_min + 1);

[~, Gdir] = imgradient(img);
Gdir = (Gdir) *pi/180;


% for edge point of the image img
for k=1:numel(I)
   i = I(k);        % y-axis
   j = J(k);        % x-axis
   
   theta = Gdir(i,j);
   
   for r=r_min:r_max
              
       cx = round(j + r*cos(theta));
       cy = round(i - r*sin(theta));   

       if (cy>=cy_min && cy<=cy_max) && (cx>= cx_min && cx<=cx_max)
          H(cy-cy_min+1, cx-cx_min+1, r-r_min + 1) = H(cy-cy_min+1, cx-cx_min+1, r-r_min+1) + 1;
       end


       cx = round(j + r*cos(pi + theta));
       cy = round(i - r*sin(pi + theta));   
        
       if (cy>=cy_min && cy<=cy_max) && (cx>= cx_min && cx<=cx_max)
          H(cy-cy_min+1, cx-cx_min+1, r-r_min + 1) = H(cy-cy_min+1, cx-cx_min+1, r-r_min+1) + 1;
       end

%        end
       
   end
end

% search for picks in the voting matrix
circles = [];
threshold = 0.55 * max(H(:));
% Gaussian filter [5 5] and sigma = 1
G = fspecial('gaussian',[3 3], 0.5);

for r = r_min:r_max
   H_r = H(:, :, r-r_min+1);
   H_r = imfilter(H_r,G,'same');
   
   tmp =  H_r - threshold;
   [cy,cx] = find(tmp>0);
   
   ind = sub2ind(size(tmp), cy, cx);
   
   cy = cy + repmat(cy_min - 1, size(cy));
   cx = cx + repmat(cx_min - 1, size(cx));
   
   circles_local = [[cy, cx], repmat(r, size(cy)), tmp(ind)];
   
   circles = [circles; circles_local ];
   
end

if length(varargin)==1
    ncircles = varargin{1};
    [~, ind] = sort(circles(:,4),'descend');
    ind = ind(1:min(ncircles, length(ind)) );
    circles = circles(ind,:);
end

display(sprintf('spent time: %f', toc));

end


