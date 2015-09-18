function [neurons_peaks, n_best] = test_fast_oopsi(double_signal, size_f, size_g)

if nargin == 1
    size_f = 5;
    size_g = 15;
end

%% set known algorithm variables V
V.Ncells = 1;
fr = 200*375/5005;
V.dt = 1/fr;

%% Filter settings
f = [ones(1, size_f), ... %zeros(1, size_f),...
    -ones(1, size_f)];
f = 2*(f/sum(abs(f)));

gaussFilter = gausswin(size_g);
gaussFilter = gaussFilter / sum(gaussFilter);

%% run algorithm for each neuron sepately
[nN, T] = size(double_signal);

n_best = zeros([nN, T]); 
neurons_peaks = zeros(nN, T);
for i = 1:nN
    [n_best_i, P_best_i, V_i] = fast_oopsi(double_signal(i,:),V);
    n_best(i,:) = n_best_i';
    
%     [val,p] =  findpeaks(n_best(i,:));
%     neurons_peaks(i,p) = val;
      
      fh = conv(n_best(i,:), f, 'same');
      neurons_peaks(i,:) = conv(fh, gaussFilter, 'same');
end

 
