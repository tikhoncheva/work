function data = prefilter_data(data, pregauss)
%prefilter data
L = ceil(sqrt((2*pregauss^2) * log(10000 / (sqrt(2*pi)*pregauss)))); %.0001 > 1/(sqrt(2*pi)*pregauss * exp(L^2/(2*pregauss^2)))
sbig = sD(1:2) + [0 -leftexclude] + L;
[x,y] = meshgrid(1:sbig(2),1:sbig(1));
g2d = exp( - (min(x - 1, sbig(2) + 1 - x).^2 + min(y - 1, sbig(1) + 1 - y).^2) / (2 * pregauss^2) );
g2d = g2d / sum(sum(g2d));
g2d = fft2(g2d,sbig(1),sbig(2));
for k = 1:size(data,3)
    u = real(ifft2(fft2(data(:,:,k),sbig(1),sbig(2)) .* g2d));
    data(:,:,k) = u(1:sD(1),1:sD(2) - leftexclude);
end