function [ circles ] = circles_hough( img, cyRange, cxRange, rRange )

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

% for edge point of the image img

    
for k=1:numel(I)
   i = I(k);        % y-axis
   j = J(k);        % x-axis
   
   for r=r_min:r_max
       r2 = r^2;
       xl = max(cx_min,j-r);
       xr = min(cx_max,j+r);
       
       for xi=xl:xr
          
           tmp = round(sqrt(r2-(xi-j)^2)); 
           yl =-tmp + i;
           yu = tmp + i;
           
           if (yl>=cy_min && yl<=cy_max)
               H(yl-cy_min+1, xi-cx_min+1, r-r_min + 1) = H(yl-cy_min+1, xi-cx_min+1, r-r_min+1) + 1;
           end
           if (yu>=cy_min && yu<=cy_max)
               H(yu-cy_min+1, xi-cx_min+1, r-r_min + 1) = H(yu-cy_min+1, xi-cx_min+1, r-r_min+1) + 1;
           end
           
       end
       
   end
end

threshold = 0.80 * max(H(:));

circles = [];
% search for picks in the voting matrix
for r = r_min:r_max
   tmp =  H(:, :, r-r_min+1) - threshold;
   [cy,cx] = find(tmp>0);
   
   cy = cy + repmat(cy_min - 1, size(cy));
   cx = cx + repmat(cx_min - 1, size(cx));
   
   circles_local = [[cy, cx], repmat(r, size(cy)) ];
   circles = [circles; circles_local ];
   
end

display(sprintf('spent time: %f', toc));

end


