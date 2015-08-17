%% Adjust red color based on cumulative function of the maximal gray values in each patch and time

clc; clear;

addpath(genpath('../Tools/piotr_toolbox_V3.26/'));
%%
% pathIn = ['.', filesep, 'signals_aligned', filesep];
pathIn = ['.', filesep, 'fs', filesep];
sname = '2014_09_10__18_20_38h';
pathOut = ['.', filesep, 'statistics_cf_of_maxgv', filesep];
% suffix = '';
%%

% fName_stiff = [pathIn, 'aligned_channel01_', sname, '.tif'];
fName_stiff = [pathIn, 'fsharp_sigma11_alpha1.1_s_', sname, '.tif'];

info = imfinfo(fName_stiff);
T = numel(info);

s = [];
for t = 1:T
    img = im2double(imread(fName_stiff, t, 'Info', info));
    s = cat(3, s, img);    
end

% normalize gray values
mi = min(s(:));
ma = max(s(:));

s = (s-mi)./(ma-mi);

%% size of a square patch

[m,n] = size(s(:,:,1));    % size of the images
w = 9;                     % width of a patch
hw = floor(w/2);           % half of the width
sh = w;                    % distance between patch centers

%% overlapping regions

pcenter_x = [hw+1:sh:n];    % patch center in each row
pcenter_y = [hw+1:sh:m];    % patch center in each column

Px = numel(pcenter_x); % n-w+1; % number of patches in each row
Py = numel(pcenter_y); % m-w+1; % number of patches in each column

H = zeros(Py,Px,T); % histogram of max values in time 

for t = 1:T
   img = s(:,:,t);
   for i = 1:Py
       for j = 1:Px
           patch = img(pcenter_y(i)-hw : min(m,pcenter_y(i)+hw), ...
                       pcenter_x(j)-hw : min(n,pcenter_x(j)+hw));
           H(i,j,t) = max(patch(:));
       end   
   end
end

%% non-overlapping regions
% Px = ceil(n/w); % number of patches in each row
% Py = ceil(m/w); % number of patches in each column
% 
% H = zeros(Py,Px,T); % histogram of max values in time 
% 
% for t = 1:T
%     img = s(:,:,t);
%     max_val = blockproc(img, [w w], @(x) max(x.data(:)), 'BorderSize', [1 1], ...
%                         'TrimBorder', false, 'PadPartialBlocks', true);
%     H(:,:,t) = max_val;                                     
% end

%% calculate cumulative function (cf) of max gray values in each patch

H_cf = H(:,:,1);
for k = 2:T
    H_cf(:,:,k) = sum(H(:,:,1:k).^2,3);
end
H_cf = H_cf/T; 

% normalize calues of H_cf
mi = min(H_cf(:));
ma = max(H_cf(:));

H_cf = (H_cf-mi)./(ma-mi);

%% highlight patches according to position of their cdf's to the mean cdf

[I,J] = find(true(Py,Px) ); % all patches are interesting
nInterPatches = size(I,1);

s_RGB = cell(1,T);

color = round(jet(256)*255);
for t =1:T
   img = s(:,:,t); 
   imgRGB = repmat(uint8(255.*img),[1 1 3]);
   img_hl = imgRGB;
   
   for k = 1:nInterPatches
   
       i = I(k);
       j = J(k);
           
       u = max(1, pcenter_y(i)-hw);
       b = min(m, pcenter_y(i)+hw);

       l = max(1, pcenter_x(j)-hw);
       r = min(n, pcenter_x(j)+hw);

       mask = zeros(m,n); mask(u:b,l:r) = 1;

       ind = floor(H_cf(i,j,t) *255)+1;
       
       maskRGB(:,:,1) = mask * color(ind,1); % round(mask*transp); 
       maskRGB(:,:,2) = mask * color(ind,2); % zeros(size(mask)); 
       maskRGB(:,:,3) = mask * color(ind,3); %zeros(size(mask)); 
       maskRGB = uint8(maskRGB);

       img_hl = img_hl + maskRGB;

   end
   s_RGB{t} = img_hl;
end
   
imwrite(s_RGB{T}, 's_intr_patches.jpg');

%% find interesting patches

thr = 0.75;
[I,J] = find(H_cf(:,:,end)>=thr ); % all patches are interesting
nInterPatches = size(I,1);

%% Apply non maximum supression on the histograms of the interesting patches

radius = 6; %2;
thr = 0.5; %0.05;

pick = false(size(H));

for k = 1:nInterPatches
    
    i = I(k);
    j = J(k);

    [subs, vals] = nonMaxSupr(reshape(H(i,j,:),T,1), radius, thr);
    subs = subs(:,1);
    pick(i,j,subs) = true;

end

%% highlight detected picks of the interesting patches
s_picks = cell(1,T);

for t =1:T
   img = s(:,:,t); 
   imgRGB = repmat(uint8(255.*img),[1 1 3]);
   img_hl = imgRGB;
   
   for k = 1:nInterPatches
   
       i = I(k);
       j = J(k);
       
       if pick(i,j,t)
           u = max(1,(i-1)*w+1); b = min(m,(i-1)*w+w);
           l = max(1,(j-1)*w+1); r = min(n,(j-1)*w+w);
           mask = zeros(m,n); mask(u:b,l:r) = 1;

           maskRGB(:,:,1) = round(mask*50); % All zeros.
           maskRGB(:,:,2) = zeros(size(mask)); % Round since we're dealing with integers.
           maskRGB(:,:,3) = zeros(size(mask)); % All zeros. 

           maskRGB = uint8(maskRGB);
           img_hl = img_hl + maskRGB;
       end   
   end
   s_picks{t} = img_hl;
end

%% Write avi
% writerObj = VideoWriter([pathOut, 's_', sname, '.avi']);
% open(writerObj);
% 
% for t = 1:T
%     writeVideo(writerObj,s_RGB{t});
% end
% 
% close(writerObj);

%% write Tiff
fileName = [pathOut, 'fsharp_sigma11_alpha1.1_s_picks_', sname, '.tif'];
tif_obj = Tiff(fileName, 'w') ;

tagstruct.ImageLength = size(s_picks{1},1);
tagstruct.ImageWidth = size(s_picks{1},2);
tagstruct.Photometric = Tiff.Photometric.RGB;
tagstruct.BitsPerSample = 8;
tagstruct.SamplesPerPixel = 3;
tagstruct.RowsPerStrip = 16;
tagstruct.PlanarConfiguration = Tiff.PlanarConfiguration.Chunky;
tagstruct.Software = 'MATLAB';

tif_obj.setTag(tagstruct);
tif_obj.write(s_picks{1});
close(tif_obj);

for t = 1:T
    imwrite(s_picks{t}, fileName,'tif', 'WriteMode','append','Compression','none');
end