#ifndef HEAP_H_
#define HEAP_H_
#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>

template <typename T>
class MinHeap {
public:
    MinHeap(int d);
    ~MinHeap();
    void add(const T& item, int priority);
    const T peek() const;
    void remove();
    bool isEmpty();
    void bubbleUp(int d);
    void trickleDown(int p);
    
    int size;
private:
    int md;
    int pos;
    std::vector<std::pair<T, int>> a;
};
template<typename T>
MinHeap<T>::MinHeap(int d){
    if(d>= 2){
      md = d;
      size = 0;
    }
    else{

    }
}
template<typename T>
MinHeap<T>::~MinHeap(){
  //delete vector<T>
}

template<typename T>
const T MinHeap<T>::peek() const {
    return a[0].first;
}
template<typename T>
void MinHeap<T>::add(const T& data, int priority) {
   //std::pair<T, int> x = std::pair<const &data, int priority>;
   // a.push_back(x);
    if(a.size() ==1){
      return;
    }
    else{
      bubbleUp(a);
    }
    //a->at(size) = data;
    bubbleUp(size);
    size++; 
}
template<typename T>
void MinHeap<T>::bubbleUp(int pos) {
  int x = (pos-1)/2;
  if (pos > 0 && (a[pos] < a[x])) {
    int z = (pos-1)/2;
    int temp = a[pos];
    a[pos] = a[z];
    a[z] = temp;
    bubbleUp(pos);
  }
}
template<typename T>
    void MinHeap<T>::remove(){
    int temp = a[size-1];
    a[size-1] = a[0];
    a[0] = temp;
    size--;
    trickleDown(0);
}
template<typename T>
void MinHeap<T>::trickleDown(int pos) {
    int child = 2*pos+1;
    if (child < size) {
      if (child+1 < size && (a[child] > a[child+1])) 
        child++;
        if (a[child] > a[pos]) {
          int temp = a[child];
          a[child] = a[pos];
          a[pos] = a[temp];
          trickleDown(a[child]);
        }
      }
  
}
#endif