function [frames] = extractFramesFromVideo( path, videoName, range)

assert(numel(range)==3);

[~, videoNameWithoutExt, ~] = fileparts(videoName);

mkdir(sprintf(['frames' filesep videoNameWithoutExt]))

mov = VideoReader([path filesep videoName]);
nFrames=mov.NumberOfFrames;

istart = range(1);
step = range(2);
istop = min(nFrames, range(3));

% assert(istop<=nFrames, sprintf('Extract Frames From Video: cannot extract more than %d frames', nFrames));

frames = cell(round((istop-istart)/step)+1,1);

for i = 1:round((istop-istart)/step)+1
    img = read(mov,(i-1)*step + 1);
    frames{i} = double(img);
%     % Write out to a JPEG file (img1.jpg, img2.jpg, etc.)
%     imwrite(img,fullfile(output_path,sprintf('img%d.jpg',i)));
end

end