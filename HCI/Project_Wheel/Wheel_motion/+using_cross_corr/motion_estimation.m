% Estimation of wheel velocity between consecutive frames
%
% Using cross correlation
%
% Input
%   frames        structure with the names of frames (result of dir-function) 
%   wheel_param   parameters of the wheel (coordinates of the center and radius)
%   alpha0        angle to the center of initial path
%   fstart        number of the first frame
%   fstop         number of the last frame
%   fstep         frame rate
% Output
%   vel_wheel     estimated velocity between consecutive frames
%                 matrix nFramePairs x 4
%                 1 column -  index of the first frame in pair
%                 2 column -  index of the second frame in pair
%                 3 column -  estimated velocity (rad/frame)
%                 4 column -  similarity value of the correlation function
%

function [vel_wheel] = motion_estimation(frames, wheel_param, alpha0, fstart, fstop, fstep, frameDir)
  
   fprintf('===========================================================\n');
   fprintf(' Motion estimation\n');
   fprintf('===========================================================\n');
   
   % wheel parameters
   x0 = wheel_param(1); y0 = wheel_param(2); R = wheel_param(3);
   
   %size of the frames
   [m,n] = size(imread([frameDir, frames(1).name]));
   
   % initial a patch location on the first frame in each pair
   rx = round(x0 + R* cos(alpha0));
   ry = round(y0 - R* sin(alpha0));
   rect = [rx-100, ry-50, 300, 100];
   
   % coordinates of points inside the patch % number of points
   x = [rect(1):rect(1)+rect(3)];
   y = [rect(2):rect(2)+rect(4)];
   sel_points = [repmat(x',numel(y),1), kron(y',ones(numel(x),1)) ];
   sel_points_ind = sub2ind([m,n], sel_points(:,2), sel_points(:,1));
   nP = size(sel_points,1);
   

   % range of possible wheel rotation angles 
   tmin = -0.05;
   tmax = 0.05;
   tstep = 0.0005;
   nIt_theta = floor((tmax-tmin)/tstep)+1;
   
   % angle between wheel center and selected points
%    alpha = atan2( y0*ones(nP,1) - sel_points(:,2), ... 
%                   sel_points(:,1) - x0*ones(nP,1))';   % 1xn row vector

   alpha = atan2( y0 - ry, rx - x0)';   % 1xn row vector   
   
   nFramePairs = floor((fstop-1-fstart)/fstep)+1;  % number of frame pairs  
   vel_wheel = zeros(nFramePairs,4);  % vector of estimated wheel velocity between two consecutive frames   
   
   for i= fstart:fstep:fstop-1
        t1 = tic;
        
        it = (i-fstart)/fstep; 
        fprintf('   iteration %d from %d \t', (i-fstart)/fstep, nFramePairs);
        
        frame = im2double(imread([frameDir, frames(i).name]));        % first frame in the pair
        frame_next = im2double(imread([frameDir, frames(i+1).name])); % second frame in the pair
        
        % shift selected points in frame according to the wheel motion theta
        X = zeros( nIt_theta, nP); % gray values of moved patch from the first frame
        Y = zeros( nIt_theta, nP); % gray values of corresponding region on the second frame
        ix = 1;
        
                    figure;
            imshow(imcrop(frame, rect))
        
        for theta = tmin:tstep:tmax
            
            % center coordinates of the moved patch
            rx_new = round( rx + theta*R*sin(alpha));
            ry_new = round( ry + theta*R*cos(alpha));
            % moved patch
            rect_new = [rx_new-100, ry_new-50, 300, 100];
            % coordinates of points inside the new patch
            x = [rect_new(1):rect_new(1)+rect_new(3)];
            y = [rect_new(2):rect_new(2)+rect_new(4)];
            sel_points_new = [repmat(x',numel(y),1), kron(y',ones(numel(x),1)) ];
            sel_points_new_ind = sub2ind( [m,n], sel_points_new(:,2), sel_points_new(:,1) );
            
            sel_points_preserved = sel_points;
            sel_points_preserved_ind = sub2ind( [m,n], sel_points_preserved(:,2), sel_points_preserved(:,1) );            
            
            figure;
            imshow(imcrop(frame_next, rect_new))
%             M(1,:) = theta*R*sin(alpha);
%             M(2,:) = theta*R*cos(alpha);
            
%             sel_points_new(:,1) = round(sel_points(:,1) +  M(1,:)'); % x axis
%             sel_points_new(:,2) = round(sel_points(:,2) +  M(2,:)'); % y axis
            
            
%             rows_to_del = (sel_points_new(:,1)>n | sel_points_new(:,1)<1) ...
%                         | (sel_points_new(:,2)>m | sel_points_new(:,2)<1);
%             sel_points_new(rows_to_del,:) = [];
%             sel_points_new_ind = sub2ind( [m,n], sel_points_new(:,2), sel_points_new(:,1) );
            
%             sel_points_preserved = sel_points(); sel_points_preserved(rows_to_del,:) = [];
%             sel_points_preserved_ind = sub2ind( [m,n], sel_points_preserved(:,2), sel_points_preserved(:,1) );
            

            X(ix, :) = frame(sel_points_preserved_ind);
            Y(ix, :) = frame_next(sel_points_new_ind);
%             X(ix, ~rows_to_del) = frame(sel_points_preserved_ind);
%             Y(ix, ~rows_to_del) = frame_next(sel_points_new_ind);
            ix = ix + 1;
            
            clear sel_points_new; clear sel_points_preserved;
            clear sel_points_new_ind; clear sel_points_preserved_ind;
        end  
        
        % calculate cross correlation between pairs of patches
        corr_coeff = corr(X', Y');
        corr_coeff = corr_coeff(1:size(corr_coeff,1)+1:end);
        
        [max_corr, max_corr_pos] = max(corr_coeff);  % take the best one

        theta = tmin + (max_corr_pos-1)*tstep;       % save corresponding rotation angle
        vel_wheel(it+1,:) = [i, i+1, theta, max_corr];
        
        fprintf(' %5.2f sec\n', toc(t1));
        
   end

   fprintf('===========================================================\n');

end