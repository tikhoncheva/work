function matches = LSP(mouse_peaks, binary_peaks)

mouse_binary = zeros(1, numel(mouse_peaks));
mouse_binary(mouse_peaks ~= 0) = 1;

%% LSP
B = binary_peaks*mouse_binary';
A = double(binary_peaks)*double(binary_peaks'); 
w = linsolve(A,B);
[val, ind] = sort(w, 'descend');
matches = [val, ind];