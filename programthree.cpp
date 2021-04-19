/***************************************************************************
* File: programthree.cpp
* Author: Milan Gulati
* Modified: 14 April 2021
* Procedures:
* main          - generate poission distubution and determine page faults for each algorithm and working set
* LRU           - using Least Recently Used algorithm, calculate page faults for a given working set size
* FIFO          - using First in First Out algorithm, calculate page faults for a given working set size
* Clock         - using Clock algorithm, calculate page faults for a given working set size
* checkPage     - used by the clock algorithm to check if page already in set
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
void LRU(int *pageStream, int *faultsLRU, int numWorkSets);                 // Least Recently Used (LRU) Algorithm
void FIFO(int *pageStream, int *faultsFIFO, int numWorkSets);               // First In First Out (FIFO) Algorithm
void CLOCK(int *pageStream, int *faultsClock, int numWorkSets);             // Clock Algorithm
bool checkPage(int currPage, clockElement *set, int working_size);          // check if page is in set
void saveData(int faults[][19], int numWorkSets);                           // print and save data to data.csv

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
        int faultsClock[19] = {0};                                          // store CLOCK faults curr experiment

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
            CLOCK(pages, faultsClock, i);                                   // calculate CLOCK faults for curr working set
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
                    faults[r][c] += faultsClock[c];                         // add CLOCK faults for curr working set to 2d faults arr
                }
            }
        }
//    }

    // now divide each element in the array by 1000 to get average
    // for(int r = 0; r < 3; r++)
    //     for(int c = 0; c < 19; c++)
    //         faults[r][c] = faults[r][c] / 1000;

    // now pass 2d array to generate data function to get csv file
    saveData(faults, numWorkSets);

    return 0;// program completed successfully
}

/***************************************************************************
* void LRU(int *pageStream, int *faultsLRU, int setNum)
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
* void FIFO(int *pageStream, int *faultsFIFO, int setNum)
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
    int working_size = setNum + 2;                                          // array is 0-19 so add 2 for 2-20
    deque<int> set;                                                         // deque for easy traversal and FIFO removal

    for(int i = 0; i < 1000; i++)                                           // iterate through each page in stream
    {
        // if the queue does not have the current page number
        // else page already in queue - do nothing
        if(!(find(set.begin(), set.end(), pageStream[i]) != set.end()))     // if deque does not have current page num
        {
            if(set.size() == working_size)                                  // if deque full
            {
                set.pop_front();                                            // remove old page from front
                set.push_back(pageStream[i]);                               // add new page to back
                faultsFIFO[setNum]++;                                       // page fault
            }
            else if(set.size() < working_size)                              // else not full
            {
                set.push_back(pageStream[i]);                               // add new page to end of deque
            }
        }
    }
}


/***************************************************************************
* void CLOCK(int *pageStream, int *faultsClock, int setNum)
* Author: Milan Gulati
* Date: 13 April 2021
* Description:  Iterates through page stream and calculates page faults for the given
*               working set size using the Clock algorithm.
*               Updates faultsClock array in main function each time fault encountered.
*
* Parameters:
*   pageStream  I/P     int *[]     Array of 1000 page numbers generated
*   faultsClock I/P     int *[]     Array of page faults for each working set size using Clock
*   setNum      I/P     int         Current set number in faultsClock array, also determines working set size
***************************************************************************/
void CLOCK(int *pageStream, int *faultsClock, int setNum)
{
    int working_size = setNum + 2;                                          // add 2 for 2-20
    int setIndex = 0;                                                       // track current index in clock
    clockElement set[working_size];                                         // array of clock elements to hold working set

    for(int i = 0; i < working_size; i++)                                   // initialize the set
    {
        set[i].page = -1;                                                   // init all pages to -1 (empty)
        set[i].use = 0;                                                     // init all use bits to 0
    }

    for(int i = 0; i < 1000; i++)                                           // iterate over the 1000 pages
    {
        bool pageFound = checkPage(pageStream[i], set, working_size);       // check if page in set already
        if(!pageFound)                                                      // if not in set
        {
            while(true)                                                     // loop over buffer until page replacement found
            {
                if(set[setIndex].use == 0)                                  // if use bit zero
                {
                    if(set[setIndex].page != -1)    faultsClock[setNum]++;  // page fault (if not first page loaded into spot)
                    set[setIndex].page = pageStream[i];                     // replace page
                    set[setIndex].use = 1;                                  // set use bit 1
                    setIndex = (setIndex + 1) % working_size;               // update frame pointer
                    break;                                                  // exit loop
                }
                else                                                        // else use bit one
                {
                    set[setIndex].use = 0;                                  // set use 0
                    setIndex = (setIndex + 1) % working_size;               // update frame pointer
                }                
            }
        }
    }
}

/***************************************************************************
* bool checkPage(int currPage, clockElement *set, int working_size)
* Author: Milan Gulati
* Date: 13 April 2021
* Description:  Checks if the current page is in the set. If found
*               it sets the use bit to 1 and returns true. Otherwise
*               returns false.
*
* Parameters:
*   currPage        I/P     int                 Current page searching for
*   set             I/P     clockElement *[]    Array of clockElements containing working set
*   working_size    I/P     int                 Working size of the set
*   checkPage       O/P     bool                Flag if page found/not found
***************************************************************************/
bool checkPage(int currPage, clockElement *set, int working_size)
{
    for(int i = 0; i < working_size; i++)                                   // iterate over set
    {
        if(set[i].page == currPage)                                         // page found
        {
            set[i].use = 1;                                                 // set use bit to 1
            return true;                                                    // return true
        }
    }
    return false;                                                           // otherwise return false
}

/***************************************************************************
* void saveData(int faults[][19], int numWorkSets)
* Author: Milan Gulati
* Date: 13 April 2021
* Description:  Saves data from passed array into data.csv.
*               Also prints data to console.
*
* Parameters:
*   faults      I/P     int [][19]      Array holding average page faults for each alg for each working set size.
*   numWorkSets I/P     int             Number of working sets being tested
***************************************************************************/
void saveData(int faults[][19], int numWorkSets)
{
    cout << "\n1000 Trial Average Page Faults for each Algorithm for each Working Set Size (2-20):\n" << endl;
    for(int r = 0; r < 3;r++)                                               // iterate over each row
    {
        if(r == 0)
            cout << "LRU:\t";
        if(r == 1)
            cout << "FIFO:\t";
        if(r == 2)
            cout << "CLOCK:\t";

        for(int c = 0; c < numWorkSets; c++)                                // iterate over each column (working size)
        {
            cout << faults[r][c] << " ";                                    // print number of faults
        }
        cout << endl;
    }
    cout << "\nData Saved to data.csv for plotting.\n\n";

    ofstream dataFile("data.csv");                                          // open/create new data file

    for(int r = 0; r < 3; r++)                                              // iterate over each row (each algorithm)
    {
        for(int c = 0; c < numWorkSets; c++)                                // iterate over each column (each working size)
        {
            dataFile << faults[r][c];                                       // save index into file
            if(c != numWorkSets)                                            // only add comma if not at end of row
            {
                dataFile << ",";                                            // comma delimiter
            }
        }
        dataFile << "\n";                                                   // next line
    }
    dataFile.close();                                                       // close the data file
}