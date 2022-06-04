/*  Gaurav Laxmandas
 *  Nathani
 *  gauravla
 */

#ifndef A1_HPP
#define A1_HPP

#include <vector>
#include <cstring>

void isort(std::vector<short int>& Xi, MPI_Comm comm) {
    /*
     * Initializations
     * count  : counter for each value on each processing element
     * fcount : total count of each value 
     * lcount : getting final count of values on the processors
     */
    int rank, size;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
    std::vector<long long int> count((size*2)-1);
    std::vector<long long int> lcount(2);
    std::vector<long long int> fcount((size*2)-1);
    /*
     * Take frequency count on each and reduce it on p 0
     */
    for (auto i = Xi.begin(); i != Xi.end(); i++) {
        count[*i+size-1]+=1;
    }
    MPI_Barrier(comm);
    MPI_Reduce(&count[0],&fcount[0],(size*2)-1,MPI_LONG_LONG,MPI_SUM,0,comm);
    /*
     * vector swaps used to free memory in case of memory crunch
     */
    std::vector<long long int>().swap(count);
    /*
     * Distribute final counts to each processor
     */
    MPI_Scatter(&fcount[0],2,MPI_LONG_LONG,&lcount[0],2,MPI_LONG_LONG,0,comm);
    std::vector<long long int>().swap(fcount);
    /*
     * resize vector and fill respective keys
     */
    Xi.resize(lcount[0]+lcount[1]);
    std::fill(&Xi[0],&Xi[lcount[0]],1+(2*rank)-size);
    std::fill(&Xi[lcount[0]],&Xi[lcount[0]+lcount[1]],2+(2*rank)-size);
} // isort

#endif // A1_HPP
