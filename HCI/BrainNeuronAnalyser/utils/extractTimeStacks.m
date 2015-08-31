function stacks = extractTimeStacks(signal, regions)

[R C T] = size(signal);
nr=size(regions, 2);

stacks = zeros(nr, T);
for reg=1:nr
    blob=regions{reg};
    for t=1:T
        bls=size(blob, 1);
        pixels=zeros(bls, 1);
        for b=1:bls
            pixels(b)=signal(blob(b, 1), blob(b, 2), t);
        end
        pixels=sort(pixels, 1, 'descend');
        stmax=round(5*bls/100)+1;
        edmax=round(20*bls/100);
        stacks(reg, t) = mean(pixels(stmax:edmax));
    end
end