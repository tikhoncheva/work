
%% path to the folder with the tif videos

path_signals = ['..', filesep, 'brain_data', filesep, 'signals', filesep, 'new_03092015', filesep, '15_09_03_animal1_CaI_Base_Net1_14_09_11_small', filesep];
readpath = path_signals;
savepath = ['new_03092015', filesep, '15_09_03_animal1_CaI_Base_Net1_14_09_11_small', filesep];

d = dir([path_signals, '*.tif']);
filenames = {d.name}';


nSignals = size(filenames,1)/2;
assert(nSignals-floor(nSignals)==0, 'Folder should contain even number of files');

for i = 1: nSignals
    sname = filenames{2*i};
    sname = sname(1:21);
    preprocessing_brain_video(sname, readpath, savepath);
   
   
end
