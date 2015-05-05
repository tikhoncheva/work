function extractFramesFromVideo( path, videoName)


[~, videoNameWithoutExt, ~] = fileparts(videoName);
output_path = ['.' filesep 'frames' filesep videoNameWithoutExt];

mkdir(sprintf(['frames' filesep videoNameWithoutExt]))

mov = VideoReader([path filesep videoName]);
% movFrames=read(mov);
nFrames=mov.NumberOfFrames;

for i = 1:100%nFrames
    img = read(mov,i);
    % Write out to a JPEG file (img1.jpg, img2.jpg, etc.)
    imwrite(img,fullfile(output_path,sprintf('img%d.jpg',i)));
end

end