addpath('utils')
addpath('libsvm')
addpath('fast_oopsi')
%% 
% original_signals_folder = '../brain_data/signals/new/';
denoiseAlignMerge_signal_folder = '../brain_data/s_ns/new/';
% denoiseAlignMerge_signal_folder = '../brain_data/s_ns/';

for s_ind = [2]
    
%% Get Info
brain_info = brainInfo(s_ind);
signal_name = brain_info.brain;
% signal_name = '2014_09_10__18_24_12h';    % for testing detection of corrupted frames
% signal_name = '2014_09_13__15_14_14h';    % for testing detection of corrupted frames
% signal_name = '2014_09_11__20_42_30h';
mouse_name  = brain_info.mouse;

mouse_time_events_file = ['Mouse_vector/mouse_time_events_', mouse_name, '.txt'];
% Generate signal
% file_name = [original_signals_folder, signal_name];
% signal_channel1 = readSignal([file_name, '__channel01.tif']);
% signal_channel2 = readSignal([file_name, '__channel02.tif']);
% original_signal = denoiseAlignMerge(signal_channel1, signal_channel2);

% original_signal = readSignal([denoiseAlignMerge_signal_folder,'s_', signal_name, '.tif']);
original_signal = importdata([denoiseAlignMerge_signal_folder,'s_', signal_name, '.mat']);

[R, C, T] = size(original_signal);
signal = original_signal;
% implay(1-original_signal)

%% detect damaged frames
% ind_damaged = find_damaged_frames(signal);

signal_damaged = signal(:,:,ind_damaged);
%% Normalize signal
% [mouse_behavious, mouse_vector, wheel_vel] = readMouseVector(mouse_time_events_file, T);
% no_motion_frames = getNoMotionFrames(mouse_vector, wheel_vel);

% [signal s_std] = normalizeMeanVariance(original_signal, no_motion_frames);
% implay(signal)

s_std = sum(signal.^2,3);
neuron_map = (s_std-min(s_std(:)))./(max(s_std(:))-min(s_std(:)));



%% Neuron segmentation
[regions, mask] = segmentationLoG(neuron_map);
out_image_pre = showNeurons(regions, neuron_map);

% Use SVM to detect real neurons
stacks = extractTimeStacks(signal, regions);
neg_stacks = etractNegativeStacks(signal, mask, 150);

[neuron_regions, neurons, probability] = detectNeurons(regions, stacks, neg_stacks, 20);
out_image = showNeurons(neuron_regions, neuron_map, probability);

%% Decorrelate
% neurons_dec = decorrelation(neurons, neg_stacks);

%% Filtering and behaviour/neurons match

[neurons_step] = stepFilter(neurons);
[neurons_foopsi, f_neurons_foopsi ] = test_fast_oopsi2(neurons);

%% plot results to compare
for i = 1:size(neurons,1)
   f =figure('Visible', 'off');
   
   subplot(4,1,1);
   plot(neurons(i,:));
   title(sprintf('Neuron %d: Fluorescence', i));
   grid on;   

   subplot(4,1,2);
   plot(neurons_step(i,:));
   title(sprintf('Neuron %d: Spike train (step filter)', i)); 
   grid on;
   
   subplot(4,1,3);
   plot(neurons_foopsi(i,:));
   title(sprintf('Neuron %d: Spike train (Fast OOPSI)', i));
   grid on;      
   
   subplot(4,1,4);
   plot(f_neurons_foopsi(i,:));
   title(sprintf('Neuron %d: Binary vector from spike train (Fast OOPSI)', i));   
   grid on;   
   
   if ~exist(['./pick_detection/default/', signal_name], 'dir')
       mkdir(['./pick_detection/default/', signal_name]);
   end
   print(f, ['./pick_detection/default/', signal_name, '/', 'neuron_', num2str(i), '_picks'],'-dpng'); 
   close all;
end

%% normalize signal
% dt = 10;
% sigma = [];
% for t = 1:T-dt
%    me = mean(neurons(:, t:t+dt-1), 2); 
%    std = sum( (neurons(:, t:t+dt-1)-repmat(me,1,dt)).^2,2)/dt;
%    sigma = [sigma, std];
% end
% sigma = min(sigma,[],2);
% 
% neurons_norm = (neurons - repmat(mean(neurons,2),1,T))./repmat(sigma+eps,1,T);
% [neurons_foopsi, f_neurons_foopsi] = test_fast_oopsi2(neurons_norm);

%%
% [mouse_step, mouse_peaks] = stepFilter(mouse_vector');
% mouse_peaks(mouse_peaks < 0.22) = 0;
% 
% matches = LSP(mouse_step, neurons_step);
% 
% best_match = neuron_regions{matches(1, 2)};
% choosen_neuron{1} = best_match;
% image_best_match = showNeurons(choosen_neuron, neuron_map);
% imwrite(image_best_match, [num2str(s_ind),'_',signal_name,'.png']);
% subplot(1, 2, s_ind); imshow(image_best_match);
end