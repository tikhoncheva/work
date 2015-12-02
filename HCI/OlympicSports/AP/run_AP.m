
%% read provided information: 
% distance matrix and frame info
cname_pos = 'long_jump';
seq_id0 = 1;
path_base_images = ['../crops/' cname_pos, '/'];

%load videos
videonames = dir(path_base_images);
idx = arrayfun(@(x)x.name(1)=='.',videonames);
videonames(idx) = [];

%load frames
seq0 = dir([path_base_images, videonames(seq_id0).name]);
idx = arrayfun(@(x)x.name(1)=='.',seq0);
seq0(idx) = [];

%----------------------%
% load distance matrix %
%----------------------%
load(['../Timo/sim_matrices_nnset/', cname_pos, '/DISTANCE_MAT.mat']);
dist_mat = DISTANCE_MAT.dist_mat;
frame_info = DISTANCE_MAT.frame_info;

%% convert distance matrix to the similarity matrix
% sigma = 100;
% sim_mat = exp(-dist_mat.^2./sigma);

% N = 4; A = rand(N,N);
% A = A.*double(~eye(size(A)));
% sim_mat = A;

sim_mat = dist_mat; 
sim_mat = sim_mat.*double(~eye(size(sim_mat)));

%% update similarity matrix using affinity propagation

N = size(sim_mat,1);                        % number of data points
p = ones(N,1)*median(sim_mat(:));           % preferences
[I,J,s] = find(sim_mat);
s = [I,J,-s];
% p = median(sim_mat(:));
[idx, netsim, ~, expref] = apclusterSparse(s, p, 'maxits', 100, 'plot');

%% visualize the result
sim_type = 'max';
visualizeKNN(sim_mat_new, frame_info, cname_pos, seq_id0, sim_type);
