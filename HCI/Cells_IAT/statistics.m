clc; clear;
% Piotr Dollar toolbox
addpath(genpath('../Tools/piotr_toolbox_V3.26/'));
addpath(genpath('../Tools/inexact_alm_rpca/'));
%%
pathIn = ['.', filesep, 'signals_aligned', filesep];
% pathIn = ['.', filesep, 's_ns', filesep];
sname = '2014_09_13__21_58_05h';
pathOut = ['.', filesep, 'statistics', filesep];
% suffix = '';
%%

fName_stiff = [pathIn, 'aligned_channel01_', sname, '.tif'];

info = imfinfo(fName_stiff);
T = numel(info);

s = [];
for t = 1:T
    img = im2double(imread(fName_stiff, t, 'Info', info));
    s = cat(3, s, img);    
end


% fName_nstiff = [pathIn, 'ns_', sname, '.tif'];
% info = imfinfo(fName_nstiff);
% T = numel(info);
% 
% ns = [];
% for k = 1:T
%     img = im2double(imread(fName_nstiff, k, 'Info', info));
%     ns = cat(3, ns, img);    
% end



%% size of a square patch

[m,n] = size(s(:,:,1));     % size of the images

w = 10; % width of a patch

Px = n-w+1; %floor(n/w)+1; % number of patches in each row
Py = m-w+1; %floor(m/w)+1; % number of patches in each column

H = zeros(Py,Px,T); % histogram of max values in time 

for t = 1:T
   img = s(:,:,t);
   
   for i = 1:Py
       for j = 1:Px
           H(i,j,t) = max(max(img(i:i+w-1,j:j+w-1)));
       end   
   end

end

% for t = 1:T
%     img = s(:,:,t);
% %     borderSize = [floor(w/2) floor(w/2)];
%     max_val = blockproc(img, [10 10], @(x) max(x.data(:)), 'BorderSize', [0 0], ...
%                                          'TrimBorder', false, 'PadPartialBlocks', true);
% end

%% find interesting histograms
thr = 0.2;

medH = median(H,3);
maxH = max(H,[],3);

diff = maxH - medH;
[I,J] = find(logical(diff>thr));
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

%% 

[I,J] = find(true(Py,Px) ); % all patches are interesting
nInterPatches = size(I,1);

s_RGB = cell(1,T);

for t =1:T
   img = s(:,:,t); 
   imgRGB = repmat(uint8(255.*img),[1 1 3]);
   img_hl = imgRGB;
   
   for k = 1:nInterPatches
   
       i = I(k);
       j = J(k);
       
       if pick(i,j,t)
           u = max(1,(i-1)*w+1); b = min(m,(i-1)*w+1+w);
           l = max(1,(j-1)*w+1); r = min(n,(j-1)*w+1+w);
           mask = zeros(m,n); mask(u:b,l:r) = 1;

           maskRGB(:,:,1) = round(mask*50); % All zeros.
           maskRGB(:,:,2) = zeros(size(mask)); % Round since we're dealing with integers.
           maskRGB(:,:,3) = zeros(size(mask)); % All zeros. 

           maskRGB = uint8(maskRGB);
           img_hl = img_hl + maskRGB;
       end   
   end
   s_RGB{t} = img_hl;
end
   
%%
% imwrite(double(s_RGB{1})/255, [pathOut, 's_', sname, '.tif']);
% for t = 2:2
%     imwrite(double(s_RGB{t})/255, [pathOut, 's_', sname, '.tif'], 'WriteMode','append', 'Compression','none', 'ColorSpace', 'rgb');
% end

writerObj = VideoWriter([pathOut, 's_', sname, '.avi']);
open(writerObj);

for t = 1:T
    writeVideo(writerObj,s_RGB{t});
end

close(writerObj);

 %%
D = reshape(H, 55*119, 374);
[A_hat E_hat iter] = inexact_alm_rpca(D);
A = reshape(A_hat, 55, 119, 374);
%%
s4 = [];
for i = 1 : T
    img = H(:,:, i);
    s4 = cat(3, s4, img);
end
implay(s4)