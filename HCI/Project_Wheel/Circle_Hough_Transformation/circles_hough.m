function [ circles ] = circles_hough( img, Rrange )
% size of the image:
[m,n] = size(img);

Rmin = Rrange(1);
Rmax = Rrange(2);

% % quantization of theta = [-90, 90]:
% dtheta = 1;

[imgGrad, imgDir] = imgradient(img, 'prewitt');

[I, J] = find(img); % find x,y of edge pixels

% initialise voting matrix H with zeros
% cy_width = (m+Rmax) - m;
% cx_width = n;    % [-n/2; n/2)
% H = zeros (cy_width, cx_width, Rmax - Rmin);

H = zeros (m, n, Rmax - Rmin + 1);

% for edge point of the image img

for k=1:numel(I)
   i = I(k);        % y-axis
   j = J(k);        % x-axis
   
   theta = imgDir(i,j);
   
   for r=Rmin:Rmax
%        for theta = -90:90
           cx = round(j - r*cos(theta*pi/180));
           cy = round(i + r*sin(theta*pi/180));
       
    %        if (cy>=m && cy<=m+Rmax) && (cx>= -n/2 && cy<n/2)
    %           H(cy,cx,r-Rmin) = H(cx,cy,r-Rmin) + 1;
    %        end
           if (cy>0 && cy<=m) && (cx> 0 && cx<=n)
              H(cy,cx,r-Rmin + 1) = H(cy,cx,r-Rmin+1) + 1;
           end        
%        end
       
   end
end

threshold = 0.8 * max(H(:));

circles = [];
% search for picks in the voting matrix
[mH, nH] = size(H(:, :, 1));
for r = Rmin:Rmax
   tmp =  H(:, :, r) - repmat(threshold, [mH, nH]) ;
   [cy,cx] = find(tmp>0);
   
   circles_local = [[cx,cy], repmat(r, size(cy)) ];
   circles = [circles; circles_local ];
   
end

end


