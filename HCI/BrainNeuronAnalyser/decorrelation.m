function Y2 = decorrelation(neurons, neg_stacks)

X = neg_stacks;
me = mean(X);
X = bsxfun(@minus, X, me);

A = var(X, [], 1);
A = diag(A);

[V,D] = eig(A);
M = V*diag(1./(diag(D)+0.0001).^(1/2))*V';

X2 = neurons;
% me2 = mean(X2);
X2 = bsxfun(@minus, X2, me);
Y2 = X2*M;