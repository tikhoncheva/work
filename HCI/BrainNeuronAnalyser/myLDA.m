function [R W] = myLDA(data)

[N,F] = size(data);

% data = bsxfun(@rdivide, data, sqrt(sum(data.^2, 2)));

Mu = mean(data,1);
data_mu = data-repmat(Mu,N,1);
sigma = cov(data_mu);
sigma = sigma + 0.01*eye(size(sigma,1));
R = chol(sigma);

W = zeros(N,F);
for n=1:N
    W(n,:) = R' \ data_mu(n,:)';
end
% W = bsxfun(@rdivide, W, sqrt(sum(W.^2, 2)));