import os
model = []
basePath = "/home/hejw005/Documents/3dcnn/data/ModelNet40/airplane/"
for root, dirs, files in os.walk("/home/hejw005/Documents/3dcnn/data/ModelNet40/airplane", topdown=True):
    for name in files:
        # model = model + ' '.join(root).join(name)
        # print root.split(basePath)[1]
        # print name
        model.append(root.split(basePath)[1] + '/' + name)
        # print root.split(basePath)[1] + '/' + name

#    for name in dirs:
#        print(os.path.join(root, name))

modelSorted = sorted(model)


model = ""

for ele in modelSorted:
	model = model + " " + ele
#	print ele

# remove the first space
model = model[1:]

print model
