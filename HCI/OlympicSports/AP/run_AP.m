clear all; clc;
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

%% normalize similarity matrix
N = size(sim_mat,1);  

% D = diag(sum(sim_mat,2));
D = repmat(1./sum(sim_mat,2),1,N);
nsim_mat = sim_mat.*D;

% % D1 = diag(sum(nsim_mat,1));
% D1 = repmat(1./sum(nsim_mat,1), N, 1);
% nsim_mat = nsim_mat.*D1;

% nsim_mat = sim_mat;
sim_type = 'max';
visualizeKNN(nsim_mat, frame_info, cname_pos, seq_id0, sim_type);

clear D D1;

%%
d = diag(nsim_mat);
nsim_mat = nsim_mat.*double(~eye(size(nsim_mat)));


%% neares neighbors of each node
knn_size = 50;

Best = {};
parfor i = 1:N
    idx = find(frame_info(:,2) ~= frame_info(i,2));
    [V, I] = sort(nsim_mat(i,idx), 'descend');
    Best{i} = [I(1:knn_size)'];
end

knn = (cell2mat(Best))';

% create knn-graph
list_knn = (knn-ones(N,knn_size))*N+repmat( (1:N)',1,knn_size );
knn_mask = false(N); knn_mask(list_knn) = true;

%% update similarity matrix using affinity propagation

N = size(nsim_mat,1);                        % number of data points
[I,J,s] = find(nsim_mat);
s = [I,J,s];
% clear I J;

p=median(s(:,3));           % preferences
% p = d; clear d;

% [idx, netsim, ~, expref] = apclusterSparse(s, p, 'maxits', 10, 'plot');
[idx, netsim, ~, expref, s_new] = apclusterSparse_kNN(s, p, knn_mask, 'maxits', 60, 'plot');%, 'dampfact', 0.8);
% [idx, netsim, ~, expref] = apclusterSparse_sumprod(s, p, 'maxits', 10, 'plot');


%% reorganize element in the new similarity matrix
sim_mat_new = zeros(N, N);
sim_mat_new(sub2ind([N,N], I, J)) = s_new(1:N*N-N,1);
sim_mat_new(1:N+1:end) = s_new(N*N-N+1:end);

%% visualize the result
sim_type = 'max';
visualizeKNN(sim_mat_new, frame_info, cname_pos, seq_id0, sim_type);
