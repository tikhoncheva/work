function [dpx,dpy] = doBlockAlignment(T,I,nblocks)
    dpx = zeros(nblocks,1);
    dpy = zeros(nblocks,1);
 
    dr = 10;
    blocksize = size(T,1)/nblocks;
 
    [xi,yi] = meshgrid(1:size(T,2),1:blocksize);
    thecen = [size(T,2)/2+1,floor(blocksize/2+1)];
    mask = (xi-thecen(1)).^2+(yi-thecen(2)).^2< dr^2;
 
    for ii = 1:nblocks
        dy = (ii-1)*size(T,1)/nblocks;
        rg = (1:size(T,1)/nblocks) + dy;
        T_ = T(rg,:);
        I_ = I(rg,:);
        T_ = bsxfun(@minus,T_,mean(T_,1));
        I_ = bsxfun(@minus,I_,mean(I_,1));
        dx = fftshift(ifft2(fft2(T_).*conj(fft2(I_))));
        theM = dx.*mask;
 
        [yy,xx] = find(theM == max(theM(:)));
 
        dpx(ii) = (xx-thecen(1));
        dpy(ii) = (yy-thecen(2));
    end
end