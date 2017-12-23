/** 
  * File:     hashTable.cpp 
  * 
  * Author1:  Lucian Ilie (ilie@uwo.ca) 
  * Author2:  Stephen Lu (slu93@uwo.ca) 
  * Date:     Fall 2017  
  * 
  *   This file contains code concerning the storage of 
  *   SMers and SGaps within a hashtable. If a spaced seed
  *   has an identity of 10011011, the SMer defined by this
  *   would be the integer produced by the binary representation
  *   of the bases aligned to 10011011 at the 1s. Similarly,
  *   the SGap would be the integer produced by the binary 
  *   representation of the bases aligned at the 0s.
  *
  */

#include "QUESS.h"

// prime numbers to be used as hash table sizes
const uint64_t hashTableSizes[] = {
 1769627, 1835027, 1900667, 1966127, 2031839, 2228483, 2359559, 2490707, 2621447, 2752679, 2883767, 3015527, 3145739, 3277283, 3408323, 3539267, 3670259, 3801143, 3932483, 4063559, 4456643, 4718699, 4980827, 5243003, 5505239, 5767187, 6029603, 6291563, 6553979, 6816527, 7079159, 7340639, 7602359, 7864799, 8126747, 8913119, 9437399, 9962207, 10485767, 11010383, 11534819, 12059123, 12583007, 13107923, 13631819, 14156543, 14680067, 15204467, 15729647, 16253423, 17825999, 18874379, 19923227, 20971799, 22020227, 23069447, 24117683, 25166423, 26214743, 27264047, 28312007, 29360147, 30410483, 31457627, 32505983, 35651783, 37749983, 39845987, 41943347, 44040383, 46137887, 48234623, 50331707, 52429067, 54526019, 56623367, 58720307, 60817763, 62915459, 65012279, 71303567, 75497999, 79691867, 83886983, 88080527, 92275307, 96470447, 100663439, 104858387, 109052183, 113246699, 117440699, 121635467, 125829239, 130023683, 142606379, 150994979, 159383759, 167772239, 176160779, 184549559, 192938003, 201327359, 209715719, 218104427, 226493747, 234882239, 243269639, 251659139, 260047367, 285215507, 301989959, 318767927, 335544323, 352321643, 369100463, 385876703, 402654059, 419432243, 436208447, 452986103, 469762067, 486539519, 503316623, 520094747, 570425399, 603979919, 637534763, 671089283, 704643287, 738198347, 771752363, 805307963, 838861103, 872415239, 905971007, 939525143, 973079279, 1006633283, 1040187419, 1140852767, 1207960679, 1275069143, 1342177379, 1409288183, 1476395699, 1543504343, 1610613119, 1677721667, 1744830587, 1811940419, 1879049087, 1946157419, 2013265967, 2080375127, 2281701827, 2415920939, 2550137039, 2684355383, 2818572539, 2952791147, 3087008663, 3221226167, 3355444187, 3489661079, 3623878823, 3758096939, 3892314659, 4026532187, 4160749883, 4563403379, 4831838783, 5100273923, 5368709219, 5637144743, 5905580687, 6174015503, 6442452119, 6710886467, 6979322123, 7247758307, 7516193123, 7784629079, 8053065599, 8321499203, 9126806147, 9663676523, 10200548819, 10737418883, 11274289319, 11811160139, 12348031523, 12884902223, 13421772839, 13958645543, 14495515943, 15032386163, 15569257247, 16106127887, 16642998803, 18253612127, 19327353083, 20401094843, 21474837719, 22548578579, 23622320927, 24696062387, 25769803799, 26843546243, 27917287907, 28991030759, 30064772327, 31138513067, 32212254947, 33285996803, 36507222923, 38654706323, 40802189423, 42949673423, 45097157927, 47244640319, 49392124247, 51539607599, 53687092307, 55834576979, 57982058579, 60129542339, 62277026327, 64424509847, 66571993199, 73014444299, 77309412407, 81604379243, 85899346727, 90194314103, 94489281203, 98784255863, 103079215439, 107374183703, 111669150239, 115964117999, 120259085183, 124554051983, 128849019059, 133143986399, 146028888179, 154618823603, 163208757527, 171798693719, 180388628579, 188978561207, 197568495647, 206158430447, 214748365067, 223338303719, 231928234787, 240518168603, 249108103547, 257698038539, 266287975727, 292057776239, 309237645803, 326417515547, 343597385507, 360777253763, 377957124803, 395136991499, 412316861267, 429496730879, 446676599987, 463856468987, 481036337207, 498216206387, 515396078039, 532575944723, 584115552323, 618475290887, 652835029643, 687194768879, 721554506879, 755914244627, 790273985219, 824633721383, 858993459587, 893353198763, 927712936643, 962072674643, 996432414899, 1030792152539, 1065151889507, 1168231105859, 1236950582039, 1305670059983, 1374389535587, 1443109012607, 1511828491883, 1580547965639, 1649267441747, 1717986918839, 1786706397767, 1855425872459, 1924145348627, 1992864827099, 2061584304323, 2130303780503, 2336462210183, 2473901164367, 2611340118887, 2748779070239, 2886218024939, 3023656976507, 3161095931639, 3298534883999, 3435973836983, 3573412791647, 3710851743923, 3848290698467, 3985729653707, 4123168604483, 4260607557707, 4672924419707, 4947802331663, 5222680234139, 5497558138979, 5772436047947, 6047313952943, 6322191860339, 6597069767699, 6871947674003, 7146825580703, 7421703488567, 7696581395627, 7971459304163, 8246337210659, 8521215117407, 9345848837267, 9895604651243, 10445360463947, 10995116279639, 11544872100683, 12094627906847, 12644383722779, 13194139536659, 13743895350023, 14293651161443, 14843406975659, 15393162789503, 15942918604343, 16492674420863, 17042430234443, 18691697672867, 19791209300867, 20890720927823, 21990232555703, 23089744183799, 24189255814847, 25288767440099, 26388279068903, 27487790694887, 28587302323787, 29686813951463, 30786325577867, 31885837205567, 32985348833687, 34084860462083, 37383395344739, 39582418600883, 41781441856823, 43980465111383, 46179488367203, 48378511622303, 50577534878987, 52776558134423, 54975581392583, 57174604644503, 59373627900407, 61572651156383, 63771674412287, 65970697666967, 68169720924167, 74766790688867, 79164837200927, 83562883712027, 87960930223163, 92358976733483, 96757023247427, 101155069756823, 105553116266999, 109951162779203, 114349209290003, 118747255800179, 123145302311783, 127543348823027, 131941395333479, 136339441846019, 149533581378263, 158329674402959, 167125767424739, 175921860444599, 184717953466703, 193514046490343, 202310139514283, 211106232536699, 219902325558107, 228698418578879, 237494511600287, 246290604623279, 255086697645023, 263882790666959, 272678883689987, 299067162755363, 316659348799919, 334251534845303, 351843720890723, 369435906934019, 387028092977819, 404620279022447, 422212465067447, 439804651111103, 457396837157483, 474989023199423, 492581209246163, 510173395291199, 527765581341227, 545357767379483, 598134325510343, 633318697599023, 668503069688723, 703687441776707, 738871813866287, 774056185954967, 809240558043419, 844424930134187, 879609302222207, 914793674313899, 949978046398607, 985162418489267, 1020346790579903, 1055531162666507, 1090715534754863};

/**
  * Name:             HashTable(uint64_t minRequiredSize)
  *
  * Description :     Creates a hashTable that is greater than minRequiredSize
  *
  * Input :
  *       Parameters:
  *           int     minRequiredSize       specified minimum size of hashtable
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           Hashtable hashTable           Hashtable of 'size' elements 
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                   [1]  Fetches appropriate size of hashtable using getNewSize
  *                   [2]  Allocates a new hashtable of 'size' elements
  *                   [3]  Initilialize counts to 0 and values to EMPTY (00000000 11111111 11111111 ... 11111111)
  *                   [4]  Sets sGapTable of hashtable to NULL
  *
  * Notes :           sGap table is set to NULL to conserve memory
  *
  */

 HashTable::HashTable(uint64_t minRequiredSize)
 {
    size = maxSize = getNewSize(minRequiredSize);

    cout << "New hash table of size: " << size << endl;
    numberOfElements = 0;
    sMerTable = new Element[size];
    for (uint64_t i = 0; i < size; ++i)
    {
        sMerTable[i].count = 0;
        sMerTable[i].value = EMPTY;
    }
    sGapTable = NULL;
}

/**
  * Name:               getNewSize(uint64_t minSize)
  *
  * Description :       Fetches the next greatest integer from hashTableSizes[i] that is greater than minSize
  *
  * Input :
  *       Parameters:
  *           int       minSize             specified minimum size of hashtable
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           None 
  *       Return:
  *           uint64_t                      specified size of hashtable
  *
  * Process Synopsis :
  *                     [1]  Iterates through hashTableSizes, which is an array of ascending primes
  *                     [2]  Once minSize is surpasses, will return the following prime.
  *
  * Notes :             NONE
  *
  */

uint64_t HashTable::getNewSize(uint64_t minSize)
{
    // returns the closest hashtable value to minSize that is greater than it
    uint64_t i = 0;
    for (i = 0; ((hashTableSizes[i] <= minSize) && (i < 439)); ++i)
        ;               // hashTableSizes has 440 elements
    if (i == 440)
        cerr << "Largest size for hash table is being used; something somewhere went wrong!" << endl;
    return(hashTableSizes[i]);
}

/**
  * Name:               getSize()
  *
  * Description :       Fetches the next greatest integer from hashTableSizes[i] that is greater than minSize
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
  *           uint64_t                      size of hashtable
  *
  * Process Synopsis :
  *
  * Notes :             NONE
  *
  */

uint64_t HashTable::getSize()
{
    return(size);
}

/**
  * Name:               insertSMer(uint64_t sMer)
  *
  * Description :       Inserts the sMer given by the parameter into hashTable H
  *
  * Input :
  *       Parameters:
  *           uint64_t  sMer                The sMer that is being inserted; the binary representation of the targeted 
  *                                         bases within a window of a read. Note that nucleotides have 4 variants and 
  *                                         can be represented with 2 bits. 
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           Element* sMerTable            updated hashtable with sMer inserted
  *       Return:
  *           bool                          If the hashTable load factor >0.85, returns false, false true.
  *
  * Process Synopsis :
  *                     [1]  Initialize place (will hold target insertion location of sMer former_table_value 
  *                          (the identity occupying place)
  *                     [2]  Utilizes findPos to locate a target insertion location, which will store the location in 'place'
  *                     [3]  If 'place' is not empty, will test if place contains an ambigious sMer (denoted by sMerTable[place].count==255)
  *                     [4]  If 'place' is empty, will create a new Element to insert into the table, which will contain the sMer identity and count.
  *                          The number of elements in the table is also incremented.
  *                     [5]  If the table load is greater than 0.85, will return false, otherwise will return true.
  *
  * Notes :             
  *
  */

bool HashTable::insertSMer(uint64_t sMer)
{
    bool table_not_full = true;
    bool value_found = true;
    uint64_t former_table_value = 0;
    uint64_t place = 0;
    if ((value_found = findPos(sMerTable, size, sMer, place)))          // sMer found; increment counter
    {
        former_table_value = sMerTable[place].value;
        if (sMerTable[place].count < 255)   //max count of 254, 255 reserved for ambigious SMers
            ++sMerTable[place].count;
    }
    else // sMer is new; insert in place of "EMPTY" or "REMOVED"
    {
#pragma omp master
    {
        if (numberOfElements % 50000000 == 0)
            cout << "element added: " << numberOfElements << endl;
        former_table_value = sMerTable[place].value;
        sMerTable[place].value = sMer;
        sMerTable[place].count = 1;
        ++numberOfElements;
        }
    }
    table_not_full=true;
    if (numberOfElements > 0.85 *size)
        table_not_full=false;
    return  table_not_full;
}

/**
  * Name:               findPos(Element *table, uint64_t tableSize, uint64_t sMer, uint64_t &place)
  *
  * Description :       Searches the hashTable for uint64_t sMer, and returns the location of it. If expected location
  *                     is empty, then returns false.
  *
  * Input :
  *       Parameters:
  *           Element   *table              The hashTable
  *           uint64_t  tableSize           The size of the hashTable
  *           uint64_t  sMer                The identity of the sMer being sought after
  *           uint64_t  &place              The location of the target sMer
  *
  * Output/Expected Changes :
  *       Parameters:
  *           uint64_t  &place              place will either be pointing to the location of sMer being sought after.
  *                                         If the location is EMPTY or REMOVED, the function will return false.
  *       Memory:
  *           None
  *       Return:
  *           bool                          If the sMer is found, returns true. If the sMer is not found, returns false.
  *
  * Process Synopsis :
  *                     [1]  Uses a double hashing function to determine the default location of the sMer
  *                     [2]  While the value is neither EMPTY or specified sMer, linearly probe
  *                     [3]  Returns whether or not the sMer is found
  *
  * Notes :             sMers are expected to be relatively sparse, given the relatively equal distribution of AGCTs within DNA.
  *                     The second hash function is dictated by another prime of a similar factor, as referred to by hashTableSizes.
  *
  */

bool HashTable::findPos(Element *table, uint64_t tableSize, uint64_t sMer, uint64_t &place)
{
    //finds the SMer in the hashtable using double hashing
    bool return_value = true;
    uint64_t former_table_value = 0;

    // hash function uses a large prime number close to the hashtable size
    place = sMer * getNewSize((uint64_t)(tableSize * 0.6)) % tableSize;
    int64_t firstREMOVED = -1;
    bool removedFound = false;

    uint64_t count=0;
    while ((table[place].value != EMPTY) && (table[place].value != sMer )){
        if ((!removedFound) && (table[place].value == REMOVED))
        {
            firstREMOVED = place;
            removedFound = true;
        }
        //linear probing
        place = (place + 1) % tableSize;
        count++;
        if (count>tableSize)
            break;
    }

    former_table_value = table[place].value;
    if (table[place].value == sMer)
        return_value = true;
    else
    {
        if (removedFound)
            place = firstREMOVED;
        return_value = false;
    }
    return return_value;
}

/**
  * Name:               removeSGaps(uint64_t pos)
  *
  * Description :       Given a position 'pos', remove all the sGaps associated with the sMer at 'pos'
  *
  * Input :
  *       Parameters:
  *           uint64_t  pos                 The position at the hashTable
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           Element* sMerTable            sGap table at 'pos' will be removed
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  If there is an sGapTable at 'pos', deallocate memory associated with it
  *
  * Notes :             
  *
  */

void HashTable::removeSGaps(uint64_t pos)
{
    if ((sGapTable != NULL) && (sGapTable[pos] != NULL))
    {
        delete [] sGapTable[pos];
        sGapTable[pos] = NULL;
    }
}

/**
  * Name:               rehashFrequentSMers(uint64_t Tc, uint64_t &peakMemory, uint64_t &currentMemory)
  *
  * Description :       rehashes the current hashTable into a smaller hashTable, by removing less frequently seen sMers
  *
  * Input :
  *       Parameters:
  *           uint64_t  Tc                  The count threshold to ascertain whether a specific sMer should be kept
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program
  *
  * Output/Expected Changes :
  *       Parameters:
  *           uint64_t  &peakMemory         peakMemory is recorded and estimated for testing
  *           uint64_t  &currentMemory      currentMemory is recorded and estimated for testing
  *       Memory:
  *           Element*  old                 old larger hashTable will be removed
  *           Element*  new                 new hashTable will be made of a smaller size
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Iterates through the old hashTable to find number of sMers of count >=Tc
  *                     [2]  Allocates a new hashTable of approximately 1.7 times this size
  *                     [3]  Iterates again through the old hashTable to fetch sMers of count >=Tc and adds them to the new hashTable
                        [4]  Deletes old hashTable and sets 'size' to new size
  *
  * Notes :             
  *
  */


void HashTable::rehashFrequentSMers(uint64_t Tc, uint64_t &peakMemory, uint64_t &currentMemory)
{
    // get new size ~= 1.7 x number of frequent sMers
    uint64_t oldSize=size;
    uint64_t frequentSMers = 0;
    for (uint64_t i = 0; i < size; ++i)
        if (sMerTable[i].count >= Tc)
            ++frequentSMers;
        uint64_t newSize = getNewSize((uint64_t)(1.7 * frequentSMers));
        maxSize = max(maxSize, newSize);
        cout << "Frequent sMers: " << frequentSMers << "; Hash them to size: " << newSize << endl;
        numberOfElements = frequentSMers;
        Element* newTable = new Element[newSize];
        for (uint64_t i = 0; i< newSize; ++i)
        {
            newTable[i].count = 0;
            newTable[i].value = EMPTY;
        }
        currentMemory+=newSize*sizeof(uint64_t);
        if (currentMemory > peakMemory)
        {   
            peakMemory = currentMemory;
#ifdef VERBOSE
            cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
        }
        // rehash the elements in the old table into the new one
        uint64_t place = 0;
        for (uint64_t i = 0; i < size; ++i)
        if (sMerTable[i].count >= Tc)
            {
        findPos(newTable, newSize, sMerTable[i].value, place);  // false always returned by findPos is ignored
        newTable[place].value = sMerTable[i].value; // count remains 0; will be used to count sGaps !!!
    }
    // reset the values and delete the old table
    Element* temp = sMerTable;
    delete [] temp;
    currentMemory-=oldSize*sizeof(uint64_t);
    sMerTable = newTable;
    size = newSize;
}

/**
  * Name:               createSGapTable(uint64_t &peakMemory, uint64_t &currentMemory)
  *
  * Description :       Creates the sGapTable for the corresponding sMerTable
  *
  * Input :
  *       Parameters:
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program
  *
  * Output/Expected Changes :
  *       Parameters:
  *           uint64_t  &peakMemory         peakMemory is recorded and estimated for testing
  *           uint64_t  &currentMemory      currentMemory is recorded and estimated for testing
  *       Memory:
  *           Element** sGapTable           sGapTable of size 'size' will be created and initialized to have NULL              
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Initialize sGapTable of size 'size' and sets all of its values to NULL
  *
  * Notes :             
  *
  */

void HashTable::createSGapTable(uint64_t &peakMemory, uint64_t &currentMemory)
{
        cout << "New sGapTable of size:  " << size << endl;
        sGapTable = new Element* [size];
        for (uint64_t i = 0; i < size; ++i)
            sGapTable[i] = NULL;
}

/**
  * Name:               insertSGap(uint64_t sMer, uint64_t sGap, uint64_t Te, omp_lock_t* lockArray)
  *
  * Description :       Inserts sGap into the corresponding sMer identity of sMerTable 
  *
  * Input :
  *       Parameters:
  *           uint64_t  sMer                The identity of the binary representation of the sMer
  *           uint64_t  sGap                The identity of the binary representation of the sGap
  *           uint64_t  Te                  The count threshold for how acceptable deviants from the strongest sGap are
  *           int       seedNumber          Current iteration of program
  *           omp_lock_t* lockArray         The lock array to prevent multiple access at a hashTable location
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           Element** sGapTable           The sGap may or may not be added to the table, depending on if there is sufficient
  *                                         support. If the sMer associated with this sGap is not found or ambigious, it will not
  *                                         be added. If there are too many deviants of sGaps for the specified sMer, it will also
  *                                         not be added
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Utilizes findPos to test if the sMer exists
  *                     [2]  If the sMer associated with the sGap is ambigious, then returns
  *                     [3]  If the sGap is the first, creates a new array with the sGap variants
  *                     [4]  If the sGap is already in the table, increment it, and check if there are too many variants
  *                     [5]  If the sGap is not the first, but the number of sGap variants is less than the max array size,
  *                          add the sGap into the existing array by creating and replacing the existing array
  *                     [6]  If the sGap is not the first, but there is a maximal number of sGap variants,
  *                          remove the least frequent sGap variant, as denoted by the count, and add the new one.
  *
  * Notes :             
  *
  */

void HashTable::insertSGap(uint64_t sMer, uint64_t sGap, uint64_t Te, int seedNumber, omp_lock_t* lockArray)
{
    uint64_t place = 0;
    if (!findPos(sMerTable, size, sMer, place))     // sMer not in table
        return;
    omp_set_lock(&(lockArray[place]));

    // sMer found in table at place
    if (sMerTable[place].count == 255)  // ambiguous sMer
    {
        omp_unset_lock(&(lockArray[place]));
        return;
    }
    // sMer is not ambiguous
    if (sMerTable[place].count == 0)    // sGap is the first one
    {
        sMerTable[place].count = 1;
        sGapTable[place] = new Element[1];
        sGapTable[place][0].value = sGap;
        sGapTable[place][0].count = 1;
        omp_unset_lock(&(lockArray[place]));
        return;
    }
    // there are previously found sGaps in table
    uint64_t sGapsAboveTe = 0; // count sGaps with freq >= Te; if >= 2, then remove sGaps  (ambiguous)
    bool sGapFound = false;
    uint64_t maxSearch= min((int)sMerTable[place].count,9-seedNumber);
    for (uint64_t i = 0; i < maxSearch; ++i)    //maxSearch is the maximum size of the existing SGaps of the SMer
    {
        if (sGapTable[place][i].value == sGap)
        {
            sGapFound = true;
            if (sGapTable[place][i].count < 255)
                ++sGapTable[place][i].count;
        }
        if (sGapTable[place][i].count >= Te)
        {
            ++sGapsAboveTe;
            if (sGapsAboveTe >= Te)    // amibiguous sMer (set counter to 255); remove its sGaps
            {
                sMerTable[place].count = 255;
                removeSGaps(place);
                omp_unset_lock(&(lockArray[place]));
                return;
            }
        }
    }
    if (!sGapFound)         // new sGap; reallocate all sGaps
    {
        if (sMerTable[place].count < 255)       // if count = 255, then sMer is ambiguous
        {
            ++sMerTable[place].count;
            if (sMerTable[place].count>9-seedNumber) 
            {
                int i = 0,rep=0,weakest=1;
                for (i = 0; i <9-seedNumber; ++i)
                {
                    if (sGapTable[place][i].count <=weakest)
                    {
                        rep=i;
                        weakest=sGapTable[place][i].count;
                    }
                }
                if (rep!=0){
                    sGapTable[place][rep].count = 1;
                    sGapTable[place][rep].value = sGap;
                }
            }
            else{       //not yet Ta
                Element* temp = sGapTable[place];
                sGapTable[place] = new Element[sMerTable[place].count];
                uint64_t i = 0;
                for (i = 0; i + 1 < sMerTable[place].count; ++i)
                {
                    sGapTable[place][i].count = temp[i].count;
                    sGapTable[place][i].value = temp[i].value;
                }
                sGapTable[place][i].count = 1;
                sGapTable[place][i].value = sGap;
                delete [] temp;
            }
        }
        else        // ambiguous sMer
        {
            sMerTable[place].count = 255;
            removeSGaps(place);
            omp_unset_lock(&(lockArray[place]));
            return;
        }
    }
    omp_unset_lock(&(lockArray[place]));
}

/**
  * Name:               removeAmbigSMers(uint64_t Tc, uint64_t Te, uint64_t Ta, uint64_t &peakMemory, uint64_t &currentMemory)
  *
  * Description :       Remove all ambigious sMers as well as removing all but the most frequent sGap for each sMer.
  *
  * Input :
  *       Parameters:
  *           uint64_t  Tc                  The count threshold in which an sMer/sGap is considered frequent
  *           uint64_t  Te                  The count threshold for how acceptable deviants from the strongest sGap are
  *           int       seedNumber          Current iteration of program
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program
  *
  * Output/Expected Changes :
  *       Parameters:
  *           uint64_t  &peakMemory         peakMemory is recorded and estimated for testing
  *           uint64_t  &currentMemory      currentMemory is recorded and estimated for testing
  *       Memory:
  *           Element*  sMerTable           The sMerTable will be rid of sMers with counts less than Tc.
  *                                         If the number of sGaps of any sMers exceed Te, than also get rid of that sMer.
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Iterates through the sMerTable, and if there is a sMer (it is not REMOVED/EMPTY),
  *                          check if there are too many sGaps over the Te threshold, and determines the sGap variant with
  *                          the highest count.
  *                     [2]  If there are too many sGaps over Te, mark the sMer as ambigious.
  *                     [3]  After finding the variant with the highest count, replaces the array of sGaps with this variant.
  *                          This variant is now assumed to be the correct variant if the sMer is found within the data.
  *                          This variant is given a score, as determined by how frequent it is compared to the next most frequent
  *                          sGap variant of the sMer.
  *
  * Notes :             REMOVED values are functionally the same as EMPTY.
  *
  */

void HashTable::removeAmbigSMers(uint64_t Tc, uint64_t Te, int seedNumber, uint64_t &peakMemory, uint64_t &currentMemory)
{
    uint64_t maxErrSGapCount = 0;
    uint64_t sGapsAboveTc = 0, sGapsAboveTe = 0, correctSGapPosition = 0, totalAmbigSMers = 0;
    for (uint64_t i = 0; i < size; ++i)
    {
     uint64_t maxCount=0;
        if ((sMerTable[i].value != EMPTY) && (sMerTable[i].value != REMOVED) && (sMerTable[i].count < 255)) // count = 255 means ambiguous
        {
            maxErrSGapCount = 0;
            sGapsAboveTc = sGapsAboveTe = 0;
            uint64_t maxSearch= min((int)sMerTable[i].count,9-seedNumber);
            for (uint64_t j = 0; j < maxSearch; ++j)
            {
                if (sGapTable[i][j].count >= Te)
                    ++sGapsAboveTe;
                else
                    maxErrSGapCount = max(maxErrSGapCount, sGapTable[i][j].count);
                if (sGapTable[i][j].count >= Tc)
                {
                    ++sGapsAboveTc;
                    if (sGapTable[i][j].count > maxCount){
                      correctSGapPosition = j;
                      maxCount=sGapTable[i][j].count;
                  }
              }
          }
            if ((sGapsAboveTe >= 2) || (sGapsAboveTc == 0)) // ambiguous sMer (set counter to 255); remove its sGaps
            {
                sMerTable[i].count = 255;
                removeSGaps(i);
                ++totalAmbigSMers;
                sMerTable[i].value = REMOVED;
            }
            else            // correct sMer; keep only the correct sGap (it is assumed that Tc >= Te)
            {               // store its score in sGapTable[i][0].count

                Element* temp = sGapTable[i];
                sGapTable[i] = new Element[1];
                sGapTable[i][0].count = min((int)(temp[correctSGapPosition].count / max(maxErrSGapCount,(uint64_t)1)), 255); // score
                sGapTable[i][0].value = temp[correctSGapPosition].value;
                sMerTable[i].count = 1;
                delete [] temp;
            }
        }
    }
    currentMemory-=(8-seedNumber)*size*sizeof(uint64_t);
    cout << "Ambiguous sMers removed: " << totalAmbigSMers << endl;
}

/**
  * Name:               getCorrectSGap(uint64_t sMer, uint64_t sGap, uint64_t& correctSGap, uint64_t Tdiff, uint64_t* diff16Bits)
  *
  * Description :       Given a sMer and its corresponding uncorrected sGap, checks the hashTable for the appropriate sGap and returns the
  *                     status of whether or not the sGap is correctable.
  *
  * Input :
  *       Parameters:
  *           uint64_t  sMer                The sMer of the read being processed
  *           uint64_t  sGap                The sGap of the read being processed associated with the sMer indicated
  *           uint64_t& correctSGap         A pointer to the identity of the correct sGap
  *           uint64_t  Tdiff               The allowance of how many bits can be different to have the uncorrected sGap be corrected
  *           uint64_t* diff16Bits          Precomputed array to determine the difference between the sGaps
  *
  * Output/Expected Changes :
  *       Parameters:
  *           uint64_t& correctSGap         correctSGap will change to the correct variant as indicated by the hashTable
  *       Memory:
  *           None
  *       Return:
  *           int                           -3: sMer not in table
  *                                         -2: sMer ambiguous
  *                                         -1: sGap too different (sMer is in table and NOT ambiguous)
  *                                          0: already correct
  *                                         >0: correctSGap NOT too different; correction can be done
  *
  * Process Synopsis :
  *                     [1]  Utilizes findpos to find the sMer being sought after
  *                     [2]  Returns a value equivalent to the status of the input sMer/sGap compared to the hashTable sGap
  *                     [3]  If the uncorrected sGap and the correct sGap are similar (<Tdiff) using getDiff, will proceed to correct
  *
  * Notes :             If the bits between the uncorrected and the correct sGap are too high, it is quite probable that the uncorrect sGap
  *                     is of an entirely different variant and could very well be fully correct. 
  *
  */

int64_t HashTable::getCorrectSGap(uint64_t sMer, uint64_t sGap, uint64_t& correctSGap, uint64_t Tdiff, uint64_t* diff16Bits)
{

    // find sMer
    uint64_t place = 0;
    if (!findPos(sMerTable, size, sMer, place))          // sMer not in table
        return -3;
    
    // sMer found at sMerTable[place]
    if (sMerTable[place].count == 255)      // sMer ambiguous
        return -2;
    
    // sMer not ambiguous; it has only one (correct) sGap at sGapTable[place][0] (because of "removeAmbigSMers")
    if (sGap == sGapTable[place][0].value)  // sGap is already correct
        return 0;
    
    //sGap is different from the correctSGap but must not be too different
    if (getDiff(sGap, sGapTable[place][0].value, diff16Bits) < Tdiff)
    {
        correctSGap = sGapTable[place][0].value;        // return the correct sGap in "correctSGap"
        return (int64_t)sGapTable[place][0].count;         // return also its score (from count)
    }
    else
        return -1;  // sGap too different
}

/**
  * Name:               getDiff(uint64_t x, uint64_t y, uint64_t* diff16Bits)
  *
  * Description :       Given two integers x and y, computes the difference in bits between them
  *
  * Input :
  *       Parameters:
  *           uint64_t  x                   The first integer
  *           uint64_t  y                   The second integer
  *           uint64_t* diff16Bits          Precomputed array to determine the difference between the sGaps
  *
  * Output/Expected Changes :
  *       Parameters:
  *           None
  *       Memory:
  *           None
  *       Return:
  *           int                           Number of different bits between x and y
  *
  * Process Synopsis :
  *                     [1]  Uses a mask to determin the difference between x and y in terms of bits.
  *
  * Notes :             
  *
  */

uint64_t HashTable::getDiff(uint64_t x, uint64_t y, uint64_t* diff16Bits)
{
    uint64_t nDiff = 0;
    uint64_t mask = (1 << 16) - 1;
    uint64_t xXORy = x ^ y;
    for (uint64_t i = 0; i < 4; ++i)
        nDiff += diff16Bits[(xXORy >> (16 * i)) & mask];
    return nDiff;
}


uint64_t HashTable::computeDiff16Bits(uint16_t x)
{
    uint64_t nDiff = 0;
    for (uint64_t i = 0; i < 8; ++i)
        if (((x >> (2 * i)) & 3) != 0)      // mask = 3
            ++nDiff;
        return nDiff;
}

/**
  * Name:               clear(uint64_t &peakMemory,uint64_t &currentMemory)
  *
  * Description :       Deallocate all memory from hashTable
  *
  * Input :
  *       Parameters:
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program
  *
  * Output/Expected Changes :
  *       Parameters:
  *           uint64_t  &peakMemory         peakMemory is recorded and estimated for testing
  *           uint64_t  &currentMemory      currentMemory is recorded and estimated for testing
  *       Memory:
  *           HashTable                     HashTable is fully deallocated from memory
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Iterates through the sGapTable, if there is allocated memory, deallocate it.
  *                     [2]  If there is allocated memory for the sMerTable, deallocate it.
  *
  * Notes :             
  *
  */

void HashTable::clear(uint64_t &peakMemory,uint64_t &currentMemory)
{
    cout << "Clear hash table of size:  " << size << endl;
    if (sGapTable != NULL)
    {
        for (uint64_t i = 0; i < size; ++i)
        {
            if (sGapTable[i] != NULL)
              delete [] sGapTable[i];

        }
        delete [] sGapTable;
        currentMemory-=size*sizeof(uint64_t);
    }
    if (sMerTable != NULL){
        delete [] sMerTable;
        currentMemory-=size*sizeof(uint64_t);
    }
}

/**
  * Name:               recreateOfMaxSize(uint64_t &peakMemory, uint64_t &currentMemory)
  *
  * Description :       Recreate a hashTable as big as the largest hashTable created during runtime prior
  *
  * Input :
  *       Parameters:
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program
  *
  * Output/Expected Changes :
  *       Parameters:
  *           uint64_t  &peakMemory         peakMemory is recorded and estimated for testing
  *           uint64_t  &currentMemory      currentMemory is recorded and estimated for testing
  *       Memory:
  *           HashTable                     HashTable is fully deallocated from memory
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Recalls the largest hashTable size created since start of runtime and allocates memory for it
  *
  * Notes :             
  *
  */

void HashTable::recreateOfMaxSize(uint64_t &peakMemory, uint64_t &currentMemory)
{
    size = maxSize;
    cout << "Recreate hash table of maxSize:  " << size << endl;
    // allocate memory, initialize with 0
    numberOfElements = 0;

    sMerTable = new Element[size];
    for (uint64_t i = 0; i < size; ++i)
    {
        sMerTable[i].count = 0;
        sMerTable[i].value = EMPTY;
    }
    sGapTable = NULL;

    currentMemory+=size*sizeof(uint64_t);
    if (currentMemory > peakMemory)
    {   
        peakMemory = currentMemory;
#ifdef VERBOSE
        cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
    }
}

/**
  * Name:               recreateOfDoubleSize(uint64_t &peakMemory, uint64_t &currentMemory)
  *
  * Description :       Recreate a hashTable of double size of the current hashTable
  *
  * Input :
  *       Parameters:
  *           uint64_t  &peakMemory         Peak memory of program
  *           uint64_t  &currentMemory      Current memory of program
  *
  * Output/Expected Changes :
  *       Parameters:
  *           uint64_t  &peakMemory         peakMemory is recorded and estimated for testing
  *           uint64_t  &currentMemory      currentMemory is recorded and estimated for testing
  *       Memory:
  *           HashTable                     A new HashTable of twice the original size will be created
  *       Return:
  *           None
  *
  * Process Synopsis :
  *                     [1]  Recalls the largest hashTable size created since start of runtime and allocates memory for it
  *
  * Notes :             Assumes that the old hashTable will be taken care of.
  *
  */

void HashTable::recreateOfDoubleSize(uint64_t &peakMemory, uint64_t &currentMemory)
{
    // get new size
    size = maxSize = getNewSize(2 * maxSize);
    cout << "Recreate hash table of double size:  " << size << endl;
    // allocate memory, initialize with 0
    numberOfElements = 0;
    sMerTable = new Element[size];
    for (uint64_t i = 0; i < size; ++i)
    {
        sMerTable[i].count = 0;
        sMerTable[i].value = EMPTY;
    }
    currentMemory+=size*sizeof(uint64_t);
    if (currentMemory > peakMemory)
    {   
        peakMemory = currentMemory;
#ifdef VERBOSE
        cout << "New peak Memory = " << (peakMemory/1048576) <<" MB" << endl << flush;
#endif
    }
}
