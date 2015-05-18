function [frames] = extractFramesFromVideo( path, videoName, range)

assert(numel(range)==2 || numel(range)==3);

[~, videoNameWithoutExt, ~] = fileparts(videoName);

mkdir(sprintf(['frames' filesep videoNameWithoutExt]))

mov = VideoReader([path filesep videoName]);
display(mov);

nFrames=mov.NumberOfFrames


if numel(range)==2
    range(2) = min(nFrames, range(2));
    frames = readFrame(mov, range);
else
    istart = range(1);
    step = range(2);
    istop = min(nFrames, range(3));

    assert(istop<=nFrames, sprintf('Extract Frames From Video: cannot extract more than %d frames', nFrames));

    frames = [];

    for i = 1:floor((istop-istart)/step)+1
        img = read(mov,(i-1)*step + 1);
        frames = cat(4, frames, img);
    end
    


end

end