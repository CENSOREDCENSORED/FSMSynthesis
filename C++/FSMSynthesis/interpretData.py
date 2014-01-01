def interpretData(seed, numPartitionGroups, numScan, sizeScan):
    f = open(str(seed) + "PowOutput.csv", 'r')
    hasReadPartitions = False
    partitionlist = []
    scanChainValueList = []
    powerList = []
    powerChunk = []

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
                scanChainValueList += scValList
                scValCount = 1
            elif powCount < numPartitionGroups:
                powCount = powCount + 1
                powerList += line.strip().split('\t')
                #print "TODO: Handle the power measurements"
            elif powCount == numPartitionGroups:
                powerChunk += [powerList]
                powerList = []
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
    
    for iteration in powerChunk:
        print "TODO: the rewards function"
        
    print partitionlist
    #print scanChainValueList
    #print powerChunk
    print "Done"
    

