/***************************************************************************
* File: programthree.cpp
* Author: Milan Gulati
* Modified: 4 April 2021
* Procedures:
* main          - generate poission distubution and determine page faults for each algorithm and working set
***************************************************************************/

#include <iostream> // input/output
#include <random>   // poission distrubution

using namespace std;

/* Function Prototypes */


/***************************************************************************
* int main()
* Author: Milan Gulati
* Date: 4 April 2021
* Description:  
*
* Parameters:
*   argc    I/P     int         Number of arguments on command line
*   argv    I/P     char *[]    Arguments listed on command line
*   main    O/P     int         Status code does nothing
***************************************************************************/
int main(int argc, char *argv[])
{
    int num_expr = 1000;    // 1000 experiments
    int num_pageNum = 1000; // stream of 1000 page nums

    // 2D Array that tracks page faults for working sets size 2-20 for each algorithm
    // Rows represent:      LRU, FIFO, CLOCK
    // Columns represent:   Working Set of 2,3,4,5...,20
    // initialize to 0 for each of the 57 (= 3 * 19) spaces
    int faults[3][19] = {0};

    int pages[1000] = {0};   // Store Page Stream

    default_random_engine gen;
    poisson_distribution<int> distribution(10);

//    int p[25] = {}; // probability that the page number generated will be 0,1,2,3,4,5....25

//    for(int j = 0; j <= 1000; j++)
//    {
        /* Iterate over 1000 experiments */
        for(int i = 0; i <= num_expr; i++)
        {
            int temp = distribution(gen);

            pages[i] = temp; // store the page into temp

    //            p[temp]++;
        }

        // now that we have the page stream
        // we need to find the page faults for working sets for each of the 3 algorithms
//    }

/*
    // output distribution
    for(int i = 0; i < 25; i++)
    {
        cout << i << ": " << string(p[i]*num_pageNum/num_expr,'*') << endl;
    }
*/

    /* LRU */
    // implement working set as a vector


    /* FIFO */
    // implement working set as a vector

    /* CLOCK */
    // implement working set as a vector


    return 0;
}
