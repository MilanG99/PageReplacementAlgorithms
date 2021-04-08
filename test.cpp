///////////////////////////////////////////////
/*
    TODO
    - Fix LRU algorithm to not move around elemnts
    - replace 'set' name everywhere
    - create clock algorithm
    - 1000 experiments? or 1000 experiments for poisson distribution
*/
////


#include <iostream>         // input/output
#include <random>           // poission distrubution
#include <algorithm>        // vectors
#include <bits/stdc++.h>    // queues

using namespace std;

int LRU(int *pages, int n, int working_size);
int FIFO(int *pages, int n, int working_size);
int CLOCK(int *pages, int n, int working_size);
void generateData(int *faults);

int main()
{
    int pages[] = {2,3,2,1,5,2,4,5,3,2,5,2};
    int n = sizeof(pages)/sizeof(pages[0]); // array length
    
    int working_size = 3; // working set size
    cout << LRU(pages, n, working_size) << " LRU" << endl;
    cout << FIFO(pages, n, working_size) << " FIFO" << endl;
    return 0;
}

int LRU(int *pages, int n, int working_size)
{
    int pageFaults = 0;
    vector<int> set;

    // iterate through each element in the page stream
    for(int i = 0; i < n; i++)
    {
        // if the set does not contain the current page num
        if(!(find(set.begin(), set.end(), pages[i]) != set.end()))
        {
            // if the set is full - replace
            if(set.size() == working_size)
            {
                set.erase(set.begin());     // delete first element of vector
                set.push_back(pages[i]);    // add page to end of the vector
                pageFaults++;   // added element so increment page faults

            }
            // else it can fit the page
            else
            {
                set.push_back(pages[i]);    // add page at current index in vector
            }
        }

        
        // else it already contains the page number - not a page fault          ----=========== FIX THIS =======================----
        else
        {
            // delete where the page is now and add it to the end
            set.erase(find(set.begin(), set.end(), pages[i]));
            set.push_back(pages[i]);
        }
        
    }
// TESTING =----------------------------------------------------------------

// end TESTING =------------------------------------------------------------

    return pageFaults;
}

int FIFO(int *pages, int n, int working_size)
{
    int pageFaults = 0;
    deque<int> set; // use deque for easy traversal and fifo removal

    // iterate through each page in stream
    for(int i = 0; i < n; i++)
    {
        // if the queue does not have the current page number
        if(!(find(set.begin(), set.end(), pages[i]) != set.end()))
        {
            // if queue is full
            if(set.size() == working_size)
            {
                set.pop_front();            // remove from front of queue
                set.push_back(pages[i]);    // add to back of queue
                pageFaults++;   // added element so increment page faults
            }
            // else queue has room
            else if(set.size() < working_size)
            {
                set.push_back(pages[i]);    // add to end of deque
            }
        }
        // else page already in queue - do nothing



        cout << endl;
        for (int i : set)
        {
            cout << i << " ";
        }
        cout << endl;
    }

    return pageFaults;
}

int CLOCK(int *pages, int n, int working_size)
{
    int pageFaults = 0;





    return pageFaults;
}


// function prints faults to console
// as well as saves the 2d array to a csv file data.csv
// use this data to create plots in excel
void generateData(int *faults)
{

}