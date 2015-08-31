function fr_m_no_motion = getNoMotionFrames(mouse_vector, wheel_vel)


count = 1;
probable_frames = [];
for i = 1:size(mouse_vector, 1)
    if mouse_vector(i) == 0 && wheel_vel(i) == 0
        probable_frames = [probable_frames; i];
    else
        if size(probable_frames, 1) >= 3
            no_motion_frames{count} = probable_frames;
            count = count + 1;
            probable_frames = [];
        end
    end
end


fr_m_no_motion = [];
for i = 1:size(no_motion_frames,2)
    fr_m_no_motion = [fr_m_no_motion, no_motion_frames{1,i}'];  
end