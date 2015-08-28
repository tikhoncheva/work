function preprocessing_brain_video(sname)
    % clc; clear;
    % addpath('./signal/new/');
    % addpath('../common', 'utils');

    
    addpath(genpath('../Tools/IAT_v0.9.1/'));
    addpath(genpath('../Tools/BM3D/'));

%     pathIn = ['.', filesep, 'signals', filesep, 'new', filesep];
    pathIn = ['.', filesep, 'signals', filesep];
    pathOut_signal_aligned = ['.', filesep, 'signals_aligned', filesep];
    pathOut_s = ['.', filesep, './s_ns', filesep];

%     sname = '2014_09_10__18_07_13h';

    %% Channel 1
    fName_tiff = [pathIn, sname, '__channel01.tif'];

    info = imfinfo(fName_tiff);
    nImages = numel(info);

    channel1 = [];
    for k = 1:nImages
        img1 = im2double(imread(fName_tiff, k, 'Info', info));
        channel1 = cat(3, channel1, img1);
    end


    %% Channel 2
    fName_tiff = [pathIn, sname, '__channel02.tif'];

    info = imfinfo(fName_tiff);
    nImages = numel(info);

    channel2 = [];
    for k = 1:nImages
        img2 = im2double(imread(fName_tiff, k, 'Info', info));
        channel2 = cat(3, channel2, img2);
    end

    %% Convert to  double

    ma1 = double(max(channel1(:)));
    mi1 = double(min(channel1(:)));

    ma2 = double(max(channel2(:)));
    mi2 = double(min(channel2(:)));

    channel1_d = []; channel2_d = [];

    for k = 1:nImages
        img1 = channel1(:,:,k);
        img1_d = (double(img1)-mi1)/(ma1-mi1);
        channel1_d = cat(3, channel1_d, img1_d);

        img2 = channel2(:,:,k);
        img2_d = (double(img2)-mi2)/(ma2-mi2);
        channel2_d = cat(3, channel2_d, img2_d);
    end


    %% Delete some frames
    nImages = size(channel1_d, 3);
    frame_list = [1:nImages];
 
    frames2del = [1]; % [1; 47; 123; 124]
 
    frame_list(frames2del) = [];
    nImages = numel(frame_list);

    %% Template image
    T1 = mean(channel1_d(:,:, frame_list),3);
    T2 = mean(channel2_d(:,:, frame_list),3);

    %% Alignment

    channel1_aligned = [];
    channel2_aligned = [];

    % parameters for ECC (Enhanced Correlation Coefficient) and Lucas-Kanade 

    % possible transformation types:  {'translation', 'euclidean','similarity','affine','homography'}
    transform = 'euclidean';

    par = [];
    par.levels =    3;
    par.iterations = 15;
    par.transform = transform;

    tic;

    for k = 1:nImages
        i = frame_list(k);    

        img1 = channel1_d(:,:,i);
        img2 = channel2_d(:,:,i);

        % Lucas-Kanade algorithm
        [LKWarp1]=iat_LucasKanade(img1, T1, par);
        [LKWarp2]=iat_LucasKanade(img2, T2, par);

        % Compute the warped image and visualize the error
        [img1aligned, ~] = iat_inverse_warping(img1, LKWarp1, par.transform, 1:size(T1,2),1:size(T1,1));
        [img2aligned, ~] = iat_inverse_warping(img2, LKWarp2, par.transform, 1:size(T2,2),1:size(T2,1));

        ind1 = (img1aligned==0); 
        ind2 = (img2aligned==0); 

        img1aligned(ind1) = T1(ind1);
        img2aligned(ind2) = T2(ind2);

        channel1_aligned = cat(3, channel1_aligned, img1aligned);
        channel2_aligned = cat(3, channel2_aligned, img2aligned);
    end

    display(sprintf('alignment done in %0.3f sec', toc));

    %%
    writeTiffFile(channel1_aligned, [pathOut_signal_aligned, 'aligned_channel01_', sname, '.tif']);
    writeTiffFile(channel2_aligned, [pathOut_signal_aligned, 'aligned_channel02_', sname, '.tif']);

    %%
    channel1_d_aligned = channel1_aligned;
    channel2_d_aligned = channel2_aligned;
    
    %% Remove gaussian noise

    sigma = 10;
    [PSNR, channel1_d_aligned] = VBM3D(channel1_d_aligned, sigma, 0, 0);
    [PSNR, channel2_d_aligned] = VBM3D(channel2_d_aligned, sigma, 0, 0);

    %% create new signal s = channel1_alligned(x,y,t)/channel1_alligned(x,y,t)

    s = double(channel1_d_aligned);
    ind_div0 = (channel2_d_aligned ==0);
    channel2_d_aligned(ind_div0) = 1;
    s = s./double(channel2_d_aligned);
    channel2_d_aligned(ind_div0) = 0;


    mi = min(s(:));
    ma = max(s(:));
    s = (s-mi)./(ma-mi);

    mi = 0.2;
    ma = 0.8;
    s = s-mi;
    s(s<0)=0;
    s = s./(ma-mi);
    s(s>1)=1;

    writeTiffFile(s, [pathOut_s, 's_', sname, '.tif']);

    s_sumsqr = sum(s.^2,3);
    imwrite(s_sumsqr, [pathOut_s, 'sscm_', sname, '.jpg'])

end