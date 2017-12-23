
/** 
  * File:     seeds.cpp 
  * 
  * Author1:  Lucian Ilie (ilie@uwo.ca) 
  * Author2:  Stephen Lu (slu93@uwo.ca) 
  * Date:     Fall 2017  
  * 
  *   This file contains code concerning the use of spaced seeds.
  *   Spaced seeds are designed to be efficiently sensitive at searching
  *   searching for approximate strings, which is highly relevant in
  *   the context of searching for erroneous information found within
  *   the reads of NGS data. Spaced Seeds were produced by Speed.
  *   Read more at http://www.csd.uwo.ca/~ilie/SpEED/
  *
  */


#include "QUESS.h"


/**
  * Name:               Seed(char* seed)
  *
  * Description :       Creates a spaced seed given a char array dictating the spaced seed sMer/sGaps.
  *
  * Input :
  *       Parameters:
  *           char*     seed            The char array containing the sMer/sGap mask identities.
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           None
  *       Return:
  *           Seed                      A seed is created based on the input char array. Relevent masks and their
  *                                     reverse complementaries are compiled.
  *
  * Process Synopsis :
  *                     [1]  Initialize all masks.
  *                     [2]  sMer masks are given a 1 whenever 1 is seen in the 'seed' input and 0 otherwise.
  *                     [3]  sGap masks are given the inverse
  *
  * Notes :             
  *
  */

Seed::Seed(char* seed)
{
    charSeed = new char[100];
    strcpy(charSeed, seed);
    length = strlen(charSeed);
    if (length > 26) { cerr << "ERROR: seed too long"; exit(EXIT_FAILURE); }
    weight  = 0;
    for (uint64_t i = 0; i < length; ++i)
        if (charSeed[i] == '1')
            ++weight;
	// compute masks
    // masks for direct seed are aligned to the left of the word
	// masks for reversed seed are aligned to the right
    // ===================================================================================
    //   EXAMPLE: general seed; the actual seeds used in QUESS are palindromes
    //   seed = 111011001; length = 9; weight = 6; reverseSeed = 100110111
    // sMerMask:   00111111 00111100 00110000 00000000 00000000 00000000 00000000 00000000
    // sGapMask:   11000000 11000011 11000000 00000000 00000000 00000000 00000000 00000000
    // sMerMaskRC: 00000000 00000000 00000000 00000000 00000000 00001100 00111100 11111100
    // sMerMaskRC: 00000000 00000000 00000000 00000000 00000000 00110011 11000011 00000011
    // ===================================================================================

	// we compute first both mask aligned to the right then shift them to the left
    sMerMask = sMerMaskRC = sGapMask = sGapMaskRC = 0;
	for (uint64_t i = 0; i < length; ++i)
    {
		sMerMask <<= 2;
		if (charSeed[i] == '1')
			sMerMask |= 3;
	}
	sMerMask <<= 2;
	sGapMask = ~sMerMask & (((uint64_t)1 << (2 * (length + 2))) - 1);
	// shift both masks to be aligned to the left
	sMerMask <<= (uint64_t)(64 - 2 * (length + 2));
	sGapMask <<= (uint64_t)(64 - 2 * (length + 2));

    // !!! THIS WORKS BECAUSE SEEDS ARE PALINDROMES !!!
    
    sMerMaskRC = sMerMask >> (64 - 2 * (length + 2));
	sGapMaskRC = sGapMask >> (64 - 2 * (length + 2));

}

/**
  * Name:               getLength(), getWeight(), getSMerMask(), getSGapMask(), getSMerMaskRC(), getSGapMaskRC()
  *
  * Description :       Returns relevant information about a particular seed
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
  *           length                    the length of the seed
  *           weight                    the number of identity=1 in the seed 
  *           sMerMask                  the sMer mask. ex.   00111111 00111100 00110000 00000000 00000000 00000000 00000000 00000000
  *           sGapMask                  the sGap mask. ex.   11000000 11000011 11000000 00000000 00000000 00000000 00000000 00000000
  *           sMerMaskRC                the sMerRC mask. ex. 00000000 00000000 00000000 00000000 00000000 00001100 00111100 11111100
  *           sGapMaskRC                the sGapRC mask. ex. 00000000 00000000 00000000 00000000 00000000 00110011 11000011 00000011
  *
  * Process Synopsis :
  *                     [1]  Fetch relevant information
  *
  * Notes :             
  *
  */

uint64_t Seed::getLength()
{   return(length);     }

uint64_t Seed::getWeight()
{   return(weight);     }

uint64_t Seed::getSMerMask()
{   return(sMerMask);   }

uint64_t Seed::getSGapMask()
{   return(sGapMask);   }

uint64_t Seed::getSMerMaskRC()
{   return(sMerMaskRC); }

uint64_t Seed::getSGapMaskRC()
{   return(sGapMaskRC); }

/**
  * Name:               printSeedInfo()
  *
  * Description :       Prints all the information
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
  *           None
  *
  * Process Synopsis :
  *                     [1]  Prints all information about the seed.
  *
  * Notes :             
  *
  */

void Seed::printSeedInfo()
{
    cout << "========== seed ==============\n";
    cout << "seed = " << charSeed << endl;
    cout << "length = " << length << endl;
    cout << "weight = " << weight << endl;
    cout << "sMerMask:   "; printIntInBinary(sMerMask);
    cout << "sMerMaskRC: "; printIntInBinary(sMerMaskRC);
    cout << "sGapMask:   "; printIntInBinary(sGapMask);
    cout << "sGapMaskRC: "; printIntInBinary(sGapMaskRC);
    cout << "========== seed ================\n";
}

/**
  * Name:             getSeed(char* &seed, uint64_t seedWeight, uint64_t numberOfSeeds, uint64_t seedNumber, uint64_t &peakMemory, uint64_t &currentMemory)
  *
  * Description :     Creates a hashTable that is greater than minRequiredSize
  *
  * Input :
  *       Parameters:
  *           char*     &seed               The char array containing the seed
  *           uint64_t  seedWeight          The weight of the seed (identity=1) (10-26)
  *           uint64_t  numberOfSeeds       The number of seeds (max 8)
  *           uint64_t  seedNumber          The current seed number (0-7)
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program
  *
  * Output/Expected Changes :
  *       Parameters:
  *           char*     &seed               seed will point to the correct spaced seed as dictated by the parameters.
  *           uint64_t  &peakMemory         peakMemory is recorded and estimated for testing
  *           uint64_t  &currentMemory      currentMemory is recorded and estimated for testing
  *       Memory:
  *           None
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Spaced Seeds are precomputed using SpEED
  *                     [2]  Spaced Seeds are fetched based on parameters.
  *
  * Notes :             SEEDS: 2^k seeds of weight w, stored as: seeds[w][k][i], 0 <= i <= k-1, 0 <= w <= 26, 0 <= k <= 3
  *                     SENSITIVITY: sensitivity(seeds[w][k][], p, 100), p = .60, .65, .70, .75, .80, .85, .90, .95
  *                     stored as: sensitivity[w][k][(p-60)/5], 0 <= (p-60)/5 <= 7
  *
  */

void getSeed(char* &seed, uint64_t seedWeight, uint64_t numberOfSeeds, uint64_t seedNumber, uint64_t &peakMemory, uint64_t &currentMemory)
{	
	if ((seedWeight < 10) || (seedWeight > 26) ||
		(numberOfSeeds < 1) || (numberOfSeeds > 8) ||       // if numberOfSeeds is not a power of 2, then get the first seeds from the next
		(seedNumber > numberOfSeeds)) {
		cout << "ERROR: wrong seed request" << endl;
		exit(EXIT_FAILURE);
	}
	

    char**** seeds_64 = new char*** [27];
    double*** sensitivity_64 = new double** [27];
    for (int w = 0; w < 27; ++w)
    {
        seeds_64[w] = new char**[4];
        sensitivity_64[w] = new double*[4];
        for (int k = 0; k < 4; ++k)
        {
            seeds_64[w][k] = new char*[(int)pow(2,k)];
            for (int i = 0; i < (int)pow(2,k); ++i)
                seeds_64[w][k][i] = new char[100];     // seeds are always of length 26 or 25 but 100 allocated should this change in the future
            sensitivity_64[w][k] = new double[8];
            for (int p = 0; p < 8; ++p)
                sensitivity_64[w][k][p] = 0;
        }
    }
    currentMemory+=27*4*16*(sizeof(char)+sizeof(double));
      if (currentMemory > peakMemory)
    {
        peakMemory = currentMemory;
#ifdef VERBOSE
        cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
    }
    // ===================================================================================
    // SEEDS for 64 bits
    // ===================================================================================
    
    
    
    
    // ============================
    // weight = 10
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[10][0][0], "11100010010000001001000111" );
    sensitivity_64[10][0][0] = 0.317124 ; // similarity = 60%, read length = 100
    sensitivity_64[10][0][1] = 0.540937 ; // similarity = 65%, read length = 100
    sensitivity_64[10][0][2] = 0.770873 ; // similarity = 70%, read length = 100
    sensitivity_64[10][0][3] = 0.927553 ; // similarity = 75%, read length = 100
    sensitivity_64[10][0][4] = 0.988597 ; // similarity = 80%, read length = 100
    sensitivity_64[10][0][5] = 0.99941 ; // similarity = 85%, read length = 100
    sensitivity_64[10][0][6] = 0.999996 ; // similarity = 90%, read length = 100
    sensitivity_64[10][0][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[10][1][0], "11001010000011000001010011" );
    strcpy(seeds_64[10][1][1], "11000001010100101010000011" );
    sensitivity_64[10][1][0] = 0.471764 ; // similarity = 60%, read length = 100
    sensitivity_64[10][1][1] = 0.709493 ; // similarity = 65%, read length = 100
    sensitivity_64[10][1][2] = 0.891667 ; // similarity = 70%, read length = 100
    sensitivity_64[10][1][3] = 0.977043 ; // similarity = 75%, read length = 100
    sensitivity_64[10][1][4] = 0.997882 ; // similarity = 80%, read length = 100
    sensitivity_64[10][1][5] = 0.999946 ; // similarity = 85%, read length = 100
    sensitivity_64[10][1][6] = 1 ; // similarity = 90%, read length = 100
    sensitivity_64[10][1][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[10][2][0], "10110000101000010100001101" );
    strcpy(seeds_64[10][2][1], "11001001001000010010010011" );
    strcpy(seeds_64[10][2][2], "10101000000111100000010101" );
    strcpy(seeds_64[10][2][3], "10100100011000011000100101" );
    sensitivity_64[10][2][0] = 0.637091 ; // similarity = 60%, read length = 100
    sensitivity_64[10][2][1] = 0.845275 ; // similarity = 65%, read length = 100
    sensitivity_64[10][2][2] = 0.959319 ; // similarity = 70%, read length = 100
    sensitivity_64[10][2][3] = 0.994559 ; // similarity = 75%, read length = 100
    sensitivity_64[10][2][4] = 0.999724 ; // similarity = 80%, read length = 100
    sensitivity_64[10][2][5] = 0.999997 ; // similarity = 85%, read length = 100
    sensitivity_64[10][2][6] = 1 ; // similarity = 90%, read length = 100
    sensitivity_64[10][2][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[10][3][0], "11000101000100100010100011" );
    strcpy(seeds_64[10][3][1], "11010010001000010001001011" );
    strcpy(seeds_64[10][3][2], "10100100001100110000100101" );
    strcpy(seeds_64[10][3][3], "10101001001000010010010101" );
    strcpy(seeds_64[10][3][4], "11000011010000001011000011" );
    strcpy(seeds_64[10][3][5], "10010001101000010110001001" );
    strcpy(seeds_64[10][3][6], "10110100000011000000101101" );
    strcpy(seeds_64[10][3][7], "11100000100100100100000111" );
    sensitivity_64[10][3][0] = 0.786943 ; // similarity = 60%, read length = 100
    sensitivity_64[10][3][1] = 0.933823 ; // similarity = 65%, read length = 100
    sensitivity_64[10][3][2] = 0.988538 ; // similarity = 70%, read length = 100
    sensitivity_64[10][3][3] = 0.999102 ; // similarity = 75%, read length = 100
    sensitivity_64[10][3][4] = 0.999977 ; // similarity = 80%, read length = 100
    sensitivity_64[10][3][5] = 1 ; // similarity = 85%, read length = 100
    sensitivity_64[10][3][6] = 1 ; // similarity = 90%, read length = 100
    sensitivity_64[10][3][7] = 1 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 11
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[11][0][0], "1110010001001001000100111" );
    sensitivity_64[11][0][0] = 0.209445 ; // similarity = 60%, read length = 100
    sensitivity_64[11][0][1] = 0.40446 ; // similarity = 65%, read length = 100
    sensitivity_64[11][0][2] = 0.649418 ; // similarity = 70%, read length = 100
    sensitivity_64[11][0][3] = 0.861161 ; // similarity = 75%, read length = 100
    sensitivity_64[11][0][4] = 0.970564 ; // similarity = 80%, read length = 100
    sensitivity_64[11][0][5] = 0.997738 ; // similarity = 85%, read length = 100
    sensitivity_64[11][0][6] = 0.999972 ; // similarity = 90%, read length = 100
    sensitivity_64[11][0][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[11][1][0], "1001011000101010001101001" );
    strcpy(seeds_64[11][1][1], "1101100100001000010011011" );
    sensitivity_64[11][1][0] = 0.333048 ; // similarity = 60%, read length = 100
    sensitivity_64[11][1][1] = 0.572043 ; // similarity = 65%, read length = 100
    sensitivity_64[11][1][2] = 0.804129 ; // similarity = 70%, read length = 100
    sensitivity_64[11][1][3] = 0.946315 ; // similarity = 75%, read length = 100
    sensitivity_64[11][1][4] = 0.993222 ; // similarity = 80%, read length = 100
    sensitivity_64[11][1][5] = 0.999749 ; // similarity = 85%, read length = 100
    sensitivity_64[11][1][6] = 0.999999 ; // similarity = 90%, read length = 100
    sensitivity_64[11][1][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[11][2][0], "1101100000011100000011011" );
    strcpy(seeds_64[11][2][1], "1111000001001001000001111" );
    strcpy(seeds_64[11][2][2], "1011001010001000101001101" );
    strcpy(seeds_64[11][2][3], "1010000100111110010000101" );
    sensitivity_64[11][2][0] = 0.473676 ; // similarity = 60%, read length = 100
    sensitivity_64[11][2][1] = 0.719848 ; // similarity = 65%, read length = 100
    sensitivity_64[11][2][2] = 0.901934 ; // similarity = 70%, read length = 100
    sensitivity_64[11][2][3] = 0.981318 ; // similarity = 75%, read length = 100
    sensitivity_64[11][2][4] = 0.998544 ; // similarity = 80%, read length = 100
    sensitivity_64[11][2][5] = 0.999972 ; // similarity = 85%, read length = 100
    sensitivity_64[11][2][6] = 1 ; // similarity = 90%, read length = 100
    sensitivity_64[11][2][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[11][3][0], "1000001010111110101000001" );
    strcpy(seeds_64[11][3][1], "1000110001011101000110001" );
    strcpy(seeds_64[11][3][2], "1100000100111110010000011" );
    strcpy(seeds_64[11][3][3], "1011100010001000100011101" );
    strcpy(seeds_64[11][3][4], "1100011001001001001100011" );
    strcpy(seeds_64[11][3][5], "1010100000111110000010101" );
    strcpy(seeds_64[11][3][6], "1101010010001000100101011" );
    strcpy(seeds_64[11][3][7], "1010100101001001010010101" );
    sensitivity_64[11][3][0] = 0.622781 ; // similarity = 60%, read length = 100
    sensitivity_64[11][3][1] = 0.841034 ; // similarity = 65%, read length = 100
    sensitivity_64[11][3][2] = 0.959719 ; // similarity = 70%, read length = 100
    sensitivity_64[11][3][3] = 0.995006 ; // similarity = 75%, read length = 100
    sensitivity_64[11][3][4] = 0.999779 ; // similarity = 80%, read length = 100
    sensitivity_64[11][3][5] = 0.999998 ; // similarity = 85%, read length = 100
    sensitivity_64[11][3][6] = 1 ; // similarity = 90%, read length = 100
    sensitivity_64[11][3][7] = 1 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 12
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[12][0][0], "11101000100100100100010111" );
    sensitivity_64[12][0][0] = 0.132813 ; // similarity = 60%, read length = 100
    sensitivity_64[12][0][1] = 0.289076 ; // similarity = 65%, read length = 100
    sensitivity_64[12][0][2] = 0.523315 ; // similarity = 70%, read length = 100
    sensitivity_64[12][0][3] = 0.772802 ; // similarity = 75%, read length = 100
    sensitivity_64[12][0][4] = 0.938027 ; // similarity = 80%, read length = 100
    sensitivity_64[12][0][5] = 0.993306 ; // similarity = 85%, read length = 100
    sensitivity_64[12][0][6] = 0.999866 ; // similarity = 90%, read length = 100
    sensitivity_64[12][0][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[12][1][0], "11011100010000001000111011" );
    strcpy(seeds_64[12][1][1], "11100010010100101001000111" );
    sensitivity_64[12][1][0] = 0.222484 ; // similarity = 60%, read length = 100
    sensitivity_64[12][1][1] = 0.436193 ; // similarity = 65%, read length = 100
    sensitivity_64[12][1][2] = 0.692909 ; // similarity = 70%, read length = 100
    sensitivity_64[12][1][3] = 0.89395 ; // similarity = 75%, read length = 100
    sensitivity_64[12][1][4] = 0.981991 ; // similarity = 80%, read length = 100
    sensitivity_64[12][1][5] = 0.999023 ; // similarity = 85%, read length = 100
    sensitivity_64[12][1][6] = 0.999993 ; // similarity = 90%, read length = 100
    sensitivity_64[12][1][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[12][2][0], "10100011100011000111000101" );
    strcpy(seeds_64[12][2][1], "11000010011100111001000011" );
    strcpy(seeds_64[12][2][2], "11011000101000010100011011" );
    strcpy(seeds_64[12][2][3], "11001110000011000001110011" );
    sensitivity_64[12][2][0] = 0.334583 ; // similarity = 60%, read length = 100
    sensitivity_64[12][2][1] = 0.582901 ; // similarity = 65%, read length = 100
    sensitivity_64[12][2][2] = 0.818079 ; // similarity = 70%, read length = 100
    sensitivity_64[12][2][3] = 0.954133 ; // similarity = 75%, read length = 100
    sensitivity_64[12][2][4] = 0.994931 ; // similarity = 80%, read length = 100
    sensitivity_64[12][2][5] = 0.999847 ; // similarity = 85%, read length = 100
    sensitivity_64[12][2][6] = 1 ; // similarity = 90%, read length = 100
    sensitivity_64[12][2][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[12][3][0], "11100010000111100001000111" );
    strcpy(seeds_64[12][3][1], "11011100010000001000111011" );
    strcpy(seeds_64[12][3][2], "10111000110000001100011101" );
    strcpy(seeds_64[12][3][3], "10000001111100111110000001" );
    strcpy(seeds_64[12][3][4], "10100100011100111000100101" );
    strcpy(seeds_64[12][3][5], "10100101001011010010100101" );
    strcpy(seeds_64[12][3][6], "10001011011000011011010001" );
    strcpy(seeds_64[12][3][7], "11001010101000010101010011" );
    sensitivity_64[12][3][0] = 0.470257 ; // similarity = 60%, read length = 100
    sensitivity_64[12][3][1] = 0.725395 ; // similarity = 65%, read length = 100
    sensitivity_64[12][3][2] = 0.90922 ; // similarity = 70%, read length = 100
    sensitivity_64[12][3][3] = 0.984378 ; // similarity = 75%, read length = 100
    sensitivity_64[12][3][4] = 0.998968 ; // similarity = 80%, read length = 100
    sensitivity_64[12][3][5] = 0.999985 ; // similarity = 85%, read length = 100
    sensitivity_64[12][3][6] = 1 ; // similarity = 90%, read length = 100
    sensitivity_64[12][3][7] = 1 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 13
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[13][0][0], "1110101001001001001010111" );
    sensitivity_64[13][0][0] = 0.0840857 ; // similarity = 60%, read length = 100
    sensitivity_64[13][0][1] = 0.204253 ; // similarity = 65%, read length = 100
    sensitivity_64[13][0][2] = 0.413693 ; // similarity = 70%, read length = 100
    sensitivity_64[13][0][3] = 0.679404 ; // similarity = 75%, read length = 100
    sensitivity_64[13][0][4] = 0.894793 ; // similarity = 80%, read length = 100
    sensitivity_64[13][0][5] = 0.985593 ; // similarity = 85%, read length = 100
    sensitivity_64[13][0][6] = 0.99961 ; // similarity = 90%, read length = 100
    sensitivity_64[13][0][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[13][1][0], "1110110100001000010110111" );
    strcpy(seeds_64[13][1][1], "1011000011011101100001101" );
    sensitivity_64[13][1][0] = 0.144027 ; // similarity = 60%, read length = 100
    sensitivity_64[13][1][1] = 0.318419 ; // similarity = 65%, read length = 100
    sensitivity_64[13][1][2] = 0.570637 ; // similarity = 70%, read length = 100
    sensitivity_64[13][1][3] = 0.81759 ; // similarity = 75%, read length = 100
    sensitivity_64[13][1][4] = 0.959011 ; // similarity = 80%, read length = 100
    sensitivity_64[13][1][5] = 0.996772 ; // similarity = 85%, read length = 100
    sensitivity_64[13][1][6] = 0.999962 ; // similarity = 90%, read length = 100
    sensitivity_64[13][1][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[13][2][0], "1001010011101011100101001" );
    strcpy(seeds_64[13][2][1], "1010100110101010110010101" );
    strcpy(seeds_64[13][2][2], "1010110001011101000110101" );
    strcpy(seeds_64[13][2][3], "1101101100001000011011011" );
    sensitivity_64[13][2][0] = 0.225131 ; // similarity = 60%, read length = 100
    sensitivity_64[13][2][1] = 0.447274 ; // similarity = 65%, read length = 100
    sensitivity_64[13][2][2] = 0.70945 ; // similarity = 70%, read length = 100
    sensitivity_64[13][2][3] = 0.905948 ; // similarity = 75%, read length = 100
    sensitivity_64[13][2][4] = 0.985667 ; // similarity = 80%, read length = 100
    sensitivity_64[13][2][5] = 0.999348 ; // similarity = 85%, read length = 100
    sensitivity_64[13][2][6] = 0.999997 ; // similarity = 90%, read length = 100
    sensitivity_64[13][2][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[13][3][0], "1110000011011101100000111" );
    strcpy(seeds_64[13][3][1], "1000001111011101111000001" );
    strcpy(seeds_64[13][3][2], "1110000110101010110000111" );
    strcpy(seeds_64[13][3][3], "1011000110011100110001101" );
    strcpy(seeds_64[13][3][4], "1000110010111110100110001" );
    strcpy(seeds_64[13][3][5], "1100111001001001001110011" );
    strcpy(seeds_64[13][3][6], "1110110100001000010110111" );
    strcpy(seeds_64[13][3][7], "1101101100001000011011011" );
    sensitivity_64[13][3][0] = 0.330091 ; // similarity = 60%, read length = 100
    sensitivity_64[13][3][1] = 0.585389 ; // similarity = 65%, read length = 100
    sensitivity_64[13][3][2] = 0.824934 ; // similarity = 70%, read length = 100
    sensitivity_64[13][3][3] = 0.958553 ; // similarity = 75%, read length = 100
    sensitivity_64[13][3][4] = 0.995889 ; // similarity = 80%, read length = 100
    sensitivity_64[13][3][5] = 0.999896 ; // similarity = 85%, read length = 100
    sensitivity_64[13][3][6] = 1 ; // similarity = 90%, read length = 100
    sensitivity_64[13][3][7] = 1 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 14
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[14][0][0], "11101010010011001001010111" );
    sensitivity_64[14][0][0] = 0.0517308 ; // similarity = 60%, read length = 100
    sensitivity_64[14][0][1] = 0.139735 ; // similarity = 65%, read length = 100
    sensitivity_64[14][0][2] = 0.315786 ; // similarity = 70%, read length = 100
    sensitivity_64[14][0][3] = 0.578074 ; // similarity = 75%, read length = 100
    sensitivity_64[14][0][4] = 0.835235 ; // similarity = 80%, read length = 100
    sensitivity_64[14][0][5] = 0.971264 ; // similarity = 85%, read length = 100
    sensitivity_64[14][0][6] = 0.998905 ; // similarity = 90%, read length = 100
    sensitivity_64[14][0][7] = 0.999999 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[14][1][0], "11100010010111101001000111" );
    strcpy(seeds_64[14][1][1], "11101011001000010011010111" );
    sensitivity_64[14][1][0] = 0.0909984 ; // similarity = 60%, read length = 100
    sensitivity_64[14][1][1] = 0.226249 ; // similarity = 65%, read length = 100
    sensitivity_64[14][1][2] = 0.456527 ; // similarity = 70%, read length = 100
    sensitivity_64[14][1][3] = 0.729192 ; // similarity = 75%, read length = 100
    sensitivity_64[14][1][4] = 0.924262 ; // similarity = 80%, read length = 100
    sensitivity_64[14][1][5] = 0.991987 ; // similarity = 85%, read length = 100
    sensitivity_64[14][1][6] = 0.999857 ; // similarity = 90%, read length = 100
    sensitivity_64[14][1][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[14][2][0], "10010010111100111101001001" );
    strcpy(seeds_64[14][2][1], "11101001000111100010010111" );
    strcpy(seeds_64[14][2][2], "11001011101000010111010011" );
    strcpy(seeds_64[14][2][3], "11110101001000010010101111" );
    sensitivity_64[14][2][0] = 0.148679 ; // similarity = 60%, read length = 100
    sensitivity_64[14][2][1] = 0.334734 ; // similarity = 65%, read length = 100
    sensitivity_64[14][2][2] = 0.59842 ; // similarity = 70%, read length = 100
    sensitivity_64[14][2][3] = 0.842522 ; // similarity = 75%, read length = 100
    sensitivity_64[14][2][4] = 0.969067 ; // similarity = 80%, read length = 100
    sensitivity_64[14][2][5] = 0.998035 ; // similarity = 85%, read length = 100
    sensitivity_64[14][2][6] = 0.999984 ; // similarity = 90%, read length = 100
    sensitivity_64[14][2][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[14][3][0], "10111010011000011001011101" );
    strcpy(seeds_64[14][3][1], "11001110001100110001110011" );
    strcpy(seeds_64[14][3][2], "11101010001011010001010111" );
    strcpy(seeds_64[14][3][3], "10000101101111110110100001" );
    strcpy(seeds_64[14][3][4], "10011100110100101100111001" );
    strcpy(seeds_64[14][3][5], "11000110110011001101100011" );
    strcpy(seeds_64[14][3][6], "11110100101000010100101111" );
    strcpy(seeds_64[14][3][7], "11111001000011000010011111" );
    sensitivity_64[14][3][0] = 0.228625 ; // similarity = 60%, read length = 100
    sensitivity_64[14][3][1] = 0.461634 ; // similarity = 65%, read length = 100
    sensitivity_64[14][3][2] = 0.730233 ; // similarity = 70%, read length = 100
    sensitivity_64[14][3][3] = 0.919908 ; // similarity = 75%, read length = 100
    sensitivity_64[14][3][4] = 0.989395 ; // similarity = 80%, read length = 100
    sensitivity_64[14][3][5] = 0.999613 ; // similarity = 85%, read length = 100
    sensitivity_64[14][3][6] = 0.999999 ; // similarity = 90%, read length = 100
    sensitivity_64[14][3][7] = 1 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 15
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[15][0][0], "1111001100101010011001111" );
    sensitivity_64[15][0][0] = 0.0318523 ; // similarity = 60%, read length = 100
    sensitivity_64[15][0][1] = 0.0946804 ; // similarity = 65%, read length = 100
    sensitivity_64[15][0][2] = 0.236427 ; // similarity = 70%, read length = 100
    sensitivity_64[15][0][3] = 0.479866 ; // similarity = 75%, read length = 100
    sensitivity_64[15][0][4] = 0.7634 ; // similarity = 80%, read length = 100
    sensitivity_64[15][0][5] = 0.948494 ; // similarity = 85%, read length = 100
    sensitivity_64[15][0][6] = 0.997279 ; // similarity = 90%, read length = 100
    sensitivity_64[15][0][7] = 0.999995 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[15][1][0], "1101111000101010001111011" );
    strcpy(seeds_64[15][1][1], "1110100110011100110010111" );
    sensitivity_64[15][1][0] = 0.0566194 ; // similarity = 60%, read length = 100
    sensitivity_64[15][1][1] = 0.156478 ; // similarity = 65%, read length = 100
    sensitivity_64[15][1][2] = 0.352963 ; // similarity = 70%, read length = 100
    sensitivity_64[15][1][3] = 0.629765 ; // similarity = 75%, read length = 100
    sensitivity_64[15][1][4] = 0.873832 ; // similarity = 80%, read length = 100
    sensitivity_64[15][1][5] = 0.982561 ; // similarity = 85%, read length = 100
    sensitivity_64[15][1][6] = 0.999555 ; // similarity = 90%, read length = 100
    sensitivity_64[15][1][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[15][2][0], "1111001100101010011001111" );
    strcpy(seeds_64[15][2][1], "1110010011101011100100111" );
    strcpy(seeds_64[15][2][2], "1110110100011100010110111" );
    strcpy(seeds_64[15][2][3], "1011101010011100101011101" );
    sensitivity_64[15][2][0] = 0.0941333 ; // similarity = 60%, read length = 100
    sensitivity_64[15][2][1] = 0.238199 ; // similarity = 65%, read length = 100
    sensitivity_64[15][2][2] = 0.480458 ; // similarity = 70%, read length = 100
    sensitivity_64[15][2][3] = 0.75588 ; // similarity = 75%, read length = 100
    sensitivity_64[15][2][4] = 0.938473 ; // similarity = 80%, read length = 100
    sensitivity_64[15][2][5] = 0.994538 ; // similarity = 85%, read length = 100
    sensitivity_64[15][2][6] = 0.999928 ; // similarity = 90%, read length = 100
    sensitivity_64[15][2][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[15][3][0], "1101110000111110000111011" );
    strcpy(seeds_64[15][3][1], "1101011101001001011101011" );
    strcpy(seeds_64[15][3][2], "1111010011001001100101111" );
    strcpy(seeds_64[15][3][3], "1011100101101011010011101" );
    strcpy(seeds_64[15][3][4], "1110001101101011011000111" );
    strcpy(seeds_64[15][3][5], "1010001011111111101000101" );
    strcpy(seeds_64[15][3][6], "1110110100011100010110111" );
    strcpy(seeds_64[15][3][7], "1101101110001000111011011" );
    sensitivity_64[15][3][0] = 0.147791 ; // similarity = 60%, read length = 100
    sensitivity_64[15][3][1] = 0.339237 ; // similarity = 65%, read length = 100
    sensitivity_64[15][3][2] = 0.60975 ; // similarity = 70%, read length = 100
    sensitivity_64[15][3][3] = 0.853826 ; // similarity = 75%, read length = 100
    sensitivity_64[15][3][4] = 0.973599 ; // similarity = 80%, read length = 100
    sensitivity_64[15][3][5] = 0.998549 ; // similarity = 85%, read length = 100
    sensitivity_64[15][3][6] = 0.999991 ; // similarity = 90%, read length = 100
    sensitivity_64[15][3][7] = 1 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 16
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[16][0][0], "11110011010100101011001111" );
    sensitivity_64[16][0][0] = 0.01925 ; // similarity = 60%, read length = 100
    sensitivity_64[16][0][1] = 0.062994 ; // similarity = 65%, read length = 100
    sensitivity_64[16][0][2] = 0.173638 ; // similarity = 70%, read length = 100
    sensitivity_64[16][0][3] = 0.390313 ; // similarity = 75%, read length = 100
    sensitivity_64[16][0][4] = 0.685237 ; // similarity = 80%, read length = 100
    sensitivity_64[16][0][5] = 0.91751 ; // similarity = 85%, read length = 100
    sensitivity_64[16][0][6] = 0.994321 ; // similarity = 90%, read length = 100
    sensitivity_64[16][0][7] = 0.999985 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[16][1][0], "11010011001111110011001011" );
    strcpy(seeds_64[16][1][1], "11101101011000011010110111" );
    sensitivity_64[16][1][0] = 0.0345853 ; // similarity = 60%, read length = 100
    sensitivity_64[16][1][1] = 0.105755 ; // similarity = 65%, read length = 100
    sensitivity_64[16][1][2] = 0.265126 ; // similarity = 70%, read length = 100
    sensitivity_64[16][1][3] = 0.527251 ; // similarity = 75%, read length = 100
    sensitivity_64[16][1][4] = 0.806976 ; // similarity = 80%, read length = 100
    sensitivity_64[16][1][5] = 0.965149 ; // similarity = 85%, read length = 100
    sensitivity_64[16][1][6] = 0.998663 ; // similarity = 90%, read length = 100
    sensitivity_64[16][1][7] = 0.999999 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[16][2][0], "11101010110100101101010111" );
    strcpy(seeds_64[16][2][1], "11110110100011000101101111" );
    strcpy(seeds_64[16][2][2], "10111100011100111000111101" );
    strcpy(seeds_64[16][2][3], "11010011011100111011001011" );
    sensitivity_64[16][2][0] = 0.0592707 ; // similarity = 60%, read length = 100
    sensitivity_64[16][2][1] = 0.167624 ; // similarity = 65%, read length = 100
    sensitivity_64[16][2][2] = 0.37881 ; // similarity = 70%, read length = 100
    sensitivity_64[16][2][3] = 0.664298 ; // similarity = 75%, read length = 100
    sensitivity_64[16][2][4] = 0.896755 ; // similarity = 80%, read length = 100
    sensitivity_64[16][2][5] = 0.987962 ; // similarity = 85%, read length = 100
    sensitivity_64[16][2][6] = 0.999767 ; // similarity = 90%, read length = 100
    sensitivity_64[16][2][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[16][3][0], "10111100010111101000111101" );
    strcpy(seeds_64[16][3][1], "11110101001011010010101111" );
    strcpy(seeds_64[16][3][2], "11111011100000000111011111" );
    strcpy(seeds_64[16][3][3], "10101001111011011110010101" );
    strcpy(seeds_64[16][3][4], "11000011011111111011000011" );
    strcpy(seeds_64[16][3][5], "11001101110100101110110011" );
    strcpy(seeds_64[16][3][6], "10111100101100110100111101" );
    strcpy(seeds_64[16][3][7], "11001100111011011100110011" );
    sensitivity_64[16][3][0] = 0.0941363 ; // similarity = 60%, read length = 100
    sensitivity_64[16][3][1] = 0.243291 ; // similarity = 65%, read length = 100
    sensitivity_64[16][3][2] = 0.493541 ; // similarity = 70%, read length = 100
    sensitivity_64[16][3][3] = 0.771271 ; // similarity = 75%, read length = 100
    sensitivity_64[16][3][4] = 0.946357 ; // similarity = 80%, read length = 100
    sensitivity_64[16][3][5] = 0.995791 ; // similarity = 85%, read length = 100
    sensitivity_64[16][3][6] = 0.999955 ; // similarity = 90%, read length = 100
    sensitivity_64[16][3][7] = 1 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 17
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[17][0][0], "1111100110101010110011111" );
    sensitivity_64[17][0][0] = 0.0116205 ; // similarity = 60%, read length = 100
    sensitivity_64[17][0][1] = 0.0413881 ; // similarity = 65%, read length = 100
    sensitivity_64[17][0][2] = 0.12451 ; // similarity = 70%, read length = 100
    sensitivity_64[17][0][3] = 0.307536 ; // similarity = 75%, read length = 100
    sensitivity_64[17][0][4] = 0.596178 ; // similarity = 80%, read length = 100
    sensitivity_64[17][0][5] = 0.871121 ; // similarity = 85%, read length = 100
    sensitivity_64[17][0][6] = 0.987821 ; // similarity = 90%, read length = 100
    sensitivity_64[17][0][7] = 0.999942 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[17][1][0], "1110111101001001011110111" );
    strcpy(seeds_64[17][1][1], "1111010011101011100101111" );
    sensitivity_64[17][1][0] = 0.0209451 ; // similarity = 60%, read length = 100
    sensitivity_64[17][1][1] = 0.0703052 ; // similarity = 65%, read length = 100
    sensitivity_64[17][1][2] = 0.194587 ; // similarity = 70%, read length = 100
    sensitivity_64[17][1][3] = 0.429943 ; // similarity = 75%, read length = 100
    sensitivity_64[17][1][4] = 0.728855 ; // similarity = 80%, read length = 100
    sensitivity_64[17][1][5] = 0.938617 ; // similarity = 85%, read length = 100
    sensitivity_64[17][1][6] = 0.996726 ; // similarity = 90%, read length = 100
    sensitivity_64[17][1][7] = 0.999995 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[17][2][0], "1111100101101011010011111" );
    strcpy(seeds_64[17][2][1], "1110110110011100110110111" );
    strcpy(seeds_64[17][2][2], "1011111011001001101111101" );
    strcpy(seeds_64[17][2][3], "1111001110101010111001111" );
    sensitivity_64[17][2][0] = 0.035627 ; // similarity = 60%, read length = 100
    sensitivity_64[17][2][1] = 0.111493 ; // similarity = 65%, read length = 100
    sensitivity_64[17][2][2] = 0.281435 ; // similarity = 70%, read length = 100
    sensitivity_64[17][2][3] = 0.554593 ; // similarity = 75%, read length = 100
    sensitivity_64[17][2][4] = 0.830977 ; // similarity = 80%, read length = 100
    sensitivity_64[17][2][5] = 0.973262 ; // similarity = 85%, read length = 100
    sensitivity_64[17][2][6] = 0.999191 ; // similarity = 90%, read length = 100
    sensitivity_64[17][2][7] = 1 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[17][3][0], "1111010011101011100101111" );
    strcpy(seeds_64[17][3][1], "1110111001101011001110111" );
    strcpy(seeds_64[17][3][2], "1110111110001000111110111" );
    strcpy(seeds_64[17][3][3], "1100110110111110110110011" );
    strcpy(seeds_64[17][3][4], "1101110101101011010111011" );
    strcpy(seeds_64[17][3][5], "1111000111011101110001111" );
    strcpy(seeds_64[17][3][6], "1101111010011100101111011" );
    strcpy(seeds_64[17][3][7], "1111001100111110011001111" );
    sensitivity_64[17][3][0] = 0.0577282 ; // similarity = 60%, read length = 100
    sensitivity_64[17][3][1] = 0.166669 ; // similarity = 65%, read length = 100
    sensitivity_64[17][3][2] = 0.380697 ; // similarity = 70%, read length = 100
    sensitivity_64[17][3][3] = 0.669269 ; // similarity = 75%, read length = 100
    sensitivity_64[17][3][4] = 0.90078 ; // similarity = 80%, read length = 100
    sensitivity_64[17][3][5] = 0.988936 ; // similarity = 85%, read length = 100
    sensitivity_64[17][3][6] = 0.999801 ; // similarity = 90%, read length = 100
    sensitivity_64[17][3][7] = 1 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 18
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[18][0][0], "11111001101011010110011111" );
    sensitivity_64[18][0][0] = 0.00699897 ; // similarity = 60%, read length = 100
    sensitivity_64[18][0][1] = 0.0273286 ; // similarity = 65%, read length = 100
    sensitivity_64[18][0][2] = 0.0901643 ; // similarity = 70%, read length = 100
    sensitivity_64[18][0][3] = 0.244797 ; // similarity = 75%, read length = 100
    sensitivity_64[18][0][4] = 0.521634 ; // similarity = 80%, read length = 100
    sensitivity_64[18][0][5] = 0.827098 ; // similarity = 85%, read length = 100
    sensitivity_64[18][0][6] = 0.980605 ; // similarity = 90%, read length = 100
    sensitivity_64[18][0][7] = 0.999885 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[18][1][0], "11110101110100101110101111" );
    strcpy(seeds_64[18][1][1], "11101110010111101001110111" );
    sensitivity_64[18][1][0] = 0.0126989 ; // similarity = 60%, read length = 100
    sensitivity_64[18][1][1] = 0.0468371 ; // similarity = 65%, read length = 100
    sensitivity_64[18][1][2] = 0.14265 ; // similarity = 70%, read length = 100
    sensitivity_64[18][1][3] = 0.347959 ; // similarity = 75%, read length = 100
    sensitivity_64[18][1][4] = 0.650226 ; // similarity = 80%, read length = 100
    sensitivity_64[18][1][5] = 0.904639 ; // similarity = 85%, read length = 100
    sensitivity_64[18][1][6] = 0.993226 ; // similarity = 90%, read length = 100
    sensitivity_64[18][1][7] = 0.999982 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[18][2][0], "11111001011011011010011111" );
    strcpy(seeds_64[18][2][1], "11111101100011000110111111" );
    strcpy(seeds_64[18][2][2], "11001110101111110101110011" );
    strcpy(seeds_64[18][2][3], "11110011101100110111001111" );
    sensitivity_64[18][2][0] = 0.0219124 ; // similarity = 60%, read length = 100
    sensitivity_64[18][2][1] = 0.0757374 ; // similarity = 65%, read length = 100
    sensitivity_64[18][2][2] = 0.211685 ; // similarity = 70%, read length = 100
    sensitivity_64[18][2][3] = 0.462892 ; // similarity = 75%, read length = 100
    sensitivity_64[18][2][4] = 0.763483 ; // similarity = 80%, read length = 100
    sensitivity_64[18][2][5] = 0.953429 ; // similarity = 85%, read length = 100
    sensitivity_64[18][2][6] = 0.998054 ; // similarity = 90%, read length = 100
    sensitivity_64[18][2][7] = 0.999998 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[18][3][0], "11101010011111111001010111" );
    strcpy(seeds_64[18][3][1], "10111101110011001110111101" );
    strcpy(seeds_64[18][3][2], "11011110101100110101111011" );
    strcpy(seeds_64[18][3][3], "11111011011000011011011111" );
    strcpy(seeds_64[18][3][4], "11000111111011011111100011" );
    strcpy(seeds_64[18][3][5], "11110111010011001011101111" );
    strcpy(seeds_64[18][3][6], "11101100111100111100110111" );
    strcpy(seeds_64[18][3][7], "11111001101011010110011111" );
    sensitivity_64[18][3][0] = 0.0362348 ; // similarity = 60%, read length = 100
    sensitivity_64[18][3][1] = 0.116322 ; // similarity = 65%, read length = 100
    sensitivity_64[18][3][2] = 0.296052 ; // similarity = 70%, read length = 100
    sensitivity_64[18][3][3] = 0.578725 ; // similarity = 75%, read length = 100
    sensitivity_64[18][3][4] = 0.850676 ; // similarity = 80%, read length = 100
    sensitivity_64[18][3][5] = 0.979059 ; // similarity = 85%, read length = 100
    sensitivity_64[18][3][6] = 0.999487 ; // similarity = 90%, read length = 100
    sensitivity_64[18][3][7] = 1 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 19
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[19][0][0], "1111011011101011101101111" );
    sensitivity_64[19][0][0] = 0.00417094 ; // similarity = 60%, read length = 100
    sensitivity_64[19][0][1] = 0.0175629 ; // similarity = 65%, read length = 100
    sensitivity_64[19][0][2] = 0.0624823 ; // similarity = 70%, read length = 100
    sensitivity_64[19][0][3] = 0.184151 ; // similarity = 75%, read length = 100
    sensitivity_64[19][0][4] = 0.430998 ; // similarity = 80%, read length = 100
    sensitivity_64[19][0][5] = 0.754774 ; // similarity = 85%, read length = 100
    sensitivity_64[19][0][6] = 0.962454 ; // similarity = 90%, read length = 100
    sensitivity_64[19][0][7] = 0.999587 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[19][1][0], "1111110111001001110111111" );
    strcpy(seeds_64[19][1][1], "1110101101111111011010111" );
    sensitivity_64[19][1][0] = 0.00748692 ; // similarity = 60%, read length = 100
    sensitivity_64[19][1][1] = 0.0299397 ; // similarity = 65%, read length = 100
    sensitivity_64[19][1][2] = 0.0993169 ; // similarity = 70%, read length = 100
    sensitivity_64[19][1][3] = 0.266549 ; // similarity = 75%, read length = 100
    sensitivity_64[19][1][4] = 0.553509 ; // similarity = 80%, read length = 100
    sensitivity_64[19][1][5] = 0.849625 ; // similarity = 85%, read length = 100
    sensitivity_64[19][1][6] = 0.984942 ; // similarity = 90%, read length = 100
    sensitivity_64[19][1][7] = 0.999925 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[19][2][0], "1110101101111111011010111" );
    strcpy(seeds_64[19][2][1], "1111011111001001111101111" );
    strcpy(seeds_64[19][2][2], "1101111010111110101111011" );
    strcpy(seeds_64[19][2][3], "1111110011101011100111111" );
    sensitivity_64[19][2][0] = 0.0127525 ; // similarity = 60%, read length = 100
    sensitivity_64[19][2][1] = 0.0481193 ; // similarity = 65%, read length = 100
    sensitivity_64[19][2][2] = 0.148141 ; // similarity = 70%, read length = 100
    sensitivity_64[19][2][3] = 0.361336 ; // similarity = 75%, read length = 100
    sensitivity_64[19][2][4] = 0.668553 ; // similarity = 80%, read length = 100
    sensitivity_64[19][2][5] = 0.915672 ; // similarity = 85%, read length = 100
    sensitivity_64[19][2][6] = 0.994809 ; // similarity = 90%, read length = 100
    sensitivity_64[19][2][7] = 0.99999 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[19][3][0], "1111111101001001011111111" );
    strcpy(seeds_64[19][3][1], "1111001111101011111001111" );
    strcpy(seeds_64[19][3][2], "1111100111011101110011111" );
    strcpy(seeds_64[19][3][3], "1110110101111111010110111" );
    strcpy(seeds_64[19][3][4], "1110111011011101101110111" );
    strcpy(seeds_64[19][3][5], "1101111110011100111111011" );
    strcpy(seeds_64[19][3][6], "1111101001111111001011111" );
    strcpy(seeds_64[19][3][7], "1110011110111110111100111" );
    sensitivity_64[19][3][0] = 0.0209223 ; // similarity = 60%, read length = 100
    sensitivity_64[19][3][1] = 0.0739285 ; // similarity = 65%, read length = 100
    sensitivity_64[19][3][2] = 0.209602 ; // similarity = 70%, read length = 100
    sensitivity_64[19][3][3] = 0.462193 ; // similarity = 75%, read length = 100
    sensitivity_64[19][3][4] = 0.764836 ; // similarity = 80%, read length = 100
    sensitivity_64[19][3][5] = 0.954594 ; // similarity = 85%, read length = 100
    sensitivity_64[19][3][6] = 0.998187 ; // similarity = 90%, read length = 100
    sensitivity_64[19][3][7] = 0.999998 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 20
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[20][0][0], "11111011011100111011011111" );
    sensitivity_64[20][0][0] = 0.0024826 ; // similarity = 60%, read length = 100
    sensitivity_64[20][0][1] = 0.0114037 ; // similarity = 65%, read length = 100
    sensitivity_64[20][0][2] = 0.0442003 ; // similarity = 70%, read length = 100
    sensitivity_64[20][0][3] = 0.14211 ; // similarity = 75%, read length = 100
    sensitivity_64[20][0][4] = 0.363767 ; // similarity = 80%, read length = 100
    sensitivity_64[20][0][5] = 0.694958 ; // similarity = 85%, read length = 100
    sensitivity_64[20][0][6] = 0.944431 ; // similarity = 90%, read length = 100
    sensitivity_64[20][0][7] = 0.999171 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[20][1][0], "11101111101011010111110111" );
    strcpy(seeds_64[20][1][1], "11111011011100111011011111" );
    sensitivity_64[20][1][0] = 0.0044813 ; // similarity = 60%, read length = 100
    sensitivity_64[20][1][1] = 0.0196219 ; // similarity = 65%, read length = 100
    sensitivity_64[20][1][2] = 0.0712818 ; // similarity = 70%, read length = 100
    sensitivity_64[20][1][3] = 0.210023 ; // similarity = 75%, read length = 100
    sensitivity_64[20][1][4] = 0.479482 ; // similarity = 80%, read length = 100
    sensitivity_64[20][1][5] = 0.801255 ; // similarity = 85%, read length = 100
    sensitivity_64[20][1][6] = 0.976074 ; // similarity = 90%, read length = 100
    sensitivity_64[20][1][7] = 0.999843 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[20][2][0], "11101110111100111101110111" );
    strcpy(seeds_64[20][2][1], "11111101101011010110111111" );
    strcpy(seeds_64[20][2][2], "11111001011111111010011111" );
    strcpy(seeds_64[20][2][3], "11110111110011001111101111" );
    sensitivity_64[20][2][0] = 0.00777603 ; // similarity = 60%, read length = 100
    sensitivity_64[20][2][1] = 0.0322119 ; // similarity = 65%, read length = 100
    sensitivity_64[20][2][2] = 0.108846 ; // similarity = 70%, read length = 100
    sensitivity_64[20][2][3] = 0.291942 ; // similarity = 75%, read length = 100
    sensitivity_64[20][2][4] = 0.593448 ; // similarity = 80%, read length = 100
    sensitivity_64[20][2][5] = 0.878648 ; // similarity = 85%, read length = 100
    sensitivity_64[20][2][6] = 0.990429 ; // similarity = 90%, read length = 100
    sensitivity_64[20][2][7] = 0.999972 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[20][3][0], "11011111010111101011111011" );
    strcpy(seeds_64[20][3][1], "11111001101111110110011111" );
    strcpy(seeds_64[20][3][2], "10110111011111111011101101" );
    strcpy(seeds_64[20][3][3], "11111100111011011100111111" );
    strcpy(seeds_64[20][3][4], "11111010111100111101011111" );
    strcpy(seeds_64[20][3][5], "11110111110011001111101111" );
    strcpy(seeds_64[20][3][6], "11101111011100111011110111" );
    strcpy(seeds_64[20][3][7], "11111111001011010011111111" );
    sensitivity_64[20][3][0] = 0.0128346 ; // similarity = 60%, read length = 100
    sensitivity_64[20][3][1] = 0.0499343 ; // similarity = 65%, read length = 100
    sensitivity_64[20][3][2] = 0.156063 ; // similarity = 70%, read length = 100
    sensitivity_64[20][3][3] = 0.380265 ; // similarity = 75%, read length = 100
    sensitivity_64[20][3][4] = 0.692594 ; // similarity = 80%, read length = 100
    sensitivity_64[20][3][5] = 0.928103 ; // similarity = 85%, read length = 100
    sensitivity_64[20][3][6] = 0.996174 ; // similarity = 90%, read length = 100
    sensitivity_64[20][3][7] = 0.999994 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 21
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[21][0][0], "1111101111011101111011111" );
    sensitivity_64[21][0][0] = 0.0014589 ; // similarity = 60%, read length = 100
    sensitivity_64[21][0][1] = 0.00720726 ; // similarity = 65%, read length = 100
    sensitivity_64[21][0][2] = 0.0300094 ; // similarity = 70%, read length = 100
    sensitivity_64[21][0][3] = 0.104183 ; // similarity = 75%, read length = 100
    sensitivity_64[21][0][4] = 0.29131 ; // similarity = 80%, read length = 100
    sensitivity_64[21][0][5] = 0.614949 ; // similarity = 85%, read length = 100
    sensitivity_64[21][0][6] = 0.912727 ; // similarity = 90%, read length = 100
    sensitivity_64[21][0][7] = 0.998097 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[21][1][0], "1110111111101011111110111" );
    strcpy(seeds_64[21][1][1], "1111110110111110110111111" );
    sensitivity_64[21][1][0] = 0.00255022 ; // similarity = 60%, read length = 100
    sensitivity_64[21][1][1] = 0.0119914 ; // similarity = 65%, read length = 100
    sensitivity_64[21][1][2] = 0.0469303 ; // similarity = 70%, read length = 100
    sensitivity_64[21][1][3] = 0.150633 ; // similarity = 75%, read length = 100
    sensitivity_64[21][1][4] = 0.381276 ; // similarity = 80%, read length = 100
    sensitivity_64[21][1][5] = 0.714793 ; // similarity = 85%, read length = 100
    sensitivity_64[21][1][6] = 0.952215 ; // similarity = 90%, read length = 100
    sensitivity_64[21][1][7] = 0.999419 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[21][2][0], "1110110111111111110110111" );
    strcpy(seeds_64[21][2][1], "1111111110011100111111111" );
    strcpy(seeds_64[21][2][2], "1111101101111111011011111" );
    strcpy(seeds_64[21][2][3], "1111011111101011111101111" );
    sensitivity_64[21][2][0] = 0.0042468 ; // similarity = 60%, read length = 100
    sensitivity_64[21][2][1] = 0.0190024 ; // similarity = 65%, read length = 100
    sensitivity_64[21][2][2] = 0.0699843 ; // similarity = 70%, read length = 100
    sensitivity_64[21][2][3] = 0.208036 ; // similarity = 75%, read length = 100
    sensitivity_64[21][2][4] = 0.477839 ; // similarity = 80%, read length = 100
    sensitivity_64[21][2][5] = 0.801315 ; // similarity = 85%, read length = 100
    sensitivity_64[21][2][6] = 0.976548 ; // similarity = 90%, read length = 100
    sensitivity_64[21][2][7] = 0.999859 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[21][3][0], "1111110110111110110111111" );
    strcpy(seeds_64[21][3][1], "1111111101101011011111111" );
    strcpy(seeds_64[21][3][2], "1111011111101011111101111" );
    strcpy(seeds_64[21][3][3], "1110111101111111011110111" );
    strcpy(seeds_64[21][3][4], "1111101111101011111011111" );
    strcpy(seeds_64[21][3][5], "1101111111011101111111011" );
    strcpy(seeds_64[21][3][6], "1111101110111110111011111" );
    strcpy(seeds_64[21][3][7], "1111110011111111100111111" );
    sensitivity_64[21][3][0] = 0.00661944 ; // similarity = 60%, read length = 100
    sensitivity_64[21][3][1] = 0.0281922 ; // similarity = 65%, read length = 100
    sensitivity_64[21][3][2] = 0.0978036 ; // similarity = 70%, read length = 100
    sensitivity_64[21][3][3] = 0.269892 ; // similarity = 75%, read length = 100
    sensitivity_64[21][3][4] = 0.566185 ; // similarity = 80%, read length = 100
    sensitivity_64[21][3][5] = 0.863203 ; // similarity = 85%, read length = 100
    sensitivity_64[21][3][6] = 0.988314 ; // similarity = 90%, read length = 100
    sensitivity_64[21][3][7] = 0.999961 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 22
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[22][0][0], "11111101110111101110111111" );
    sensitivity_64[22][0][0] = 0.000862423 ; // similarity = 60%, read length = 100
    sensitivity_64[22][0][1] = 0.00462448 ; // similarity = 65%, read length = 100
    sensitivity_64[22][0][2] = 0.0208314 ; // similarity = 70%, read length = 100
    sensitivity_64[22][0][3] = 0.0782219 ; // similarity = 75%, read length = 100
    sensitivity_64[22][0][4] = 0.237405 ; // similarity = 80%, read length = 100
    sensitivity_64[22][0][5] = 0.546637 ; // similarity = 85%, read length = 100
    sensitivity_64[22][0][6] = 0.878451 ; // similarity = 90%, read length = 100
    sensitivity_64[22][0][7] = 0.996276 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[22][1][0], "11111110111100111101111111" );
    strcpy(seeds_64[22][1][1], "11110111111011011111101111" );
    sensitivity_64[22][1][0] = 0.00150578 ; // similarity = 60%, read length = 100
    sensitivity_64[22][1][1] = 0.00772467 ; // similarity = 65%, read length = 100
    sensitivity_64[22][1][2] = 0.0329132 ; // similarity = 70%, read length = 100
    sensitivity_64[22][1][3] = 0.115054 ; // similarity = 75%, read length = 100
    sensitivity_64[22][1][4] = 0.317989 ; // similarity = 80%, read length = 100
    sensitivity_64[22][1][5] = 0.650994 ; // similarity = 85%, read length = 100
    sensitivity_64[22][1][6] = 0.929711 ; // similarity = 90%, read length = 100
    sensitivity_64[22][1][7] = 0.998763 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[22][2][0], "11111110111011011101111111" );
    strcpy(seeds_64[22][2][1], "11111011011111111011011111" );
    strcpy(seeds_64[22][2][2], "11110111110111101111101111" );
    strcpy(seeds_64[22][2][3], "11101111111100111111110111" );
    sensitivity_64[22][2][0] = 0.0025844 ; // similarity = 60%, read length = 100
    sensitivity_64[22][2][1] = 0.0126041 ; // similarity = 65%, read length = 100
    sensitivity_64[22][2][2] = 0.0504496 ; // similarity = 70%, read length = 100
    sensitivity_64[22][2][3] = 0.163074 ; // similarity = 75%, read length = 100
    sensitivity_64[22][2][4] = 0.408457 ; // similarity = 80%, read length = 100
    sensitivity_64[22][2][5] = 0.745108 ; // similarity = 85%, read length = 100
    sensitivity_64[22][2][6] = 0.962644 ; // similarity = 90%, read length = 100
    sensitivity_64[22][2][7] = 0.999648 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[22][3][0], "11111111011011011011111111" );
    strcpy(seeds_64[22][3][1], "11111011110111101111011111" );
    strcpy(seeds_64[22][3][2], "11011111101111110111111011" );
    strcpy(seeds_64[22][3][3], "11101101111111111110110111" );
    strcpy(seeds_64[22][3][4], "11110111110111101111101111" );
    strcpy(seeds_64[22][3][5], "11111100111111111100111111" );
    strcpy(seeds_64[22][3][6], "11110111111100111111101111" );
    strcpy(seeds_64[22][3][7], "11111110111011011101111111" );
    sensitivity_64[22][3][0] = 0.00407968 ; // similarity = 60%, read length = 100
    sensitivity_64[22][3][1] = 0.0189536 ; // similarity = 65%, read length = 100
    sensitivity_64[22][3][2] = 0.0715521 ; // similarity = 70%, read length = 100
    sensitivity_64[22][3][3] = 0.215117 ; // similarity = 75%, read length = 100
    sensitivity_64[22][3][4] = 0.492837 ; // similarity = 80%, read length = 100
    sensitivity_64[22][3][5] = 0.815112 ; // similarity = 85%, read length = 100
    sensitivity_64[22][3][6] = 0.979716 ; // similarity = 90%, read length = 100
    sensitivity_64[22][3][7] = 0.999884 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 23
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[23][0][0], "1111111101111111011111111" );
    sensitivity_64[23][0][0] = 0.000465274 ; // similarity = 60%, read length = 100
    sensitivity_64[23][0][1] = 0.00267454 ; // similarity = 65%, read length = 100
    sensitivity_64[23][0][2] = 0.0129477 ; // similarity = 70%, read length = 100
    sensitivity_64[23][0][3] = 0.0526846 ; // similarity = 75%, read length = 100
    sensitivity_64[23][0][4] = 0.175896 ; // similarity = 80%, read length = 100
    sensitivity_64[23][0][5] = 0.453954 ; // similarity = 85%, read length = 100
    sensitivity_64[23][0][6] = 0.820533 ; // similarity = 90%, read length = 100
    sensitivity_64[23][0][7] = 0.992055 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[23][1][0], "1111111111011101111111111" );
    strcpy(seeds_64[23][1][1], "1111101111111111111011111" );
    sensitivity_64[23][1][0] = 0.000792638 ; // similarity = 60%, read length = 100
    sensitivity_64[23][1][1] = 0.0043694 ; // similarity = 65%, read length = 100
    sensitivity_64[23][1][2] = 0.0200762 ; // similarity = 70%, read length = 100
    sensitivity_64[23][1][3] = 0.0764964 ; // similarity = 75%, read length = 100
    sensitivity_64[23][1][4] = 0.234876 ; // similarity = 80%, read length = 100
    sensitivity_64[23][1][5] = 0.545646 ; // similarity = 85%, read length = 100
    sensitivity_64[23][1][6] = 0.880125 ; // similarity = 90%, read length = 100
    sensitivity_64[23][1][7] = 0.996622 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[23][2][0], "1111011111111111111101111" );
    strcpy(seeds_64[23][2][1], "1111111111101011111111111" );
    strcpy(seeds_64[23][2][2], "1111110111111111110111111" );
    strcpy(seeds_64[23][2][3], "1111111101111111011111111" );
    sensitivity_64[23][2][0] = 0.00125681 ; // similarity = 60%, read length = 100
    sensitivity_64[23][2][1] = 0.00659701 ; // similarity = 65%, read length = 100
    sensitivity_64[23][2][2] = 0.0286729 ; // similarity = 70%, read length = 100
    sensitivity_64[23][2][3] = 0.102439 ; // similarity = 75%, read length = 100
    sensitivity_64[23][2][4] = 0.291365 ; // similarity = 80%, read length = 100
    sensitivity_64[23][2][5] = 0.619122 ; // similarity = 85%, read length = 100
    sensitivity_64[23][2][6] = 0.916556 ; // similarity = 90%, read length = 100
    sensitivity_64[23][2][7] = 0.998349 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[23][3][0], "1111111101111111011111111" );
    strcpy(seeds_64[23][3][1], "1111110111111111110111111" );
    strcpy(seeds_64[23][3][2], "1111111111101011111111111" );
    strcpy(seeds_64[23][3][3], "1111111011111111101111111" );
    strcpy(seeds_64[23][3][4], "1110111111111111111110111" );
    strcpy(seeds_64[23][3][5], "1111111110111110111111111" );
    strcpy(seeds_64[23][3][6], "1111111111011101111111111" );
    strcpy(seeds_64[23][3][7], "1111101111111111111011111" );
    sensitivity_64[23][3][0] = 0.00186413 ; // similarity = 60%, read length = 100
    sensitivity_64[23][3][1] = 0.00938539 ; // similarity = 65%, read length = 100
    sensitivity_64[23][3][2] = 0.0389372 ; // similarity = 70%, read length = 100
    sensitivity_64[23][3][3] = 0.131698 ; // similarity = 75%, read length = 100
    sensitivity_64[23][3][4] = 0.350196 ; // similarity = 80%, read length = 100
    sensitivity_64[23][3][5] = 0.686765 ; // similarity = 85%, read length = 100
    sensitivity_64[23][3][6] = 0.943944 ; // similarity = 90%, read length = 100
    sensitivity_64[23][3][7] = 0.999247 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 24
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[24][0][0], "11111111101111110111111111" );
    sensitivity_64[24][0][0] = 0.000275755 ; // similarity = 60%, read length = 100
    sensitivity_64[24][0][1] = 0.00171983 ; // similarity = 65%, read length = 100
    sensitivity_64[24][0][2] = 0.00899388 ; // similarity = 70%, read length = 100
    sensitivity_64[24][0][3] = 0.0394467 ; // similarity = 75%, read length = 100
    sensitivity_64[24][0][4] = 0.142084 ; // similarity = 80%, read length = 100
    sensitivity_64[24][0][5] = 0.39713 ; // similarity = 85%, read length = 100
    sensitivity_64[24][0][6] = 0.777175 ; // similarity = 90%, read length = 100
    sensitivity_64[24][0][7] = 0.987387 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[24][1][0], "11111101111111111110111111" );
    strcpy(seeds_64[24][1][1], "11111111110111101111111111" );
    sensitivity_64[24][1][0] = 0.000470532 ; // similarity = 60%, read length = 100
    sensitivity_64[24][1][1] = 0.00281843 ; // similarity = 65%, read length = 100
    sensitivity_64[24][1][2] = 0.0140169 ; // similarity = 70%, read length = 100
    sensitivity_64[24][1][3] = 0.057727 ; // similarity = 75%, read length = 100
    sensitivity_64[24][1][4] = 0.191915 ; // similarity = 80%, read length = 100
    sensitivity_64[24][1][5] = 0.484456 ; // similarity = 85%, read length = 100
    sensitivity_64[24][1][6] = 0.844427 ; // similarity = 90%, read length = 100
    sensitivity_64[24][1][7] = 0.99429 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[24][2][0], "11110111111111111111101111" );
    strcpy(seeds_64[24][2][1], "11111111111011011111111111" );
    strcpy(seeds_64[24][2][2], "11111110111111111101111111" );
    strcpy(seeds_64[24][2][3], "11111111101111110111111111" );
    sensitivity_64[24][2][0] = 0.000755262 ; // similarity = 60%, read length = 100
    sensitivity_64[24][2][1] = 0.00431414 ; // similarity = 65%, read length = 100
    sensitivity_64[24][2][2] = 0.0203306 ; // similarity = 70%, read length = 100
    sensitivity_64[24][2][3] = 0.0786665 ; // similarity = 75%, read length = 100
    sensitivity_64[24][2][4] = 0.242724 ; // similarity = 80%, read length = 100
    sensitivity_64[24][2][5] = 0.560213 ; // similarity = 85%, read length = 100
    sensitivity_64[24][2][6] = 0.889854 ; // similarity = 90%, read length = 100
    sensitivity_64[24][2][7] = 0.99724 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[24][3][0], "11111110111111111101111111" );
    strcpy(seeds_64[24][3][1], "11111011111111111111011111" );
    strcpy(seeds_64[24][3][2], "11111111111100111111111111" );
    strcpy(seeds_64[24][3][3], "11111111111011011111111111" );
    strcpy(seeds_64[24][3][4], "11111111101111110111111111" );
    strcpy(seeds_64[24][3][5], "11101111111111111111110111" );
    strcpy(seeds_64[24][3][6], "11111111110111101111111111" );
    strcpy(seeds_64[24][3][7], "11111101111111111110111111" );
    sensitivity_64[24][3][0] = 0.00112754 ; // similarity = 60%, read length = 100
    sensitivity_64[24][3][1] = 0.00617913 ; // similarity = 65%, read length = 100
    sensitivity_64[24][3][2] = 0.0278056 ; // similarity = 70%, read length = 100
    sensitivity_64[24][3][3] = 0.101945 ; // similarity = 75%, read length = 100
    sensitivity_64[24][3][4] = 0.29449 ; // similarity = 80%, read length = 100
    sensitivity_64[24][3][5] = 0.627753 ; // similarity = 85%, read length = 100
    sensitivity_64[24][3][6] = 0.922399 ; // similarity = 90%, read length = 100
    sensitivity_64[24][3][7] = 0.998649 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 25
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[25][0][0], "1111111111111111111111111" );
    sensitivity_64[25][0][0] = 8.81321e-05 ; // similarity = 60%, read length = 100
    sensitivity_64[25][0][1] = 0.000572986 ; // similarity = 65%, read length = 100
    sensitivity_64[25][0][2] = 0.00314926 ; // similarity = 70%, read length = 100
    sensitivity_64[25][0][3] = 0.0148123 ; // similarity = 75%, read length = 100
    sensitivity_64[25][0][4] = 0.0596059 ; // similarity = 80%, read length = 100
    sensitivity_64[25][0][5] = 0.200377 ; // similarity = 85%, read length = 100
    sensitivity_64[25][0][6] = 0.523272 ; // similarity = 90%, read length = 100
    sensitivity_64[25][0][7] = 0.911738 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[25][1][0], "1111111111111111111111111" );
    strcpy(seeds_64[25][1][1], "1111111111111111111111111" );
    sensitivity_64[25][1][0] = 8.81321e-05 ; // similarity = 60%, read length = 100
    sensitivity_64[25][1][1] = 0.000572986 ; // similarity = 65%, read length = 100
    sensitivity_64[25][1][2] = 0.00314926 ; // similarity = 70%, read length = 100
    sensitivity_64[25][1][3] = 0.0148123 ; // similarity = 75%, read length = 100
    sensitivity_64[25][1][4] = 0.0596059 ; // similarity = 80%, read length = 100
    sensitivity_64[25][1][5] = 0.200377 ; // similarity = 85%, read length = 100
    sensitivity_64[25][1][6] = 0.523272 ; // similarity = 90%, read length = 100
    sensitivity_64[25][1][7] = 0.911738 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[25][2][0], "1111111111111111111111111" );
    strcpy(seeds_64[25][2][1], "1111111111111111111111111" );
    strcpy(seeds_64[25][2][2], "1111111111111111111111111" );
    strcpy(seeds_64[25][2][3], "1111111111111111111111111" );
    sensitivity_64[25][2][0] = 8.81321e-05 ; // similarity = 60%, read length = 100
    sensitivity_64[25][2][1] = 0.000572986 ; // similarity = 65%, read length = 100
    sensitivity_64[25][2][2] = 0.00314926 ; // similarity = 70%, read length = 100
    sensitivity_64[25][2][3] = 0.0148123 ; // similarity = 75%, read length = 100
    sensitivity_64[25][2][4] = 0.0596059 ; // similarity = 80%, read length = 100
    sensitivity_64[25][2][5] = 0.200377 ; // similarity = 85%, read length = 100
    sensitivity_64[25][2][6] = 0.523272 ; // similarity = 90%, read length = 100
    sensitivity_64[25][2][7] = 0.911738 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[25][3][0], "1111111111111111111111111" );
    strcpy(seeds_64[25][3][1], "1111111111111111111111111" );
    strcpy(seeds_64[25][3][2], "1111111111111111111111111" );
    strcpy(seeds_64[25][3][3], "1111111111111111111111111" );
    strcpy(seeds_64[25][3][4], "1111111111111111111111111" );
    strcpy(seeds_64[25][3][5], "1111111111111111111111111" );
    strcpy(seeds_64[25][3][6], "1111111111111111111111111" );
    strcpy(seeds_64[25][3][7], "1111111111111111111111111" );
    sensitivity_64[25][3][0] = 8.81321e-05 ; // similarity = 60%, read length = 100
    sensitivity_64[25][3][1] = 0.000572986 ; // similarity = 65%, read length = 100
    sensitivity_64[25][3][2] = 0.00314926 ; // similarity = 70%, read length = 100
    sensitivity_64[25][3][3] = 0.0148123 ; // similarity = 75%, read length = 100
    sensitivity_64[25][3][4] = 0.0596059 ; // similarity = 80%, read length = 100
    sensitivity_64[25][3][5] = 0.200377 ; // similarity = 85%, read length = 100
    sensitivity_64[25][3][6] = 0.523272 ; // similarity = 90%, read length = 100
    sensitivity_64[25][3][7] = 0.911738 ; // similarity = 95%, read length = 100
    // ============================
    // weight = 26
    // ============================
    // ========
    // 1 seed(s)
    strcpy(seeds_64[26][0][0], "11111111111111111111111111" );
    sensitivity_64[26][0][0] = 5.21974e-05 ; // similarity = 60%, read length = 100
    sensitivity_64[26][0][1] = 0.000367676 ; // similarity = 65%, read length = 100
    sensitivity_64[26][0][2] = 0.00217687 ; // similarity = 70%, read length = 100
    sensitivity_64[26][0][3] = 0.0109797 ; // similarity = 75%, read length = 100
    sensitivity_64[26][0][4] = 0.0472533 ; // similarity = 80%, read length = 100
    sensitivity_64[26][0][5] = 0.16995 ; // similarity = 85%, read length = 100
    sensitivity_64[26][0][6] = 0.476642 ; // similarity = 90%, read length = 100
    sensitivity_64[26][0][7] = 0.890143 ; // similarity = 95%, read length = 100
    // ========
    // 2 seed(s)
    strcpy(seeds_64[26][1][0], "11111111111111111111111111" );
    strcpy(seeds_64[26][1][1], "11111111111111111111111111" );
    sensitivity_64[26][1][0] = 5.21974e-05 ; // similarity = 60%, read length = 100
    sensitivity_64[26][1][1] = 0.000367676 ; // similarity = 65%, read length = 100
    sensitivity_64[26][1][2] = 0.00217687 ; // similarity = 70%, read length = 100
    sensitivity_64[26][1][3] = 0.0109797 ; // similarity = 75%, read length = 100
    sensitivity_64[26][1][4] = 0.0472533 ; // similarity = 80%, read length = 100
    sensitivity_64[26][1][5] = 0.16995 ; // similarity = 85%, read length = 100
    sensitivity_64[26][1][6] = 0.476642 ; // similarity = 90%, read length = 100
    sensitivity_64[26][1][7] = 0.890143 ; // similarity = 95%, read length = 100
    // ========
    // 4 seed(s)
    strcpy(seeds_64[26][2][0], "11111111111111111111111111" );
    strcpy(seeds_64[26][2][1], "11111111111111111111111111" );
    strcpy(seeds_64[26][2][2], "11111111111111111111111111" );
    strcpy(seeds_64[26][2][3], "11111111111111111111111111" );
    sensitivity_64[26][2][0] = 5.21974e-05 ; // similarity = 60%, read length = 100
    sensitivity_64[26][2][1] = 0.000367676 ; // similarity = 65%, read length = 100
    sensitivity_64[26][2][2] = 0.00217687 ; // similarity = 70%, read length = 100
    sensitivity_64[26][2][3] = 0.0109797 ; // similarity = 75%, read length = 100
    sensitivity_64[26][2][4] = 0.0472533 ; // similarity = 80%, read length = 100
    sensitivity_64[26][2][5] = 0.16995 ; // similarity = 85%, read length = 100
    sensitivity_64[26][2][6] = 0.476642 ; // similarity = 90%, read length = 100
    sensitivity_64[26][2][7] = 0.890143 ; // similarity = 95%, read length = 100
    // ========
    // 8 seed(s)
    strcpy(seeds_64[26][3][0], "11111111111111111111111111" );
    strcpy(seeds_64[26][3][1], "11111111111111111111111111" );
    strcpy(seeds_64[26][3][2], "11111111111111111111111111" );
    strcpy(seeds_64[26][3][3], "11111111111111111111111111" );
    strcpy(seeds_64[26][3][4], "11111111111111111111111111" );
    strcpy(seeds_64[26][3][5], "11111111111111111111111111" );
    strcpy(seeds_64[26][3][6], "11111111111111111111111111" );
    strcpy(seeds_64[26][3][7], "11111111111111111111111111" );
    sensitivity_64[26][3][0] = 5.21974e-05 ; // similarity = 60%, read length = 100
    sensitivity_64[26][3][1] = 0.000367676 ; // similarity = 65%, read length = 100
    sensitivity_64[26][3][2] = 0.00217687 ; // similarity = 70%, read length = 100
    sensitivity_64[26][3][3] = 0.0109797 ; // similarity = 75%, read length = 100
    sensitivity_64[26][3][4] = 0.0472533 ; // similarity = 80%, read length = 100
    sensitivity_64[26][3][5] = 0.16995 ; // similarity = 85%, read length = 100
    sensitivity_64[26][3][6] = 0.476642 ; // similarity = 90%, read length = 100
    sensitivity_64[26][3][7] = 0.890143 ; // similarity = 95%, read length = 100
    
    
    // ======================
    // return the right seed

    int k = 0;
    while ((int)pow(2,k) < numberOfSeeds)
        ++k;
    strcpy(seed, seeds_64[seedWeight][k][seedNumber]);
}
