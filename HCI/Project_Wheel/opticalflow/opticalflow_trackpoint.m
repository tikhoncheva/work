function [ynew, xnew] = opticalflow_trackpoint(imgA, imgB, windowsize, y, x)

[m,n] = size(imgA);

assert( size(imgA,3) == 1);
assert( size(imgB,1) == m & size(imgB,2) == n);

[m,n] = size(imgA);

% window size (n pixels)
wx = windowsize(1);    
wy = windowsize(2);

[Ix, Iy] = imgradientxy(imgA);
% h = fspecial('prewitt');
% Ix = imfilter(imgA, h,'symmetric');
% Iy = imfilter(imgA, h','symmetric');
It = double(imgA-imgB);


    i = y;
    j = x;

    wi = [max(1, i-(wy-1)/2): min(m, i+(wy-1)/2)];
    wj = [max(1, j-(wx-1)/2): min(m, j+(wx-1)/2)];

    A1 = Ix(wi, wj)';
    A2 = Iy(wi, wj)';

    A1 = A1(:);
    A2 = A2(:);

    A = [A2, A1];

    b = It(wi, wj)';
    b = b(:);

    b = A'*b;
    A = A'*A;

    u = pinv(A)*b;

    ynew = y + u(2);
    xnew = x + u(1);        


end





























