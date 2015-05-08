function [U,V] = Lucas_Kanade(imgA, imgB, windowsize)

[m,n] = size(imgA);

assert( size(imgA,3) == 1);
assert( size(imgB,1) == m & size(imgB,2) == n);

[m,n] = size(imgA);

U = zeros([m,n]);
V = zeros([m,n]);

% window size (n pixels)
wx = windowsize(1);    
wy = windowsize(2);

[Ix, Iy] = imgradientxy(imgA);
It = double(imgA-imgB);

for i=1:m
    wi = [max(1, i-(wy-1)/2): min(m, i+(wy-1)/2)];
    for j=1:n
        wj = [max(1, j-(wx-1)/2): min(m, j+(wx-1)/2)];
        
        A1 = Ix(wi, wj);
        A2 = Iy(wi, wj);
        
        A1 = A1';
        A2 = A2';
        
        A1 = A1(:);
        A2 = A2(:);
        
        A = [A2, A1];
        
        b = It(wi, wj);
        b = b';
        b = b(:);
        
        b = A'*b;
        A = A'*A;
        
        x = pinv(A)*b;
        
        U(i,j) = x(2);
        V(i,j) = x(1);
        
    end 
end

U(isnan(U) )=0;
V(isnan(V) )=0;

end





























