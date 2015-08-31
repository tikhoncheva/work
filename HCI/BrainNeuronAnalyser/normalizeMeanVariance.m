function [signal_n, s_std]= normalizeMeanVariance(signal, frames)
[R C T] = size(signal);

%% Normalize signal
% s_mean = mean(s_cut,3);
s_med = median(signal(:, :, frames),3);

s_norm = signal - repmat(s_med,1,1,T);

mi = min(s_norm(:));
ma = max(s_norm(:));
s_norm = (s_norm-mi)/(ma-mi);

s_std = sum(s_norm.^2, 3)/T;
signal_n = s_norm./repmat(s_std,1,1,T);

mi = min(signal_n(:));
ma = max(signal_n(:));
signal_n = (signal_n-mi)/(ma-mi);
