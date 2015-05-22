function [ circles ] = circle_detection( img, votes_dir, cyRange, cxRange, rRange, varargin )
tic

r_min = rRange(1);
r_max = rRange(2);

cx_min = cxRange(1);
cx_max = cxRange(2);

cy_min = cyRange(1);
cy_max = cyRange(2);

% let points vore for circle centers
H = hough_voting_circles(img, votes_dir, cyRange, cxRange, rRange);


% search for picks in the voting matrix
circles = [];
threshold = 0.55 * max(H(:));
% select 5 most prominent picks for each radius
npicks_per_r = 5;


for r = r_min:r_max
   H_r = H(:, :, r-r_min+1);
   tmp =  H_r - threshold;
   tmp(tmp<0) = 0;
   
   [picks, pos] = findpeaks(tmp(:));
   
   [I,J] = ind2sub(size(H_r), pos);
   
   [picks_sort, pos_sort] = sort(picks, 'descend');
   
   cy = I(pos_sort(1:min(npicks_per_r, numel(picks_sort)) ));
   cx = J(pos_sort(1:min(npicks_per_r, numel(picks_sort)) ));
   
   tmp = picks_sort(1:min(npicks_per_r, numel(picks_sort)) );

   cy = cy + repmat(cy_min - 1, size(cy));
   cx = cx + repmat(cy_min - 1, size(cx));
   
   circles_local = [[cy, cx], repmat(r, size(cy)), tmp];
   
   circles = [circles; circles_local ];
end


% if user want to get only n best circles (circles with the most votes)
if length(varargin)==1
    nbest = varargin{1};
    [~, ind] = sort(circles(:,4),'descend');
    ind = ind(1:min(nbest, length(ind)) );
    circles = circles(ind,:);
end

display(sprintf('spent time: %f', toc));

end


