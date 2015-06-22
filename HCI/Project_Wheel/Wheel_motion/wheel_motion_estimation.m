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
% error         model error 

function [theta, error, nIn_x, nIn_y, nIn] = wheel_motion_estimation( points, V, wheel_param, norm)
    
    % (x0, y0) center of the wheel, R - it's center
    x0 = wheel_param(1); y0 = wheel_param(2); R = wheel_param(3);
    
    % number of points
    n = size(points,1);
    
    % calculate angle between points and wheel center
    alpha = atan2( y0*ones(n,1) - points(:,2), ...
                   points(:,1) - x0*ones(n,1))';   % 1xn row vector

    switch norm
        
        case 'l1'
            
            % build model
            theta = [0:0.001:pi/8]'; % mx1 column-vector
            theta = [-theta(end:-1:2); theta];
            
            m = numel(theta); 

            mx = theta * R * sin(alpha);  
            my = theta * R * cos(alpha);

            % find best theta    
            error = sum(abs(mx - repmat(V(1,:), m, 1)), 2) ...
                  + sum(abs(my - repmat(V(2,:), m, 1)), 2);
            
            [min_er, min_pos] = min(error);
            
            theta = theta(min_pos);
            error = min_er;  
            
            error_mx = abs(R*theta*sin(alpha)-V(1,:));
            error_my = abs(R*theta*cos(alpha)-V(2,:));
            
            ind_inliers_x = error_mx<0.001 ;
            ind_inliers_y = error_my<0.001 ;
            
            nIn_x = sum(ind_inliers_x);
            nIn_y = sum(ind_inliers_y);
            nIn = sum(ind_inliers_x & ind_inliers_y);
            
            
        case 'l2'
            
            theta =  (V(1,:)* sin(alpha)' + V(2,:)*cos(alpha)')/R/n;
            
            error_mx = (R*theta*sin(alpha)-V(1,:)).^2;
            error_my = (R*theta*cos(alpha)-V(2,:)).^2;
            
            ind_inliers_x = error_mx<0.001 ;
            ind_inliers_y = error_my<0.001 ;

            nIn_x = sum(ind_inliers_x);
            nIn_y = sum(ind_inliers_y);
            nIn = sum(ind_inliers_x & ind_inliers_y);
            
            error = sum(error_mx) ...
                  + sum(error_my);
        
    end
        
end