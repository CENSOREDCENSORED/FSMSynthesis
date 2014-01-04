import operator

def interpretData(seed, numPartitionGroups, numScan, sizeScan):
    f = open(str(seed) + "PowOutput.csv", 'r')
    hasReadPartitions = False
    partitionlist = []
    scanChainValueList = []
    powerListLine = []
    powerList = []

    runCount = 0
    scValCount = 0
    powCount = 0
    spacecount = 0
    
    for line in f:
        #print line

        if runCount == 1:
            if scValCount != 1 and not line.startswith("Run"):
                #print "TODO: Handle the sc vals"
                scValList = [line.strip().split(',')]
                scValList[0].pop()
                scanChainValueList += scValList
                scValCount = 1
            elif powCount < numPartitionGroups:
                powCount = powCount + 1
                powerListLine += line.strip().split('\t')
                #print "TODO: Handle the power measurements"
            elif powCount == numPartitionGroups:
                powerList += [powerListLine]
                powerListLine = []
                powCount = 0
                scValCount = 0
                runCount = 0
                
        
        if line.startswith("Run"):
            hasReadPartitions = True
            runCount = 1


        if not hasReadPartitions:
            partitionLine = line.strip().split('\t')
            for partition in partitionLine:
                partitionSet = partition.strip().split(',')
                partitionlist += [partitionSet]

    f.close()

    rewardsList = genRewardsList(0.5, powerList, partitionlist, scanChainValueList)

    rewardsList.sort(key=operator.itemgetter(0),reverse=True)

    scPowCountList = []

    for i in range(numScan):
        scPowCountList += [[i,0]]

    for rewardsData in rewardsList:
        #print rewardsData
        scanChainVals = rewardsData[1]
        for scVal in scanChainVals:
            scPowCountList[int(scVal)][1] = scPowCountList[int(scVal)][1] + rewardsData[0]


    scPowCountList.sort(key=operator.itemgetter(1),reverse=True)
    for i in range(numScan):
        print scPowCountList[i]
       
    #print partitionlist
    #print scanChainValueList
    #print powerList
    print "Done"
    
def genRewardsList(threshold, powerList, partitionlist, scanChainValueList):
    rewardsList = []
    for j in range(len(powerList)):
        iteration = powerList[j]
        #print "TODO: the rewards function"
        for i in range(len(iteration)):
            powVal = float(iteration[i])
            if (powVal > threshold):
                rewardsList += [[powVal, partitionlist[i], scanChainValueList[j]]]
    
    return rewardsList
    
