 
function [x,y] = get_circle_points(img, circles)
    [m,n] = size(img);
    k = size(circles,1);    % number of detected circles
    D = 360;
    x = zeros(k,D);
    y = zeros(k,D);
    
    for i=1:k
       cy = circles(i,1);
       cx = circles(i,2);
       r = circles(i,3);
       
       for d=1:D
           theta = d*2*pi/D;
           xx = cx - r * cos(theta);
           yy = cy + r * sin(theta);
           if (xx>=1 && xx<=n && yy>=1 && yy<=m)
               x(i,d) = xx;
               y(i,d) = yy;
           else
               x(i,d) = NaN;
               y(i,d) = NaN;               
           end
%            x(i,d) = min(n, max(1, cx - r * cos(theta) ));
%            y(i,d) = min(m, max(1, cy + r * sin(theta) ));
       end
       
    end   
    
end
