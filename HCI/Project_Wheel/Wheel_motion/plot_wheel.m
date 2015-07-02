
function plot_wheel(frames, wheel_param, nStart, step, vel_wheel, pathFrames, Suffix, videoName)
    
   fprintf('===========================================================\n');
   fprintf(' Save frames\n');
   fprintf('===========================================================\n');
   x0 = wheel_param(1); y0 = wheel_param(2); R = wheel_param(3);
   
   if isempty(vel_wheel)
%        A = dlmread([pathFrames, 'results/', videoName, '_wheel_motion_without_OF.txt'], '\t');
       A = dlmread([pathFrames, 'results/', videoName, Suffix, '.txt'], '\t');
       vel_wheel(:,3) = A(:,3);  
   end
   
   % create neccessary folder to save files, if it does not exit
   if ~exist( [pathFrames, 'results/output', Suffix], 'dir')
        mkdir([pathFrames, 'results/output', Suffix]);
   end
   % wheel
   ang=0:0.01:2*pi;
   x1 = R*cos(ang); y1 = R*sin(ang);
   
   nIterations = size(vel_wheel, 1);
   theta = pi/2;
   for i=1:nIterations
       fprintf( 'iteration %d from %d \n', i, nIterations);
       
       frame = imread([pathFrames, frames(nStart + step*(i-1)).name]); 

       if i==1
           theta = pi/2;
       else
           theta = theta - vel_wheel(i,3);
       end

       f1 = figure('Visible', 'off'); 
       imagesc(frame), colormap(gray), hold on;
       plot(x0+x1,y0+y1,'LineWidth',2,'Color','green'), hold on;
       plot([x0-R*cos(theta), x0+R*cos(theta)], ...
            [y0+R*sin(theta), y0-R*sin(theta)], 'LineWidth',2,'Color','green'), hold on;
       plot([x0-R*cos(theta+pi/2), x0+R*cos(theta+pi/2)], ...
            [y0+R*sin(theta+pi/2), y0-R*sin(theta+pi/2)], 'LineWidth',2,'Color','green'), hold on;
       plot([x0-R*cos(theta+pi/4), x0+R*cos(theta+pi/4)], ...
            [y0+R*sin(theta+pi/4), y0-R*sin(theta+pi/4)], 'LineWidth',2,'Color','green'), hold on;
       plot([x0-R*cos(theta-pi/4), x0+R*cos(theta-pi/4)], ...
            [y0+R*sin(theta-pi/4), y0-R*sin(theta-pi/4)], 'LineWidth',2,'Color','green'), hold off;

       saveas(f1, [pathFrames, 'results/output', Suffix, filesep,  sprintf('frame-%05d.jpg', nStart + step*(i-1))]);        
       close all;
   end
   fprintf('===========================================================\n');

   
   fprintf('===========================================================\n');
   fprintf(' Save video\n');
   fprintf('===========================================================\n');   
   
   makeVideoFromFrames([pathFrames, 'results', filesep, 'output', Suffix],...
                      [pathFrames, 'results', filesep], ...
                      [videoName, Suffix]);
   
   fprintf('===========================================================\n');
end