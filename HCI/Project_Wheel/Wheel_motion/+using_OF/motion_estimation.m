% Estimation of wheel velocity between consecutive frames

% Using Optical Flow

% Input
%   frames        structure with the names of frames (result of dir-function) 
%   wheel_param   parameters of the wheel (coordinates of the center and radius)
%   margin        width of the area around detected circle
%   OF_param      parameters for the optical flow algorithm

%   fstart        number of the first frame
%   fstop         number of the last frame
%   fstep         frame rate
%   frameDir      path to the folder with frames
% Output
%   vel_wheel     estimated velocity between consecutive frames
%                 matrix nFramePairs x 4
%                 1 column -  index of the first frame in pair
%                 2 column -  index of the second frame in pair
%                 3 column -  estimated velocity (rad/frame)
%                 4 column -  similarity value of the correlation function
%

function [vel_wheel] = motion_estimation(frames, wheel_param, margin, OF_param,  fstart, fstop, fstep, frameDir)
  
   fprintf('===========================================================\n');
   fprintf(' Motion estimation\n');
   fprintf('===========================================================\n');
   
   addpath(genpath('../../../Tools/OpticalFlow'));
   
   % wheel parameters
   x0 = wheel_param(1); y0 = wheel_param(2); R = wheel_param(3);
   
   %size of the frames
   [m,n] = size(imread([frameDir, frames(1).name]));
   
   
   % select area around wheels
   points = [repmat((1:m)',n,1), kron((1:n)',ones(m,1)) ];
   rad = sqrt((points(:,1)-y0).^2 + (points(:,2)-x0).^2);
   
   sel_points_ind = ( rad>=R-2*margin & rad<=R-margin);%+margin );
   sel_points = points(sel_points_ind,1:2);
   sel_points(:,[1 2]) = sel_points(:,[2 1]);
   
   % number of frame pairs  
   nFramePairs = floor((fstop-1-fstart)/fstep)+1; 
   
   vel_wheel = zeros(nFramePairs,4);  % vector of estimated wheel velocity between two consecutive frames   
   
   ind_prev = fstart;
   frame_prev = im2double(imread([frameDir, frames(ind_prev).name])); 
   
   eps = 0.0005;  % bound, under which we do not distinguish optical flow
   
   for i= fstart+fstep:fstep:fstop-1
        t1 = tic;
        
        it = (i-fstart)/fstep; 
        fprintf('   iteration %d from %d \t', (i-fstart)/fstep, nFramePairs);
        
        ind_next = i;
        frame_next = im2double(imread([frameDir, frames(i).name]));
        
        % calculate optical flow
        [Vx, Vy] = Coarse2FineTwoFrames(frame_prev, frame_next, OF_param);
        
        Vabs = sqrt(Vx.^2 + Vy.^2);
        Vabs(Vabs<1) = 0; Vx(Vabs<1) = 0; Vy(Vabs<1) = 0;    
        V = [ Vx(sel_points_ind), Vy(sel_points_ind)]';
   
%         [theta, er, ~] = ME( sel_points, V, wheel_param, 'l1');
        [theta, er] = using_OF.greedy_ME( sel_points, V, wheel_param, 'l1');

        if abs(theta)>eps
           frame_prev = frame_next;    
           ind_prev = ind_next;
        end
        
        vel_wheel(it+1,:) = [ind_prev, ind_next, theta, er];
       
        
        fprintf(' %5.2f sec\n', toc(t1));
        
   end

   fprintf('===========================================================\n');

end