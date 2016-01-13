function [neurons, neuron_circles, neuron_map] = loadNeurons(s_ind, n_neurons)

if nargin == 1
    n_neurons = 12;
end
use_mat = true;

brain_info = brainInfo(s_ind);
signal_name = brain_info.brain;
mouse_name  = brain_info.mouse;
folder = brain_info.folder;

folder = ['/hci', folder(8:end)];
if use_mat
    merged_signal_file = [folder,signal_name, '.mat'];
%     merged_signal_file = ['/hci',merged_signal_file(8:end)];
    signal = importdata(merged_signal_file);
else
    merged_signal_file = [folder,signal_name, '.tif'];
    signal = readSignal(merged_signal_file);
end

s_std = sum(signal.^2,3);
neuron_map = (s_std-min(s_std(:)))./(max(s_std(:))-min(s_std(:)));

[regions, mask] = segmentationLoG(neuron_map);
stacks = extractTimeStacks(signal, regions);
neg_stacks = etractNegativeStacks(signal, mask, 150);
[neuron_regions, neurons, probability] = detectNeurons(regions, stacks, neg_stacks, min(size(stacks,1), n_neurons));

% for i=1:size(neuron_regions,2)
%     fprintf('%d %d %d %d\n', neuron_regions{i}(1,:), neuron_regions{i}(end,:));
% end
% fprintf('\n');
%% Prepare data
neuron_circles = regionToCircles(neuron_regions);

% for i=1:size(neuron_circles,1)
%     fprintf('%d %d %d\n', neuron_circles(i,:));
% end
% fprintf('\n');

% neurons = extractTimeStacks(signal, neuron_regions);
% neurons = normilizeNeurons(neurons, mouse_vector);

% [neurons] = stepFilter(neurons);
% [neurons, neurons_peaks] = test_fast_oopsi2(neurons);