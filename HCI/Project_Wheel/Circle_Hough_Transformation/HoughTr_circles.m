function [ H ] = HoughTr_circles( img, img_angles, cyRange, cxRange, rRange)

tic

[I, J] = find(img); % find x,y of edge pixels

r_min = rRange(1);
r_max = rRange(2);

cx_min = cxRange(1);
cx_max = cxRange(2);

cy_min = cyRange(1);
cy_max = cyRange(2);

H = zeros (cy_max-cy_min+1, cx_max-cx_min+1, r_max - r_min + 1);

figure, imshow(img, []), hold on;

for k=1:numel(I)
    
   i = I(k);        % y-axis
   j = J(k);        % x-axis
   
   theta = img_angles(i,j);
   for r=r_min:r_max           

           cx = round(j + r*cos(theta));
           cy = round(i + r*sin(theta));   

           if 0
                plot(j, i , 'r*');
                line([j, cx], [i, cy], 'Color', 'r');
           end

           if (cy>=cy_min && cy<=cy_max) && (cx>= cx_min && cx<=cx_max)
              H(cy-cy_min+1, cx-cx_min+1, r-r_min + 1) = H(cy-cy_min+1, cx-cx_min+1, r-r_min+1) + 1;
           end

%            cx = round(j + r*cos(pi + dtheta));
%            cy = round(i + r*sin(pi + dtheta));       
% 
%            if (cy>=cy_min && cy<=cy_max) && (cx>= cx_min && cx<=cx_max)
%               H(cy-cy_min+1, cx-cx_min+1, r-r_min + 1) = H(cy-cy_min+1, cx-cx_min+1, r-r_min+1) + 1;
%            end

   end % for r
       

end
% hold off;

end


