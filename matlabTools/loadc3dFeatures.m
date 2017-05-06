%% feature load
function [fea, label] = loadc3dFeatures(feaName)
fea = load(feaName);
label = fea(:,end);
fea = fea(:,1:end-1);
