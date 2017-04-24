import os

# the total number of the photos, where every 16 is a batch
Num_photo = 256

# get Model list
def getModelList():
	model = []
	basePath = "/home/hejw005/Documents/3dcnn/data/ModelNet40"
	for root, dirs, files in os.walk(basePath, topdown=True):
		for name in files:
			if(name[-4:] == '.jpg'):
				model.append(root.split(basePath)[1] + '/' + name)

	print 'extract Model list done'

	modelSorted = sorted(model)

	return modelSorted

basePath = "/home/hejw005/Documents/3dcnn/data/ModelNet40"

myModelList = getModelList()

for ele in myModelList:
	print basePath + ele
	print basePath + ele[:-11] + '00' + ele[-8:]
	os.rename(basePath + ele, basePath + ele[:-11] + '00' + ele[-8:])
	
print 'rename done'