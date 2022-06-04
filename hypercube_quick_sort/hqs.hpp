#ifndef HQS_HPP
#define HQS_HPP

#include <vector>
#include <cstring>

using namespace std;

void hyperquick_sort(vector<int>& Xi, MPI_Comm comm) {
    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    //Local Sort
    sort(Xi.begin(), Xi.end());

    int dim = log(size)/log(2);
    MPI_Comm sub_comm;
    MPI_Status status;
    int hq_dim, subcube;
    int pivot, neighbor;
    int l, r, m, a, b, j;
    int send_sz, recv_sz;
    vector<int> send_list, orig_list, recv_list;
    for (int i=0; i<dim; i++) {
        //Pick pivot on P0 & bcast to all
        if (rank == 0) {
            pivot = Xi[(int)Xi.size()/2];
        }
        MPI_Bcast(&pivot, 1, MPI_INT, 0, comm);

        //Find position of pivot in your array using binary search
        l = 0;
        r = Xi.size() - 1;
        m;
        while (l < r) {
            m = l + (r - l) / 2;
            if (Xi[m] <= pivot) l = m + 1;
            else r = m;
        }

        //Exchange sizes of the sublists to be exchanged
        if (rank / (size/2) == 0) {
            neighbor = rank + (size/2);
            send_sz = Xi.size()-l-1;
            MPI_Send(&send_sz, 1, MPI_INT, neighbor, 0, comm);
            MPI_Recv(&recv_sz, 1, MPI_INT, neighbor, 1, comm, &status);
            orig_list.assign(Xi.begin(), Xi.begin()+l+1);
            send_list.assign(Xi.begin()+l+1, Xi.end());
        } else {
            neighbor = rank - (size/2);
            send_sz = l+1;
            MPI_Send(&send_sz, 1, MPI_INT, neighbor, 1, comm);
            MPI_Recv(&recv_sz, 1, MPI_INT, neighbor, 0, comm, &status);
            send_list.assign(Xi.begin(), Xi.begin()+l+1);
            orig_list.assign(Xi.begin()+l+1, Xi.end());
        }
        MPI_Barrier(comm);
        
        //Exchange the sublists
        recv_list.resize(recv_sz);
        if (rank / (size/2) == 0) {
            MPI_Send(&send_list[0], send_sz, MPI_INT, neighbor, 2, comm);
        } else {
            MPI_Recv(&recv_list[0], recv_sz, MPI_INT, neighbor, 2, comm, &status);
        }
        MPI_Barrier(comm);

        if (rank / (size/2) == 0) {
            MPI_Recv(&recv_list[0], recv_sz, MPI_INT, neighbor, 3, comm, &status);
        } else {
            MPI_Send(&send_list[0], send_sz, MPI_INT, neighbor, 3, comm);
        }
        MPI_Barrier(comm);

        //Merge low and high sublists while sorting them
        Xi.resize(recv_sz + orig_list.size());
        a = 0;
        b = 0;
        j = 0;
        for (j = 0; j < Xi.size(); j++) {
            if (recv_list[a] <= orig_list[b]) {
                Xi[j] = recv_list[a];
                a++;
                if (a == recv_sz) {
                    a = -1;
                    j++;
                    break;
                }
            } else {
                Xi[j] = orig_list[b];
                b++;
                if (b == orig_list.size()) {
                    b = -1;
                    j++;
                    break;
                }
            }
        }
        if (a == -1) {
            while (b < orig_list.size()) Xi[j++] = orig_list[b++];
        } else {
            while (a < recv_sz) Xi[j++] = recv_list[a++];
        }
        
        //Divide the cube into 2 subcubes
        hq_dim = log(size)/log(2);
        subcube = floor(rank/(int)pow(2,(hq_dim-1)));
        MPI_Barrier(comm);
        MPI_Comm_split(comm, subcube, rank, &sub_comm);
        MPI_Comm_size(sub_comm, &size);
        MPI_Comm_rank(sub_comm, &rank);
        comm = sub_comm;
    }
}
#endif // HQS_HPP
