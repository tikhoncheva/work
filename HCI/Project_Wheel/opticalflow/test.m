 
img1 = imread('coins.png');
if (ndims(img1)==3)
    img1 = rgb2gray(img1);
end;
img1 = double(img1);
[m,n] = size(img1);

img2 = zeros(size(img1));
img2(5:end, 5:end) = img1(1:end-5+1,1:end-5+1);


img1Edges = edge(img1, 'canny');

[I,J] = find(img1Edges);

% k = 687;
% y = I(k);
% x = J(k);

[U,V] = Lucas_Kanade(img1, img2, [5,5], [I, J]);

quiver(U, V)

