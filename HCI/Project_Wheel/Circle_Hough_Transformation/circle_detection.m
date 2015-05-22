function [ circles ] = circle_detection( img, votes_dir, cyRange, cxRange, rRange, varargin )
tic

r_min = rRange(1);
r_max = rRange(2);

cx_min = cxRange(1);
cx_max = cxRange(2);

cy_min = cyRange(1);
cy_max = cyRange(2);

% let points vore for circle centers
H = HoughTr_circles(img, votes_dir, cyRange, cxRange, rRange);


% search for picks in the voting matrix
circles = [];


h = fspecial('gaussian', [10, 10], 2);
% select 5 most prominent picks for each radius
npicks_per_r = 10;

% parameters of non maximum suppression
radius = 10;
thr = 1.5;


for r = r_min:r_max
    
   H_r = H(:, :, r-r_min+1);
   H_r = imfilter(H_r, h);
   
   [picks_pos,picks_val] = nonMaxSupr(double(H_r), radius, thr);    
   
   [~, ind] = sort(picks_val, 'descend');
   
   cy = picks_pos( ind(1:min(npicks_per_r, numel(picks_pos)) ), 1);
   cx = picks_pos( ind(1:min(npicks_per_r, numel(picks_pos)) ), 2);

   cy = cy + repmat(cy_min - 1, size(cy));
   cx = cx + repmat(cx_min - 1, size(cx));
   
   circles_local = [[cy, cx], repmat(r, size(cy))];
   
   circles = [circles; circles_local ];
   
end


% % kmean clustering in 3d feature space
% nclusters = 10;
% [ind, centers] = kmeans(circles(:,1:3), nclusters);
% 
% circle_cluster = [];
% for i=1:nclusters
%     circle_cluster = [circle_cluster; ...
%                centers(i,1), centers(i,2), centers(i,3), numel(find(ind==i))];
% end

% kmean clustering in 2d feature space
nclusters = 10;
[ind, centers] = kmeans(circles(:,1:2), nclusters);

circle_cluster = [];
for i=1:nclusters
    rad = circles(ind==i, 3);
    ni = numel(find(ind==i));
    minrad = min(rad);
    maxrad = max(rad);
    
    circle_cluster = [circle_cluster; ...
                      [centers(i,1), centers(i,2), minrad, ni]; ...
                      [centers(i,1), centers(i,2), maxrad, ni] ];           
end

circles = circle_cluster;

% if user want to get only n best circles (circles with the most votes)
if length(varargin)==1
    nbest = min( varargin{1}, size(circles,1));
    [~, ind] = sort(circles(:,4),'descend');
    ind = ind(1:nbest);
    circles = circles(ind,:);
end

display(sprintf('spent time: %f', toc));

end


