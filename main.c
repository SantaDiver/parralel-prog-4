/* C Example */
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm> 
 
using std::vector;
using std::cout;
using std::ofstream;
using std::endl;
using std::ifstream;
using std::ios;
using std::max;
 
typedef unsigned int uint;

double* calcByRows(uint m, uint n, vector<vector<double>> &left, vector<double> &right, int rank, int size)
{
  uint l = m*rank/size;
  uint r = m*(rank+1)/size;
  uint length = r-l+1;
  double* part = new double[length];
  
  for (uint i=l; i<r; ++i)
  {
    part[i-l] = 0;
    for(uint j=0; j<n; ++j)
    {
      part[i-l] += left[i][j] * right[j]; 
    }
  }
  
  return part;
}

double* calcByCols(uint m, uint n, vector<vector<double>> &left, vector<double> &right, int rank, int size)
{
	uint l = n*rank/size;
  uint r = n*(rank+1)/size;
  uint length = r-l+1;
  double* part = new double[m];
  
  for (uint i=0; i<m; ++i)
  {
    part[i] = 0;
    for(uint j=l; j<r; ++j)
    {
      part[i] += left[i][j] * right[j]; 
    }
  }
  
  return part;
}

int main (int argc, char* argv[])
{
	MPI_Init (&argc, &argv);      /* starts MPI */  
	
  if (argc != 4) return -1;
  
  int rank;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
  
  int size;
  MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */
  
  char leftFileName[256];
  strcpy(leftFileName, argv[1]);
  
  char rightFileName[256];
  strcpy(rightFileName, argv[2]);
  
  char outFileName[256];
  strcpy(outFileName, argv[3]);
  
  vector< vector<double> > left;
  ifstream leftFile(leftFileName);
  
  uint m;
  uint n;
  
  if (leftFile.is_open())
  {
    leftFile >> m;
    leftFile >> n;
    
    double cur;
    for(uint i=0; i<m; ++i)
    {
    	left.push_back(vector<double>());
    	for (uint j=0; j<n; ++j)
    	{
    		leftFile >> cur;
    		left[i].push_back(cur);
    	}
    }
    leftFile.close();
  }
  else
  {
  	return -1;
  }
  
  vector<double> right;
  ifstream rightFile(rightFileName);
  
  if (rightFile.is_open())
  {
    uint n2;
    rightFile >> n2;
    
    if (n2 != n) return -1;
    rightFile >> n2;
    
    double cur;
  	for (uint j=0; j<n; ++j)
  	{
  		rightFile >> cur;
  		right.push_back(cur);
  	}
    rightFile.close();
  }
  else
  {
  	return -1;
  }
  
  if (rank == 0) 
  {
    double begin=MPI_Wtime();
    
    double *part;
    if (m > n)
    {
      part = calcByRows(m, n, left, right, rank, size);
    }
  	else
  	{
      part = calcByCols(m, n, left, right, rank, size);
  	}
    
    double end=MPI_Wtime();
    
    MPI_Status stat;
    
    // double* result = new double[m];
    vector<double> result(m);
    
    if (m > n)
    {
      for(uint i=0; i<m/size; ++i)
      {
        result[i] = part[i];
      }
      uint maxim = 0;
      for (uint i=1; i<size; ++i)
  		{
  		  uint l = m*i/size;
	      uint r = m*(i+1)/size;
	      uint length = r-l;
        double* recieved_part = new double[length];

  			MPI_Recv(recieved_part, length, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &stat);
  			
  			for (uint j=0; j<length; ++j)
  			{
  			  result[j+l] = recieved_part[j];
  			}
  			
  			delete[] recieved_part;
  		}  
    }
    else
    {
			for (uint j=0; j<m; ++j)
			{
			  result[j] = part[j];
			}
      for (uint i=1; i<size; ++i)
  		{
        double* recieved_part = new double[m];

  			MPI_Recv(recieved_part, m, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &stat);
  			for (uint j=0; j<m; ++j)
  			{
  			  result[j] += recieved_part[j];
  			}
  			
  			delete[] recieved_part;
  		} 
      
    }
    delete[] part;
  
    ofstream outputFile;
    outputFile.open(outFileName, ios::out | ios::binary);
    outputFile << m << " ";
    for (uint j=0; j<m; ++j)
		{
      outputFile << result[j] << " ";
		}
		outputFile.close();
  }
  else
  {
    double begin=MPI_Wtime();
    
    double *part;
    if (m > n)
    {
      part = calcByRows(m, n, left, right, rank, size);
    }
  	else
  	{
      part = calcByCols(m, n, left, right, rank, size);
  	}
  	uint l = m*rank/size;
    uint r = m*(rank+1)/size;
    uint length = r-l;
  	MPI_Send(part, length, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    delete part;
  	
  	double end=MPI_Wtime();
  }

  MPI_Finalize();
  return 0;
}