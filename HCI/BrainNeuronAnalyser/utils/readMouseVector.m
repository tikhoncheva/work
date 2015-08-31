function [paw_right_events, mouse_right_paw_events, wheel_vel] = readMouseVector(mouse_time_events_text, out_frames)

% first line
[str1, str2, str3, str4] = textread(mouse_time_events_text, ...
'%s %s %s %s', 1);

% fill events vector
[frames, paw_vel, paw_isLeft, wheel_vel] = textread(mouse_time_events_text, ...
'%d %f %d %f', 'headerlines', 1);

for i = 2:size(paw_isLeft, 1)-1
    if paw_isLeft(i) == 0 && paw_isLeft(i-1) == 1 && paw_isLeft(i+1) == 1 
        paw_isLeft(i) = 1;
    end
end

paw_right_events = paw_vel;
paw_right_events(paw_isLeft == 0) = 0;

fill_frames = 5005 - size(paw_right_events, 1);
paw_right_events = [paw_right_events; zeros(fill_frames, 1)];

%% sample mouse vector to the frame rate of the brain
T = 5005;
Tb = out_frames;

sync_factor = T / Tb;
mouse_right_paw_events = zeros(Tb, 1);

samples = int32(round(linspace(1,T,Tb+1)));
% samples = samples(2:end-1);

for k = 1:Tb
    actions = paw_right_events(samples(k):samples(k+1));
    actions = sort(actions, 1, 'descend');
    value = mean(actions(1:3));
    
    mouse_right_paw_events(k) = value;
end

mi = min(mouse_right_paw_events(:));
ma = max(mouse_right_paw_events(:));
mouse_right_paw_events = (mouse_right_paw_events-mi)./(ma-mi);
