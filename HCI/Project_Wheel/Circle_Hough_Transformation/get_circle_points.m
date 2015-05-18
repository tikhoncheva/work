 
function [x,y] = get_circle_points(img, circles)
    [m,n] = size(img);
    k = size(circles,1);    % number of detected circles
    D = 120;
    x = zeros(k,D);
    y = zeros(k,D);
    
    for i=1:k
       cy = circles(i,1);
       cx = circles(i,2);
       r = circles(i,3);
       
       for d=1:D
           theta = d*2*pi/D;
           x(i,d) = min(n, max(1, cx - r * cos(theta) ));
           y(i,d) = min(m, max(1, cy + r * sin(theta) ));
       end
       
    end   
    
end
