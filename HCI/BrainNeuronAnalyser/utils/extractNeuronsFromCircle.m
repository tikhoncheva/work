function neurons = extractNeuronsFromCircle(signal, circles)

predictionInserter = vision.ShapeInserter('Shape','Circles','Fill', true, 'FillColor','Custom', 'CustomFillColor',uint8(255));

[R C T] = size(signal);
nc=size(circles, 1);

neurons = zeros(nc, T);
for i=1:nc
    img = zeros(R, C);
    img = step(predictionInserter, img, circles(i, :));
%     imshow(img)
    reg=bwboundaries(img);
    blob = reg{1};
    for t=1:T
        bls=size(blob, 1);
        pixels=zeros(bls, 1);
        for b=1:bls
            pixels(b)=signal(blob(b, 1), blob(b, 2), t);
        end
        pixels=sort(pixels, 1, 'descend');
        stmax=round(5*bls/100)+1;
        edmax=round(20*bls/100);
        neurons(i, t) = mean(pixels(stmax:edmax));
    end
end