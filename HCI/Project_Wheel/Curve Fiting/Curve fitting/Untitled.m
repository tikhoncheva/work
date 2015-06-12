%function Untitled()

% clear all;
close all;
clc;

imDir = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus_1/cal_1_day/Network_1_neu/frames_Trial_201410151540411/';


images = dir([imDir,'*.jpg']);
nFrame = size(images,1);

res = [];


for fr=1:nFrame
    im1 = rgb2gray(imread([imDir, images(fr).name]));

       
    im_t = adapthisteq(im1);
    im_t = imgaussfilt(im_t, 2);
   
    [centers, radii] = imfindcircles(im_t,[10 15],'ObjectPolarity','dark', 'Sensitivity',0.95);
    
    res = [res; centers];
    
    % % To check detections frame by frame
    % if numel(centers)
    %     for p=1:size(centers,1)
    %         im1 = insertShape(im1, 'circle', [centers(p,:), 1], 'LineWidth', 5,'Color','red');
    %     end
    % end
    %f1=imshow(im1);
    % hold on; 
    %f2=imshow(imflow); set(f2, 'AlphaData',0.4);
    %waitforbuttonpress;
    
    
    fprintf('%d ',fr);
end

fprintf('\n ');



A= round(res);
resMat = zeros(640, 1280);
for i=1:size(A,1)
resMat(A(i,2),A(i,1))=resMat(A(i,2),A(i,1))+1;
end

%subplot(2,1,1); f1=imshow(im1); hold on;  f2=imagesc(resMat); set(f2, 'alphadata',0.5); subplot(2,1,2); imshow(im1);

points = res;
save( [imDir 'results/small_circle_centers_095.mat'], 'points');


