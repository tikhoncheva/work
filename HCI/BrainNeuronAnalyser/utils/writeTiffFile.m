function writeTiffFile(frames, fileName)
% frames = s_bk_sub;
% fileName = [sname, '_bk_sub_th', num2str(bk_th), '.tif'];

if ~isa(frames, 'uint16')
    frames = uint16(frames*65535);
end
dim = length(size(frames));
if dim == 4
    [h w ch T] = size(frames);
else
    [h w T] = size(frames);
    ch = 1;
end

tif_obj = Tiff(fileName, 'w') ;

tagstruct.ImageLength = h;
tagstruct.ImageWidth = w;

if dim == 4
tagstruct.Photometric = Tiff.Photometric.RGB;
else
tagstruct.Photometric = Tiff.Photometric.MinIsBlack;
end

tagstruct.BitsPerSample = 16;
tagstruct.SamplesPerPixel = ch;
tagstruct.RowsPerStrip = 16;
tagstruct.PlanarConfiguration = Tiff.PlanarConfiguration.Chunky;
tagstruct.Software = 'MATLAB';

tif_obj.setTag(tagstruct);

if dim == 4
    tif_obj.write(frames(:, :, :, 1));
else
    tif_obj.write(frames(:, :, 1));
end
close(tif_obj);

for t = 1:T
    if dim == 4
        imwrite(frames(:, :, :,t), fileName,'tif', 'WriteMode','append','Compression','none');
    else
        imwrite(frames(:, :, t), fileName,'tif', 'WriteMode','append','Compression','none');
    end
end