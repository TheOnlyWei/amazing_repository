// Created by Mark Allen Weiss
// Modified by Wei Shi
#include <iostream>
#include "disjoint_set.h"
using namespace std;

DisjSets::DisjSets() {}
/**
 * Construct the disjoint sets object.
 * numElements is the initial number of disjoint sets.
 */
DisjSets::DisjSets( const int& numElements ) : set_(numElements, -1) {}
DisjSets::DisjSets( int&& numElements ) : set_(std::move(numElements), -1) {}

/**
 * Union two disjoint sets.
 * For simplicity, we assume root1 and root2 are distinct
 * and represent set names.
 * root1 is the root of set 1.
 * root2 is the root of set 2.
 * rank of height doesn't change because we union by rank of height
 */
void DisjSets::UnionSets( int root1, int root2 ) {
    if (root1 == root2) {
      return;
    }
    if( set_[ root2 ] < set_[ root1 ] ) { // root2 is deeper
        set_[ root1 ] = root2; // Make root2 new root
    } else {
        if( set_[ root1 ] == set_[ root2 ] )
            --set_[ root1 ];          // Update height if same
        set_[ root2 ] = root1;        // Make root1 new root
    }
}


/**
 * Perform a find.
 * Error checks omitted again for simplicity.
 * Return the set containing x.

int DisjSets::find( int x ) const {
    if( set_[ x ] < 0 )
        return x;
    else
        return find( set_[ x ] );
}
 */
size_t DisjSets::Find( const size_t& x ) const {
  if( set_[ x ] < 0 )
      return x;
  else
      return Find( set_[ x ] ); // automatically calls mutator find
}

/**
 * Perform a find with path compression.
 * Error checks omitted again for simplicity.
 * Return the set containing x.
 */
size_t DisjSets::Find( const size_t& x) {
    if( set_[ x ] < 0 )
        return x;
    else
        return set_[ x ] = Find( set_[ x ] ); // automatically calls mutator find
}

void DisjSets::Print() const {
	for(const auto& i: set_) {
		cout << i << ' ';
	}
	cout << endl;
}

size_t DisjSets::Size() const {
	return set_.size();
}

DisjSets& DisjSets::operator=(const DisjSets& rhs) {
	set_ = rhs.set_;
  return *this;
}
