function D = clique_normalization(X, frame_info, knn_size, iter_num, lambda)
%   Function for finding cliques of each node and using their strength  
%   for normalization of the affinity matrix.
%   Input arguments:
%   X - affinity (similarity) matrix
%   ...

    num_nodes = size(X,1);
    num_videos = max(frame_info(:,1));
    
    Best = {};
    parfor i = 1:num_nodes
        idx = find(frame_info(:,1) ~= frame_info(i,1));
        [V, I] = sort(X(i,idx), 'descend');
        Best{i} = [V(1:knn_size)', I(1:knn_size)'];
    end
    
    for k = 1:iter_num
        disp(k)
        
        if (k == 1)
            Y = X;
        end
        
        clear YY
        parfor i = 1:num_nodes
            y = zeros(1, num_nodes);
            for j = 1:num_nodes
                knn_i = Best{i}(1:knn_size,2);
                knn_j = Best{j}(1:knn_size,2);
                knn_mean = mean2(Y(knn_i, knn_j)) / (numel(knn_i) * numel(knn_j));
                y(1,j) = (lambda * X(i,j) + knn_mean) / (lambda + 1);
            end
            YY{i,1} = y;
        end
        Y = cell2mat(YY);
        D{k} = Y;
    end
        
end