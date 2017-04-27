import os
model = []
# modelCategory = 'bed'
# basePath = "/home/hejw005/Documents/3dcnn/data/ModelNet40/"+modelCategory
basePath = "/home/hejw005/Documents/3dcnn/data/ModelNet40/"
for root, dirs, files in os.walk(basePath, topdown=True):
    for name in files:
        # model = model + ' '.join(root).join(name)
        # print root.split(basePath)[1]
        # print name
        if(name[-4:] != '.jpg'):
        # model.append(root.split(basePath)[1] + '/' + name)
        	print root.split(basePath)[1] + '/' + name

print 'done'
#    for name in dirs:
#        print(os.path.join(root, name))

modelSorted = sorted(model)

for ele in modelSorted:
	print ele

