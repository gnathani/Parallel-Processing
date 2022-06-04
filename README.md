# Prallel-Processing
Assignments as part of Academic Coursework -  (OpenMP, MPI, CUDA, MapReduce)

count_sort:
The aim of the program is to sort the integers in a fixed range using counting sort. The program takes input as the number of integers to be sorted. The user has to set the number of processors (p) in MPI and the integers are in normal distribution in the range -p to p.

hypercube_quick_sort:
The aim of the program is to sort large number of integers by simulating a hypercube in MPI and using parallel quicksort on hypercube. The user has to give input as the nuumber of integers to be sorted. 

rooting_trees:
The aim of the program is to find unique roots in a forest of trees. The input has to be a text file with each line representing a parent child relationship of the form "child_id[space]parent_id". The second argument will be output file that will containg the unique root_ids as output at the end.

gaussian_kernel:
The aim of the program is to calculate the Gaussian filter on a matrix. The program takes the size of square matrix and a constant as input. 
