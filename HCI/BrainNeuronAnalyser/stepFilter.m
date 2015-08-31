function [binary_signal neurons_peaks] = stepFilter(double_signal, size_f)
if nargin == 1
    size_f = 3;
end

[nN T] = size(double_signal);

f = [ones(1, size_f), ... %zeros(1, size_f),...
    -ones(1, size_f)];
f = 2*(f/sum(abs(f)));

binary_signal = [];
binary_peaks = false(nN, T);
neurons_peaks = zeros(nN, T);

% figure;
for i = 1:nN
    h=double_signal(i,:);
    h = h-min(h);
%     h(h<0)=0;

    fh = conv(h, f, 'same');
    fh(fh<0) = 0;
    
    [val,p] =  findpeaks(fh);
%     binary_peaks(i,p) = true;
    neurons_peaks(i,p) = val;
    
%     fh = fh(numel(f):end);
%     subplot(nN, 1, i); plot(h,'color','g'); hold on; plot(fh,'color','r'); hold on;
%     plot(p, val, 'bo');

    binary_signal = [binary_signal; fh];
end
