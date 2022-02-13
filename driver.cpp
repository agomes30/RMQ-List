// CMSC 341 - Spring 2020 - Project 2
//
// driver.cpp - a simple driver for the RMQList class
//
// NOTE: This program uses int keys and values.  You should be sure to
// test RMQList with *different* key and value types.
//
// You are allowed to use STL containers and libraries in your test
// program.

#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <functional>
#include <algorithm>
#include "rmqlist.h"

using namespace std;

// *******************************************************
// KeyVal: a simple class to store integer key/value pairs
// *******************************************************
class KeyVal {
public:
  KeyVal() { _data = pair<int,int>(0,0); }
  KeyVal(int key, int val) { _data = pair<int,int>(key, val); }
  int getKey() const { return _data.first; }
  int getVal() const { return _data.second; }
private:
  pair<int,int> _data;
};

// Function prototypes
bool operator<(const KeyVal& kv1, const KeyVal& kv2) { return kv1.getKey() < kv2.getKey(); }
int minimum(const vector<KeyVal> &data, int first, int last);
int bsearch(const vector<KeyVal> &A, int key);

// **********************************************
// main() - demonstrates use of the RMQList class
// **********************************************
int main() {

  // A small example.  Insert (keys[i], i) into an RMQList object
  vector<int> keys = {34, 16, 58, -24, 53, 7, 97, 92, -12, 45, 9, 0, -1, 20, 77};

  RMQList<int,int> rmq;

  cout << "\nInsert data:\n";
  for (int i = 0; i < keys.size(); i++) {
    cout << "  key = " << keys[i] << ", value = " << i << endl;
    rmq.insert(keys[i], i);
  }

  cout << "\nDump list:\n";
  rmq.dumpList();

  cout << "\nRMQ query:\n";
  cout << "  query(45,77) = " << rmq.query(45, 77) << endl;

  cout << "\nDump RMQ table info:\n";
  rmq.dumpTable();

  cout << "\nremove(92), remove(97), remove(-24), update(77,100):\n";
  rmq.remove(92);
  rmq.remove(97);
  rmq.remove(-24);
  rmq.update(77,100);

  cout << "\nDump list:\n";
  rmq.dumpList();
  cout << "\nDump RMQ table info:\n";
  rmq.dumpTable();
  cout << "\nRMQ query:\n";
  cout << "  query(0,20) = " << rmq.query(0,20) << endl;

  cout << "\nDump RMQ table info:\n";
  rmq.dumpTable();

  cout << "\nClear and dump list:\n";
  rmq.clear();
  rmq.dumpList();


  // A medium-sized test.  Insert ~1000 entries. Check the result of
  // query() against the "brute force" RMQ solution.
  
  cout << "*****************************************\n";
  cout << "Insert lots of data; check speed of query\n";
  cout << "*****************************************\n";

  const int NUMGEN = 1000;

  cout << "\nInsert " << NUMGEN << " random key/value pairs.\n";

  // This is all random number generation stuff
  // Seed sequences for random number generators
  std::seed_seq seed1 = {31, 41, 59};
  std::seed_seq seed2 = {27, 18, 28};
  // Mersenne twister random number engine
  mt19937 engine1(seed1), engine2(seed2);
  uniform_int_distribution<int> key_dist(-15000, 15000);
  auto key_gen = bind(key_dist, engine1);
  uniform_int_distribution<int> value_dist(0, 50000);
  auto value_gen = bind(value_dist, engine2);

  // Generate the data; store in a vector as well as inserting into
  // the RMQList.  The set is used to check if a key has already
  // occurred so that we don't insert duplicate keys into the vector.

  vector<KeyVal> kv;
  set<int> keySet;

  for ( int i = 0; i < NUMGEN; i++ )  {
    int key = key_gen();                 // generate a key
    int value = value_gen();             // generate a new value
    if ( keySet.count( key ) == 0 ) {    // if it's a new key...
      kv.push_back(KeyVal(key, value));  // ...put it in the vector
      keySet.insert(key);                // ...and in the set
    }
    rmq.insert( key, value );            // insert (key,value) in the
					 // RMQList
  }
  std::sort(kv.begin(), kv.end());       // Sort the vector by key
					 // values.  Need to do this
					 // so that it is in the same
					 // order as the RMQList

  cout << "\nTotal of " << kv.size() << " key/value pairs inserted.\n";

  // Test ALL possible queries.  Compare the answer from query() with
  // the brute force computation from minimum().  This can be slow.
  cout << "\nTesting correctness of query()...";
  for (int i = 0; i < kv.size()-1; i++) {
    for (int j = i+1; j < kv.size(); j++) {
      int rmqq = rmq.query( kv[i].getKey(), kv[j].getKey() );
      int minq = minimum(kv, kv[i].getKey(), kv[j].getKey() );
      if (rmqq != minq) {
	cout << "Fail! (" << i << ", " << j << "); rmq ="
	     << rmqq << ", minq = " << minq << endl;
      }
    }
  }
  cout << "done!\n";

  rmq.dumpTable();
  
  return 0;
}

int minimum(const vector<KeyVal>& data, int k1, int k2) {
  int indx1 = bsearch(data, k1);
  int indx2 = bsearch(data, k2);
  if (indx1 > indx2) swap(indx1, indx2);

  int minVal = data[indx1].getVal();
  for (int i = indx1+1; i <= indx2; i++) {
    minVal = ( data[i].getVal() < minVal ? data[i].getVal() : minVal );
  }
  return minVal;
}

int bsearch(const vector<KeyVal>& A, int key) {
  int n = A.size();
  int p = 0, r = n - 1;
  int q;

  while (p != r) {
    q = (p+r)/2;
    if (key <= A[q].getKey()) {
      r = q;
    } else {
      p = q+1;
    }
  }

  if ( key == A[p].getKey() ) {
    return p;
  } else {
    return -p;
  }
}