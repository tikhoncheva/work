function [regions, mask] = segmentationLoG(neuron_map)

[R C] = size(neuron_map);

min_scale = 1.5;
max_scale = 4.5;
lin_scale = min_scale:0.01:max_scale;
result_log = zeros(R, C, size(lin_scale, 2));

i=1;
for scale=lin_scale
    filt_size =  2*ceil(3*scale)+1;
    h = -scale^2 * fspecial('log', [1 1]*filt_size, scale);
    result_log(:, :, i) = (imfilter(neuron_map,h));
    i=i+1;
end
result = max(result_log, [], 3);


mask_nms = imregionalmax(result);
clear maxima_p
[maxima_p(:, 1), maxima_p(:, 2)] = find(mask_nms==1);
maxima_p(:, 3) = result(mask_nms==1);
maxima_p_sort = sortrows(maxima_p, -3);
maxima_p_sort = maxima_p_sort(maxima_p_sort(:, 3)>0.05, :);

fillInserterBW = vision.ShapeInserter('Shape','Circles','Fill',true, 'FillColor', 'Custom','CustomFillColor',uint8(255));


min_distance = 6;

clear blobs
count=1;
mask = zeros(R, C, 'uint8');
sqrt_two = sqrt(2);
used_p = [];
for mp=1:size(maxima_p_sort, 1)
    r=maxima_p_sort(mp, 1); c=maxima_p_sort(mp, 2);
    far = true;
    for i=1:size(used_p, 1)
        if sqrt((r-used_p(i, 1))^2 + (c-used_p(i, 2))^2) < min_distance
            far = false;
            break
        end
    end
    if far
        used_p = [used_p; r, c];
        [s] = find(result_log(r, c, :)==maxima_p_sort(mp, 3));

%         fprintf('Pick #%d: %2.2f -> [%3d,%3d] scale %.3f\n', size(used_p, 1), maxima_p_sort(mp, 3), r, c, lin_scale(s));
        radius = sqrt_two*lin_scale(s);
        circle = int32([c r radius]);
        mask = step(fillInserterBW, mask, circle);
        
        % Save pixels of the circle as blob
        blob_img = zeros(R, C, 'uint8');
        blob_img = step(fillInserterBW, blob_img, circle);
        clear blob
        [blob(:, 1), blob(:, 2)] = find(blob_img~=0);
        blobs{count}=blob;
        count=count+1;
    end
end
regions = blobs;