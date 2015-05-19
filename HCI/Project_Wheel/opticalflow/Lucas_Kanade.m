function [U,V] = Lucas_Kanade(imgA, imgB, windowsize,varargin)

imgA = double(imgA);
imgB = double(imgB);

[m,n] = size(imgA);

assert( size(imgA,3) == 1);
assert( size(imgB,1) == m & size(imgB,2) == n);

[m,n] = size(imgA);

U = zeros([m,n]);
V = zeros([m,n]);

% window size (n pixels)
wx = windowsize(1);    
wy = windowsize(2);

% Image gradient
hx = [-1,1;...
      -1,1];  
hy = hx'; 

Ix = conv2(imgA, hx) + conv2(imgB, hx);
Ix = Ix(1:size(Ix,1)-1, 1:size(Ix,2)-1);

Iy = conv2(imgA, hy) + conv2(imgB, hy);
Iy = Iy(1:size(Iy,1)-1, 1:size(Iy,2)-1);

% Image time derivative
It = -(conv2(imgA, ones(2)) - conv2(imgB, ones(2)));
It = It(1:size(It,1)-1, 1:size(It,2)-1);

% track selected points
if (nargin == 4)
    points = varargin{1};
    for k=1:size(points,1)
        i = points(k,1);
        j = points(k,2);

        if (i<1 || i>m)
            continue
        end
        
        if (j<1 || j>n)
            continue
        end

        wi = [max(1, i-(wy-1)/2): min(m, i+(wy-1)/2)];
        wj = [max(1, j-(wx-1)/2): min(n, j+(wx-1)/2)];

        A1 = Ix(wi, wj);
        A2 = Iy(wi, wj);

        A1 = A1(:);
        A2 = A2(:);

        A = [A2, A1];

        b = It(wi, wj);
        b = b(:);

        b = A'*b;
        A = A'*A;

        x = pinv(A)*b;

        U(i,j) = x(2);
        V(i,j) = x(1);

    end
    
else %calculate optical flow for each pixel

    for i = 1+(wy-1)/2 : m-(wy-1)/2
        for j = 1+(wx-1)/2 : n-(wx-1)/2;

            wi = [i-(wy-1)/2 : i+(wy-1)/2];
            wj = [j-(wx-1)/2 : j+(wx-1)/2];

            A1 = Ix(wi, wj);
            A2 = Iy(wi, wj);

            A1 = A1(:);
            A2 = A2(:);

            A = [A2, A1];

            b = It(wi, wj);
            b = b(:);

            b = A'*b;
            A = A'*A;

            x = pinv(A)*b;

            U(i,j) = x(2);
            V(i,j) = x(1);
        end

    end
    
end

U(isnan(U) )=0;
V(isnan(V) )=0;

end
