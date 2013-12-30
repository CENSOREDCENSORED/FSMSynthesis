def interpretData(seed):
    f = open(str(seed) + "PowOutput.csv", 'r')
    hasReadPartitions = False
    partitionlist = []
    for line in f:
        #print line
        if line.startswith("Run"):
            hasReadPartitions = True

        if not hasReadPartitions:
            partitionlist += line.strip().split('\t')


    print partitionlist
    f.close()
