#ifndef UTIL_H
#define UTIL_H
#include <functional>
#include <utility>
// Only for pairs of std::hash-able types for simplicity.
// // You can of course template this struct to allow other hash functions
struct pair_hash {
	template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
    	auto h1 = std::hash<T1>{}(p.first);
    	auto h2 = std::hash<T2>{}(p.second);
    	// Mainly for demonstration purposes, i.e. works but is overly simple
    	// In the real world, use sth. like boost.hash_combine
    	return h1 ^ h2;  
    }
};


#endif
