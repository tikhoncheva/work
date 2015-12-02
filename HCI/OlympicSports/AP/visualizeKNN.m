function [] = visualizeKNN(dist_mat, frame_info, cname_pos, seq_id0, sim_type)

% cname_pos = 'long_jump';
% seq_id0 = 1;
path_base_images = ['../crops/' cname_pos, '/'];
% sim_type = 'max';

%load videos
videonames = dir(path_base_images);
idx = arrayfun(@(x)x.name(1)=='.',videonames);
videonames(idx) = [];

%load frames
seq0 = dir([path_base_images, videonames(seq_id0).name]);
idx = arrayfun(@(x)x.name(1)=='.',seq0);
seq0(idx) = [];



%% ADJUST TO BORIS DATA STRUCTURE
% seqs = unique(frame_info(:,1));
% frame_info_new = [];
% for i = 1:numel(seqs)
%     seq_frames = find(frame_info(:,1) == seqs(i));
%     frame_info_new = cat(2, frame_info_new, [1:size(seq_frames, 1)]);
% end
% frame_info(:, 2) = frame_info(:, 1);
% frame_info(:, 1) = [1:size(dist_mat,1)];
% frame_info(:, 3) = frame_info_new;

%% --------------------------------------------------------------------------------
% visualize nearest neighbour for frames of seq0 from other seq of the same class
% (only one per seq is allowed!)
%% --------------------------------------------------------------------------------
N_nn = 7;
N_disc_steps = 7;
step_size = floor(numel(seq0) / (N_disc_steps-1));

counter_images = 0;
figure(1);
clf;
hold on
for i = 1:step_size:numel(seq0)
    %plot orig image
    counter_images = counter_images + 1;
    subplot(N_disc_steps, N_nn + 1, counter_images);
    imshow([path_base_images, videonames(seq_id0).name, '/', seq0(i).name]);
    
    %get distMat-id of frame0
    seq0_distMat_ids = frame_info(:,2) == seq_id0;
    frame0_distMat_id = frame_info(:,3) == i;
    res_distMat_id = seq0_distMat_ids .* frame0_distMat_id;
    res_distMat_id = find(res_distMat_id == 1);
    
    %find initial NN-candidates
    nn_cand = find(frame_info(:,2) ~= seq_id0);
    dist_frame02nn_cand = dist_mat(res_distMat_id, nn_cand);
    
    if sim_type == 'max'
        [~, frame0_NN] = sort(dist_frame02nn_cand, 'descend');
        dist_frame02nn_cand_sort = dist_frame02nn_cand(frame0_NN);
    elseif sim_type == 'min'
        [~, frame0_NN] = sort(dist_frame02nn_cand, 'ascend');
        dist_frame02nn_cand_sort = dist_frame02nn_cand(frame0_NN);
    end
    nn_cand = nn_cand(frame0_NN);
    nn_cand_info = frame_info(nn_cand, :);
%     nn_cand = nn_cand(1:N_nn);
    
    for j = 1:N_nn
        
        %load frames
        seq1 = dir([path_base_images, videonames(frame_info(nn_cand(1),2)).name]);
        idx = arrayfun(@(x)x.name(1)=='.',seq1);
        seq1(idx) = [];
        
        counter_images = counter_images + 1;
        subplot(N_disc_steps, N_nn + 1, counter_images);
        imshow([path_base_images, videonames(frame_info(nn_cand(1),2)).name,...
            '/', seq1(frame_info(nn_cand(1),3)).name]);
        title(sprintf('%g', dist_frame02nn_cand_sort(1)), 'FontSize', 6)
        
        %update candidates
        nn_cand(nn_cand_info(:,2) == nn_cand_info(1,2)) = [];
        dist_frame02nn_cand_sort(nn_cand_info(:,2) == nn_cand_info(1,2)) = [];
        nn_cand_info(nn_cand_info(:,2) == nn_cand_info(1,2),:) = [];
%         dist_frame02nn_cand_sort(1) = [];
    end
end

ha = axes('Position',[0 0 1 1],'Xlim',[0 1],'Ylim',...
    [0 1],'Box','off','Visible','off','Units','normalized', 'clipping', 'off');

text(0.5, 1,['\bf KNN'...
    ', class: ', cname_pos,...
    ', seq-no.: ', num2str(seq_id0)],...
    'HorizontalAlignment' ,'center','VerticalAlignment', 'top')


% end