%% this file was created for knn analysis the features extracted with C3D
% network

feaPath = '/home/hejw005/Documents/3dcnn/data/ModelNet40Fea';
feaLayer = 'fc7-1'; % one of the {'fc6-1','fc7-1'};
clipSet = '1'; % one of the {'1','17'};
testSet = 'test';
trainSet = 'train';

% load in data

featureTrainFile = [feaPath '/' feaLayer '/' trainSet clipSet '.fea'];
if exist(['feaTrain' feaLayer '.mat'],'file')
    load(['feaTrain' feaLayer])
    load(['labelTrain' feaLayer])
else
    [feaTrain,labelTrain] = loadc3dFeatures(featureTrainFile);
    save(['feaTrain' feaLayer],'feaTrain');
    save(['labelTrain' feaLayer],'labelTrain')
end

if exist(['feaTest' feaLayer '.mat'],'file')
    load(['feaTest' feaLayer])
    load(['labelTest' feaLayer])
else
    featureTestFile = [feaPath '/' feaLayer '/' testSet clipSet '.fea'];
    [feaTest,labelTest] = loadc3dFeatures(featureTestFile);
    save(['feaTest' feaLayer],'feaTest')
    save(['labelTest' feaLayer],'labelTest')
end

disp(['loaded done'])
% construct KNN classifier
mdl = fitcknn(feaTrain,labelTrain,'NumNeighbors',1);


preLabel = predict(mdl,feaTest);

acc = sum(preLabel == labelTest) / length(labelTest)



