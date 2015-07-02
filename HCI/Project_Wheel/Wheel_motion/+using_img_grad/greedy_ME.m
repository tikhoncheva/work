%% Greedy approach for wheel motion estimation
% call ME-function in greeady way
 
% Find best theta, thats fits the model
%    min_theta || M - V||_l1
% where M is the modeled velocity of the given points:
% M = (Mx1y1, Mx2y2, ..., Mxnyn),
%
% Mxy = (mx, my)
% mx = theta * R * sin(alpha)
% my = theta * R * cos(alpha)
%
% Input 
% points        points on the wheel, points = {(x_i,y_i)}, i=1..n,
% V             given velocity of the points, V = (Vx1y1, Vx2y2, ..., Vxnyn)
% wheel_param   parameters of the wheel, (x0, y0, R)
% norm = {'l1','l2'}
%
% Output
% theta         estimated velocity of the wheel
% error         model error 

function [theta, error] = greedy_ME( points, V, wheel_param, norm)

    % (x0, y0) center of the wheel, R - it's center
    x0 = wheel_param(1); y0 = wheel_param(2); R = wheel_param(3);
    
    % number of points
    n = size(points,1);
    
    % calculate angle between points and wheel center
    alpha = atan2( y0*ones(n,1) - points(:,2), ...
                   points(:,1) - x0*ones(n,1))';   % 1xn row vector
    
    % linear velocity of the given points
    Vabs = sqrt(V(1,:).^2 + V(2,:).^2);
    
    ind = false(1, numel(Vabs));
    
    nOut1 = 1;
    nOut2 = 0;
    
    while (nOut1~=nOut2 && nOut2~=n)
        
       [theta, ~, ~] = using_OF.ME( points(~ind,1:2), V(1:2,~ind), wheel_param, norm); 
       
       % Modeled velocity
       M(1,:) = theta*R*sin(alpha);
       M(2,:) = theta*R*cos(alpha);
       Mabs = sqrt(M(1,:).^2 + M(2,:).^2);
       
       ind = (Mabs - Vabs) > 3;
       nOut1 = nOut2;
       nOut2 = sum(ind);
       
       %
%        figure;
%        plot([1:numel(Vabs)], Vabs, 'r.'), hold on;
%        plot([1:numel(Mabs)], Mabs, 'b.'), hold off;
%        xlabel('points');
%        ylabel('linear velocity');
       %
        
    end
    
    % calculate an error
    switch norm
        case 'l1'

            error_x = abs(M(1,:) - V(1,:));
            error_y = abs(M(2,:) - V(2,:));

        case 'l2'

            error_x = (M(1,:)-V(1,:)).^2;
            error_y = (M(2,:)-V(2,:)).^2;
    end
    
    error = sum(error_x) + sum(error_y);

end 