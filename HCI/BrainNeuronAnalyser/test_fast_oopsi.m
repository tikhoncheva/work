function [n_best] = test_fast_oopsi(double_signal, size_f, size_g)

if nargin == 1
    size_f = 5;
    size_g = 15;
end

%% set known algorithm variables V
V.Ncells = 1;
fr = 200*375/5005;
V.dt = 1/fr;


%% run algorithm for each neuron sepately
[nN, T] = size(double_signal);

n_best = zeros([nN, T]); 
for i = 1:nN
    [n_best_i, P_best_i, V_i] = fast_oopsi(double_signal(i,:),V);
    n_best(i,:) = n_best_i';
end

 
