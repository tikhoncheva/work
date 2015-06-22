function [nFrames] = extractFramesFromVideo( pathVideo, pathFrames, videoName)

    newFolder = ['frames_', videoName, '_big_set'];

    % Create new folder if it doesn't exist already.
    if ~exist([pathFrames,newFolder], 'dir')
      mkdir([pathFrames,newFolder]);
    end

    mov = VideoReader([pathVideo, videoName, '.avi']);
%     display(mov);

    nFrames = 0;
    while hasFrame(mov)
        img = readFrame(mov);
        nFrames = nFrames+1;
        imwrite(img, [pathFrames, newFolder, filesep, sprintf('frame-%05d.jpg', nFrames)]);
    end


end