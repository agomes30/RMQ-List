// CMSC 341 - Spring 2020 - Project 2
#ifndef _RMQLIST_H
#define _RMQLIST_H

#include <iostream>
#include <stdexcept>
#include <cmath>

using std::swap;
using std::ostream;
using std::cout;
using std::endl;
using std::sqrt;
using std::range_error;
using std::invalid_argument;

// Macro for a two-argument min function
#define MIN(a, b)  ((a) < (b) ? (a) : (b))

// forward declarations
template <class K, class V> class RMQList;
template <class K, class V> class Node;
template <class K, class V> ostream& operator<<(ostream &sout, const Node<K,V> &x);

// *********************************************
// Node - node class for the RMQList linked list
//   key and value are templated (types K and V)
// *********************************************

template <class K, class V>
class Node {
  friend RMQList<K, V>;
public:
  Node(K key = K(), V value = V(), Node<K, V> *next = nullptr) {
    _key = key;
    _value = value;
    _next = next;
  }
  friend ostream& operator<< <K,V>(ostream &sout, const Node<K,V> &x);
private:
  K _key;
  V _value;
  Node *_next;
};

// Overloaded insertion operator for Node
template <class K, class V>
ostream& operator<<(ostream &sout, const Node<K,V> &x) {
  sout << "Key: " << x._key << ", Value: " << x._value;
  return sout;
}

// *******************************************************
// RMQList - list container (linked list) with RMQ support
//   key and value are templated (types K and V)
// *******************************************************

template <class K, class V>
class RMQList {
public:
  // Create an empty RMQList object
  RMQList();
  
  // Destructor, Copy Constructor, Assignment Operator
  ~RMQList();
  RMQList(const RMQList<K,V> &rhs);
  const RMQList<K,V>& operator=(const RMQList<K,V> &rhs);
  
  // In-line function. Returns the size (number of elements).
  int size() const { return _listSize; }

  // In-line function. Returns true if the list is empty; false
  // otherwise.
  bool empty() const { return _head == nullptr; }
  
  // Insert an element into the list; list must be kept in increasing
  // order by key; duplicate keys are not allowed, so return false if
  // there is already an entry with the specified key, true otherwise.
  // Should check if key > largest current key and, if so, append to
  // the end of the list without iteration.
  bool insert(const K& key, const V& value);

  // Remove an element from the list; return false if no element
  // exists with the specified key value, true otherwise
  bool remove(const K& key);

  // Update value for the element with given key; return false if
  // there is no element with the given key, true otherwise
  bool update(const K& key, const V& value);
  
  // RMQ Query for k1 to k2.  Throws exceptions (see documentation).
  V query(const K& k1, const K& k2);

  // Dump the list entries
  void dumpList() const;
  
  // Dump the RMQ info and table.  What gets dumped depends on which
  // RMQ method is used.
  void dumpTable() const;

  // Clear the data data strucures
  void clear();
  
 private:
  Node<K,V> *_head;
  Node<K,V> *_tail;
  int _listSize;
  int tracker;

  // **********************************
  // Private variables for RMQ go here!
  // **********************************
  K* arrayOne; //array of keys
  V** arrayTwo; //array of values

  // *******************************************
  // Declarations for private functions go here!
  // *******************************************
  void addArrayOne(); //forms one dimentional array of keys
  void addArrayTwo(); //forms two dimentional array of values
  int search(K key); //binary search
};


template <class K, class V>
RMQList<K,V>::RMQList() {
  _head = nullptr;
  _tail = nullptr;
  _listSize = 0;
  arrayOne = nullptr;
  arrayTwo = nullptr;
}

template <class K, class V>
RMQList<K,V>::~RMQList() {

// deletes all lists with keys and values
  if (empty() == false) {
    clear();
  }
}

template <class K, class V>
RMQList<K,V>::RMQList(const RMQList<K,V> &rhs) {
  //using insert to create lists
  if (rhs.empty() != 0){
    Node<K,V> *node = rhs.head;
    while (node != nullptr){
      insert(node -> _key, node -> _value);
      node = node -> _next;

    }
  }

}

template <class K, class V>
const RMQList<K,V>& RMQList<K,V>::operator=(const RMQList<K,V> &rhs) {
  if (this != &rhs){
    clear(); // if the current object is not the same, clear before advancing
  }

  //using insert to create lists
  Node<K,V> *node = rhs.head;
  while (node != nullptr){
    insert(node -> _key, node -> _value);
    node = node -> _next;
  }
  return *this;
  

}

template <class K, class V>
bool RMQList<K,V>::insert(const K& key, const V& value) {

  

  bool flag = false;

  if (empty() == false){
    Node<K,V> *curr = _head;
    Node<K,V> * insert;
    Node<K,V> *linker;

    while((curr != nullptr) and (curr -> _key != key) and (flag == false)){
      if(curr -> _key > key){
        //inserting at the beginning
        if (curr == _head){ 
          insert = new Node<K,V> (key, value);
          insert -> _next = _head;
          _head = insert;
          flag = true;
          _listSize ++;
        }
        else{
          //inserting at the middle
          insert = new Node<K,V> (key, value);
          insert -> _next = curr;
          linker -> _next = insert;
          flag = true;
          _listSize++;
        }
        
      }
      //inserting at the end
      else if (curr -> _next == nullptr){
        insert = new Node<K,V> (key, value);
        curr -> _next = insert;
        _tail = curr -> _next;
        flag = true;
        _listSize ++;
      }
      
      linker = curr;
      curr = curr -> _next;
    }
    
  }
  else {
    //inserting the first element of the linked list (currently empty)
    _head = new Node<K,V> (key,value);
    _tail = _head;
    flag = true;
    _listSize ++;
  }

  //if a node is added, update the arrays
  if (flag == true){
    addArrayOne();
    addArrayTwo();
  }
  return flag;
}

template <class K, class V>
bool RMQList<K,V>::remove(const K& key) {
  bool flag = true;
  if (_head != nullptr){
    flag = false;
  }

  
  Node<K,V> *node = _head; //node traverses the linked list
  Node<K,V> *temp; // temporary holder before getting deleted
  Node<K,V> *lastNode; //the trailing pointer

  int stop = _listSize;
  

 
  while ((node != nullptr) and (stop == _listSize)){
    if (node->_key == key) {
        // first node of the linked list
        if (node == _head) {
          temp = _head;
          _head = _head->_next;
          delete temp;
          node = _head;
          _listSize--;
        } 
        //exists somewhere in the middle
        else if (node->_next != nullptr){
          temp = node;
          lastNode->_next = node->_next;
          node = node->_next;
          delete temp;
          _listSize--;
        }
        //last node of the linked list
        else if (node->_next == nullptr) {
          temp = node;
          node = lastNode;
          delete temp;
          node->_next = nullptr;
          _listSize--;
        }
      
      flag = true;  
    }
    lastNode = node;
    node = node->_next; //advancing the node
  }
  //updating the array
  addArrayOne();
  addArrayTwo();
  return flag;
  
  
}


template <class K, class V>
bool RMQList<K,V>::update(const K& key, const V& value) {
 
  bool flag = false;
  for (Node<K,V> *node = _head; node != nullptr; node = node -> _next){
    //if the key matches, simply update the value associated with the key
    if (key == node->_key){
      node -> _value = value;
      flag = true;
    }
  }
  return flag;
}

template <class K, class V>
int RMQList<K,V>::search(K key){

   int n = size();
  int p = 0, r = n - 1;
  int q;

  while (p != r) {
    q = (p+r)/2;
    if (key <= arrayOne[q]) {
      r = q;
    } else {
      p = q+1;
    }
  }

  if ( key == arrayOne[p]) {
    return p;
  } else {
    return -p;
  }

}

template <class K, class V>
V RMQList<K,V>::query(const K& k1, const K& k2) {
  V value;
  if (empty() == true){
    throw range_error("The list is empty.");
  }
  else{
    int keyOne = search(k1);
    int keyTwo = search(k2);

    if (keyOne < 0 or keyTwo < 0){
      throw invalid_argument("Key not found.");
    }
    else{
      value = arrayTwo[keyOne][keyTwo];
    }

  }
  return value;
}

template <class K, class V>
void RMQList<K,V>::dumpList() const {
  //dumping node
  Node<K,V> *node = _head;
  while (node != nullptr){
    cout << *node << endl; //printing the node content
    node = node -> _next;  //advancing through the linked list
  }
}

template <class K, class V>
void RMQList<K,V>::dumpTable() const {
  //dumping the contents of the 2d array
  for (int i = 0; i < size(); i++){
    for (int j = i; j < size(); j++){
      cout << arrayTwo[i][j] << " ";
    }
    cout << "\n";
  }
}

template <class K, class V>
void RMQList<K,V>::clear() {
  
  if (_head != nullptr){
    Node<K,V> *node = _head;
    Node<K,V> *temp;

    while (node != nullptr){
      temp = node -> _next;
      delete node;
      node = temp;
    }
    _head = nullptr;
    //deleting 1d array
    delete [] arrayOne;
    arrayOne = nullptr;

    //deleting invidual elements
    for (int i = 0; i < size(); i++){
      delete [] arrayTwo[i];
    }
    //deleting the array
    delete [] arrayTwo;
    arrayTwo = nullptr;

  }
  _listSize = 0;
}

template <class K, class V>
void RMQList<K,V>::addArrayOne() {
  
  //deleting arrayOne everytime to be later set to new array
  delete [] arrayOne;

  //Dynamically allocating 1-D key array
  K *newArray = new K[_listSize];
  
  int count = 0;
  //node pointing to the head
  Node<K,V> *node = _head;
  while ((count < size()) and (node != nullptr)){
    newArray[count] = node -> _key;
    node = node -> _next;
    count++;

  }

  //setting the old array to the modified array
  arrayOne = newArray;

}
template <class K, class V>
void RMQList<K,V>::addArrayTwo(){

  if (arrayTwo != nullptr){
    for (int i = 0; i < tracker; i++){
      delete [] arrayTwo[i];
    } 
    delete [] arrayTwo;
  }

  //Dynamically allocating 2-D value array
  V **newArray = new V*[_listSize];

  //Node pointing to the head
  Node <K,V> *node = _head;
  Node <K,V> *temp = node;

  for (int i = 0; i < size(); i++){
    newArray[i] = new V[_listSize];
    
    for (int j = i; j < size(); j++){
        if (i == j){
        newArray[i][j] = temp -> _value;
      }
      
        else if (temp -> _value < newArray[i][j-1]){
        newArray[i][j] = temp -> _value;
      }
      
      else{
        newArray[i][j] = newArray[i][j-1];
      }
      temp = temp -> _next;
    }
    node = node -> _next;
    temp = node;

  }
  tracker = _listSize;
  arrayTwo = newArray;
}

#endif