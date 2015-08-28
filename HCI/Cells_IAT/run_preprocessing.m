
%% path to the folder with the tif videos
path_signals = ['.', filesep, 'signals', filesep, 'new', filesep];

d = dir([path_signals, '*.tif']);
filenames = {d.name}';


nSignals = size(filenames,1)/2;
assert(nSignals-floor(nSignals)==0, 'Folder should contain even number of files');

for i = 1: nSignals
    sname = filenames{2*i};
    sname = sname(1:21);
    preprocessing_brain_video(sname);
   
   
end
