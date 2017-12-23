/** 
  * File:     read.cpp 
  * 
  * Author1:  Lucian Ilie (ilie@uwo.ca) 
  * Author2:  Stephen Lu (slu93@uwo.ca) 
  * Date:     Fall 2017  
  * 
  *   This file contains code concerning the extraction
  *   of reads from a fastx file. These reads are converted 
  *   to their binary representation, and stored for further
  *   processing by QUESS.
  *
  */

#include "QUESS.h"


/**
  * Name:             Read()
  *
  * Description :     Default constructor
  *
  * Input :
  *       Parameters:
  *           None
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           None
  *       Return:
  *           Read                      Returns a blank read
  *
  * Process Synopsis :
  *
  * Notes :             
  *
  */

Read::Read()
{
    length = numberOfNs = 0;
}

// nucleotide-wise reverse complements of all 8-bit values, that is, bit-negation: A = 00 --rc--> 11 = T; C = 01 --rc--> 10 = G, and so on
const uint8_t byteRC[256] = {
    255, 191, 127, 63, 239, 175, 111, 47, 223, 159, 95, 31, 207, 143, 79, 15, 251, 187, 123, 59, 235, 171, 107, 43, 219, 155, 91, 27, 203, 139, 75, 11, 247, 183, 119, 55, 231, 167, 103, 39, 215, 151, 87, 23, 199, 135, 71, 7, 243, 179, 115, 51, 227, 163, 99, 35, 211, 147, 83, 19, 195, 131, 67, 3, 254, 190, 126, 62, 238, 174, 110, 46, 222, 158, 94, 30, 206, 142, 78, 14, 250, 186, 122, 58, 234, 170, 106, 42, 218, 154, 90, 26, 202, 138, 74, 10, 246, 182, 118, 54, 230, 166, 102, 38, 214, 150, 86, 22, 198, 134, 70, 6, 242, 178, 114, 50, 226, 162, 98, 34, 210, 146, 82, 18, 194, 130, 66, 2, 253, 189, 125, 61, 237, 173, 109, 45, 221, 157, 93, 29, 205, 141, 77, 13, 249, 185, 121, 57, 233, 169, 105, 41, 217, 153, 89, 25, 201, 137, 73, 9, 245, 181, 117, 53, 229, 165, 101, 37, 213, 149, 85, 21, 197, 133, 69, 5, 241, 177, 113, 49, 225, 161, 97, 33, 209, 145, 81, 17, 193, 129, 65, 1, 252, 188, 124, 60, 236, 172, 108, 44, 220, 156, 92, 28, 204, 140, 76, 12, 248, 184, 120, 56, 232, 168, 104, 40, 216, 152, 88, 24, 200, 136, 72, 8, 244, 180, 116, 52, 228, 164, 100, 36, 212, 148, 84, 20, 196, 132, 68, 4, 240, 176, 112, 48, 224, 160, 96, 32, 208, 144, 80, 16, 192, 128, 64, 0
};

/**
  * Name:               computeRevCompl(uint64_t x)
  *
  * Description :       Computes the reverse complement of integer x
  *
  * Input :
  *       Parameters:
  *           uint64_t  x               Integer to be given the reverse complement of
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           None
  *       Return:                       
  *           int64_t                   Returns the reverse complement
  *
  * Process Synopsis :
  *                     [1]  Iterates through the integer using masks 
  *
  * Notes :             Ex. Reverse complement of 000101011101 is 010001010111.
  *
  */

uint64_t computeRevCompl(uint64_t x)
{
    // computes the reverse complement of integer x
    uint64_t y = 0, mask = (1 << 8) - 1;
    for (uint64_t i = 0; i < 7; ++i) {
        y |= byteRC[(uint8_t)(x & mask)];
        y <<= 8;
        x >>= 8;
    }
    y |= byteRC[(uint8_t)(x & mask)];
    return(y);
}

/**
  * Name:               Read(char* inputString)
  *
  * Description :       Constructor of a read given an input string
  *
  * Input :
  *       Parameters:
  *           char*     inputString     A char array containing the characters of the read 
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           None
  *       Return:                       
  *           Read                      Returns a read that will have the char and binary specifications
  *                                     as defined by inputString
  *
  * Process Synopsis :
  *                     [1]  Creates a new uint64_t array containing the binary representation of the read
  *                     [2]  Iterates through the nucleotides of the read, and assigns the bases a 2-bit identity
  *                     [3]  A/a=00, T/t=11, C/c=01, G/g=10, N is randomized
  *
  * Notes :             Note that A/T and G/C are complementary, which helps when masking them and finding their 
  *                     sMers and sGaps.
  *
  */

Read::Read(char* inputString)
{
    // create charRead
    strcpy(charRead, inputString);

    // create binary read and char read
    // last (possibly incomplete) byte is aligned left, e.g.: ACGT ACGT ACGT ACG_
    length = strlen(inputString);
    
    uint64_t binLength = length * 2;    // length in bits
    uint64_t binBytes = binLength / 8 + ((binLength % 8 == 0) ? 0 : 1);   // length in bytes
    binRead = new uint8_t [binBytes];
    for (uint64_t i = 0; i < binBytes; ++i)
        binRead[i] = 0;
    numberOfNs = 0;
    for (uint64_t currCharPos = 0, currByte = 0; currCharPos < length; ++currCharPos)
    {   switch (inputString[currCharPos])
        {   case 'A':
                binRead[currByte] <<= 2;
                break;
            case 'a':
                binRead[currByte] <<= 2;
                break;
            case 'C':
                binRead[currByte] <<= 2;
                binRead[currByte] |= 1;
                break;
            case 'c':
                binRead[currByte] <<= 2;
                binRead[currByte] |= 1;
                break;
            case 'G':
                binRead[currByte] <<= 2;
                binRead[currByte] |= 2;
                break;
            case 'g':
                binRead[currByte] <<= 2;
                binRead[currByte] |= 2;
                break;
            case 'T':
                binRead[currByte] <<= 2;
                binRead[currByte] |= 3;
                break;
            case 't':
                binRead[currByte] <<= 2;
                binRead[currByte] |= 3;
                break;
            default:
                binRead[currByte] <<= 2;
                binRead[currByte] |= rand() % 4; // replace N's with random bases
                ++numberOfNs;
                break;
        }
        if (currCharPos % 4 == 3)   // move to next byte
            ++currByte;             // currCharPos % 4 == 0 next iteration
    }
    // shift left last byte of binRead if incomplete; also, symmetrically shift right the first byte of binReadRC
    uint64_t lastByteEmpty = ((binLength % 8 == 0) ? 0 : (8 - binLength % 8));
    binRead[binBytes-1] <<=  lastByteEmpty;
}

/**
  * Name:               getLength()
  *
  * Description :       Returns the length of the read
  *
  * Input :
  *       Parameters:
  *           None
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           None
  *       Return:                       
  *           uint64_t                  Returns the length of the read
  *
  * Process Synopsis :
  *                     
  *
  * Notes :             
  *
  */

int64_t Read::getLength()
{
    return length;
}

/**
  * Name:               insertSMersOfRead(HashTable& H, Seed& seed)
  *
  * Description :       Inserts all of the sMers of this read
  *
  * Input :
  *       Parameters:
  *           HashTable& H              The hashTable to be inserted into
  *           Seed&      seed           The reference seed to draw the correct mask for this iteration
  *
  * Output/Expected Changes :
  *       Parameters:
  *           HashTable& H              Hashtable will have these inserted sMers after function completion
  *       Memory:
  *           None
  *       Return:                       
  *           bool                      Returns true if all sMers of the read are inserted
  *
  * Process Synopsis :
  *                     [1]  Fetches the sMer masks
  *                     [2]  Targets a window and a windowRC at the start location (beginning for window, end for windowRC)
  *                     [3]  Fetches the identity of the sMer and its sMerRC by ANDing the sMer masks
  *                     [4]  Takes the lesser of the two and attempts to add it using HashTable.insertSMer.
  *                     [5]  Working window is shifted 2 bits to the right, returns true when complete
  *
  * Notes :             
  *
  */

bool Read::insertSMersOfRead(HashTable& H, Seed& seed)
{
    // Inserts all of the S-Mers found in this read into the hashtable, is expected to be done in parallel
    if (numberOfNs > length / 2)        // skip reads with half N's
        return true;
    if (seed.getLength() + 2 > length)      // seed too long for read; do nothing (+2 is needed for sGap that includes adjacent positions)
        return true;
    uint64_t sMerMask = seed.getSMerMask(), sMerMaskRC = seed.getSMerMaskRC();
    uint64_t binLength = length * 2;                                                    // length in bits
    uint64_t maskLength = 2 * (seed.getLength() + 2);                                   // mask length in bits
    uint64_t binBytes = binLength / 8 + ((binLength % 8 == 0) ? 0 : 1);   // length in bytes
    uint64_t sMer = 0, sMerRC = 0, minSMer = 0;             // minSMer = min(sMer,sMerRC) is in the table
    uint64_t window = 0, windowRC = 0;                      // 64-bit windows to slide through the read and readRC resp.
    // sMerMask, sGapMask are aligned to the left, sMerMastRC, sGapMaskRC to the right
    // initialize working windows;
    window = windowRC = 0;
    for (uint64_t i = 0; i < min((uint64_t)8, binBytes); ++i)
    {
        window   |= ((uint64_t)binRead[i]         << 8 * (7 - i));
        windowRC |= ((uint64_t)byteRC[binRead[i]] << 8 * i      );
    }
    
    // there are (binLength - masklength)/2 + 1 possible shifts to consider
    for (uint64_t shift = 0; shift <= (binLength - maskLength) / 2; ++shift)
    {
        // compute sMer and sMerRC and insert the min in the table
        sMer   = (window   & sMerMask) >> (64 - maskLength);
        sMerRC = (windowRC & sMerMaskRC);
        // the min is taken to reduce redundancy
        minSMer = min(sMer, sMerRC);
        if(H.insertSMer(minSMer) == false)          // table too full; need to restart
            return false;

        // update working windows
        window <<= 2;
        windowRC >>= 2;
        if (shift%4 == 3)   // add new bytes (if any) every 4 shifts (shifts 3, 7, 11, ...)
        {
            uint64_t nextByte = 8 + (shift + 1) / 4 - 1;
            if (binBytes > nextByte)  // if unprocesssed bytes exist
            {
                window |= (uint64_t)binRead[nextByte];
                windowRC |= ((uint64_t)(byteRC[binRead[nextByte]]) << 56);
            }
        }
    }
    return true;        // all SMers successfully inserted
}

/**
  * Name:               insertSGapsOfRead(HashTable& H, Seed& seed, uint64_t Te, uint64_t Ta, omp_lock_t* lockArray)
  *
  * Description :       Inserts all of the sGaps of the sMers of this read
  *
  * Input :
  *       Parameters:
  *           HashTable&  H             The hashTable to be inserted into
  *           Seed&       seed          The reference seed to draw the correct mask for this iteration
  *           uint64_t    Te            The count threshold for how acceptable deviants from the strongest sGap are
  *           int         seedNumber    Current iteration of program
  *           omp_lock_t* lockArray     The lock array to prevent multiple access at a hashTable location
  *
  * Output/Expected Changes :
  *       Parameters:
  *           HashTable& H              Hashtable will have these inserted sMers/sGaps after function completion
  *       Memory:
  *           None
  *       Return:                       
  *           None
  *
  * Process Synopsis :
  *                     [1]  Fetches the sMer/sGap masks
  *                     [2]  Targets a window and a windowRC at the start location (beginning for window, end for windowRC)
  *                     [3]  Fetches the identity of the sMer/sGap and its sMerRC/sGapRC by ANDing the sMer/sGap masks
  *                     [4]  Takes the lesser of the two sGaps and attempts to add it using HashTable.insertSGap.
  *                     [5]  Working window is shifted 2 bits to the right, returns true when complete
  *
  * Notes :             
  *
  */

void Read::insertSGapsOfRead(HashTable& H, Seed& seed, uint64_t Te, int seedNumber, omp_lock_t* lockArray)
{
    // Inserts all the SGaps of the read into the hashtable, expected to be done in parallel
    if (numberOfNs > length / 2)        // skip reads with half N's
        return;
    if (seed.getLength() + 2 > length)      // seed too long for read; do nothing (+2 is needed for the sGap -- adjacent positions needed)
        return;
    uint64_t sMerMask = seed.getSMerMask(), sMerMaskRC = seed.getSMerMaskRC(), sGapMask = seed.getSGapMask(), sGapMaskRC = seed.getSGapMaskRC();
    uint64_t binLength = length * 2;                                                    // length in bits
    uint64_t maskLength = 2 * (seed.getLength() + 2);                                   // mask length in bits
    uint64_t binBytes = binLength / 8 + ((binLength % 8 == 0) ? 0 : 1);   // length in bytes
    uint64_t sMer = 0, sMerRC = 0, minSMer = 0, sGap = 0, sGapRC = 0, minSGap = 0;      // min SMer = min(sMer,sMerRC) is the table, minSGap is the corresponding sGap
    uint64_t window = 0, windowRC = 0;                      // 64-bit windows to slide through the read and readRC resp.
    // sMerMask, sGapMask are applied to the left, sMerMastRC, sGapMaskRC to the right
    // initialize working windows
    window = windowRC = 0;
    for (uint64_t i = 0; i < min((uint64_t)8, binBytes); ++i)
    {
        window   |= ((uint64_t)binRead[i]         << 8 * (7 - i));
        windowRC |= ((uint64_t)byteRC[binRead[i]] << 8 * i      );
    }
    
    // there are (binLength - masklength)/2 + 1 possible shifts to consider
    for (uint64_t shift = 0; shift <= (binLength - maskLength) / 2; ++shift)
    {
        // compute sMer and sMerRC and search the min in the table
        sMer   = (window   & sMerMask) >> (64 - maskLength);
        sGap   = (window   & sGapMask) >> (64 - maskLength);
        sMerRC = (windowRC & sMerMaskRC);
        sGapRC = (windowRC & sGapMaskRC);
        // the min is taken to reduce redundancy
        minSMer = min(sMer, sMerRC);
        minSGap = (sMer < sMerRC) ? sGap : sGapRC;        
        H.insertSGap(minSMer, minSGap, Te, seedNumber, lockArray);
        // update working windows
        window <<= 2;
        windowRC >>= 2;
        if (shift%4 == 3)   // add new bytes (if any) every 4 shifts (shifts 3, 7, 11, ...)
        {
            uint64_t nextByte = 8 + (shift + 1) / 4 - 1;
            if (binBytes > nextByte)  // if unprocesssed bytes exist
            {
                window |= (uint64_t)binRead[nextByte];
                windowRC |= ((uint64_t)(byteRC[binRead[nextByte]]) << 56);
            }
        }
    }
    
}

/**
  * Name:               correctBinaryReadFast(HashTable& H, Seed& seed, uint64_t Tdiff, uint64_t* diff16Bits, uint64_t &corr)
  *
  * Description :       corrects the 2-bit representation to the correct variant as determined by the hashTable
  *
  * Input :
  *       Parameters:
  *           HashTable& H             The hashTable to be inserted into
  *           Seed&      seed          The reference seed to draw the correct mask for this iteration
  *           uint64_t   Tdiff         The acceptable difference between the old read sGap and the replacing read sGap
  *           uint64_t*  diff16Bits    Precomputed array to determine the difference between the sGaps
  *           uint64_t   &corr         The number of corrections made so far
  *
  * Output/Expected Changes :
  *       Parameters:
  *           uint64_t   &corr         The number of corrections made this read is added to the grand seed total
  *       Memory:
  *           uint8_t    binRead       binRead may be changed if the correct sGaps can be used instead of the old ones.
  *       Return:                       
  *           uint64_t                 Returns the number of erroneous positions of the read.
  *
  * Process Synopsis :
  *                     [1]  Initializes sMer/sGap masks and window/RC targets at start locations (same as construction)
  *                     [2]  Fetches the identity of the sMer/sGap and its sMerRC/sGapRC by ANDing the sMer/sGap masks
  *                     [3]  Takes the lesser of the two sGaps and sets the correct sGap given by getCorrectSGap()
  *                     [4]  Shifts the masks and repeats the 2-3 if there were errors found in the first iteration, at max 3 times
  *
  * Notes :             Assumed that the first sGap associated with the sMer is the correct one, as computed by removeAmbig 
  *                     Note that multiple iterations are required because changes on prior iterations may change outcome of following
  *                     iterations.
  *
  */


int64_t Read::correctBinaryReadFast(HashTable& H, Seed& seed, uint64_t Tdiff, uint64_t* diff16Bits, uint64_t &corr)
{
    // corrects a read given only one correct variant of each SGap
    if (numberOfNs > length / 2)        // skip reads with half N's
        return -1;
    if (seed.getLength() + 2 > length)      // seed too long for read; do nothing (+2 is needed for the sGap -- adjacent positions needed)
        return -1;
    uint64_t sMerMask = seed.getSMerMask(), sMerMaskRC = seed.getSMerMaskRC(), sGapMask = seed.getSGapMask(), sGapMaskRC = seed.getSGapMaskRC();
    uint64_t binLength = length * 2;                                                    // length in bits
    uint64_t maskLength = 2 * (seed.getLength() + 2);                                   // mask length in bits
    uint64_t binBytes = binLength / 8 + ((binLength % 8 == 0) ? 0 : 1);   // length in bytes
    uint64_t sMer = 0, sMerRC = 0, minSMer = 0, sGap = 0, sGapRC = 0, minSGap = 0;      // min SMer = min(sMer,sMerRC) is the table, minSGap is the corresponding sGap
    uint64_t correctSGapInTable = 0, correctSGap = 0, correctSGapRC = 0;                // correctSGapInTable is the correct sGap in hash table (corrspondign to minSGap); correctSGap is the direct correct sGap and correctSGapRC is the rev compl correct sGap
    uint64_t window = 0, windowRC = 0;                      // 64-bit windows to slide through the read and readRC resp.
    uint8_t processedByte = 0;
    uint64_t leftmostTwoBits = (uint64_t)3 << 62;         // processedByte accumulates processed bits (as they fall off shifted at the end of working window)
    // sMerMask, sGapMask are applied to the left, sMerMastRC, sGapMaskRC to the right
    
    int64_t errPositions = 1;   // will store the number of erroneous positions suspected
    uint64_t numberOfShifts = (binLength - maskLength) / 2 + 1;
    uint64_t iterations = 0;
    
    while ((iterations < 3) && (errPositions > 0))
    {
        errPositions = 0;
        
        // CORRECT READ
        // initialize working windows
        window = windowRC = 0;
        for (uint64_t i = 0; i < min((uint64_t)8, binBytes); ++i)
        {
            window   |= ((uint64_t)binRead[i]         << 8 * (7 - i));
            windowRC |= ((uint64_t)byteRC[binRead[i]] << 8 * i      );
        }
        // there are  numberOfShifts = (binLength - masklength) / 2 + 1 possible shifts to consider
        for (uint64_t shift = 0; shift < numberOfShifts; ++shift)
        {
            // compute sMer and sMerRC and search the min in the table
            sMer   = (window   & sMerMask) >> (64 - maskLength);
            sGap   = (window   & sGapMask) >> (64 - maskLength);
            sMerRC = (windowRC & sMerMaskRC);
            sGapRC = (windowRC & sGapMaskRC);
            
            // perform correction: change both working windows and store changes to be copied in binRead[]
            bool directSMerStored = (sMer < sMerRC);
            minSMer = min(sMer, sMerRC);
            minSGap = (directSMerStored) ? sGap : sGapRC;
            // get correctSGapInTable; if it exists, then correct working windows
            // getCorrectSGap returns: -3: sMer not in table; -2: sMer ambiguous; -1: sGap too different (sMer is in table and NOT ambiguous)
            //              0: already correct; count >= 1: correctSGap NOT too different; correction can be done
            int64_t corrStatus = H.getCorrectSGap(minSMer, minSGap, correctSGapInTable, Tdiff, diff16Bits);
            if (corrStatus != 0)    // count erroneous positions
                ++errPositions;
            if (corrStatus > 0)     // can be corrected
            {
                ++corr;
                if (directSMerStored)  // correctSGapInTable is the direct sGap but aligned right
                {
                    correctSGap   = correctSGapInTable << (64 - maskLength);
                    correctSGapRC = computeRevCompl(correctSGap) & sGapMaskRC;
                }
                else        // correctSGapInTable is the rev compl sGap
                {
                    correctSGapRC = correctSGapInTable;
                    correctSGap   = computeRevCompl(correctSGapRC) & sGapMask;      // the correct direct sGap
                }
                // change direct window
                window &= (~ sGapMask);                          // set on 0 all mask bits
                window |= correctSGap;                          // set correctly the mask bits
                // change RC window
                windowRC &= (~ sGapMaskRC);                      // set on 0 all mask bits
                windowRC |= correctSGapRC;                      // set correctly the mask bits
            }   // done changing working windows
            
            // store the leftmost 2 processed bits to be copied back into binRead[] (before they fall off the end of the working window)
            processedByte |= (uint8_t)((window & leftmostTwoBits) >> (56 + 2 * (shift % 4)));
            
            // copy corrections (accumulated in processedByte) back to binRead[] every 4 shifts
            if (shift % 4 == 3)
            {
                binRead[(shift - 3) / 4] = processedByte;
                processedByte = 0;
            }
            
            // update working windows
            window <<= 2;
            windowRC >>= 2;
            if (shift % 4 == 3)   // add new bytes (if any) every 4 shifts (shifts 3, 7, 11, ...)
            {
                uint64_t nextByte = 8 + (shift + 1) / 4 - 1;
                if (binBytes > nextByte)  // if unprocesssed bytes exist
                {
                    window |= (uint64_t)binRead[nextByte];
                    windowRC |= ((uint64_t)(byteRC[binRead[nextByte]]) << 56);
                }
            }
        }       // done all shifts
        // still need to copy the content of the working window (maskLength bits) in binRead
        // continue shifting from where we left off: shift = (binLength - maskLength) / 2 + 1
        for (uint64_t shift = numberOfShifts; shift < binLength / 2; ++shift)
        {
            processedByte |= (uint8_t)((window & leftmostTwoBits) >> (56 + 2 * (shift % 4)));
            if (shift % 4 == 3)
            {
                binRead[(shift - 3) / 4] = processedByte;
                processedByte = 0;
            }
            window <<= 2;
        }
        if (binLength - (binBytes - 1) * 8 < 8)
        { // copy also the last, incomplete, processed byte
            binRead[binBytes - 1] = processedByte;
        }
        
        processedByte = 0; // reset
        // DONE ONE PASS CORRECTING
        
        ++iterations;
    }
    return errPositions;
}

/**
  * Name:               correctCharRead()
  *
  * Description :       Changes the string array associated with the read to match its assumed correct 2-bit representation
  *
  * Input :
  *       Parameters:
  *           None
  *
  * Output/Expected Changes :
  *       Parameters:
  *           Non
  *       Memory:
  *           char*     charRead                Changed to be the same as the 2-bit representation
  *       Return:                       
  *           None
  *
  * Process Synopsis :
  *                     [1]  Iterates through the charRead, and changes it to match the 2-bit representation
  *
  * Notes :             It is assumed that the 2-bit representation has been corrected already
  *
  */

void Read::correctCharRead()
{
    if (numberOfNs > length / 2)        // skip reads with half N's
        return;
    uint64_t charValue = 0, currByte = 0, currByteValue = binRead[0];
    for (uint64_t pos = 0; pos < length; ++pos)
    {
        charValue = (currByteValue & (3 << 6)) >> 6;    // leftmost 2 bits of the byte shifted right
        switch (charValue)
        {   case 0:
                charRead[pos] = 'A';
                break;
            case 1:
                charRead[pos] = 'C';
                break;
            case 2:
                charRead[pos] = 'G';
                break;
            case 3:
                charRead[pos] = 'T';
                break;
        }
        currByteValue <<= 2;
        if (pos % 4 == 3)   // move to next byte
        {
            ++currByte;             // currCharPos % 4 == 0 next iteration
            currByteValue = binRead[currByte];
        }
    }
}

/**
  * Name:               clear()
  *
  * Description :       Deletes the read
  *
  * Input :
  *       Parameters:
  *           None
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           uint8_t   binRead             Deallocate memory associated with the 2-bit representation
  *       Return:
  *           None
  *
  * Process Synopsis :
  *
  *
  * Notes :             
  *
  */

void Read::clear()
{
    delete [] binRead;
}

/**
  * Name:               outputRead(char* outputString)
  *
  * Description :       returns the string array of the read (nucleotide identities)
  *
  * Input :
  *       Parameters:
  *           char*     outputString        The output string of the read
  *
  * Output/Expected Changes :
  *       Parameters:
  *           char*     outputString        Will contain the nucleotide identities of the read
  *       Memory:
  *           None
  *       Return:
  *           None
  *
  * Process Synopsis :
  *
  *
  * Notes :             
  *
  */

void Read::outputRead(char* outputString)
{
    strcpy(outputString, charRead);
}

