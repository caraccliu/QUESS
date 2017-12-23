/** 
  * File:     QUESS.cpp 
  * 
  * Author1:  Lucian Ilie (ilie@uwo.ca) 
  * Author2:  Stephen Lu (slu93@uwo.ca) 
  * Date:     Fall 2017  
  * 
  *   QUality Error correction using Spaced Seeds
  *
  *   This file contains code concerning the main functions of
  *   QUESS. QUESS is an error correction program designed to 
  *   efficiently correct NGS data using spaced seeds.
  *   Read more at:
  *
  */


#include "QUESS.h"
//#define VERBOSE



/**
  * Name:               void show_usage() {
  *
  * Description :       Shows usage
  *
  * Input :
  *
  * Process Synopsis :
  *
  * Notes :             
  *
  */
void show_usage()
{
  cerr << "Usage: ./QUESS <option(s)>\n"
       << "Required:\n"
       << "\t-i,--input-file <filename>\t\tSpecify the target fastx file\n"
       << "\t-g,--genome-length <genome length>\tSpecify the approximate length of the genome\n\n"
       << "Optional:\n"
       << "\t-h,--help\t\t\t\tShows Help\n"
       << "\t-n,--number-of-seeds\t\t\tSpecify number of seeds 1-8 (default 8)\n"
       << "\t-w,--weight <weight>\t\t\tSpecify weight of seeds 10-26 (default to determined by program)\n\n"
       << "Example Usage:\n"
       << "./QUESS -g 2000000 -i file.fastq"
       << endl;
}


/**
  * Name:               bool legal_int(char *str) {
  *
  * Description :       determine intger
  *
  * Input :
  *       Parameters:
  *           char*     str               input char array
  *
  *       Return:
  *           bool                        true if integer    
  * Process Synopsis :
  *
  * Notes :             
  *
  */


bool legal_int(char *str) {
    while (*str)
        if (!isdigit(*str++))
            return false;
    return true;
}

/**
  * Name:               int main(int argc, char* argv[])
  *
  * Description :       Main function of program
  *
  * Input :
  *       Parameters:
  *           int       argc                Number of arguments passed to program 
  *           char*     argv[]              An array containing the aforementioned arguments
  *
  *
  * Process Synopsis :
  *                     [1]  Creates the working temp files using input arguments
  *                     [2]  Compute seeds and bucket size (number of reads simultaneously read)
  *                     [3]  For each seed:
  *                     [4]         Insert the sMers of the input fastx file
  *                     [5]         Rehash the aforementioned sMers into only values with high frequency
  *                     [6]         Insert the sGaps of the input fastx file
  *                     [7]         Determine the sGaps with the highest amount of support/delete ambigious sMers
  *                     [8]         Correct the temp fastx file based on the correct sGaps
  *                     [9]  Delete the working files and finalize results
  *
  * Notes :             
  *
  */

int main(int argc, char* argv[])
{

  time_t iteration_time_start, iteration_time_end, total_time_start, total_time_end;
  time(&total_time_start);
  int weight = 0, numberOfSeeds = 0;
  uint64_t genomeLength=0;
  char *inputFileName = new char [1000];
  bool setFile=false;
  if (argc < 3) {
      show_usage();
      return 1;
  }
  for (int i = 1; i < argc; ++i) {
      string arg = argv[i];
      if ((arg == "-h") || (arg == "--help")) {
          show_usage();
          return 0;
      } 
      else {
        if ((arg == "-g") || (arg == "--genome-size")){
          if (i+1 <argc && legal_int(argv[i+1])){
              genomeLength=strtoull(argv[i+1], NULL, 10);
          }
          else{
            cerr << "--genome-size requires a integer argument! Run ./QUESS --help for all options!"<<endl; 
            exit(1);
          }
        }
        if ((arg == "-i") || (arg == "--input-file")){
          if (i+1 <argc){
            inputFileName=argv[i+1];
    	    setFile=true;
          }
          else{
            cerr << "--input-file requires a filename! Run ./QUESS --help for all options!"<<endl; 
            exit(1);
          }
        }
        if ((arg == "-w") || (arg == "--weight")){
          if (i+1 <argc && legal_int(argv[i+1]) && strtoull(argv[i+1], NULL, 10)>=10 && strtoull(argv[i+1], NULL, 10)<=26 ){
            weight=strtoull(argv[i+1], NULL, 10);
          }
          else{
            cerr << "--weight requires an integer between 10 to 26! Run ./QUESS --help for all options!"<<endl; 
            exit(1);
          }
        }
        if ((arg == "-n") || (arg == "--number-of-seeds")){
          if (i+1 <argc && legal_int(argv[i+1]) && strtoull(argv[i+1], NULL, 10)>=1 && strtoull(argv[i+1], NULL, 10)<=8 ){
            numberOfSeeds=strtoull(argv[i+1], NULL, 10);
          }
          else{
            cerr << "--number-of-seeds requires an integer between 1 to 8! Run ./QUESS --help for all options!"<<endl; 
            exit(1);
          }
        }
      }
    }
    // correcting parameters; ABOVE T means >= T, BELOW means < T
    // Tc and Te will be computed later; Tdiff is given here
  int Tc = 8;       // Frequent s_mers or s_gaps: count >= Tc; default = 8 (will be recomputed later)
  int Te = Tc / 2;        // Erroneous s_mers or s_gaps: count < Te:  Te < Tc; default = 4 (will be Te = Tc/2)
  int Tdiff = 2;     // Maximum s_gap difference allowed for replacing an s_gap with another; default = 2
	
	
    uint64_t peakMemory=0,currentMemory=0;
  // ============== SEEDS =================
    int64_t numberOfReads = 0;
    int64_t readLength = 0, totalReadLength = 0;    // readLength = (int64_t) (totalReadLength / numberOfReads)
    
    if (numberOfSeeds == 0)
      numberOfSeeds = 8;
    if (genomeLength==0){
      cerr << "--genome-length is required! Run ./QUESS --help for all options!"<<endl;  
      exit(1);
    }
    if (setFile==false){
      cerr << "--input-file is required! Run ./QUESS --help for all options!"<<endl;
      exit(1);
    }
    // ============ FILES ====================
    // make a copy of input: input.ext -> input_copy_w_k.ext (w = weight, k = number of seeds)
    // create output: input.ext -> input_corrected_w_k.ext
    // argv[3] = original dataset (FASTX)
    // inputTempFile = only the reads from datasetName
    // outputTempFile = only reads but corrected; moved into inputTempFile after each iteration
    // outputFile = corrected dataset (FASTX); created at the end with headers and scores from original dataset and corrected reads from outputTempFile

    char* inputTempFileName = new char[10000];
    char* outputTempFileName = new char[10000];
    char* outputFileName = new char[10000];
    ifstream inputTempFile;
    fstream outputTempFile;
    ofstream outputFile;

    createWorkingFiles(inputTempFileName, outputTempFileName, inputFileName, outputFileName, outputFile,numberOfReads, totalReadLength,peakMemory,currentMemory);
    cout <<inputFileName<<endl;
    if (totalReadLength>1000000000 && weight==0)
        weight=20; 
    if (totalReadLength<=1000000000 && weight==0)
        weight=16;
    char** seeds = new char*[numberOfSeeds]; // seed will hold the actual seed provided by the function getSeed
    for (int64_t i = 0; i < numberOfSeeds; ++i)
    {
        seeds[i] = new char[1000];
      getSeed(seeds[i],weight, numberOfSeeds, i, peakMemory, currentMemory);
    }
    
    cout << "============ SEEDS ============\n";
    cout << "weight = " << weight << endl;
    cout << "numberOfSeeds = " << numberOfSeeds << endl;
    cout << "seeds:" << endl;
    for (int64_t i = 0; i < numberOfSeeds; ++i)
        cout << seeds[i] << endl;

    //===== useful precomputed arrays =======
    uint64_t* diff16Bits = new uint64_t[1 << 16];
    for (uint64_t i = 0; i < (1 << 16); ++i)
        diff16Bits[i] = HashTable::computeDiff16Bits(i);
    
    currentMemory+=3*10000*sizeof(char)+(1 << 16)*sizeof(uint64_t)+5*sizeof(uint64_t)+4*sizeof(int)+numberOfSeeds*1000*sizeof(char)+430*sizeof(uint64_t)+sizeof(ifstream)+sizeof(ofstream)+sizeof(fstream)+4*sizeof(time_t);
    if (currentMemory > peakMemory)
    {   
        peakMemory = currentMemory;
#ifdef VERBOSE
        cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
    }

    // 2 * "bucketSize" = the number of reads loaded at a time
    uint64_t bucketSize = uint64_t(genomeLength * 10 / MAX_READ_LENGTH);

    //======== START CORRECTING =================
    readLength = (int64_t)(totalReadLength / numberOfReads);
    computeTc(readLength, numberOfReads, genomeLength, weight, 0.005, Tc);
    HashTable H((uint64_t)(genomeLength*2));          // starting hash size
    uint64_t size=genomeLength*2;

    currentMemory+=size*sizeof(uint64_t);
    if (currentMemory > peakMemory)
    {   
        peakMemory = currentMemory;
#ifdef VERBOSE
        cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
    }

    cout << "\n=============== QUESS V 1.0.1 ==============\n";
    
    for (int64_t seedNumber = 0; seedNumber < numberOfSeeds; ++seedNumber)
    {

        cout << "\n=============== SEED " << seedNumber <<  " ==============\n";
        time(&iteration_time_start);

        Seed currentSeed = Seed(seeds[seedNumber]);

        inputTempFile.open(inputTempFileName);      // in: uncorrected reads
        if (!inputTempFile.is_open()) {   cerr << "Cannot open input temp file: " << inputTempFileName << endl; exit(1); }
        outputTempFile.open(outputTempFileName, ios::out);  // out: corrected reads
        if (!outputTempFile.is_open()) {   cerr << "Cannot open output temp file: " << outputTempFileName << endl; exit(1); }
        Te = max(2,(seedNumber<=3) ? Tc/4 : Tc/2); 
        Tdiff = (seedNumber <= 3) ? 4 : 2; 
        
        insertSMers(seedNumber, currentSeed, H, inputTempFile, bucketSize, peakMemory,currentMemory);
        size=H.getSize(); 
        rehashFrequentSMers(H, Tc,peakMemory,currentMemory);
        insertSGaps(currentSeed, H, inputTempFile, Te, bucketSize, (int)seedNumber, peakMemory,currentMemory);
        removeAmbiguousSMers(H, Tc, Te, (int)seedNumber, peakMemory,currentMemory);

        correct(seedNumber, currentSeed, H, inputTempFile, outputTempFile, Tdiff, diff16Bits, bucketSize,peakMemory,currentMemory);

        cout << "\n==== PARAMETERS ====\n";
        cout << "genomeLength = " << genomeLength << endl;
        cout << "number of reads = " << numberOfReads << endl;
        cout << "read length (weighted avg.) = " << readLength << endl;
        cout << "seed weight = " << weight << endl;
        cout << "Peak Memory Usage = " << peakMemory/1048576 << " MB"<<endl;       
        cout << "Current Memory Usage = " << currentMemory/1048576 << " MB"<<endl;     
        cout << "Tc    = " << Tc << endl;
        cout << "Te    = " << Te << endl;
        cout << "Tdiff = " << Tdiff << endl;
        cout << "====================\n" << endl;
        inputTempFile.close();
        outputTempFile.close();
        H.clear(peakMemory,currentMemory);
        swapFiles(inputTempFileName, outputTempFileName, seedNumber, numberOfSeeds);
        
        time(&iteration_time_end);
        cout << "\n============ DONE SEED " << seedNumber << " (" << difftime(iteration_time_end, iteration_time_start) << "s) ===========\n" << endl;
    }

    // ======== create the "outputFile" ===========
    // headers and scores from "datasetName" with corrected reads from "outputTempFile"

    createOutputFile(inputFileName, outputTempFileName, outputTempFile, outputFileName, outputFile);
    time(&total_time_end);
    cout << "\n=================== END (" << difftime(total_time_end, total_time_start) << "s) ======================" << endl;
    cout << "==================================================" << endl;
    return 0;
}


/**
  * Name:               createWorkingFiles(char* inputTempFileName, char* outputTempFileName, char* datasetName, char* outputFileName,  std::ofstream& outputFile, int64_t& numberOfReads, int64_t& totalReadLength,uint64_t &peakMemory, uint64_t &currentMemory)
  *
  * Description :       Creates the working files for the runtime of the program
  *
  * Input :
  *       Parameters:
  *           char*     inputTempFileName   filename of copy of original reads              
  *           char*     outputTempFileName  filename of correct reads of input file
  *           char*     datasetName         original file
  *           char*     outputFileName      output file
  *           std::ofstream& outputFile     pointer to output file
  *           int64_t&  numberOfReads       number of reads in original file
  *           int64_t&  totalReadLength     total number of base pairs of original file
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program       
  *
  * Output/Expected Changes :
  *       Parameters:
  *           std::ofstream& outputFile     Output file will be pointed to do write in later
  *           uint64_t  &peakMemory         peakMemory is recorded and estimated for testing
  *           uint64_t  &currentMemory      currentMemory is recorded and estimated for testing
  *       Memory:
  *           None
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Use string functions to create files
  *                     [2]  Copy the original dataset into the copy of the dataset with only relevant information
  *                             Discard Quality and comments and only keep the identity of the reads
  *
  * Notes :             
  *                     original.ext: e.g. "ext" = "fastx"                 - datasetName
  *                     copy READS ONLY in original_temp_copy.ext          - inputTempFile
  *                     correct READS ONLY in original_temp_corrected.ext  - outputTempFile
  *                     corrected file: original_corrected.ext             - outputFile
  *                     fasta files alternate between comment and identitiy
  *                     fastq files alternate between comment, identity, comment, and quality.
  *                     fasta files are identified by '@', fastq are identified by '>'
  *
  */

void createWorkingFiles(char* inputTempFileName, char* outputTempFileName, char* datasetName, char* outputFileName,  std::ofstream& outputFile, int64_t& numberOfReads, int64_t& totalReadLength,uint64_t &peakMemory, uint64_t &currentMemory)
{
    char *inputTempFileNameExtension = new char[10000];
    currentMemory+=10000*sizeof(char);
    if (currentMemory > peakMemory)
    {   
        peakMemory = currentMemory;
#ifdef VERBOSE
        cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
    }
    char* lastDot = strrchr(datasetName,'.');
    if (lastDot)
        {   memcpy(inputTempFileName, datasetName, lastDot - datasetName);
            memcpy(outputTempFileName, datasetName, lastDot - datasetName);
            memcpy(outputFileName, datasetName, lastDot - datasetName);
            memcpy(inputTempFileNameExtension, lastDot, strlen(datasetName) - (lastDot - datasetName));
        }
        else
            {   strcpy(inputTempFileName, datasetName);
                strcpy(outputTempFileName, datasetName);
                strcpy(outputFileName, datasetName);
                strcpy(inputTempFileNameExtension, "");
            }
            strcat(inputTempFileName,"_temp_copy");
            strcat(inputTempFileName, inputTempFileNameExtension);

            strcat(outputTempFileName,"_temp_corrected");
            strcat(outputTempFileName, inputTempFileNameExtension);

            strcat(outputFileName,"_QUESS_corrected");
            strcat(outputFileName, inputTempFileNameExtension);

            cout << "original dataset: " << datasetName << endl;
            cout << "temp uncorrected file: " << inputTempFileName << endl;
            cout << "temp corrected file:   " << outputTempFileName << endl;
            cout << "corrected file:   " << outputFileName << endl;

    // datasetName = original dataset (FASTX)
    // inputTempFile = only the reads from datasetName
    // outputTempFile = only reads but corrected
    // outputFile = corrected dataset (FASTX)

    // ======= copy ony the reads from "datasetFile" to "inputTempFile" =========

            std::ifstream datasetFile;
            datasetFile.open(datasetName);
            if (!datasetFile.is_open()) {   cerr << "Cannot open input dataset file: " << datasetName << endl; exit(1); }

            std::ofstream copyReads;
            copyReads.open(inputTempFileName);
            if (!copyReads.is_open()) {   cerr << "Cannot open input temp file: " << inputTempFileName << endl; exit(1); }

            char* tempRead = new char[MAX_READ_LENGTH];
            char fastx;
            currentMemory+=MAX_READ_LENGTH*sizeof(char);
            if (currentMemory > peakMemory)
            {   
                peakMemory = currentMemory;
#ifdef VERBOSE
                cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
            }
    while (datasetFile.getline(tempRead, MAX_READ_LENGTH))      // read header
    {
        fastx = tempRead[0];
        datasetFile.getline(tempRead, MAX_READ_LENGTH); // read read
        ++numberOfReads;
        totalReadLength += strlen(tempRead);
        copyReads << tempRead << '\n';
        if (fastx == '@')     // FASTQ
        {
            datasetFile.getline(tempRead, MAX_READ_LENGTH); // skip read score header (FASTQ)
            datasetFile.getline(tempRead, MAX_READ_LENGTH); // skip read scores (FASTQ)
        }
        else
            if (fastx != '>')
            {
                cerr << "input data is not a correct FASTA or FASTQ file" << endl;
                exit(1);
            }
    }
    datasetFile.close();
    copyReads.close();
    cout << "numberOfReads = " << numberOfReads << endl;
    cout << "totalReadLength = " << totalReadLength << endl;
}

/**
  * Name:               computeTc(int64_t readLength, int64_t numberOfReads, int64_t genomeLength, int64_t weight, long double error, int &Tc)  
  *
  * Description :       Computes a threshold to ascertain the correctness of a specific sMer given parameters
  *
  * Input :
  *       Parameters:
  *           int64_t   readLength          The read length of the input files    
  *           int64_t   numberOfReads       The number of reads in the fastx file
  *           int64_t   genomeLength        The estimated genome length as inputed
  *           int64_t   weight              The weight of the seeds
  *           long double error             The estimated error per base at each position
  *           int       &Tc                 The count threshold to ascertain whether a specific sMer should be kept
  *
  * Output/Expected Changes :
  *       Parameters:
  *           int       &Tc                 Tc will be computed
  *       Memory:
  *           None
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Compute qc and qe, the average number of correct/incorrect bases given an error within a read
  *                     [2]  Compute lambda_c and lambda_e, which total number of correct/incorrect bases in the entire file
  *                     [3]  While the probability of the average case of number of errors is greater than the average case of
  *                          correct bases, increase Tc until it is greater than that.
  *
  * Notes :             
  *                     Tc is modelled after the Poisson distribution
  *
  */

void computeTc(int64_t readLength, int64_t numberOfReads, int64_t genomeLength, int64_t seedWeight, long double error, int &Tc)
{
    // Tc = 2 + the smallest t such that Poisson(lambda_c, t) > Poisson(lambda_e, t)
    //qc is the average number of correct bases within a read given error e 
    //qe is the average number of incorrect bases within a read given error e 
    //l_c is expected to be about 3*1/e larger than l_e
    //P(l_c,t) is an expression of the average case (l_c) occuring t times within the interval (number of reads)
    //P(l_e,t) is an expression of the average case (l_e) occuring t times within the interval (number of reads)
    long double lambda_c = ((readLength - seedWeight) * pow((1 - error), seedWeight) * numberOfReads) / (double) genomeLength;
    long double lambda_e = ((readLength - seedWeight) * pow((1 - error), seedWeight - 1) * error * numberOfReads) / (double) (3 * genomeLength);
 
    Tc = 1;
    int maxIter = 0;
    while ((exp(-lambda_e)*pow(lambda_e,Tc) > exp(-lambda_c)*pow(lambda_c,Tc))  && (maxIter++ < 2000000))
        ++Tc;
    Tc = min(max(Tc, 4), 254);  // 4 <= Tc <= 254
	Tc+=2;
	//Tc/=3;
}
/**
  * Name:               insertSMers(int64_t seedNumber, Seed& currentSeed, HashTable& H, std::ifstream& inputTempFile, int64_t bucketSize, uint64_t &peakMemory, uint64_t &currentMemory)
  *
  * Description :       Overhead function to insert sMers of the read
  *
  * Input :
  *       Parameters:
  *           int64_t   seedNumber          The current iteration
  *           Seed&     currentSeed         Current identity of the spaced seed
  *           HashTable& H                  The hashTable
  *           std::ifstream& inputTempFile  The pointer to the copy of the input file
  *           int64_t   bucketSize          The number of reads being processed in parallel using openMP
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program       

  *
  * Output/Expected Changes :
  *       Parameters:
  *           Element* sMerTable            updated hashtable with sMers inserted
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program       
  *       Memory:
  *           None
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Creates 2-D arrays that will will contain bucket*read_length chars.
  *                     [2]  Insert each read in parallel using openMP
  *                     [3]  If load exceeds 0.85, resets the hashTable
  *                     [4]  Deallocate memory of the 2-D arrays
  *
  * Notes :             
  *
  */

void insertSMers(int64_t seedNumber, Seed& currentSeed, HashTable& H, std::ifstream& inputTempFile, int64_t bucketSize, uint64_t &peakMemory, uint64_t &currentMemory)
{
    time_t t_start, t_end;
    cout << "\n\n============ INSERT S-MERS ============\n";
    time(&t_start);
    
    // recreate hash table H with max size already used
    if (seedNumber != 0)
        H.recreateOfMaxSize(peakMemory,currentMemory);

    int getBucketNumber = 0;
    int processBucketNumber = 0;
    int64_t currBucketSize = 0;
    char** currBucket = new char* [bucketSize];
    for (int64_t i = 0; i < bucketSize; ++i)
        currBucket[i] = new char [MAX_READ_LENGTH];
    int64_t nextBucketSize = 0;
    char** nextBucket = new char* [bucketSize];
    for (int64_t i = 0; i < bucketSize; ++i)
        nextBucket[i] = new char [MAX_READ_LENGTH];
    
    currentMemory+=2*bucketSize*MAX_READ_LENGTH*sizeof(char);
    if (currentMemory > peakMemory)
    {   
        peakMemory = currentMemory;
#ifdef VERBOSE
        cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
    }

    // insert all sMers of all reads
    bool hashTableNotFull = false;          // whether hashTable is 85 % full or not
    bool endOfFile = false;
    bool done = false;
    while (!hashTableNotFull)          // work is restarted with double size as long as hash table becomes full (hashTableNotFull = false)
    {
        currentMemory+=bucketSize*(2*sizeof(uint64_t)+MAX_READ_LENGTH*sizeof(char)+sizeof(uint8_t));
        if (currentMemory > peakMemory)
        {   
            peakMemory = currentMemory;
#ifdef VERBOSE
            cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
        }
        hashTableNotFull = true;

        inputTempFile.clear();              // file from beginning
        inputTempFile.seekg(0, ios::beg);
        
        // load first bucket
        getBucketNumber++;
        currBucketSize = 0;
        while ((currBucketSize < bucketSize) && (!endOfFile))
            if (inputTempFile.getline(currBucket[currBucketSize], MAX_READ_LENGTH))
                ++currBucketSize;
            else
                endOfFile = true;

        // start processing buckets
            done = false;
            while (!done && hashTableNotFull)
            {
                if (endOfFile)
                    done = true;
#pragma omp parallel shared(hashTableNotFull)
                {
                    Read currentRead;
                // load next bucket (one thread; others need not wait; they process current bucket)
#pragma omp single nowait
                    {
                        nextBucketSize = 0;
                        if (!endOfFile)
                        {
                            while ((nextBucketSize < bucketSize) && (!endOfFile))
                                if (inputTempFile.getline(nextBucket[nextBucketSize], MAX_READ_LENGTH))
                                    ++nextBucketSize;
                                else
                                    endOfFile = true;
                            }
                            getBucketNumber++;
                } // ### end omp single no wait


#pragma omp for schedule(dynamic) 
                for (int64_t i = 0 ; i < currBucketSize; ++i)
                {
                    currentRead = Read(currBucket[i]);
                    if (hashTableNotFull)
                    {
                        if (i % 50000000 == 0 && i!=0)
                            cout << "Read " << i << endl;
                        hashTableNotFull = currentRead.insertSMersOfRead(H, currentSeed);  // hashTableNotFull becomes false if 80% full
                    }
                    currentRead.clear();
                    
                } // ### end omp for schedule (dynamic)
#pragma omp single nowait
                {
                  processBucketNumber++;
                }
            } // ### end omp parallel
            
            if (hashTableNotFull && (nextBucketSize > 0))  // hash table not full; continue processing buckets by moving next bucket into current
            {
                currBucketSize = nextBucketSize;
                for (int64_t j = 0; j < currBucketSize; ++j)
                    strcpy(currBucket[j], nextBucket[j]);
            }
        }

        currentMemory-=bucketSize*(2*sizeof(uint64_t)+MAX_READ_LENGTH*sizeof(char)+sizeof(uint8_t));
        if (!hashTableNotFull)     // hash table full; double size and restart processing reads from beginning of file
        {
         getBucketNumber=0;
         processBucketNumber=0;
         H.clear(peakMemory,currentMemory);
         H.recreateOfDoubleSize(peakMemory,currentMemory);
     }
    }
    for (int64_t i = 0; i < bucketSize; ++i)
    {
        delete [] currBucket[i];
        delete [] nextBucket[i];
    }
    delete [] currBucket;
    delete [] nextBucket;
    currentMemory-=2*bucketSize*MAX_READ_LENGTH*sizeof(char);
    time(&t_end);
    cout << "============ DONE inserting sMers (" << difftime(t_end,t_start) << "s) ===========\n" << endl;
}

/**
  * Name:               rehashFrequentSMers(HashTable& H, int Tc, uint64_t &peakMemory,uint64_t &currentMemory)
  *
  * Description :       Overhead function to rehash sMers of hashTable
  *
  * Input :
  *       Parameters:
  *           HashTable& H                  The hashTable
  *           int       &Tc                 The count threshold to ascertain whether a specific sMer should be kept
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program       

  *
  * Output/Expected Changes :
  *       Parameters:
  *           Element* sMerTable            remade hashTable with rehashed values
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program       
  *       Memory:
  *           None
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Invoke the HashTable function to rehash into a smaller hashTable
  *
  * Notes :             
  *
  */

void rehashFrequentSMers(HashTable& H, int Tc, uint64_t &peakMemory,uint64_t &currentMemory)
{
    cout << "\n============ REHASH FREQUENT S-MERS ============\n";
    time_t t_start, t_end;
    time(&t_start);
    H.rehashFrequentSMers(Tc,peakMemory,currentMemory);
    time(&t_end);
    cout << "============ DONE rehashing frequent sMers (" << difftime(t_end,t_start) << "s) ===========\n" << endl;
}

/**
  * Name:               insertSGaps(Seed& currentSeed, HashTable& H, std::ifstream& inputTempFile, int Te, int64_t bucketSize, int Ta, uint64_t &peakMemory,uint64_t &currentMemory)
  *
  * Description :       Overhead function to insert sGaps of the read
  *
  * Input :
  *       Parameters:
  *           Seed&     currentSeed         Current identity of the spaced seed
  *           HashTable& H                  The hashTable
  *           std::ifstream& inputTempFile  The pointer to the copy of the input file
  *           int       Te                  The count threshold for how acceptable deviants from the strongest sGap are
  *           int64_t   bucketSize          The number of reads being processed in parallel using openMP
  *           int       seedNumber          current iteration of program
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program       

  *
  * Output/Expected Changes :
  *       Parameters:
  *           Element** sGapTable           sGap table created with sGaps inserted
  *           uint64_t  &peakMemory         peakMemory is recorded and estimated for testing
  *           uint64_t  &currentMemory      currentMemory is recorded and estimated for testing
  *       Memory:
  *           None
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Creates sGapTable of same size as sMerTable
  *                     [2]  Creates 2-D arrays that will will contain bucket*read_length chars.
  *                     [3]  Insert each read in parallel using openMP
  *                     [4]  If load exceeds 0.85, resets the hashTable
  *                     [5]  Deallocate memory of the 2-D arrays
  *
  * Notes :             
  *
  */

void insertSGaps(Seed& currentSeed, HashTable& H, std::ifstream& inputTempFile, int Te, int64_t bucketSize, int seedNumber, uint64_t &peakMemory, uint64_t &currentMemory)
{
    cout << "\n============ INSERT S-GAPS ============\n";
    time_t t_start, t_end;
    time(&t_start);
    
    H.createSGapTable(peakMemory,currentMemory);
    currentMemory+=0.84*(8-seedNumber)*H.getSize()*sizeof(uint64_t);
    currentMemory+=H.getSize()*sizeof(uint64_t);
        if (currentMemory > peakMemory)
        {   
            peakMemory = currentMemory;
#ifdef VERBOSE
            cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
        }
    uint64_t numberOfLocks = H.getSize();   
    omp_lock_t* lockArray = new omp_lock_t [numberOfLocks];
    for (uint64_t i = 0; i < numberOfLocks; ++i)
        omp_init_lock(&(lockArray[i]));
    int64_t currBucketSize = 0;       
    char** currBucket = new char* [bucketSize];
    for (int64_t i = 0; i < bucketSize; ++i)
        currBucket[i] = new char [MAX_READ_LENGTH];
    int64_t nextBucketSize = 0;       
    char** nextBucket = new char* [bucketSize];
    for (int64_t i = 0; i < bucketSize; ++i)
        nextBucket[i] = new char [MAX_READ_LENGTH];

    currentMemory+=2*bucketSize*MAX_READ_LENGTH*sizeof(char)+numberOfLocks*sizeof(omp_lock_t*);
    if (currentMemory > peakMemory)
    {   
        peakMemory = currentMemory;
#ifdef VERBOSE
        cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
    }

    bool endOfFile = false;
    bool done = false;
    inputTempFile.clear();              // file from beginning
    inputTempFile.seekg(0, ios::beg);
    currBucketSize = 0;
    // insert all sGaps of all reads
    
    while ((currBucketSize < bucketSize) && (!endOfFile))
        if (inputTempFile.getline(currBucket[currBucketSize], MAX_READ_LENGTH))
            ++currBucketSize;
        else
            endOfFile = true;

    // start processing buckets
        done = false;
        while (!done)
        {


            currentMemory+=bucketSize*(2*sizeof(uint64_t)+MAX_READ_LENGTH*sizeof(char)+sizeof(uint8_t));
            if (currentMemory > peakMemory)
            {   
                peakMemory = currentMemory;
#ifdef VERBOSE
                cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
            }
            if (endOfFile)
                done = true;
#pragma omp parallel
            {
                Read currentRead;
            // load next bucket (one thread; others need not wait; they process current bucket)
#pragma omp single nowait
                {
                    nextBucketSize = 0;
                    if (!endOfFile)
                    {
                        while ((nextBucketSize < bucketSize) && (!endOfFile))
                            if (inputTempFile.getline(nextBucket[nextBucketSize], MAX_READ_LENGTH))
                                ++nextBucketSize;
                            else
                                endOfFile = true;
                        }
            } // ### end omp single no wait
            
            // process current bucket
#pragma omp for schedule(dynamic)
            for (int64_t i = 0 ; i < currBucketSize; ++i)
            {
                currentRead = Read(currBucket[i]);
                if (i % 50000000 == 0 && i!=0)
                   cout << "read " << i << endl;
               currentRead.insertSGapsOfRead(H, currentSeed, Te, seedNumber, lockArray);
               currentRead.clear();
            } // ### end omp for schedule (dynamic)
        } // ### end omp parallel

        // copy next bucket into current bucket
        if (nextBucketSize > 0)  // hash table not full; continue processing buckets by moving next bucket into current
        {
            currBucketSize = nextBucketSize;
            for (int64_t j = 0; j < currBucketSize; ++j)
                strcpy(currBucket[j], nextBucket[j]);
        }
        currentMemory-=bucketSize*(2*sizeof(uint64_t)+MAX_READ_LENGTH*sizeof(char)+sizeof(uint8_t));
    }
#pragma omp single nowait    
    for (uint64_t i = 0; i < numberOfLocks; ++i)
        omp_destroy_lock(&(lockArray[i]));
    for (int64_t i = 0; i < bucketSize; ++i)
    {
        delete [] currBucket[i];
        delete [] nextBucket[i];
    }

    currentMemory-=(2*bucketSize*MAX_READ_LENGTH*sizeof(char)+numberOfLocks*sizeof(omp_lock_t*));
    delete [] currBucket;
    delete [] nextBucket;
    time(&t_end);
    cout << "============ DONE inserting sGaps (" << difftime(t_end,t_start) << "s) ===========\n" << endl;
}

/**
  * Name:               removeAmbiguousSMers(HashTable& H, int Tc, int Te, int Ta, uint64_t &peakMemory,uint64_t &currentMemory)
  *
  * Description :       Overhead function to remove ambigious sMers
  *
  * Input :
  *       Parameters:
  *           HashTable& H                  The hashTable
  *           uint64_t  Tc                  The count threshold in which an sMer/sGap is considered frequent
  *           uint64_t  Te                  The count threshold for how acceptable deviants from the strongest sGap are
  *           uint64_t  Ta                  The threshold of allowable deviant sGaps
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program       
  *
  * Output/Expected Changes :
  *       Parameters:
  *           Element*  sMerTable           updated hashtable with ambigious values removed
  *           Element** sGapTable           updated hashtable with only most frequent sGaps
  *           uint64_t  &peakMemory         peakMemory is recorded and estimated for testing
  *           uint64_t  &currentMemory      currentMemory is recorded and estimated for testing
  *       Memory:
  *           None
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Utilizes Hashtable function to remove ambigious sMers and only keep the most frequent sGaps
  *
  * Notes :             
  *
  */

void removeAmbiguousSMers(HashTable& H, int Tc, int Te, int seedNumber, uint64_t &peakMemory,uint64_t &currentMemory)
{
    cout << "\n============ REMOVE AMBIGUOUS S-MERS ============\n\n";
    time_t t_start, t_end;
    time(&t_start);
    // remove ambiguous sMers to speed up correction
    H.removeAmbigSMers(Tc, Te, seedNumber, peakMemory,currentMemory);
    time(&t_end);
    cout << "============ DONE removing ambiguous sMers (" << difftime(t_end,t_start) << "s) ===========\n" << endl;
}

/**
  * Name:               correct(int64_t seedNumber, Seed& currentSeed, HashTable& H, std::ifstream& inputTempFile, std::fstream& outputTempFile,  int Tdiff, uint64_t* diff16Bits, int64_t bucketSize,uint64_t &peakMemory,uint64_t &currentMemory)
  *
  * Description :       Overhead function to correct the reads of the fastx file
  *
  * Input :
  *       Parameters:
  *           int64_t   seedNumber          Current iteration
  *           Seed&     currentSeed         Current identity of the spaced seed
  *           HashTable& H                  The hashTable
  *           std::ifstream& inputTempFile  The pointer to the copy of the input file
  *           std::fstream& outputTempFile  The pointer to the temp output file
  *           uint64_t  Tdiff               The allowance of how many bits can be different to have the uncorrected sGap be corrected
  *           uint64_t* diff16Bits          Precomputed array to determine the difference between the sGaps
  *           int64_t   bucketSize          The number of reads being processed in parallel using openMP
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program       

  *
  * Output/Expected Changes :
  *       Parameters:
  *           Element** sGapTable           sGap table created with sGaps inserted
  *           std::fstream& outputTempFile  Correct variants will be written in the temp output file
  *           uint64_t  &peakMemory         peakMemory is recorded and estimated for testing
  *           uint64_t  &currentMemory      currentMemory is recorded and estimated for testing
  *       Memory:
  *           None
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Creates 2-D arrays that will will contain bucket*read_length chars.
  *                     [2]  Insert each read in parallel using openMP
  *                     [3]  Outputs the correct variant of the sGap into the read sGap and prints into the temp file
  *                     [4]  Deallocate memory of the 2-D arrays
  *
  * Notes :             
  *
  */

void correct(int64_t seedNumber, Seed& currentSeed, HashTable& H, std::ifstream& inputTempFile, std::fstream& outputTempFile,  int Tdiff, uint64_t* diff16Bits, int64_t bucketSize,uint64_t &peakMemory,uint64_t &currentMemory)
{
    cout << "\n============ CORRECT ============\n";
    time_t t_start, t_end;
    time(&t_start);

    int64_t currBucketSize = 0;
    char** currBucket = new char* [bucketSize];
    for (int64_t i = 0; i < bucketSize; ++i)
        currBucket[i] = new char [MAX_READ_LENGTH];
    int64_t nextBucketSize = 0;
    char** nextBucket = new char* [bucketSize];
    for (int64_t i = 0; i < bucketSize; ++i)
        nextBucket[i] = new char [MAX_READ_LENGTH];
    char** outputBucket = new char* [bucketSize];
    for (int64_t i = 0; i < bucketSize; ++i)
        outputBucket[i] = new char [MAX_READ_LENGTH];

    currentMemory+=3*bucketSize*MAX_READ_LENGTH*sizeof(char);
    if (currentMemory > peakMemory)
    {   
        peakMemory = currentMemory;
#ifdef VERBOSE
        cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
    }
    // correct all reads
    bool endOfFile = false;
    bool done = false;
    inputTempFile.clear();              // file from beginning
    inputTempFile.seekg(0, ios::beg);
    
    // load first bucket
    currBucketSize = 0;
    uint64_t corr=0;
    while ((currBucketSize < bucketSize) && (!endOfFile))
        if (inputTempFile.getline(currBucket[currBucketSize], MAX_READ_LENGTH))
            ++currBucketSize;
        else
            endOfFile = true;

    // start processing buckets
        done = false;
        while (!done)
        {
            currentMemory+=bucketSize*(2*sizeof(uint64_t)+MAX_READ_LENGTH*sizeof(char)+sizeof(uint8_t));
            if (currentMemory > peakMemory)
            {   
                peakMemory = currentMemory;
#ifdef VERBOSE
            cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
        }
        if (endOfFile)
            done = true;
#pragma omp parallel
        {
            Read currentRead;
            // load next bucket (one thread; others need not wait; they process current bucket)
#pragma omp single nowait
            {
                nextBucketSize = 0;
                if (!endOfFile)
                {
                    while ((nextBucketSize < bucketSize) && (!endOfFile))
                        if (inputTempFile.getline(nextBucket[nextBucketSize], MAX_READ_LENGTH))
                            ++nextBucketSize;
                        else
                            endOfFile = true;
                    }
            } // ### end omp single no wait
            // process current bucket
#pragma omp for schedule(dynamic)
            for (int64_t i = 0 ; i < currBucketSize; ++i)
            {
                currentRead = Read(currBucket[i]);
                int64_t errPositions = currentRead.correctBinaryReadFast(H, currentSeed, Tdiff, diff16Bits,corr);
                if (errPositions > -1)  // errPositions == -1 means read was not corrected
                    if  ((seedNumber >= 2) || errPositions == 0)   // for the first two seeds implement corrections only when "perfectly" corrected
                        currentRead.correctCharRead();             // i.e., no positions with score != 0
                    currentRead.outputRead(outputBucket[i]);
                    currentRead.clear();
            } // ### end omp for schedule (dynamic)
            // output corrected bucket to file
#pragma omp single nowait
            {
                for (int64_t i = 0 ; i < currBucketSize; ++i)
                    outputTempFile << outputBucket[i] << '\n';
            }
        } // ### end omp parallel
        
        // copy next bucket into current bucket
        if (nextBucketSize > 0)  // hash table not full; continue processing buckets by moving next bucket into current
        {
            currBucketSize = nextBucketSize;
            for (int64_t j = 0; j < currBucketSize; ++j)
                strcpy(currBucket[j], nextBucket[j]);
        }
        currentMemory-=bucketSize*(2*sizeof(uint64_t)+MAX_READ_LENGTH*sizeof(char)+sizeof(uint8_t));
    }
    for (int64_t i = 0; i < bucketSize; ++i)
    {
        delete [] currBucket[i] ;
        delete [] nextBucket[i];
        delete [] outputBucket[i];
    }

    currentMemory-=3*bucketSize*MAX_READ_LENGTH*sizeof(char);
    delete [] currBucket;
    delete [] nextBucket;
    delete [] outputBucket;
    time(&t_end);
    cout << "Number of Corrected Positions this iteration: " << corr << endl;
    cout << "============ DONE correcting (" << difftime(t_end,t_start) << "s) ===========\n" << endl;
}

/**
  * Name:               swapFiles(char* inputTempFileName, char* outputTempFileName, int64_t seedNumber, int64_t numberOfSeeds)
  *
  * Description :       Swaps files so that the next iteration of seeds can start
  *
  * Input :
  *       Parameters:
  *           char*     inputTempFileName   Old input temp file
  *           char*     outputTempFileName  Old output temp file
  *           int64_t   seedNumber          Current iteration of program
  *           int64_t   numberOfSeeds       Total Number of iterations
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           None
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Uses system commands to move the output file into the input file for the next iteration if this 
  *                          is not the final iteration
  *
  * Notes :             
  *
  */

void swapFiles(char* inputTempFileName, char* outputTempFileName, int64_t seedNumber, int64_t numberOfSeeds)
{
    // remove copy of input file
    char * copy_command = new char [10000];
    strcpy(copy_command, "rm ");
    strcat(copy_command, inputTempFileName);
    cout << "delete copy of input " << copy_command << endl;
    system(copy_command);

    // move corrected file in place of input copy to correct again; except at the end
    if (seedNumber < numberOfSeeds - 1)
    {
        strcpy(copy_command, "mv ");
        strcat(copy_command, outputTempFileName);
        strcat(copy_command, " ");
        strcat(copy_command, inputTempFileName);
        cout << "copy_command: " << copy_command << endl;
        system(copy_command);
    }
}

/**
  * Name:               createOutputFile(char* datasetName, char *outputTempFileName, std::fstream &outputTempFile, char *outputFileName, std::ofstream &outputFile)
  *
  * Description :       creates the final output file and copies all of the corrections into this one with all the comments included
  *
  * Input :
  *       Parameters:
  *           char*     datasetName         original file
  *           char*     outputTempFileName  latest output temp file name
  *           std::fstream &outputTempFile  pointer to latest output temp file
  *           char*     outputFileName      final output filename
  *           std::fstream &outputFile      pointer to final output file
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           std::fstream &outputFile      final output file created
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Opens the latest output temp file and original file and final output file at the same
  *                     [2]  At every read, will take the original file comments/qualities and the latest output temp file reads 
  *                          and output them into the final output file
  *
  * Notes :             
  *
  */

void createOutputFile(char* datasetName, char *outputTempFileName, std::fstream &outputTempFile, char *outputFileName, std::ofstream &outputFile)
{
    std::ifstream datasetFile;
    datasetFile.open(datasetName);      // open original dataset
    if (!datasetFile.is_open()) {   cerr << "Cannot open input dataset file: " << datasetName << endl; exit(1); }
    
    outputTempFile.open(outputTempFileName, ios::in);
    if (!outputTempFile.is_open()) {   cerr << "Cannot open output file: " << outputTempFileName << endl; exit(1); }
    
    outputFile.open(outputFileName, ios::out);
    if (!outputFile.is_open()) {   cerr << "Cannot open output file: " << outputFileName << endl; exit(1); }
    
    char* datasetLine = new char[MAX_READ_LENGTH];
    char* correctedReadLine = new char[MAX_READ_LENGTH];
    char fastx;
    while (datasetFile.getline(datasetLine, MAX_READ_LENGTH))         // read header from dataset
    {
        outputFile << datasetLine << '\n';                            // put read header in outputFile
        fastx = datasetLine[0];
        outputTempFile.getline(correctedReadLine, MAX_READ_LENGTH);   // read corrected read from outputTempFile
        outputFile << correctedReadLine << '\n';                      // put corrected read in outputFile
        datasetFile.getline(datasetLine, MAX_READ_LENGTH);            // skip uncorrected read from dataset

        if (fastx == '@')     // FASTQ
        {
            datasetFile.getline(datasetLine, MAX_READ_LENGTH);        // read score header from dataset
            outputFile << datasetLine << '\n';                        // put score header in outputFile
            datasetFile.getline(datasetLine, MAX_READ_LENGTH);        // read score from dataset
            outputFile << datasetLine << '\n';                        // put score in outputFile
        }
    }
    datasetFile.close();
    outputTempFile.close();
    outputFile.close();
    
    // remove outputTempFile
    char * copy_command = new char [10000];
    strcpy(copy_command, "rm ");
    strcat(copy_command, outputTempFileName);
    cout << "delete copy of input " << copy_command << endl;
    system(copy_command);
    delete [] datasetLine;
    delete [] correctedReadLine;
    
}

/**
  * Name:               printIntInBinary(uint8_t n)
  *
  * Description :       prints integer n as a binary
  * Input :
  *       Parameters:
  *           uint64_t  n                   Target integer
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           none
  *       Return:
  *           None
  *
  * Process Synopsis :
  *
  *
  * Notes :             
  *
  */

void printIntInBinary(uint8_t n)
{
    int i = 0;
    for (i=7; i>=0; --i) {
        if ((((uint8_t)1<<i) & n) != 0)
            cout << "1";
        else
            cout << "0";
        if (i == 0)
            cout << " ";
    }

}

