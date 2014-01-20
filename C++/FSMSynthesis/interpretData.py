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
    if len(rewardsList) == 0:
        rewardsList = genRewardsList(0.4, powerList, partitionlist, scanChainValueList)
    if len(rewardsList) == 0:
        rewardsList = genRewardsList(0.3, powerList, partitionlist, scanChainValueList)
    if len(rewardsList) == 0:
        rewardsList = genRewardsList(0.2, powerList, partitionlist, scanChainValueList)
    if len(rewardsList) == 0:
        print "Trojan not detected"
        return
    rewardsList.sort(key=operator.itemgetter(0),reverse=True)
    print rewardsList[0]

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


    staticSCPowCountList = []
    for i in range(numScan):
        staticSCPowCountList += [[i,[]]]
    for rewardsData in rewardsList:
        #print rewardsData
        dynamicScanChainNums = rewardsData[1]
        staticScanChainVals = rewardsData[2]
        for i in range(numScan):
            if i not in dynamicScanChainNums:
                staticScanChainVal = staticScanChainVals[i]
                staticSCPowCountList[i][1] += [staticScanChainVal]
            
        #print rewardsData

    #for i in range(numScan):
    #    print staticSCPowCountList[i]

    staticSCPowCountStats = []
    for i in range(numScan):
        staticSCVals = staticSCPowCountList[i][1]
        listStats = []
        for j in range(sizeScan):
            listStats += [0]
        for val in staticSCVals:
            intVal = int(val,16)
            for j in range(sizeScan):
                if (intVal & 1) == 0:
                    listStats[j] -= 1
                else:
                    listStats[j] += 1
                intVal = intVal >> 1
        staticSCPowCountStats += [[i, listStats]]

    for staticSCVal in staticSCPowCountStats:
        print staticSCVal
    
    #print partitionlist
    #print scanChainValueList
    #print powerList

    #f = open(str(seed) + "Round2Input", 'w')
    f = open("Round2Input.txt", 'w')
    f.write(str(seed) + "\n")
    f.write(str(numPartitionGroups) + "\n")
    f.write(str(numScan) + "\n")
    f.write(str(sizeScan) + "\n")
    for staticPowStats in staticSCPowCountStats:
        listThing = staticPowStats[1]
        currNum = 0
        for i in range(sizeScan):
            elem = listThing[i]
            if elem > 0:
                currNum |= (0x1 << i)

        f.write(str(currNum) + "\n")

    for elem in scPowCountList:
        f.write(str(elem[0]) + "\n")
    
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
    
