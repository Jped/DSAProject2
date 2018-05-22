// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, SPRING 2018

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <array>
using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
  unsigned int val1;
  unsigned int val2;
  char val3;
  string val4;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

  ifstream input(filename.c_str());
  if (!input) {
    cerr << "Error: could not open " << filename << endl;
    exit(1);
  }

  // The first line indicates the size
  string line;
  getline(input, line);
  stringstream ss(line);
  int size;
  ss >> size;

  // Load the data
  for (int i = 0; i < size; i++) {
    getline(input, line);
    stringstream ss2(line);
    Data *pData = new Data();
    ss2 >> pData->val1 >> pData->val2 >> pData->val3 >> pData->val4;
    l.push_back(pData);
  }

  input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

  ofstream output(filename.c_str());
  if (!output) {
    cerr << "Error: could not open " << filename << endl;
    exit(1);
  }

  // Write the size first
  int size = l.size();
  output << size << endl;

  // Write the data
  for (list<Data *>::const_iterator ipD = l.begin(); ipD != l.end(); ipD++) {
    output << (*ipD)->val1 << " "
	   << (*ipD)->val2 << " "
	   << (*ipD)->val3 << " "
	   << (*ipD)->val4 << endl;
  }

  output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &, int field);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
  string filename;
  cout << "Enter name of input file: ";
  cin >> filename;
  list<Data *> theList;
  loadDataList(theList, filename);

  cout << "Data loaded.  Executing sort..." << endl;

  int field = 0;
  cout << "Enter an integer from 1 - 4, representing field to sort: ";
  try {
    cin >> field;
    if (field < 1 || field > 4) {
      cerr << "Error: invalid field" << endl;
      exit(1);
    }
  }
  catch (...) {
    cerr << "Error: invalid field" << endl;
    exit(1);
  }

  clock_t t1 = clock();
  sortDataList(theList, field);
  clock_t t2 = clock();
  double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

  cout << "Sort finished. CPU time was " << timeDiff << " seconds." << endl;

  cout << "Enter name of output file: ";
  cin >> filename;
  writeDataList(theList, filename);

  return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.
array<array<Data *,20000>,127>bins;
array<array<Data *,100>,65536>bins2;
array<array<Data *,100>,65536>bins22;
int counters[127] = { 0 };
int counters2[65536] = { 0 };
int counters22[65536] = { 0 };
int counters4[65536] = { 0 };
int counters42[65536] = { 0 };
int counters43[65536] = { 0 };
array<array<Data *,250>,65536>bins4;
array<array<Data *,250>,65536>bins42;

list<Data *>::iterator it;
int val;
int maxCount;
int curCount;
Data *pval;
unsigned int x;
list<Data *>::iterator j;

void sortDataList(list<Data *> &l, int field) {
  // Fill in this function
  //see if the for loop is better than the while loop...
  list<Data *>::iterator begin = l.begin();
  list<Data *>::iterator end = l.end();
  if (field == 1) {
    it    = next(begin);
    begin--;
    for(it;it!=end;it++){
      x = (*it) ->val1;
      j = prev(it);
      while (j!=begin && ((*j)->val1)>x){
        swap(*j,*next(j));
        j--;
      }
   }
 } else if(field ==2){
    for(it=begin;it!=end;it++){
      val  = ((*it)->val2) & 0xffff;
      bins2[val][counters2[val]] = *it;
      counters2[val]++;
    }
    for(int i= 0; i<65536;i++){
      maxCount = counters2[i];
      for(curCount=0;curCount<maxCount;++curCount){
        val = ((bins2[i][curCount]->val2) >> 16) & 0xffff;
        bins22[val][counters22[val]] = bins2[i][curCount];
        counters22[val]++;
      }
    }
    it = l.begin();
    for(int i= 0; i<65536;i++){
      maxCount = counters22[i];
      for(curCount=0;curCount<maxCount;curCount++){
        (*it) = bins22[i][curCount];
        it++;
      }
    }
 } else if(field == 3) {

      for(it=begin;it!=end;it++){
        val = (int)((*it)->val3);
        bins[val][counters[val]] = *it;
        counters[val]++;
      }
      it = begin;
      for(int i=0;i<127;i++){
        maxCount = counters[i];
        for(curCount=0;curCount<maxCount;curCount++){
            (*it) = bins[i][curCount];
            it++;
        }
      }
 }else if (field == 4){
   for(it=begin;it!=end;it++){
     val  = (int)(((*it)->val4).c_str()[5])  + ((int)(((*it)->val4).c_str()[4]) << 8);
     bins4[val][counters4[val]] = *it;
     counters4[val]++;
   }
   Data *t;
   for(int i= 66; i<65536;i++){
     maxCount = counters4[i];
     for(curCount=0;curCount<maxCount;curCount++){
       t = bins4[i][curCount];
       val  = (int)((t->val4).c_str()[3])  + ((int)((t->val4).c_str()[2]) << 8);
       bins42[val][counters42[val]] = t;
       counters42[val]++;
     }
   }
   for(int i= 66; i<65536;i++){
     maxCount = counters42[i];
     for(curCount=0;curCount<maxCount;curCount++){
       t = bins42[i][curCount];
       val  = (int)((t->val4).c_str()[1])  + ((int)((t->val4).c_str()[0]) << 8);
       bins4[val][counters43[val]] = t;
       counters43[val]++;
     }
   }
   it = l.begin();
   for(int i= 66; i<65536;i++){
     maxCount = counters43[i];
     for(curCount=0;curCount<maxCount;curCount++){
       (*it) = bins4[i][curCount];
       it++;
     }
   }
 }
}
