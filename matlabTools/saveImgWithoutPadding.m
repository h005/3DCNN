%% this script for save the image without padding
function saveImgWithoutPadding(img, imgName)
fig = figure;
image(img,'CDataMapping','scaled')
colormap jet


axis off
% save the figure first
savePath = imgName;
saveas(fig,savePath);
R = imread(savePath);
Rgray = rgb2gray(R);
Rcol = sum(Rgray);
Rrow = sum(Rgray,2);
RrowGrad = abs(Rrow(2:end) - Rrow(1:end-1));
[RrowGrad,RrowIndex] = sort(RrowGrad,'descend');

RcolGrad = abs(Rcol(2:end) - Rcol(1:end-1));
[RcolGrad,RcolIndex] = sort(RcolGrad,'descend');

rowSize = [RrowIndex(1),RrowIndex(2)];
colSize = [RcolIndex(1),RcolIndex(2)];
rowSize = sort(rowSize);
colSize = sort(colSize);

R = R(rowSize(1):rowSize(2),colSize(1):colSize(2),:);

imwrite(R,savePath);

close all

disp(['save to ' savePath])