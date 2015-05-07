 
path = '/export/home/etikhonc/Documents/Work/Videos/Wheel/Maus 4 Cal/4 weeks 0p.o/Naehe Network 1 old/';
fileName = 'Trial_201411122034231.avi';

inframe = extractFramesFromVideo(path, fileName);

n = size(inframe);
threshold = 10;
outframe  = (cell(n-1));


B = inframe{1};

figure, hold on;
for t = 1:n-1
    outframe{t} = abs(inframe{t+1}-B);
    outframe{t}(outframe{t}<threshold) = 0;
    
    imshow(outframe{t});
    
end




