function circles = regionToCircles(regions)

R = size(regions, 2);
circles = zeros(R,3);
for r=1:R
    pixels=regions{r};
    x0=min(pixels(:, 2));
    x1=max(pixels(:, 2));
    y0=min(pixels(:, 1));
    y1=max(pixels(:, 1));
    circles(r,:) = floor([min(128,(x1+x0)/2), min(64,(y1+y0)/2), (x1-x0)/2]);
end