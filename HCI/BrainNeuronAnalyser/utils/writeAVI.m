function writeAVI(frames, video_name_output)

s = length(size(frames));
if s == 3
    [H, W, T] = size(frames);
    ch = 1;
else
    [H, W, ch, T] = size(frames);
end

videoW = VideoWriter([video_name_output,'.avi']);
videoW.FrameRate = 1;
open(videoW);

for k=1:T
    if ch == 1
        f = reshape(frames(:,:,k), H,W);
    else
        f = reshape(frames(:,:,:,k), [H,W,ch]);
    end
    writeVideo(videoW,f);
end
close(videoW);