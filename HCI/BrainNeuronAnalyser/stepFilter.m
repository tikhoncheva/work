function [neurons_peaks] = stepFilter(double_signal, size_f, size_g)
% default size_f = 5;, size_g = 


if nargin == 1
    size_f = 5;
    size_g = 15;
end

[nN, T] = size(double_signal);

f = [ones(1, size_f), ... %zeros(1, size_f),...
    -ones(1, size_f)];
f = 2*(f/sum(abs(f)));

gaussFilter = gausswin(size_g);
gaussFilter = gaussFilter / sum(gaussFilter);

% scale = 10;
% filt_size =  2*ceil(3*scale)+1;
% f = -scale^2 * fspecial('log', [1 filt_size], scale);
% size_f = floor(filt_size/2);

neurons_peaks = zeros(nN, T);
% figure;
for i = 1:nN
    h=double_signal(i,:);
    h = h-min(h);
%     h(h<0)=0;

    fh = conv(h, f, 'same');
%     fh = fh(size_f:end-size_f-1);
%     fh(fh<0) = 0;
    fh = conv(fh, gaussFilter, 'same');
%     fh = fh(size_f:end-size_f-1);
%     figure; plot(h,'r');hold on;
%     plot(fh,'b'); hold off;
    neurons_peaks(i,:) = fh;

%     [val,p] =  findpeaks(fh);
%     binary_peaks(i,p) = true;
%     neurons_peaks(i,p) = val;

%     fh = fh(numel(f):end);
%     subplot(nN, 1, i); plot(h,'color','g'); hold on;
% plot(fh,'color','r'); hold on;
%     plot(p, val, 'bo');
end 