///////////////////////////////////////////////
/*
    TODO
    - Fix LRU algorithm to not move around elemnts
    - replace 'set' name everywhere
    - 1000 experiments? or 1000 experiments for poisson distribution
*/
////


#include <iostream>         // input/output
#include <random>           // poission distrubution
#include <algorithm>        // vectors
#include <bits/stdc++.h>    // queues

using namespace std;

/* Clock Struct */
struct clockElement
{
    int page;   // store the page number
    int use;    // store the use bit (0 or 1)
};

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
    cout << CLOCK(pages, n, working_size) << " CLOCK" << endl;
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


/*
        cout << endl;
        for (int i : set)
        {
            cout << i << " ";
        }
        cout << endl;
*/
    }

    return pageFaults;
}

int CLOCK(int *pages, int n, int working_size)
{
    int pageFaults = 0;
    clockElement set[working_size]; // array of clock elements to hold working set
   
    // init the set
    for(int i = 0; i < working_size; i++)
    {
        set[i].page = -1;   // init all pages to -1
        set[i].use = 0;     // init all use bits to 0
    }

    int setInd = 0;       // start at index zero of set
    int pageInd = 0;      // start at index zero of pages
    while(pageInd != n)   // while still pages
    {
        int currPage = pages[pageInd];    // get current page

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
                    pageFaults++;  // page faults start being counted after first working_size number of pages loaded into set

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
    return pageFaults;
}

/*
        cout << endl;
        for (int a = 0; a < working_size; a++)
        {
            cout << "(" << set[a].page << " " << set[a].use << ") ";
        }
        cout << "\npageInd: " << pageInd << " setInd: " << setInd;
        cout << "\ninSet: " << inSet << " usefull: " << useFull;
        cout << endl;

        */

/*
    /// iterate over the page stream
    for(int p = 0; p < n; p++)
    {
        // iterate over the circular buffer clock
        for(int i = 0; i < working_size; i++)
        {
            // first check if the page is in the set already and if all usebits full
            bool inSet = false;
            bool useFull = true;
            for(int c = 0; c < working_size; c++)
            {
                if(set[c].page == pages[p])
                { 
                    inSet = true;
                    set[c].use = 1;
                }
                if(set[c].use == 0) useFull = false;    // at least one use bit is empty
            }

            if(inSet)   p++;    // if in set go to next page

            // executes if page not in set and all usebit full
            if(!inSet && useFull)
            {
                // set all usebits to 0 and replace current page
                for(int c = 0; c < working_size; c++)
                    set[c].use = 0;
                set[i].page = pages[p];
                set[i].use = 1;
                pageFaults++;
                cout << "page fault!" << endl;
            }
            // executes if page is not in the set
            else if(!inSet && !useFull)
            {
                // current usebit is a 1 set to zero and continue moving hand (next index)
                if(set[i].use == 1)
                {
                    set[i].use = 0;
                }
                // current usebit is 0 replace page here - page fault
                else
                { 
                    if(set[i].page != -1) pageFaults++; // if not first entries
                    set[i].use = 1;
                    set[i].page = pages[p];
                    cout << "page fault!" << endl;
                }
            }

            cout << endl;
            cout << "page looking for: " << pages[p] << endl;
            for (int a = 0; a < working_size; a++)
            {
                cout << "(" << set[a].page << " " << set[a].use << ") ";
            }
            cout << "\ninSet: " << inSet << " usefull: " << useFull;
            cout << endl;
        }
    }
*/


// function prints faults to console
// as well as saves the 2d array to a csv file data.csv
// use this data to create plots in excel
void generateData(int *faults)
{

}