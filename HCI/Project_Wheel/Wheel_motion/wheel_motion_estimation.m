%% Estimate motion of the wheel given velocity of the single points
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

function [theta] = wheel_motion_estimation( points, V, wheel_param, norm)
    
    % (x0, y0) center of the wheel, R - it's center
    x0 = wheel_param(1); y0 = wheel_param(2); R = wheel_param(3);
    
    % number of points
    n = size(points,1);
    
    % calculate angle between points and wheel center
    alpha = atan2( y0*ones(n,1) - points(:,2), ...
                   points(:,1) - x0*ones(n,1))';   % 1xn row vector
               
    % build model
    theta = [ 0:0.01:pi/4]'; % 79x1 column-vector
    m = numel(theta); % m =79
    
    mx = theta * R * sin(alpha);  
    my = theta * R * cos(alpha);
    
    M = [mx, my]; % m x (n+n)
    
    % find best theta
    switch norm
        
        case 'l1'
            
            error = 
            
        
        
        case 'l2'
        
    end
        
    
    
    


end