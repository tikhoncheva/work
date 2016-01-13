function [pos_peaks, pos_pks_s, neg_peaks, neg_pks_s] = bestPeaks(vector, window)


[pos_pks,pos_peaks,width] = findpeaks(vector,'MinPeakDistance',window);%,'MinPeakWidth',4);
[pos_pks_s, I] = sort(pos_pks,'descend');
pos_peaks = pos_peaks(I);
% pos_peaks = pos_peaks(1:5);
pos_pks_s = vector(pos_peaks);
% width = width(I);

[neg_pks,neg_peaks] = findpeaks(max(vector)-vector,'MinPeakDistance',window); %,'MinPeakHeight',max(max(vector)-vector)*0.2
neg_pks = neg_pks(2:end-1);
neg_peaks = neg_peaks(2:end-1);

[neg_pks_s, I] = sort(neg_pks,'descend');
neg_peaks = neg_peaks(I);
% neg_peaks = neg_peaks(1:10);
neg_pks_s = vector(neg_peaks);