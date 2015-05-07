function [frames] = extractFramesFromVideo( path, videoName)


[~, videoNameWithoutExt, ~] = fileparts(videoName);

mkdir(sprintf(['frames' filesep videoNameWithoutExt]))

mov = VideoReader([path filesep videoName]);
nFrames=mov.NumberOfFrames;
d = 200;

frames = cell(round(nFrames/d)+1);
for i = 1:nFrames/d
    img = read(mov,(i-1)*d + 1);
    frames{i} = double(img);
%     % Write out to a JPEG file (img1.jpg, img2.jpg, etc.)
%     imwrite(img,fullfile(output_path,sprintf('img%d.jpg',i)));
end

end