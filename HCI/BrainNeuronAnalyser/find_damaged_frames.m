%% detect damaged frames in a given signal
% damaged == everything blurred, alignment looks creepy

% Output
% frame_ind     index of damaged frames
function [ind_damaged_frames] = find_damaged_frames(signal)
rng('default');

[R, C, T] = size(signal);
ind_damaged_frames = (1:T);          % per default all frames are good

N = 100;    % number of patches
S = 8;      % size of one patch SxS

% randomly smaple signal and convert them into 1D signal
stack_1Dsignals = zeros(N, T);
for i = 1:N
   lt_corner = [randi(R-S+1), randi(C-S-1)];
   patch = signal(lt_corner(1):lt_corner(1)+S-1, ...
                  lt_corner(2):lt_corner(2)+S-1, :);
      
   % convert patch into 1D signal
   for t = 1:T
        pixels = patch(:,:,t);
        pixels=sort(pixels, 'descend');
%         stmax=round(5*S/100)+1;
%         edmax=round(20*S/100);
%         val = mean(pixels(stmax:edmax));
        val = median(pixels(:));
        stack_1Dsignals(i,t) = abs(max(pixels(:)) - min(pixels(:)));
   end
end

% create binary vector of picks
stack_1Dsignals_b = false(N, T);
for i = 1:N
   s = stack_1Dsignals(i,:);
   [~, loc] = findpeaks(s);
   stack_1Dsignals_b(i,loc) = true;
end

mean_stack_1Dsignals = mean(stack_1Dsignals,2);
diff = abs(stack_1Dsignals-repmat(mean_stack_1Dsignals,1,T));
diff_curv1 = sum(diff,1);
diff_curv2 = sqrt(sum(diff.^2,1));

[val1, ind1] = sort(diff_curv1, 'descend');
[val2, ind2] = sort(diff_curv2, 'descend');

val1_med = median(val1);
val2_med = median(val2);

val1_dist = abs(val1-val1_med);
val2_dist = abs(val2-val2_med);

ths1 = 0.4*max(val1_dist);
ths2 = 0.2*max(val2_dist);

frame_ind1 = ind1(val1_dist>ths1);
frame_ind2 = ind2(val2_dist>ths2);

%% plot diff-values in time
frames_to_del1 = find(val1_dist>ths1);

figure;
plot(diff_curv1, 'r-'), hold on;
plot(val1_med*ones(1,T), 'r--'), hold on;
plot(frame_ind1,diff_curv1(frame_ind1), 'kx');
title('decision curve l1 norm');

frames_to_del2 = find(val2_dist>ths2);
figure;
plot(diff_curv2, 'b-'), hold on;
plot(val2_med*ones(1,T), 'b--');
plot(frame_ind2,diff_curv2(frame_ind2), 'kx');
title('decision curve l2 norm ');
%% plot signal of patches on the suspicious frames
% ind_suspect = [119, 122, (123:128), 132:138];
% Nbad = numel(ind_suspect);
% 
% Ymin = min(stack_1Dsignals(:));
% Ymax = max(stack_1Dsignals(:));
% figure; 
% for j = 1:Nbad
%     frame = ind_suspect(j);
%     subplot(Nbad,1,j);
%     plot(stack_1Dsignals(:,frame), 'r-'); hold on;
%     plot(mean_stack_1Dsignals', 'b-'); hold on;
% %     plot(find(stack_1D_signals_b(:,frame))', stack_1D_signals(stack_1D_signals_b(:,frame),frame), 'r*');
%     ylim([Ymin Ymax]);
%     title(sprintf('frame %d', frame));
% end

%% plot signal of patches on the normal frames

% ind_good = (1:T); ind_good(ind_suspect)=[];
% Ngood = numel(ind_good);
% 
% ind_rand = randi(Ngood,1,Nbad);
% ind_good = ind_good(ind_rand);
% Ngood = Nbad;
% 
% figure; 
% for j = 1:Ngood
%     frame = ind_good(j);
%     subplot(Nbad,1,j);
%     plot(stack_1Dsignals(:,frame), 'r-'); hold on;
%     plot(mean_stack_1Dsignals', 'b-'); hold on;
% %     plot(find(stack_1D_signals_b(:,frame))', stack_1D_signals(stack_1D_signals_b(:,frame),frame), 'r*');
%     ylim([Ymin Ymax]);
%     title(sprintf('frame %d', frame));
% end


ind_damaged_frames = frame_ind1;
clear R C T N S stack_patches lt_corner path;
end