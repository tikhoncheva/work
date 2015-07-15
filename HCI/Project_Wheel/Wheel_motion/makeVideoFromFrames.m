function makeVideoFromFrames(pathFrames, pathVideo, videoName)

addpath(genpath('../../Tools/kamarain_image_alignment/.'));

% extract all frames names
framefiles = dir([pathFrames filesep '*.jpg']) ;    

% read all frames from the order
Nframes = length(framefiles);   
frames = cell(1,Nframes); % cell of the images

writerObj = VideoWriter([pathVideo filesep videoName]);
open(writerObj);

for i=1:Nframes
    currentframename = framefiles(i).name;
    frames{i} = imread([pathFrames filesep currentframename]);
    writeVideo(writerObj,frames{i});
end

close(writerObj);

end