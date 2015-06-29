function plot_wheel(pathFrames, videoName, frames, nStart, step, wheel_param, vel_wheel_l1_greedy)

   x0 = wheel_param(1); y0 = wheel_param(2); R = wheel_param(3);
   
   if isempty(vel_wheel_l1_greedy)
       A = dlmread([pathFrames, 'results/', videoName, '_wheel_motion_l1_greedy_each3frame.txt'], '\t');
       vel_wheel_l1_greedy(:,3) = A(:,3);  
   end
  
   nIterations = size(vel_wheel_l1_greedy, 1);
   theta = pi/2;
   for i=1:nIterations

       frame = imread([pathFrames, frames(nStart + step*(i-1)).name]); 

       ang=0:0.01:2*pi;
       x1 = R*cos(ang); y1 = R*sin(ang);

       if i==1
           theta = pi/2;
       else
           theta = theta - vel_wheel_l1_greedy(i,3);
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


       saveas(f1, [pathFrames, 'results/output_wheel_motion_greedy', filesep,  sprintf('frame-%05d.jpg', nStart + step*(i-1))]);

   end

   makeVideoFromFrames([pathFrames, 'results', filesep, 'output_wheel_motion_greedy'],...
                      [pathFrames, 'results', filesep], ...
                      [videoName, '_wheel_motion_greedy']);

end