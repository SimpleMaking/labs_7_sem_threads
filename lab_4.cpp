#include <iostream>
#include <string>
#include "mpi.h"

using namespace std;

int main(int argc, char** argv)
{
    char msgs[6][20] = { "you are stupid!", "fuckin pussy!", "run yo mouth!", "shut a fuck up!", "bite yo ass!", "go to my dick.."};
    srand(time(0));
    MPI_Status status;
    int rank, count = 0, random, sybs_count_husb = 0, sybs_count_wife = 0;  
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    while (count < 5)
    {
        random = rand() % 6;
        char* first_msg = msgs[random];
        random = rand() % 6;
        char* second_msg = msgs[random];
        if (rank == 0)
        {
            MPI_Send(first_msg, 20, MPI_CHAR, 1, 5, MPI_COMM_WORLD);
            MPI_Recv(second_msg, 20, MPI_CHAR, 1, 5, MPI_COMM_WORLD, &status);
           /* MPI_Probe(MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);*/
        }
        if (rank == 1)
        {
            MPI_Send(second_msg, 20, MPI_CHAR, 0, 5, MPI_COMM_WORLD);
            MPI_Recv(first_msg, 20, MPI_CHAR, 0, 5, MPI_COMM_WORLD, &status);
           /* MPI_Probe(MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);*/
        }
        if (rank == 0)
        {
            cout << "Husband" << "\n";
            cout << "sent: " << first_msg << "\n" << "received: " << second_msg << "\n\n";
            sybs_count_husb += strlen(second_msg);
        }
        else if (rank == 1)
        {
            cout << "Wife" << "\n";
            cout << "sent: " << second_msg << "\n" << "received: " << first_msg << "\n\n";
            sybs_count_wife += strlen(first_msg);
        }

        count += 1;
    }
    if (rank == 0)
    {
        cout << "symbols count from received husband msgs: " << sybs_count_husb << "\n\n";
    }
    else if (rank == 1)
    {
        cout << "symbols count from received wife msgs: " << sybs_count_wife << "\n\n";
    }
    MPI_Finalize();
    return 0;
}
