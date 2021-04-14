/***************************************************************************
* File: programthree.cpp
* Author: Milan Gulati
* Modified: 14 April 2021
* Procedures:
* main          - generate poission distubution and determine page faults for each algorithm and working set
* LRU           - using Least Recently Used algorithm, calculate page faults for a given working set size
* FIFO          - using First in First Out algorithm, calculate page faults for a given working set size
* Clock         - using Clock algorithm, calculate page faults for a given working set size
* saveData      - save page fault averages to a csv file and print results to console
***************************************************************************/

/* Libraries */
#include <iostream>                                                         // input/output
#include <random>                                                           // poission distrubution
#include <algorithm>                                                        // vectors and find()
#include <bits/stdc++.h>                                                    // queues
#include <fstream>                                                          // write csv file

using namespace std;                                                        // declare namespace

/* Clock Element Struct */
struct clockElement
{
    int page;                                                               // page number
    int use;                                                                // use bit (0 or 1)
};

/* Function Prototypes */
void LRU(int *pageStream, int *pageFaults, int numWorkSets);                // Least Recently Used (LRU) Algorithm
void FIFO(int *pageStream, int *pageFaults, int numWorkSets);               // First In First Out (FIFO) Algorithm
void CLOCK(int *pageStream, int *pageFaults, int numWorkSets);              // Clock Algorithm
void saveData(int faults[][19], int numAlgs, int numWorkSets);              // print and save data to data.csv


/***************************************************************************
* int main(int argc, char *argv[])
* Author: Milan Gulati
* Date: 13 April 2021
* Description:  Generates poission distribution for page stream geneartion.
                Each of the 1000 experiments, page faults are found for each
                of the three algorithms for each working set size. At conclusion
                of the 1000 experiments, the average page faults for each working set
                size for each algorithm is saved into a csv file.
*
* Parameters:
*   argc    I/P     int         Number of arguments on command line
*   argv    I/P     char *[]    Arguments listed on command line
*   main    O/P     int         Status code returns 0 on successful completion
***************************************************************************/
int main(int argc, char *argv[])
{
    //int num_expr = 1000;    // 1000 experiments
    //int num_pageNum = 1000; // stream of 1000 page nums

    // 2D Array that tracks page faults for working sets size 2-20 for each algorithm
    // Rows represent:      LRU, FIFO, CLOCK
    // Columns represent:   Working Set of 2,3,4,5...,20
    // initialize to 0 for each of the  (= 3 * 19) spaces
    int faults[3][19] = {0};                                                // faults for LRU, FIFO, CLOCK (in order), for working set sizes 2-20                                              
    int pages[1000] = {0};                                                  // store 1000 generated pages

    default_random_engine gen;                                              // random number generator
    poisson_distribution<int> distribution(10);                             // poisson distribution function using lambda 10

//    for(int e = 0; e < 1000; e++)
//    {
        int faultsLRU[19] = {0};                                            // store LRU faults curr experiment
        int faultsFIFO[19] = {0};                                           // store FIFO faults curr experiment 
        int faultsCLOCK[19] = {0};                                          // store CLOCK faults curr experiment

        // Do 1000 experiments
        // to get 1000 page nums for this round
        for(int i = 0; i <= 1000; i++)
        {
            int page = distribution(gen);                                   // pass a randomly generated number into poisson function
            pages[i] = page;                                                // store new page into page stream
        }

        //int numPages = sizeof(pages)/sizeof(pages[0]);  // number of pages
        //int numPages = 1000;    // number of pages
        int numWorkSets = sizeof(faultsFIFO)/sizeof(faultsFIFO[0]);         // number of working set sizes (19 represents array containing sizes 2-20)

        // get the faults for each working set size
        for(int i = 0; i < numWorkSets; i++)
        {
            LRU(pages, faultsLRU, i);                                       // calculate LRU faults for curr working set
            FIFO(pages, faultsFIFO, i);                                     // calculate FIFO faults for curr working set
            CLOCK(pages, faultsCLOCK, i);                                   // calculate CLOCK faults for curr working set
        }

        // with the faults calculated for the current experiment
        // add the faults to the corresponding index in the 2d array to get sum
        for(int r = 0; r < 3; r++)
        {
            for(int c = 0; c < 19; c++)
            {
                if(r == 0)                                                  // LRU row
                {
                    faults[r][c] += faultsLRU[c];                           // add LRU faults for curr working set to 2d faults arr
                }
                if(r == 1)                                                  // FIFO row
                {
                    faults[r][c] += faultsFIFO[c];                          // add FIFO faults for curr working set to 2d faults arr
                }
                if(r == 2)                                                  // CLOCK row
                {
                    faults[r][c] += faultsCLOCK[c];                         // add CLOCK faults for curr working set to 2d faults arr
                }
            }
        }
//    }

    // now divide each element in the array by 1000 to get average
    // for(int r = 0; r < 3; r++)
    //     for(int c = 0; c < 19; c++)
    //         faults[r][c] = faults[r][c] / 1000;

    // now pass 2d array to generate data function to get csv file
    saveData(faults, 3, numWorkSets);

    return 0;// program completed successfully
}

/***************************************************************************
* int LRU(int *pageStream, int *faultsLRU, int setNum)
* Author: Milan Gulati
* Date: 13 April 2021
* Description:  Iterates through page stream and calculates page faults for the given
*               working set size using the Least Recently Used algorithm.
*               Updates faultsLRU array in main function each time fault encountered.
*
* Parameters:
*   pageStream  I/P     int *[]     Array of 1000 page numbers generated
*   faultsLRU   I/P     int *[]     Array of page faults for each working set size using LRU
*   setNum      I/P     int         Current set number in faultsLRU array, also determines working set size
***************************************************************************/
void LRU(int *pageStream, int *faultsLRU, int setNum)
{
    int working_size = setNum + 2;                                          // working set size is place in faults array + 2
    vector<int> set;                                                        // contains working set

    for(int i = 0; i < 1000; i++)                                           // iterate through each element in page stream
    {
        if(!(find(set.begin(), set.end(), pageStream[i]) != set.end()))     // if set does not contain desired page
        {
            if(set.size() == working_size)                                  // if set is full - replace
            {
                set.erase(set.begin());                                     // delete first element of vector
                set.push_back(pageStream[i]);                               // append new page to end
                faultsLRU[setNum]++;                                        // increment page faults
            }
            else if(set.size() < working_size)                              // else if can fit the page - append page
            {
                set.push_back(pageStream[i]);                               // add page at current index in vector
            }
        }
        else                                                                // else set already contains page - no page fault
        {
            set.erase(find(set.begin(), set.end(), pageStream[i]));         // erase page at current location
            set.push_back(pageStream[i]);                                   // put page at the end
        }
    }
}

/***************************************************************************
* int FIFO(int *pageStream, int *faultsFIFO, int setNum)
* Author: Milan Gulati
* Date: 13 April 2021
* Description:  Iterates through page stream and calculates page faults for the given
*               working set size using the First in First Out algorithm.
*               Updates faultsFIFO array in main function each time fault encountered.
*
* Parameters:
*   pageStream  I/P     int *[]     Array of 1000 page numbers generated
*   faultsFIFO  I/P     int *[]     Array of page faults for each working set size using FIFO
*   setNum      I/P     int         Current set number in faultsFIFO array, also determines working set size
***************************************************************************/
void FIFO(int *pageStream, int *faultsFIFO, int setNum)
{
    int working_size = setNum + 2;   // array is 0-19 so add 2 for 2-20
    deque<int> set; // use deque for easy traversal and FIFO removal

    for(int i = 0; i < 1000; i++)   // iterate through each page in stream
    {
        // if the queue does not have the current page number
        // else page already in queue - do nothing
        if(!(find(set.begin(), set.end(), pageStream[i]) != set.end())) // if deque does not have current page num
        {
            // if queue is full
            if(set.size() == working_size)
            {
                set.pop_front();            // remove from front of queue
                set.push_back(pageStream[i]);    // add to back of queue
                faultsFIFO[setNum]++;   // another page fault
            }
            // else if queue has room
            else if(set.size() < working_size)
            {
                set.push_back(pageStream[i]);    // add to end of deque
            }
        }
    }
}

void CLOCK(int *pageStream, int *pageFaults, int setNum)
{
    int working_size = setNum + 2;   // array is 0-19 so add 2 for 2-20
    clockElement set[working_size]; // array of clock elements to hold working set
   
    // init the set
    for(int i = 0; i < working_size; i++)
    {
        set[i].page = -1;   // init all pages to -1
        set[i].use = 0;     // init all use bits to 0
    }

    int setInd = 0;       // start at index zero of set
    int pageInd = 0;      // start at index zero of pages
    while(pageInd != 1000)   // while still pages
    {
        int currPage = pageStream[pageInd];    // get current page

        // first check if the page is in the set already and if all usebits full
        bool inSet = false;
        bool useFull = true;

        for(int c = 0; c < working_size; c++)
            if(set[c].use == 0) useFull = false;    // at least one use bit is empty

        for(int c = 0; c < working_size; c++)
        {
            // PAGE IN SET
            if(set[c].page == currPage)
            { 
                inSet = true;
                set[c].use = 1;  // reaffirm use bit as 1
                //setInd = c;   // update current set index
                break;
            }
            
            else    // ELSE PAGE NOT MATCH SET USE BIT TO ZERO AND GO TO NEXT PAGE
            {
                set[setInd].use = 0;
                //setInd = c;   // update current set index
            }

            if(setInd + 1 < working_size)
                setInd++; // increment counter if at end
            else 
                setInd = 0;  // else go to start again
            
        }

        // PAGE NOT IN SET AND ALL USE BITS FULL
        if(!inSet && useFull)
            for(int c = 0; c < working_size; c++)   // iterate over the set
                set[c].use = 0; // set all usebits to 0     

        // PAGE NOT IN SET
        if(!inSet)
        {
            while(set[setInd].use == 1)
            {
                set[setInd].use = 0;

                // go to next element in working set
                if(setInd + 1 < working_size)
                    setInd++; // increment counter if at end
                else 
                    setInd = 0;  // else go to start again
            }
            // REPLACE PAGE now use bit should be freed for current page
            if(set[setInd].use == 0)
            {
                if(set[setInd].page != -1)
                    pageFaults[setNum]++;  // page faults start being counted after first working_size number of pages loaded into set

                set[setInd].page = currPage;  // set current page to curr page
                set[setInd].use = 1;          // set use bit to 1
            }

            // go to next element in working set
            if(setInd + 1 < working_size)
                setInd++; // increment counter if at end
            else 
                setInd = 0;  // else go to start again
        }
        pageInd++; // go to next page
    }
}

// given 2d array, print to csv and to console
// when printing to csv, add row and column labels
void saveData(int faults[][19], int numAlgs, int numWorkSets)
{
    // print to console
    cout << "\n1000 Trial Average Page Faults for each Algorithm for each Working Set Size (2-20):\n" << endl;
    for(int r = 0; r < numAlgs;r++)
    {
        if(r == 0)
            cout << "LRU:\t";
        if(r == 1)
            cout << "FIFO:\t";
        if(r == 2)
            cout << "CLOCK:\t";

        for(int c = 0; c < numWorkSets;c++)
        {
            cout << faults[r][c] << " ";
        }
        cout << endl;
    }
    cout << "\nData Saved to data.csv for plotting.\n\n";

    ofstream dataFile("data.csv");      // open/create new data file

    for(int r = 0; r < numAlgs; r++)    // iterate over faults array to save to csv
    {
        for(int c = 0; c < numWorkSets; c++)
        {
            dataFile << faults[r][c];   // save index into file
            if(c != numWorkSets)        // only add comma if not at end of row
            {
                dataFile << ",";        // comma delimiter
            }
        }
        dataFile << "\n";               // next line
    }
    dataFile.close();   // close the data file
}