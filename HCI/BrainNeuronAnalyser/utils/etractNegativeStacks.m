function neg_stacks = etractNegativeStacks(signal, mask, neg_num)
[R C T] = size(signal);

%% Pick up random negative stacks
fillInserterBW = vision.ShapeInserter('Shape','Circles','Fill',true, 'FillColor', 'Custom','CustomFillColor',uint8(255));
radius=2;
% neg_num=200;

pos_mask = mask;
neg_mask=zeros(R, C, 'uint8');
count=1;
while count <= neg_num
    x=randi(C, [1, 1]);
    y=randi(R, [1, 1]);
    %r=randi(radius, [1, 1]);
    r=radius;
    
    circle = int32([x y r]);
    blob_img = zeros(R, C, 'uint8');
    blob_img = step(fillInserterBW, blob_img, circle);
    if all(blob_img.*pos_mask ==0)
%         fprintf('%d neg samples extracted\n', count);
        clear reg
        [reg(:, 1), reg(:, 2)] = find(blob_img~=0);
        neg_regions{count} = reg;
        count=count+1;
        
        pos_mask = step(fillInserterBW, pos_mask, circle);
        neg_mask = step(fillInserterBW, neg_mask, circle);
    end
end
fprintf('%d neg samples extracted\n', count-1);

% test_img = cat(3, mask, neg_mask, zeros(R, C, 'uint8'));
% figure(32);
% subplot(1, 3, 1); imshow(mask);
% subplot(1, 3, 2); imshow(neg_mask);
% subplot(1, 3, 3); imshow(test_img);

%% Create negative stacks
nnr=size(neg_regions, 2);

neg_stacks = zeros(nnr, T);
for reg=1:nnr
    blob=neg_regions{reg};
    for t=1:T
        bls=size(blob, 1);
        pixels=zeros(bls, 1);
        for b=1:bls
            pixels(b)=signal(blob(b, 1), blob(b, 2), t);
        end
        pixels=sort(pixels, 1, 'descend');
        if bls > 5
            stmax=round(5*bls/100)+1;
            edmax=round(20*bls/100);
            neg_stacks(reg, t) = mean(pixels(stmax:edmax));
        else
            neg_stacks(reg, t) = mean(pixels);
        end
    end
end