addpath('utils')

%% 
original_signals_folder = 'signals/';
figure;
for s_ind = [1 2]
    
%% Get Info
brain_info = brainInfo(s_ind);
signal_name = brain_info.brain;
mouse_name  = brain_info.mouse;

mouse_time_events_file = ['Mouse_vector/mouse_time_events_', mouse_name, '.txt'];
% Generate signal
file_name = [original_signals_folder, signal_name];
signal_channel1 = readSignal([file_name, '__channel01.tif']);
signal_channel2 = readSignal([file_name, '__channel02.tif']);

original_signal = denoiseAlignMerge(signal_channel1, signal_channel2);
[R C T] = size(original_signal);

% implay(1-original_signal)

%% Normalize signal
[mouse_behavious, mouse_vector, wheel_vel] = readMouseVector(mouse_time_events_file, T);
no_motion_frames = getNoMotionFrames(mouse_vector, wheel_vel);

[signal s_std] = normalizeMeanVariance(original_signal, no_motion_frames);
% implay(signal)

neuron_map = (s_std-min(s_std(:)))./(max(s_std(:))-min(s_std(:)));

%% Neuron segmentation
[regions, mask] = segmentationLoG(neuron_map);
out_image_pre = showNeurons(regions, neuron_map);

% Use SVM to detect real neurons
stacks = extractTimeStacks(signal, regions);
neg_stacks = etractNegativeStacks(signal, mask, 200);

[neuron_regions, neurons, probability] = detectNeurons(regions, stacks, neg_stacks, 20);
out_image = showNeurons(neuron_regions, neuron_map, probability);

%% Decorrelate
% neurons_dec = decorrelation(neurons, neg_stacks);

%% Filtering and behaviour/neurons match
[neurons_step neuron_peaks] = stepFilter(neurons);

[mouse_step mouse_peaks] = stepFilter(mouse_vector');
mouse_peaks(mouse_peaks < 0.22) = 0;

matches = LSP(mouse_step, neurons_step);

best_match = neuron_regions{matches(1, 2)};
choosen_neuron{1} = best_match;
image_best_match = showNeurons(choosen_neuron, neuron_map);
imwrite(image_best_match, [num2str(s_ind),'_',signal_name,'.png']);
subplot(1, 2, s_ind); imshow(image_best_match);
end