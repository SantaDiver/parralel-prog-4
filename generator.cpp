#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <cstring>

using namespace std;

typedef unsigned int uint;

#define ii for(uint i=0; i<left.getRows(); ++i) { 
#define jj for(uint j=0; j<right.getCols(); ++j) { 
#define kk for(uint k=0; k<left.getCols(); ++k) {
#define matrix }}}
#define calc result.addToElem(left, right, i, j, k);

#define FORALLELEMS for(uint i=0; i<rows; ++i) { for(uint j=0; j<cols; ++j) {
#define EXEC }}

class Matrix 
{
    vector< vector <double> > values;
    uint cols;
    uint rows;
public:
    Matrix(uint nrows, uint ncols , double fMin, double fMax)
    {
        cols = ncols;
        rows = nrows;
        FORALLELEMS
            if (j == 0) values.push_back(vector<double>());
            values[i].push_back(0);
        EXEC
        
        fillRandElems(fMin, fMax);
    }
    
    Matrix(string path)
    {
        ifstream matrixFileStream(path.c_str());
        matrixFileStream >> rows;
        matrixFileStream >> cols;
        
        FORALLELEMS
                double readValue;
                matrixFileStream >> readValue;
                if (j == 0) values.push_back(vector<double>());
                values[i].push_back(readValue);
        EXEC
    }
    
    double operator() (uint i, uint j) const
    {
        if (i < 0 || j < 0 || i > rows || j > cols)
            throw "Некорректные значения индексов!";
        else return values[i][j];
    }
    
    void addToElem(Matrix left, Matrix right, uint i, uint j, uint k)
    {
        values[i][j] += left(i, k)*right(k, j);
    }
    
    inline uint getCols() const
    {
        return cols;
    }
    
    inline uint getRows() const
    {
        return rows;
    }
    
    void writeToBinaryFile(string path) const
    {
        ofstream outputFile;
        outputFile.open(path.c_str(), ios::out | ios::binary);
        outputFile << rows << " ";
        outputFile << cols << " ";
        FORALLELEMS
            outputFile << values[i][j] << " ";
        EXEC
    }
    
    void print() const
    {
        FORALLELEMS
            cout << values[i][j] << "  ";
            if (j == cols-1) cout << endl;
        EXEC
    }
    
    void fillRandElems(double fMin, double fMax)
    {
        srand(time(0));
        FORALLELEMS
            double f = (double)rand() / RAND_MAX;
            values[i][j] = fMin + f * (fMax - fMin);
        EXEC
    }
    
    
};

int main(int argc, char **argv)
{
    if (argc == 4)
    {
        char fileName[256];
        strcpy(fileName, argv[1]);
        
        int k = atoi(argv[2]);
        int n = atoi(argv[3]);
        
        Matrix m(k, n, -1000, 1000);
        m.writeToBinaryFile(fileName);
    }
    else if (argc == 3)
    {
        char fileName[256];
        strcpy(fileName, argv[1]);
        
        int k = atoi(argv[2]);
        
        Matrix m(k, 1, -1000, 1000);
        m.writeToBinaryFile(fileName);
    }
    else
    {
        return -1;
    }
    return 0;
}