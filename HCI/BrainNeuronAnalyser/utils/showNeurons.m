function out_image = showNeurons(regions, image, probability)

nr = size(regions, 2);

if nargin < 2
    image = zeros(64, 128);
end
if nargin < 3
    probability=ones(1, nr);
end

nc = 256;
colors = uint8(jet(nc)*nc);
out_image = cat(3, image, image, image);
for reg=1:nr
    val = floor(probability(reg)*(nc-1))+1;
    predictionInserter = vision.ShapeInserter('Shape','Circles','BorderColor','Custom','CustomBorderColor',colors(val, :));
    
    pixels=regions{reg};
    pxn=size(pixels, 1);
    x0=min(pixels(:, 2));
    x1=max(pixels(:, 2));
    y0=min(pixels(:, 1));
    y1=max(pixels(:, 1));
    circle=int32([(x1+x0)/2, (y1+y0)/2, (x1-x0)/2]);
    out_image = step(predictionInserter, out_image, circle);
end