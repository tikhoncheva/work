function [ circles ] = circles_hough_polar( img, cyRange, cxRange, rRange )
tic
% size of the image:
[m,n] = size(img);
[I, J] = find(img); % find x,y of edge pixels

r_min = rRange(1);
r_max = rRange(2);

cx_min = cxRange(1);
cx_max = cxRange(2);

cy_min = cyRange(1);
cy_max = cyRange(2);

H = zeros (cy_max-cy_min+1, cx_max-cx_min+1, r_max - r_min + 1);

% [Ix, Iy] = imgradient(img, 'prewitt');


% for edge point of the image img
for k=1:numel(I)
   i = I(k);        % y-axis
   j = J(k);        % x-axis
   
   for r=r_min:r_max
       
       for dtheta = 0:360
           
           theta = dtheta *pi/180;
           cx = round(j - r*cos(theta));
           cy = round(i + r*sin(theta));   
           
           if (cy>=cy_min && cy<=cy_max) && (cx>= cx_min && cx<=cx_max)
              H(cy-cy_min+1, cx-cx_min+1, r-r_min + 1) = H(cy-cy_min+1, cx-cx_min+1, r-r_min+1) + 1;
           end

       end
       
   end
end

% search for picks in the voting matrix
circles = [];
threshold = 0.80 * max(H(:));
% radius = 2;
% mask = zeros(size(H(:, :, 1)));
% mask(1:radius:end, 1:radius:end) = 1;

for r = r_min:r_max
   H_r = H(:, :, r-r_min+1);
%    tmp =  H_r - threshold;
%    [cy,cx] = find(tmp>0);
   [~, indmax] = max(H_r(:));
   [cy, cx] = ind2sub(size(H_r), indmax);
   
   cy = cy + repmat(cy_min - 1, size(cy));
   cx = cx + repmat(cx_min - 1, size(cx));
   
   circles_local = [[cy, cx], repmat(r, size(cy)) ];
   
   circles = [circles; circles_local ];
   
end

% circles = mean(circles,1);

display(sprintf('spent time: %f', toc));

end


