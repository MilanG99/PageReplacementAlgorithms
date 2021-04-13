/***************************************************************************
* File: programthree.cpp
* Author: Milan Gulati
* Modified: 8 April 2021
* Procedures:
* main          - generate poission distubution and determine page faults for each algorithm and working set
***************************************************************************/

/* Libraries */
#include <iostream> // input/output
#include <random>   // poission distrubution
#include <algorithm>        // vectors and find()
#include <bits/stdc++.h>    // queues
#include <fstream>  // write csv file

using namespace std;

/* Clock Struct */
struct clockElement
{
    int page;   // store the page number
    int use;    // store the use bit (0 or 1)
};

/* Function Prototypes */
void LRU(int *pageStream, int numPages, int *pageFaults, int numWorkSets);      // Least Recently Used (LRU) Algorithm
void FIFO(int *pageStream, int numPages, int *pageFaults, int numWorkSets);     // First In First Out (FIFO) Algorithm
void CLOCK(int *pageStream, int numPages, int *pageFaults, int numWorkSets);    // CLOCK Algorithm
void generateData(int faults[][19], int numAlgs, int numWorkSets);  // print and save data to data.csv


/***************************************************************************
* int main()
* Author: Milan Gulati
* Date: 8 April 2021
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
    int num_expr = 1000;    // 1000 experiments
    int num_pageNum = 1000; // stream of 1000 page nums

    // 2D Array that tracks page faults for working sets size 2-20 for each algorithm
    // Rows represent:      LRU, FIFO, CLOCK
    // Columns represent:   Working Set of 2,3,4,5...,20
    // initialize to 0 for each of the  (= 3 * 19) spaces
    int faults[3][19] = {0};

    int pages[1000] = {0};   // Store Page Stream

    default_random_engine gen;
    poisson_distribution<int> distribution(10);

//    for(int j = 0; j <= 1000; j++)
//    {

        int faultsLRU[19] = {0};    // store LRU faults for this experiment
        int faultsFIFO[19] = {0};   // store FIFO faults for this experiment 
        int faultsCLOCK[19] = {0};  // store CLOCK faults for this experiment

        // Do 1000 experiments
        // to get 1000 page nums for this round
        for(int i = 0; i <= num_pageNum; i++)
        {
            int temp = distribution(gen);
            pages[i] = temp; // store the page into temp
        }

        int numPages = sizeof(pages)/sizeof(pages[0]);  // number of pages
        int numWorkSets = sizeof(faultsFIFO)/sizeof(faultsFIFO[0]);   // number of working sets for each alg to use (19)

        LRU(pages, numPages, faultsLRU, numWorkSets);   // find faults for each working set with LRU for this round
        FIFO(pages, numPages, faultsFIFO, numWorkSets); // find faults for each working set with FIFO for this round
        CLOCK(pages, numPages, faultsCLOCK, numWorkSets);// find faults for each working set with CLOCK for this round

        // with the faults calculated for the current experiment
        // add the faults to the corresponding index in the 2d array to get sum
        for(int r = 0; r < 3; r++)
        {
            for(int c = 0; c < 19; c++)
            {
                if(r == 0)  // LRU row
                {
                    faults[r][c] += faultsLRU[c];   // add LRU faults for that working set to 2d array
                }
                if(r == 1)  // FIFO row
                {
                    faults[r][c] += faultsFIFO[c];  // add FIFO faults for that working set to 2d array
                }
                if(r == 2)  // CLOCK row
                {
                    faults[r][c] += faultsCLOCK[c]; // add CLOCK faults for that working set to 2d array
                }
            }
        }
//    }

    // now divide each element in the array by 1000 to get average

    // now pass 2d array to generate data function to get csv file
    generateData(faults, 3, numWorkSets);

    return 0;   // return 0 on successfully completing program
}

void LRU(int *pageStream, int numPages, int *pageFaults, int numWorkSets)
{
    for(int w = 0; w < numWorkSets; w++)
    {
        int working_size = w + 2;
        vector<int> set;

        // iterate through each element in the page stream
        for(int i = 0; i < numPages; i++)
        {
            // if the set does not contain the current page num
            if(!(find(set.begin(), set.end(), pageStream[i]) != set.end()))
            {
                // if the set is full - replace
                if(set.size() == working_size)
                {
                    set.erase(set.begin());     // delete first element of vector
                    set.push_back(pageStream[i]);    // add page to end of the vector
                    pageFaults[w]++;   // added element so increment page faults
                }
                // else if it can fit the page
                else if(set.size() < working_size)
                {
                    set.push_back(pageStream[i]);    // add page at current index in vector
                }
            }

            // else it already contains the page number - not a page fault          ----=========== FIX THIS =======================----
            else
            {
                // delete where the page is now and add it to the end
                set.erase(find(set.begin(), set.end(), pageStream[i]));
                set.push_back(pageStream[i]);
            }
        }
    }
}

void FIFO(int *pageStream, int numPages, int *pageFaults, int numWorkSets)
{
    for(int w = 0; w < numWorkSets; w++)
    {
        int working_size = w + 2;   // array is 0-19 so add 2 for 2-20
        deque<int> set; // use deque for easy traversal and fifo removal

        // iterate through each page in stream
        for(int i = 0; i < numPages; i++)
        {
            // if the queue does not have the current page number
            if(!(find(set.begin(), set.end(), pageStream[i]) != set.end()))
            {
                // if queue is full
                if(set.size() == working_size)
                {
                    set.pop_front();            // remove from front of queue
                    set.push_back(pageStream[i]);    // add to back of queue
                    pageFaults[w]++;   // another page fault
                }
                // else if queue has room
                else if(set.size() < working_size)
                {
                    set.push_back(pageStream[i]);    // add to end of deque
                }
            }
            // else page already in queue - do nothing
        }
    }
}

void CLOCK(int *pageStream, int numPages, int *pageFaults, int numWorkSets)
{
    for(int w = 0; w < numWorkSets; w++)
    {

    }
}

        /*
        int working_size = w + 2;   // array is 0-19 so add 2 for 2-20
        clockElement set[working_size]; // array of clock elements to hold working set
        for(int i = 0; i < working_size; i++)
        {
            set[i].page = -1;   // init all pages to -1
            set[i].use = 0;     // init all use bits to 0
        }

        // at this point we have page stream array
        // at this point we have an initialized buffer

        for(int i = 0; i < working_size; i++)
        {
            for(int p = 0; p < numPages; p++)
            {
                if((set[i].page == pageStream[p]) && (set[i].use == 1))
                {
                    // do nothing
                }
                else if((set[i].page == pageStream[p]) && (set[i].use == 0))
                {
                    // is page trying to add and usebit zero, set use bit to 1 and continue to next page
                    set[i].use = 1;
                }
                else if((set[i].page != pageStream[p]) && (set[i].use == 1))
                {
                    // not page trying to add and use bit one, set use bit to 0 and continue
                    set[i].use = 0;
                }
                else if((set[i].page != pageStream[p]) && (set[i].use == 0))
                {
                    // not page trying to add and use bit is zero, replace the page and set use bit to one
                    set[i].page = pageStream[p];
                    set[i].use = 1;
                    pageFaults[w]++;    // page fault occured
                }
            }
        }
        */

// given 2d array, print to csv and to console
// when printing to csv, add row and column labels
void generateData(int faults[][19], int numAlgs, int numWorkSets)
{
    // print to console
    cout << "\nAverage Page Faults for each Algorithm for each Working Set Size (2-20):\n" << endl;
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

    for(int r = 0; r < numAlgs; r++)    // iterate over faults array to save
    {
        for(int c = 0; c < numWorkSets; c++)
        {
            dataFile << faults[r][c];   // save index into file
            if(c != numWorkSets)        // only add comma if not at end of row
            {
                dataFile << ",";
            }
        }
        dataFile << "\n";
    }

    dataFile.close();   // close the data file
}