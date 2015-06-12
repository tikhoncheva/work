

function [filename_scircle_centers] = detection_small_circle_centers(mainDir, videoName, filename_scircle_centers)

    frameDir = [mainDir, 'frames_', videoName, filesep];

    images = dir([frameDir,'*.jpg']);
    nFrame = size(images,1);

    res = [];

    for fr=1:nFrame
        im1 = rgb2gray(imread([frameDir, images(fr).name]));


        im_t = adapthisteq(im1);
        im_t = imgaussfilt(im_t, 2);

        [centers, radii] = imfindcircles(im_t,[10 15],'ObjectPolarity','dark', 'Sensitivity',0.95);

        res = [res; centers];

    end


    A= round(res);
    resMat = zeros(640, 1280);
    for i=1:size(A,1)
    resMat(A(i,2),A(i,1))=resMat(A(i,2),A(i,1))+1;
    end

    points = res;
    
    save( [mainDir, '/', filename_scircle_centers], 'points');

end
