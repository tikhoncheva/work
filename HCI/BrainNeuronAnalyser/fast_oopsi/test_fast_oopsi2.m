function [n_best, neurons_peaks] = test_fast_oopsi2(double_signal, size_f, size_g)

if nargin == 1
    size_f = 5;
    size_g = 15;
end

%% set known algorithm variables V
V.Ncells = 1;
fr = 200*375/5005;
V.dt = 1/fr;

%% Filter (discrete LoG)
% d_LoG = [0, 0.1 ,0.1, 2, 0.1, 0.1, 0];

sigma = 4;
filt_size =  3; %2*ceil(3*scale)+1;

d_LoG = -fspecial('log', [1,filt_size], sigma);
d_G = fspecial('gaussian', [1,filt_size], sigma);

%% run algorithm for each neuron sepately
[nN, T] = size(double_signal);

n_best = zeros([nN, T]); 
neurons_peaks = zeros(nN, T);
for i = 1:nN
    [n_best_i, P_best_i, V_i] = fast_oopsi(double_signal(i,:),V);
    n_best(i,:) = n_best_i';
    
%     [val,p] =  findpeaks(n_best(i,:));
%     neurons_peaks(i,p) = val;
      
      neurons_peaks(i,:) = conv(n_best(i,:), d_G, 'same');

end

 
