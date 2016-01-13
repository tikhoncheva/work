addpath('utils'); %, '../common', '..')
% close all
window = 8;
videos = importdata('videos6.mat');
neuron_circles = importdata('neuron_circles_12.mat');

s_ind = 1;
n_neurons = 12;

[neurons, neuron_circles_original, neuron_map] = loadNeuronsFromCircles(s_ind, neuron_circles);
[N,T] = size(neurons);
n_feat = N*window;


nn = T-window;
data = zeros(nn,n_feat);
for n=1:nn
    data(n,:) = embedding(neurons(:,n:n+window-1));
end

[R W] = myLDA(data);


scores = W * W';

% frame = 240;
frame = 35;

figure(1); imagesc(scores); title('LDA similarity');
fig2 = figure(2); imagesc(scores(frame,:)); title(['LDA similarity ',num2str(frame)]);

figure(10); imagesc(W); title('W');
figure(11); imagesc(R); title('R');

[pos_peaks, pos_pks, neg_peaks, neg_pks] = bestPeaks(scores(frame,:), 1);
pos_peaks_lda = pos_peaks;
pos_pks_lda = pos_pks;
neg_peaks_lda = neg_peaks;
neg_pks_lda = neg_pks;

%%
out_folder = ['result/LDA_similarity/video',num2str(s_ind),'/Positive_',num2str(frame)];
mkdir(out_folder);
saveas(fig2,[out_folder,'/lda_',num2str(frame),'_similarity.png']);
% video = loadMouseVideo(1);
% produceVideoSimilarFramesDetection(out_folder,1, video, window, pos_peaks(2:6)+repmat(window/2,1,5), pos_pks, neg_peaks(1:10)+repmat(window/2,1,10), neg_pks);
video = videos{s_ind};


s_ind_test = s_ind;
np = 4^2;
nn = 4^2;

pos_fig = [-1919 -175 1920 1098];
fig = figure(101);
for j=1:min(np,size(pos_peaks,2))
    subplot(ceil(sqrt(np)),floor(sqrt(np)),j); imshow(video(:,:,pos_peaks(j))); 
    title([num2str(pos_peaks(j)),'-',num2str(pos_peaks(j)+window), ...
        ', score ',num2str(pos_pks(j))]);
end
set(fig,'Position',pos_fig);
F=getframe(fig); out=F.cdata;

out_video = zeros(size(out,1),size(out,2),window, 'uint8');
fig = figure(101);
for i=0:window-1
    for j=1:min(np,size(pos_peaks,2))
        img = video(:,:,pos_peaks(j)+i);
        subplot(ceil(sqrt(np)),floor(sqrt(np)),j); imshow(img); title([num2str(pos_peaks(j)),'-',num2str(pos_peaks(j)+window), ', score ',num2str(pos_pks(j))]);
    end
    set(fig,'Position',pos_fig);
    F=getframe(fig);
    out=F.cdata;
    out_video(:,:,i+1)=out(:,:,1);
end
writeTiffFile(out_video, [out_folder,'/positive_sequences_',num2str(s_ind_test),'.tif'])
writeAVI(out_video, [out_folder,'/positive_sequences_',num2str(s_ind_test)])

fig = figure(101);
for j=1:min(nn,size(neg_peaks,2))
    subplot(ceil(sqrt(nn)),floor(sqrt(nn)),j); imshow(video(:,:,neg_peaks(j))); 
    title([num2str(neg_peaks(j)),'-',num2str(neg_peaks(j)+window), ...
        ', score ',num2str(neg_pks(j))]);
end
set(fig,'Position',pos_fig);
F=getframe(fig); out=F.cdata;

out_video = zeros(size(out,1),size(out,2),window, 'uint8');
fig = figure(101);
for i=0:window-1
    for j=1:min(nn,size(neg_peaks,2))
        img = video(:,:,neg_peaks(j)+i);
        subplot(ceil(sqrt(nn)),floor(sqrt(nn)),j); imshow(img); title([num2str(neg_peaks(j)),'-',num2str(neg_peaks(j)+window), ', score ',num2str(neg_pks(j))]);
    end
    set(fig,'Position',pos_fig);
    F=getframe(fig);
    out=F.cdata;
    out_video(:,:,i+1)=out(:,:,1);
end
writeTiffFile(out_video, [out_folder,'/negative_sequences_',num2str(s_ind_test),'.tif'])
writeAVI(out_video, [out_folder,'/negative_sequences_',num2str(s_ind_test)])
