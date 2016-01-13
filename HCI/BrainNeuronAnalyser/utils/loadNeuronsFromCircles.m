function [neurons_out, new_circles, neuron_map] = loadNeuronsFromCircles(s_ind, circles)

% brain_info = brainInfo(s_ind);
% signal_name = brain_info.brain;
% folder = brain_info.folder;
% 
% merged_signal_file = [folder,signal_name, '.mat'];
% signal = importdata(merged_signal_file);

[neurons, neuron_circles, neuron_map] = loadNeurons(s_ind, 50);

% out=[];
% out_img = showCircles(circles, neuron_map, ones(1,12));
% out_img = showCircles(neuron_circles, out_img, zeros(1,50));
% fig = figure(21); imshow(out_img);
% F=getframe(fig);
% out=F.cdata;
% imwrite(out, out_file_name);


neurons_out = zeros(size(circles,1), size(neurons,2));

new_circles = [];
for i=1:size(circles)
    [m, mi] = min(sum( (neuron_circles - repmat(circles(i,:), size(neuron_circles,1),1)).^2 , 2));
    neurons_out(i,:) = neurons(mi,:);
    new_circles = [new_circles; neuron_circles(mi,:)];
end
