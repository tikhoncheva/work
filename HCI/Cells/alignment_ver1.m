%%

fName_tiff = './2014_09_10__17_08_13h__channel01.tif';

folderName = fName_tiff(3:end-4);
channelnumb = folderName(end);

if ~exist(folderName, 'dir')
  mkdir(folderName);
end;

info = imfinfo(fName_tiff);
nImages = numel(info);

images = [];
for k = 1:nImages
    img1 = im2double(imread(fName_tiff, k, 'Info', info));
    images = cat(3, images, img1);
end



%%
[m,n] = size(images(:,:,1));
xshift = [-20:20];
yshift = [-10:10];

%%

nImages = size(images, 3);

corr_coeff_max = zeros(nImages-1,1);

aligned_images = zeros(3*m, 3*n, nImages);

img1 = images(:,:,1);
aligned_images(m+1:2*m,n+1:2*n,1) = img1;
% imwrite(aligned_images(:,:,1), ['.', filesep, 'results_normxcorr', filesep, 'channel0', channelnumb, filesep,  sprintf('frame-%05d.jpg', 1)], 'jpg');

offset_x = zeros(nImages,1);
offset_y = zeros(nImages,1);

offset_x(1) = 0; offset_y(1) = 0;

for k = 2:nImages
    fprintf('Iteration %d from %d \n', k-1, nImages-1);
    img2 = images(:,:,k);
    
    corr_coeff = zeros(numel(xshift)*numel(yshift), 7);
    
    for i = 1:numel(yshift)
        
        p = ceil((sign(yshift(i)) + 1)/2);
        ystart = 1 - (1-p)*yshift(i);
        yend   = m - p*yshift(i);
            
        for j = 1:numel(xshift)
            
            q = ceil((sign(xshift(j)) + 1)/2);
            xstart = 1 - (1-q)*xshift(j);
            xend   = n - q*xshift(j);
            
            img2_cut = img2(ystart:yend, xstart:xend);
    
            c = normxcorr2(img2_cut,img1);
            c_max = max(abs(c(:)));
            [ypeak, xpeak] = find(abs(c)==c_max);
            
            corr_coeff( (i-1)*numel(xshift)+j,1) = c_max;
            corr_coeff( (i-1)*numel(xshift)+j,2) = ypeak-size(img2_cut,1);
            corr_coeff( (i-1)*numel(xshift)+j,3) = xpeak-size(img2_cut,2);
            
            % parameters of croped rectangle from the image_next
            corr_coeff( (i-1)*numel(xshift)+j,4) = ystart;
            corr_coeff( (i-1)*numel(xshift)+j,5) = xstart;
            corr_coeff( (i-1)*numel(xshift)+j,6) = yend - ystart + 1;
            corr_coeff( (i-1)*numel(xshift)+j,7) = xend - xstart + 1;
    
        end
    end
    
    [c_max, maxpos] = max(corr_coeff(:,1));
    
    % offset of the matched rectangle wrt img1
    corr_coeff_max(k-1) = c_max;
    y0 = corr_coeff(maxpos,2);
    x0 = corr_coeff(maxpos,3);
    
    % offset of the matched rectangle wrt img2
    y = corr_coeff( maxpos,4);
    x = corr_coeff( maxpos,5);
    
%     height = corr_coeff( maxpos,6);
%     width  = corr_coeff( maxpos,7);
%     
%     % coordinates of the overlapped part wrt img1
%     yl = max(1, y0 - y);
%     xl = max(1, x0 - x);
%     
%     yr = min(m, y0 - y + m);
%     xr = min(n, x0 - x + n);
%     
%     % coordinates of the overlapped part wrt img2
%     Yl = max(1, y - y0 + 1);
%     Xl = max(1, x - x0 + 1);
%     
%     Yr = min(m, y - y0 + 1 + m);
%     Xr = min(n, x - x0 + 1 + n);
    
%     % overlapped part
%     common_part = zeros(m,n);
%     common_part(yl:yr,xl:xr) = img2(Yl:Yr, Xl:Xr);
     
    X0 = x0 - x;
    Y0 = y0 - y;
    aligned_images(m+1+Y0:2*m+Y0, n+1+X0:2*n+X0,k) = img2; 
               
    offset_x(k) = X0;
    offset_y(k) = Y0;               
%     imwrite(aligned_images(:,:,k), ['.', filesep, 'results_normxcorr', filesep, 'channel0', channelnumb, filsep,  sprintf('frame-%05d.jpg', k)], 'jpg');           
    
    img1 = img2;
end

figure;
plot(corr_coeff_max);
xlabel('frames');
ylabel('max correlation coefficient');
title('Normalized 2-D cross-correlation between frames');

%%
% save alignment shifts in a file
fileID = fopen(['.', filesep, 'offset_channel0', channelnumb, '.txt'],'w');
fprintf(fileID,'  offset x   \t offset y \n');
fprintf(fileID,'  %5d        \t     %5d  \n', [offset_x, offset_y]');
fclose(fileID);
   
%%
% imwrite(aligned_images(:,:,1), ['.', filesep, 'results_normxcorr', filesep, ...
%                                               'channel0', channelnumb, filesep, ...
%                                               'aligned_channel0', channelnumb, '_ver1.tif']);
% for k = 2:nImages
%     imwrite(aligned_images(:,:,k), ['.', filesep, 'results_normxcorr', filesep, ...
%                                               'channel0', channelnumb, filesep, ...
%                                               'aligned_channel0', channelnumb, '_ver1.tif'], 'WriteMode','append');
% end

