%% interpolate signal if it contains NaN values

% Input
% signal 
% 3dmask    logical matrix of the same size as signal with 1s, 
%           where the signal has NaN entries
% Output
% insignal  interpolated signal without NaN values
function [insignal] = interpolate(signal, mask3D)
   
    [R,C,T] = size(signal);
    insignal = signal;
    
    I = repmat((1:R)', C, 1);
    J = kron((1:C)', ones(R,1));
    
    for k = 1:size(I,1)
       i = I(k);
       j = J(k);
       t = find(~mask3D(i,j,:));
       
       % stretch the first non-NaN value between video begin and the
       % occurring position of this value
       insignal(i,j,1:t(1)-1) = signal(i,j,t(1));
       % stretch the last non-NaN value between it current position and the end of a video
       insignal(i,j,t(end)+1:end) = signal(i,j,t(end));
       % linearly interpolate inbetween
       for p = 1:numel(t)-1
           if (t(p+1)-t(p))>1
              a = signal(i,j, t(p));
              b = signal(i,j, t(p+1));
              n = (t(p+1)-t(p));
              x = (t(p)+1:t(p+1)-1);
              insignal(i,j,x) = a + (b-a)*(x-t(p))/n;
           end
       end
    end

    mask3D = isnan(insignal);
    assert(sum(mask3D(:))==0, 'signal still contains NaN entries!');
end
