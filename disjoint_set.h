#ifndef DISJ_SETS_H
#define DISJ_SETS_H

// DisjSets class
//
// CONSTRUCTION: with int representing initial number of sets
//
// ******************PUBLIC OPERATIONS*********************
// void union( root1, root2 ) --> Merge two sets
// int find( x )              --> Return set containing x
// ******************ERRORS********************************
// No error checking is performed

#include <vector>
using namespace std;

/**
 * Disjoint set class.
 * Use union by rank and path compression.
 * Elements in the set are numbered starting at 0.
 */
class DisjSets
{
  public:
    explicit DisjSets();
    explicit DisjSets( const int& numElements );
    explicit DisjSets( int&& numElements );
    // int find( int x ) const;
    size_t Find( const size_t& x ) const;
    size_t Find( const size_t& x );
    void UnionSets( int root1, int root2 );
    void Print() const;
    // void link(const size_t& e1, const size_t& e2);
    size_t Size() const;
    DisjSets& operator=(const DisjSets& rhs);

  private:
    vector<int> set_;
};

#endif
