/***************************************************************************
* File: programthree.cpp
* Author: Milan Gulati
* Modified: 4 April 2021
* Procedures:
* main          - generate poission distubution and determine page faults for each algorithm and working set
***************************************************************************/

#include <iostream> // input/output
#include <random>   // poission distrubution
#include <algorithm>        // vectors
#include <bits/stdc++.h>    // queues

using namespace std;

/* Function Prototypes */
void LRU(int *pageStream, int numPages, int *pageFaults, int numWorkSets);      // Least Recently Used (LRU) Algorithm
void FIFO(int *pageStream, int numPages, int *pageFaults, int numWorkSets);     // First In First Out (FIFO) Algorithm
void CLOCK(int *pageStream, int numPages, int *pageFaults, int numWorkSets);    // CLOCK Algorithm
void generateData(int *faults);


/***************************************************************************
* int main()
* Author: Milan Gulati
* Date: 4 April 2021
* Description:  
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

//    int p[25] = {}; // probabilpageFaultsity that the page number generated will be 0,1,2,3,4,5....25

//    for(int j = 0; j <= 1000; j++)
//    {

        int faultsLRU[19] = {0};    // store LRU faults for this experiment
        int faultsFIFO[19] = {0};   // store FIFO faults for this experiment 
        int faultsCLOCK[19] = {0};  // store CLOCK faults for this experiment

        // Do 1000 experiments
        // to get 1000 page nums for this round
        for(int i = 0; i <= num_expr; i++)
        {
            int temp = distribution(gen);
            pages[i] = temp; // store the page into temp
        }
        int numPages = sizeof(pages)/sizeof(pages[0]);  // number of pages
        int numWorkSets = sizeof(faultsFIFO)/sizeof(faultsFIFO[0]);   // number of working sets for each alg to use (19)

        LRU(pages, numPages, faultsLRU, numWorkSets);   // find faults for each working set with LRU for this round
        FIFO(pages, numPages, faultsFIFO, numWorkSets); // find faults for each working set with FIFO for this round
        // find faults for each working set with CLOCK for this round

        cout << "LRU: " << endl;
        for(int c = 0; c < numWorkSets; c++) { cout << faultsLRU[c] << " ";}
        cout << "\nFIFO: " << endl;
        for(int c = 0; c < numWorkSets; c++) { cout << faultsFIFO[c] << " ";}

        // with the faults calculated for the current experiment
        // add the faults to the corresponding index in the 2d array to get sum

//    }

    // now divide each element in the array by 1000 to get average

    // now pass 2d array to generate data function to get csv file

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
                }
                // else it can fit the page
                else
                {
                    set.push_back(pageStream[i]);    // add page at current index in vector
                }

                pageFaults[w]++;   // added element so increment page faults
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
                }
                // else queue has room
                else if(set.size() < working_size)
                {
                    set.push_back(pageStream[i]);    // add to end of deque
                }

                pageFaults[w]++;   // another page fault
            }
            // else page already in queue - do nothing
        }
    }
}

void CLOCK(int *pageStream, int numPages, int *pageFaults, int numWorkSets)
{

}

// given 2d array, print to csv and to console
// when printing to csv, add row and column labels
void generateData(int *faults)
{

}



