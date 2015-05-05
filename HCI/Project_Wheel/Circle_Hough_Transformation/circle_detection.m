%% Detection of circle objects on an image using Hough Transformation
%
% Input: img        image

% Output: img2      image with detected circle structures

function [ img2 ] = circle_detection( datapath, imgName )

datapath = './';
imgName = 'img1.jpg';

% Piotr Dollar toolbox
addpath(genpath('/export/home/etikhonc/Documents/Tools/piotr_toolbox_V3.26/'));

% Edge extraction
addpath(genpath('/export/home/etikhonc/Documents/Tools/edges-master/'));
load /export/home/etikhonc/Documents/Tools/edges-master/edgesModel.mat   % model

img = imread ([datapath filesep imgName]);      % should be already be in gray scale    

% Edge Detection
imgEdges = edge(img ,'canny');                  % extract edges
figure
    imagesc(imgEdges),colormap(gray), hold off;


% % Piotr Dollar Edge Detector
% rgbImg = repmat(img,[1 1 3]);
% % find edge points
% E = imresize(edgesDetect(imresize(rgbImg,2), model),0.5);
% 
% % apply non-maximum suppression
% % [subs, ~] = nonMaxSupr(double(E), radius, thr);
% % subs = 2*floor(subs/2) + 1;
% 
% highthr = 0.1;
% lowthr = 0.025;
% E(E>highthr) = highthr;
% E(E<lowthr) = lowthr;
% E = 1/highthr * E;
% 
% figure
%     imagesc(E),colormap(gray), hold off;

% Circles detection
voting_matrix = circles_hough(imgEdges);



end

