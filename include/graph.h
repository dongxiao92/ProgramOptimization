#ifndef GRAPH_H
#define GRAPH_H

#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <cassert>

#include "type.h"
#include "utils.h"

class Graph{
public:

	Graph():initialized(false), \
			edge_cnt(0), vertex_cnt(0), max_degree(0){}
	
	void ConstructFromEdge(const std::string&path){
		std::ifstream inf(path.c_str());
		//first line contains the vertex count
		std::string line;
		getline(inf, line, '\n');
		vertex_cnt = std::stoi(line); 
		//second line contains the edge count
		getline(inf, line, '\n');
		edge_cnt = std::stoi(line); 
		//each line is a edge
#ifdef DEBUG
		printf("start iterate edges \n");
#endif
		bool found_ring = false;
		while(getline(inf, line, '\n')){
			unsigned long sl =  std::stoul(line.substr(0, line.find(" ")));
			EdgeId_t s = sl;
			if( s!=sl ){
				throw std::out_of_range("vertex id out of range");
			}
			unsigned long dl = std::stoul(line.substr(line.find(" ")+1));
			EdgeId_t d = dl;
			if( d!=dl ){
				throw std::out_of_range("vertex id out of range");
			}
			if(s == d){
				//this is a ring, skip
				if(!found_ring){
					printf("find ring \n");
					found_ring = true;
				}
				continue;
			}
			auto&& edge = std::make_pair(s, d);
			edges.push_back(edge);
		}
		//update edge count, as there may be rings.
		edge_cnt = edges.size();
		inf.close();
#ifdef DEBUG
		printf("construct vertex-based representation \n");
#endif
		//construct the vertex representation
		std::vector<char>dup_flags;
		for(auto&& e:edges){
			VertexId_t s1 = e.first;
			VertexId_t s2 = e.second;
			if(s1 == s2){
				//there is a ring, skip
				continue;
			}
			if( s1>=vertex_cnt||s2>=vertex_cnt){
				throw std::logic_error("vertex id is larger than vertex count");
			}
			if(vertex_to_neighbors.find(s1)==vertex_to_neighbors.end()){
				vertex_to_neighbors.insert(std::make_pair(s1, std::unordered_set<VertexId_t>()));
			}
			if(vertex_to_neighbors[s1].find(s2)!=vertex_to_neighbors[s1].end()){
			    //the mirror edge has been handled, skil
			    dup_flags.push_back(1);
			    continue;
			}
			vertex_to_neighbors[s1].insert(s2);
			if(vertex_to_neighbors.find(s2)==vertex_to_neighbors.end()){
				vertex_to_neighbors.insert(std::make_pair(s2, std::unordered_set<VertexId_t>()));
			}
			vertex_to_neighbors[s2].insert(s1);
			dup_flags.push_back(0);
		}
		//remove dup edges
		std::vector<std::pair<VertexId_t, VertexId_t>>filtered_edges;
		for(int i=0; i<dup_flags.size(); i+=1){
		    if(dup_flags[i]==0){
			filtered_edges.push_back(edges[i]);
		    }
		}
		edges = filtered_edges;
		edge_cnt = edges.size();
		//check the vertex count
		if(vertex_cnt > vertex_to_neighbors.size()){
			printf("there exist vertex with no edges\n");
		}else if(vertex_cnt<vertex_to_neighbors.size()){
			throw std::logic_error("inconsistent vertex count");
		}
		//find the max degree
		max_degree = 0;
		for(auto& item:vertex_to_neighbors){
			unsigned int degree = item.second.size();
			max_degree = max_degree>degree?max_degree:degree;
		}
		// compute the degree of each vertex
		for(auto& i: vertex_to_neighbors){
			auto&& neighbors = i.second;
			auto size = neighbors.size();
			EdgeCount_t degree = static_cast<EdgeCount_t>(size);
			if( size!=degree ){
				throw::std::out_of_range("degree outof range.");
			}
			vertex_to_degree.insert(std::make_pair(i.first, degree));
		}
		initialized = true;
	}

	EdgeCount_t getEdgeCount(){
		if(initialized)
			return edge_cnt;
		return 0;
	}
	VertexCount_t getVertexCount(){
		if(initialized)
			return vertex_cnt;
		return 0;
	}
	EdgeCount_t getMaxDegree(){
		if(initialized)
			return max_degree;
		return 0;
	}

	using VertexRep=std::unordered_map<VertexId_t, std::unordered_set<VertexId_t>>;
	VertexRep& getVertexRep(){
		assert(initialized);
		return vertex_to_neighbors;
	}
	using EdgeRep=std::vector<std::pair<VertexId_t, VertexId_t>>;
	EdgeRep& getEdgeRep(){
		assert(initialized);
		return edges;
	}
	void PrintSummary(){
		assert(initialized);
		printf("total vertex:%d,total edges:%d\n", vertex_cnt, edge_cnt);
	}
	using Degree=std::unordered_map<VertexId_t, EdgeCount_t>;
	Degree& getDegrees(){
		return vertex_to_degree;
	}
private:
	bool initialized;

	EdgeCount_t edge_cnt;
	VertexCount_t vertex_cnt;
	EdgeCount_t max_degree;
	// vertex-cenreric representation
	std::unordered_map<VertexId_t, std::unordered_set<EdgeId_t>> vertex_to_neighbors;
	// edge-centric representation
	std::vector<std::pair<VertexId_t, VertexId_t>> edges;
	// degrees of each vertex
	std::unordered_map<VertexId_t, EdgeCount_t> vertex_to_degree;
};

#endif
