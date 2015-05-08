 
img1 = imread('coins.png');
img1 = double(img1)/255;
[m,n] = size(img1);
% figure, imshow(img1);

img2 = zeros(size(img1));
img2(5:end, 5:end) = img1(1:end-5+1,1:end-5+1);
% figure, imshow(img2);


% [u, v] = opticalflow(img1, img2, [21 21]);
% [u1,v1] = LucasKanade(img1, img2, 21);

% [u, v] = opticalflow_edgepoints(img1, img2, [31 31]);

img1Edges = edge(img1, 'canny');

[I,J] = find(img1Edges);

k = 687;
y = I(k);
x = J(k);

[ynew, xnew] = opticalflow_trackpoint(img1, img2, [11 11], y, x);

% u = ynew - y
% v = xnew - x

figure,
     imshow(img1), hold on;
     plot(x, y, 'b*');
hold off;    


figure,
     imshow(img2), hold on;
     plot(x, y, 'b*');
     plot(xnew, ynew, 'r*');
hold off;   

