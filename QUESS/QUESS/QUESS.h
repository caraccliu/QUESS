//
//  QUESS.h
//

#ifndef ____QUESS__
#define ____QUESS__

#ifndef TEST
#define TEST 1
#endif

#ifndef DEBUG1
#define DEBUG1 0
#endif

#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <stdint.h>
#include <cstdlib> 
#include <math.h>
#include <string.h>
#include <assert.h>
#include <omp.h>
#include <string.h>

using namespace std;



// =============================
// ======== Seed ===============

void getSeed(char* &seed, uint64_t seedWeight, uint64_t numberOfSeeds, uint64_t seedNumber,uint64_t& peakMemory, uint64_t& currMemory);


// ============================================================
// ====================== HashTable class =====================
// ============== (definitions in hashTable.cpp) ==============

// double hashing used; hash = Elem % size, step = 1 + Elem % (size - 1)
// rehash when 80% full - hard coded


typedef struct
{
    uint64_t count : 8;          // 8 bits for counter
    uint64_t value : 56;        // 56 bits for sMer or sGap
}
Element;

class HashTable  // class for hash table
{
private:
    uint64_t size, numberOfElements;                      // capacity and actual number of elements
    uint64_t maxSize;               // maxSize stores the maximum necesary size to avoid rehashing each iteration
    static const uint64_t EMPTY =   ((uint64_t)1<<56) - 1; // element removed from table: 00000000 11111111 11111111 ... 11111111
    static const uint64_t REMOVED = ((uint64_t)1<<56) - 2; // element removed from table: 00000000 11111111 11111111 ... 11111110
    Element* sMerTable;                 // sMers; .count = count, .value = sMer
    Element** sGapTable;                // array of sGaps for each sMer

public:
    HashTable(uint64_t minRequiredSize);
        // new hash table of prime size > minRequiredSize

    uint64_t getNewSize(uint64_t minSize);
        // find the smallest prime in the table larger than size
    
    uint64_t getSize();
        // return the current size of the hash table

    bool insertSMer(uint64_t sMer);
        // insert sMer with count = 1 or increment count if already in table
        // return true if table not 50% full and false otherwise
        // when false, then maxSize of hashtable has been doubled; restart processing all the reads
    
    bool findPos(Element* table, uint64_t tableSize, uint64_t sMer, uint64_t& place);
        // find the position pos of sMer in table; return true if found and false otherwise

    uint64_t removeSMersCount1();
        // remove sMers with count = 1 to create spQUESS
    
    void removePos(uint64_t pos);
        // remove sMer at position pos, including sGaps if any
    
    void removeSGaps(uint64_t pos);
        // remove the sGaps (for ambiguous sMers)
    
    void rehashFrequentSMers(uint64_t Tc,uint64_t& peakMemory, uint64_t& currentMemory);
        // rehash to keep only frequent smers (count >= Tc)
    
    void createSGapTable(uint64_t& peakMemory, uint64_t& currentMemory);
        // create the sGapTable array
    
    void insertSGap(uint64_t sMer, uint64_t sGap, uint64_t Te, int seedNumber, omp_lock_t* lockArray);
        // insert sGap for sMer; here sMer.count = number of sGaps !!!; sMers with more than one sGap with count >= Te are deemed ambiguous and removed
    
    void removeAmbigSMers(uint64_t Tc, uint64_t Te , int seedNumber, uint64_t& peakMemory, uint64_t& currentMemory);
        // sMer is ambiguous is there are two sGaps in its list with count >= Te or if there is no sGap with count >= Tc
        // remove ambiguous ones and rearrange the sGap arrays to contain only the correct sGap
        // the count of the single sGap contains the score = correctSGap.count / maxErrSGap.count (no more than 255)
    
    int64_t getCorrectSGap(uint64_t sMer, uint64_t sGap, uint64_t& correctSGap, uint64_t Tdiff, uint64_t* diff16Bits);
        // given current sMer and sGap, get the correctSGap; return score > 0 if it exists:
        // (1) sMer is in table
        // (2) sGap is different from correctSGap but the difference is < Tdiff (not too different)
        // note that because of "removeAmbigSMers" the sMer has only the correct sGap (count >= Tc)
        // return 0 if sMer is in table but correctSGap = currentSGap
        // return -1 if sMer not in table or sGap is too different

    static uint64_t getDiff(uint64_t x, uint64_t y, uint64_t* diff16Bits);
        // compute the number of different 2-bit blocks of x and y
        // diff16Bits (precomputed) gives the number of non-zero 2-bit blocks
        // needed only inside HashTable

    static uint64_t computeDiff16Bits(uint16_t x);
        // compute the number of non-zero 2-bit blocks in a uint16_t integer x

    void clear(uint64_t& peakMemory, uint64_t& currentMemory);
        // delete the hash table to prepare it for next iteration (different seed)

    void recreateOfMaxSize(uint64_t& peakMemory, uint64_t& currentMemory);
    // clear and reallocate hash table of size = maxSize (saved from previous work)
    
    void recreateOfDoubleSize(uint64_t& peakMemory, uint64_t& currentMemory);
    // clear and reallocate hash table of size = 2 * maxSize
    
};

// ========================================================
// ====================== Seed class ======================
// =============== (definitions in seed.cpp) ==============

class Seed  // class for multiple spQUESSd seeds
{
private:
    uint64_t weight, length;     // weight = # 1's
    char* charSeed;         // the seeds
    uint64_t sMerMask, sGapMask, sMerMaskRC, sGapMaskRC;
public:
    Seed(char* seed);
    uint64_t getLength();
    uint64_t getWeight();
    uint64_t getSMerMask();
    uint64_t getSGapMask();
    uint64_t getSMerMaskRC();
    uint64_t getSGapMaskRC();
    void printSeedInfo();
};


// ========================================================
// ====================== Read class ======================
// =============== (definitions in read.cpp) ==============

#define MAX_READ_LENGTH 302 

class Read  // class for DNA Sequencing reads
{
private:
    uint64_t length, numberOfNs;            // length = # characters, numberOfNs = # chars not in {A,C,G,T}
    char charRead[MAX_READ_LENGTH];         // read characters
    uint8_t *binRead;                       // read in binary (last incomplete block aligned left)
    
public:
    Read();
    
    Read(char* inputString);
        // create read with sequence "inputString"

    int64_t getLength();
        // return char length
    
    bool insertSMersOfRead(HashTable& H, Seed& seed);
        // insert all sMers of the read into hash table
        // return true if all sMers were successfully inserted (table not 50% full)
        // when false, then maxSize of hashtable has been doubled; restart processing all the reads
    
    void insertSGapsOfRead(HashTable& H, Seed& seed, uint64_t Te, int seedNumber, omp_lock_t* lockArray);
        // insert all sGaps of the read into hash table
    
    int64_t correctBinaryReadFast(HashTable& H, Seed& seed, uint64_t Tdiff, uint64_t* diff16Bits, uint64_t &corr);
        // correct binary read
        // return -1 if read could not be corrected (> 50% N's or seed too long)
        // return the number of erroneous positions suspected

    void correctCharRead();
        // move corrections from binRead to charRead

    void clear();
        // free space for binary read
    
    void outputRead(char* outputString);
        // copy (corrected) read info into the outputString

};


//===============================================
//============ main functions =================


void show_usage();
    //shows usage
bool legal_int(char *);
    // tests for legal int
void createWorkingFiles(char* inputTempFileName, char* outputTempFileName, char* datasetName, char* outputFileName,  std::ofstream& outputFile, int64_t& numberOfReads, int64_t& totalReadLength,uint64_t& peakMemory, uint64_t& currentMemory);
    // create file names
    // copy the reads only from "datasetName" to "inputTempFile"
void computeTc(int64_t readLength, int64_t numberOfReads, int64_t genomeLength, int64_t weight, long double error, int &Tc);
    // compute Tc
void insertSMers(int64_t seedNumber, Seed& currentSeed, HashTable& H, std::ifstream& inputTempFile, int64_t bucketSize,uint64_t& peakMemory, uint64_t& currentMemory);
    // insert all sMers of all reads in "inputFile"
void rehashFrequentSMers(HashTable &H, int Tc,uint64_t& peakMemory, uint64_t& currentMemory);
    // rehash to keep only frequent smers (count >= Tc)
void insertSGaps(Seed& currentSeed, HashTable& H, std::ifstream& inputTempFile, int Te, int64_t bucketSize,int seedNumber,uint64_t& peakMemory, uint64_t& currentMemory);
    // insert all SGaps of all reads in"inputTempFile"
void removeAmbiguousSMers(HashTable& H, int Tc, int Te, int seedNumber, uint64_t& peakMemory, uint64_t& currentMemory);
    // remove ambiguous sMers from H
void correct(int64_t seedNumber, Seed& currentSeed, HashTable& H, std::ifstream& inputTempFile, std::fstream& outputTempFile, int Tdiff, uint64_t* diff16Bits, int64_t bucketSize,uint64_t& peakMemory, uint64_t& currentMemory);
    // correct all reads in "inputTempFile" and put them in "outputTempFile"
void swapFiles(char* inputTempFileName, char* outputTempFileName, int64_t seedNumber, int64_t numberOfSeeds);
    // delete "inputTempFileName" and replQUESS it with "outputTempFileName"; except for the last iteration
void createOutputFile(char* datasetName, char *outputTempFileName, std::fstream &outputTempFile, char *outputFileName, std::ofstream &outputFile);
    // create "outputFile" with headers and scores from "datasetName" and corrected reads from "outputTempFile"

void printIntInBinary(uint8_t n);
#endif /* defined(____QUESS__) */
