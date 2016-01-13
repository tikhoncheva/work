function result = embedding(vector)
[N, W] = size(vector);
result = reshape(vector',[1, N*W]);

end
