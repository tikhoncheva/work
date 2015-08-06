%% Adjust red color based on the positions of the patch's cdfs with
% respect to mean cdf

clc; clear;

%%
% pathIn = ['.', filesep, 'signals_aligned', filesep];
pathIn = ['.', filesep, 's_ns', filesep];
sname = '2014_09_10__17_08_13h';
pathOut = ['.', filesep, 'statistics_cdf', filesep];
% suffix = '';
%%

% fName_stiff = [pathIn, 'aligned_channel01_', sname, '.tif'];
fName_stiff = [pathIn, 's_', sname, '.tif'];

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

%% calculate cdf of max gray values in each patch

x = [0:0.005:1];

H_cdf = zeros(Py,Px, numel(x));
for k = 2:numel(x)
    B = H<=x(k);
    H_cdf(:,:,k) = sum(B,3);
end

H_cdf = H_cdf/T; clear B;

%% mean cdf

cdf_mean = mean(reshape(H_cdf, Px*Py, size(H_cdf,3)));

%% for each cdf decide, if it was shifted to the left or to the right
% with respect to the mean cdf

H_cdf_mean = reshape(repmat(cdf_mean, Py*Px,1), Py, Px, numel(cdf_mean));
diff = H_cdf_mean - H_cdf;
diff(abs(diff)<0.00005) = 0;

diff_min = min(diff(:));
diff_max = max(diff(:));


%% normalization of the gray values
HL = [];
for t=1:T
   % values of the signals at the time t 
   val = H(:,:,t);
   % find the probability of the signal to have the value less or equal val
   diff = abs(repmat(val, 1,1, size(H_cdf,3))- H_cdf);
   [~, min_diff_pos] = min(diff, [], 3);
   ind = sub2ind(size(H_cdf_mean), repmat([1:Py]',Px,1), ...
                                   kron([1:Px]', ones(Py,1)), ...
                                   min_diff_pos(:));
   val_of_mean_cdf = reshape(H_cdf_mean(ind), Py, Px);
   HL = cat(3, HL, (val_of_mean_cdf-val) );
end

%%
HL = (HL-diff_min)./(diff_max-diff_min);
HL = 1 - HL;


%% find interesting histograms

[I,J] = find(true(Py,Px) ); % all patches are interesting
nInterPatches = size(I,1);

%% highlight patches according to position of their cdf's to the mean cdf
s_RGB = cell(1,T);

for t =1:T
   img = s(:,:,t); 
   imgRGB = repmat(uint8(255.*img),[1 1 3]);
   img_hl = imgRGB;
   
   for k = 1:nInterPatches
   
       i = I(k);
       j = J(k);
       
       if 1 %pick(i,j,t)
           
           u = max(1, pcenter_y(i)-hw);
           b = min(m, pcenter_y(i)+hw);
           
           l = max(1, pcenter_x(j)-hw);
           r = min(n, pcenter_x(j)+hw);
           
           mask = zeros(m,n); mask(u:b,l:r) = 1;
            
           transp = HL(i,j,t) *255; % transparency
           
           maskRGB(:,:,1) = round(mask*transp); 
           maskRGB(:,:,2) = zeros(size(mask)); 
           maskRGB(:,:,3) = zeros(size(mask)); 
           maskRGB = uint8(maskRGB);
           
           img_hl = img_hl + maskRGB;
       end   
   end
   s_RGB{t} = img_hl;
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
fileName = [pathOut, 's_', sname, '.tif'];
tif_obj = Tiff(fileName, 'w') ;

tagstruct.ImageLength = size(s_RGB{1},1);
tagstruct.ImageWidth = size(s_RGB{1},2);
tagstruct.Photometric = Tiff.Photometric.RGB;
tagstruct.BitsPerSample = 8;
tagstruct.SamplesPerPixel = 3;
tagstruct.RowsPerStrip = 16;
tagstruct.PlanarConfiguration = Tiff.PlanarConfiguration.Chunky;
tagstruct.Software = 'MATLAB';

tif_obj.setTag(tagstruct);
tif_obj.write(s_RGB{1});
close(tif_obj);

for t = 1:T
    imwrite(s_RGB{t}, fileName,'tif', 'WriteMode','append','Compression','none');
end

