import os # imported to use the getcwd() function, which allows the writeFrequencyFile
          # function to print the directory the file was created in.


#produce manager created as a class in order to cut down on work having to be redone,
#as data that requires persistance can be stored with the class instance
class ProduceManager:

    #parameterized constructor. As everything depends on this one python class,
    #a default constructor was not given. takes two strings as arguments, both
    #should be file names, ideally with file extensions.
    def __init__(self, inputFileName, outputFileName): 
        self.dataFileName = inputFileName #set instance's dataFileName to input file name given by C++
        self.outputFileName = outputFileName #set instance's outputFileName to output file name given by C++
        self.produceDict = {} #dictionary to be used to hold produce data from data file

    #this method returns no data, and populates the instances produce dictionary with items sold today
    #along with the quantity of each item sold. 
    def totalProduceFrequencies(self):
        #open a file whose name matches that given upon instantiation, in read mode.
        produceRecord = open(self.dataFileName, 'r') 
        #iterate over open file stream, line by line. For loop automatically stops when EOF is reached
        for itemLine in produceRecord:
           produceItem = itemLine.strip() #strip whitespace from both ends of string on each line
           #search this instance's produceDict for a key matching the word retrieved from the file'
           #if it is not there...
           if (produceItem not in self.produceDict): 
               self.produceDict[produceItem] = 1 #include it in the dictionary by giving it a value of one
           else: #if it's already in there...
               self.produceDict[produceItem] += 1 #...increment its value by one.
        produceRecord.close() #always clean up after yourself! file closed!
    
    #this method returns no data, and prints a prettified list of all items sold today, followed
    #by the quantity of the items sold today
    def printAllProduceFrequencies(self):
        print(" ======================= ") #this is just the list header
        #print formatted title line, with the "Produce Name" left justified in a 12-character-width field,
        #followed by a right-justified "Quantity" header, in an 8-character-width field
        print("|%(produceName)-12s\t%(itemQty)8s|" % {"produceName" : "Produce Name", "itemQty" : "Quantity"})
        print("|=======================|") #also just a header
        for item in self.produceDict.items(): #iterate over each item in this instance's dictionary of item names/qtys
            print("|%(produceName)-12s\t%(itemQty)8s|" % {"produceName" : item[0], "itemQty" : item[1]})
        print("|-----------------------|") #just a footer!
        input("Press any key to continue...") #pause program, prompt user to hit enter to continue


    #this method is used to return a single item's quantity sold today, as found in the
    #instance's produce dictionary. Takes one string, itemName, as a search key
    def returnProduceFrequency(self, itemName):
        match = False #set booolean match to false as a defualt state
        for item in self.produceDict.items(): #iterate over the instance's produce dictionary
            if item[0] == itemName: #if the key (item[0]) matches the key itemName...
                return item[1] #...return the value, its quantity (item[1])
                match = True #turn match to true
        if (match == False or itemName == 'quit'): #if 'quit' is given or no match found...
           return -1 #...return -1.
    
    #This method is used to put the data that was parsed from the given input file
    # in a cleaner format in the given output file name, namely, following the format:
    # <itemName> <itemQty>\n  (notice the *single* space between itemName and itemQty.
    def writeFrequencyFile(self):
        #create a file of the given output name, assign it to the stream in write mode
        outputFile = open(self.outputFileName, 'w') 
        print("File Name: ", self.outputFileName) #print the filename so the user knows what to look for
        for produceItem in self.produceDict.items(): #iterate over each item in the instance's produce dictionary
            #create a line to write to the file in the format of <itemName><space><itemQty><newline>
            fileLine = produceItem[0] + " " + str(produceItem[1]) + "\n" 
            #write line to file
            outputFile.write(fileLine)    
        #print a message to user showing the location of the file as created.
        print("Wrote to Location: " + os.getcwd() + "\\" + self.outputFileName)