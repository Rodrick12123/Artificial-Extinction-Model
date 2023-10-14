folder = ''

seeds = range(10, 21)


for s in seeds:
        header = "update tasks_solved\n"
        outputFileName = "munged_ReplicationData" + "SEED" + str(s) + ".dat"
        outFile = open(outputFileName, 'w')
        outFile.write(header)
        fname = folder + "SEED" + str(s) + ".data"
        curFile = open(fname, 'r')
        for line in curFile:
            if (line[0] != "u"):
                splitline = line.split(',')
                outstring1 = "{} {}\n".format(splitline[0], splitline[1])
                outFile.write(outstring1)
        curFile.close()
outFile.close()