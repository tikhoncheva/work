function [detected_regions, detection, probability]= detectNeurons(regions, positive_stacks, negative_stacks, num_detection)
addpath('./libsvm');

nr  = size(positive_stacks, 1);
nnr = size(negative_stacks, 1);

%% Learn SVM single-class
neg_stacks_perm = negative_stacks(randperm(nnr), :);
model_onecl = libsvmtrain(ones(nnr, 1), neg_stacks_perm, '-s 2');
[predicted_label, accuracy, decision_values] = libsvmpredict(-1*ones(nr, 1), positive_stacks, model_onecl);

dv = decision_values;
dv = 1-(dv-min(dv))./(max(dv)-min(dv));

%% Learn SVM two-class over previous best results

% Prepare training set and validation set
dv2 = [[1:nr]', dv];
dv2 = sortrows(dv2, -2);
pos_split = floor(nr*0.20);
positive = positive_stacks(dv2(1:pos_split, 1), :);

pos_tr = round(0.6*pos_split);
pos_training = positive(1:pos_tr, :);
pos_valid = positive(pos_tr+1:end, :);

negative = negative_stacks(randperm(nnr), :);
neg_tr = round(0.7*nnr);
neg_training = negative(1:neg_tr, :);
neg_valid = negative(neg_tr+1:end, :);

nsamples = pos_tr+neg_tr;
weight=[neg_tr/nsamples pos_tr/nsamples];

max_acc = 0;
max_c = 0;
max_g = 0;
% lin_c = [0.1:0.1:2, 5:5:50, 60:10:500];
lin_c = [0.6:0.2:1];
for c = lin_c
    for g = 0.6 : 0.2 : 1
%         disp(['-t 2 -c ',num2str(c),' -g ', num2str(g)]);
        model_twocl = libsvmtrain([ones(pos_tr, 1); -1*ones(neg_tr, 1)], [pos_training; neg_training], ['-s 0 -t 2 -c ',num2str(c),' -g ', num2str(g),' -w1 ',num2str(weight(1)),' -w-1 ', num2str(weight(2))]);
        [predicted_label, accuracy, prob_estimates] = libsvmpredict([ones(pos_split-pos_tr, 1); -1*ones(nnr-neg_tr, 1)], [pos_valid; neg_valid], model_twocl);
        if accuracy(1) >= max_acc
            max_acc = accuracy(1);
            max_c = c;
            max_g = g;
        end
    end
end

% merge training set and validation set
nsamples = pos_split+nnr;
weight=[nnr/nsamples pos_split/nsamples];

model_twocl = libsvmtrain([ones(pos_split, 1); -1*ones(nnr, 1)], [positive; negative], ['-s 0 -t 2 -c ',num2str(max_c),' -g ', num2str(max_g),' -w1 ',num2str(weight(1)),' -w-1 ', num2str(weight(2))]);
W = model_twocl.sv_coef' * full(model_twocl.SVs);
% plot(W);

%% Predict Neurons
[predicted_label, accuracy, probability_estimates] = libsvmpredict(ones(nr, 1), positive_stacks, model_twocl);
pe = [[1:nr]', probability_estimates];
pe = sortrows(pe, -2);

detection = positive_stacks(pe(1:num_detection, 1), :);
probability = pe(1:num_detection, 2);
probability = (probability-min(probability(:)))./(max(probability(:))-min(probability(:)));

count=1;
for i=1:num_detection
    k = pe(i, 1);
    blob = regions{k};
    detected_regions{count} = blob;
    count=count+1;
end
