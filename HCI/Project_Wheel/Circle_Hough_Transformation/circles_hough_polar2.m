function [ circles ] = circles_hough_polar( img, cyRange, cxRange, rRange, ncircles )
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

H = zeros (cy_max-cy_min+1, cx_max-cx_min+1);

if (cx_max-cx_min > cy_max-cy_min)
    c = 1; 
else
    c = 2;
end

[Ix, Iy] = imgradientxy(img);
tan_theta = Iy./Ix;

%% first stage: detect center of circles

   
switch c
%
   case 1   
       for k=1:numel(I)
           y = I(k);        % y-axis
           x = J(k);        % x-axis

           t = tan_theta(y,x);

           for b = cy_min:cy_max
               a = round(-b/t + (x+y/t));
               if (a>=cx_min && a<=cx_max && t~=0)
                  H(b-cy_min+1, a-cx_min+1) = H(b-cy_min+1, a-cx_min+1) + 1;
               end               
           end

       end
%
   case 2
       for k=1:numel(I)
           y = I(k);        % y-axis
           x = J(k);        % x-axis

           t = tan_theta(y,x);

           for a = cx_min:cx_max
               b = round(-t*a + (y+x*t));
               if (b>=cy_min && b<=cy_max && a~=x)
                  H(b-cy_min+1, a-cx_min+1) = H(b-cy_min+1, a-cx_min+1) + 1;
               end
           end 

       end

end

% threshold = 0.55 * max(H(:));
% H = H - threshold;
% H(H<0) = 0;

[picks, pos] = findpeaks(H(:));
[I,J] = ind2sub(size(H), pos);

[picks_sort, pos_sort] = sort(picks, 'descend');

cy = I(pos_sort(1:ncircles));
cx = J(pos_sort(1:ncircles));

cy = cy + repmat(cy_min - 1, size(cy));
cx = cx + repmat(cx_min - 1, size(cx));
   


%% stage 2 : search for radius

display(sprintf('spent time: %f', toc));

end


