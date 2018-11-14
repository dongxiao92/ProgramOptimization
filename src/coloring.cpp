#include "graph.h"
#include "type.h"
#include "timer.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <cstring>

using namespace std;

void CheckGraphCloloring(Graph&g, Color_t* colors){
	auto edgeRep = g.getEdgeRep();
	//count how many colors are used.
	unordered_set<Color_t>used_colors;
	for(auto e: edgeRep){
		VertexId_t v1 = e.first;
		VertexId_t v2 = e.second;
		//check if endpotins share the same color
		if(colors[v1]==colors[v2]){
			printf("vertex%d and vertex%s share color\n", v1, v2);
		}
		used_colors.insert(colors[v1]);
		used_colors.insert(colors[v2]);
	}
	printf("Use %d colors\n", used_colors.size());
}


void ColoringVertexBased(Graph& g, Color_t* colors)
{
	//initialize colors
	VertexCount_t vex_cnt = g.getVertexCount();
	memset(colors, 0, sizeof(Color_t)*vex_cnt);
	//initialized conf
	unordered_set<VertexId_t> conflicts;
	for(VertexId_t v=0; v<vex_cnt; v+=1){
		conflicts.insert(v);
	}
	
	const static ColorCount_t MAX_COLORS=g.getMaxDegree()+1;
#ifdef DEBUG
	printf("max degree:%d\n", g.getMaxDegree());
#endif
	char* forbidden = new char[MAX_COLORS];
	//get Vertex-based representation
	auto vex_rep = g.getVertexRep();
	
	Timer<> timer;
	timer.start();
	while(!conflicts.empty()){
		//give a coloring
		for(VertexId_t v: conflicts){
			memset(forbidden, 0, sizeof(char)*MAX_COLORS);
			for(VertexId_t n: vex_rep[v]){
				//mark the color of adj vex as used
				forbidden[colors[n]] = 1;
			}
			Color_t c = 0;
			for(; c<MAX_COLORS; c+=1){
				if(forbidden[c] == 0){
					colors[v] = c;
					break;
				}
			}
			if(c == MAX_COLORS){
				assert(false);
			}
		}
#ifdef DEBUG
		printf("finsh a coloring turn \n");
#endif
		//update conflicts
		unordered_set<VertexId_t>new_conflicts;
		for(VertexId_t v: conflicts){
			for(VertexId_t n:vex_rep[v]){
				if(colors[v] == colors[n]){
					VertexId_t min_v = min(v, n);
#ifdef DEBUG
		printf("%d and %d have same color:%d \n", v, n, colors[v]);
#endif
					new_conflicts.insert(min_v);
				}
			}
		}
#ifdef DEBUG
		printf("finsh update conflicts \n");
#endif
		//update conflicts
		conflicts.clear();
		conflicts.insert(new_conflicts.begin(), new_conflicts.end());
#ifdef DEBUG
		for(auto&s: conflicts){
			printf("%d \n", s);
		}
#endif
		printf("conflicting set size:%d\n", conflicts.size());
	}
	float elapsedTime = timer.getElapsedTime();
	printf("cost time:%f ms\n", elapsedTime);

}

void ColoringEdgeBased(Graph&g, Color_t* colors)
{
	//initialize vertex_forbidden
	unordered_map<VertexId_t, unordered_set<Color_t>> vForbidden;
	const VertexCount_t vex_cnt = g.getVertexCount();
	memset(colors, 0, sizeof(Color_t)*vex_cnt);
	for(VertexId_t v; v<vex_cnt; v+=1){
		vForbidden[v] = unordered_set<Color_t>();
	}

	const ColorCount_t MAX_COLORS = g.getMaxDegree()+1;
	//get edge-based representation
	auto edge_rep = g.getEdgeRep();
	//get vertex-based representation
	auto vex_rep = g.getVertexRep();

	bool color_done = false;
	Timer<> timer;
	timer.start();
	while(!color_done){
		//assign color according to vForbidden
		for(VertexId_t v=0; v<vex_cnt; v+=1){
			if(colors[v]!=0)
				continue;
			Color_t c = 1;
			for(; c<MAX_COLORS; c+=1){
				if(vForbidden[v].find(c) == vForbidden[v].end()){
					colors[v] = c;
					//add c to forbidden colors of v's neighbors
					for(auto&&n:vex_rep[v]){
						vForbidden[n].insert(c);
					}
					break;
				}
			}
			if(c == MAX_COLORS){
				assert(false);
			}
		}
#ifdef DEBUG
		printf("finsh a coloring turn \n");
		//collect conflict vertex
		unordered_set<VertexId_t>conflicts;
#endif
		//detect conflics
		bool found_conflict = false;
		for(auto&&e : edge_rep){
			VertexId_t v1 = e.first;
			VertexId_t v2 = e.second;
			if( (colors[v1]==colors[v2]) && colors[v1]!=0 ){
				//mark the smaller endpoint as uncolored.
				colors[min(v1, v2)] = 0;
#ifdef DEBUG
				conflicts.insert(min(v1, v2));
				//printf("%d and %d have the same color %d\n", v1, v2, colors[v1]);
#endif
				found_conflict = true;
			}
			//update vForbidden
			if(colors[v1]!=0){
				vForbidden[v2].insert(colors[v1]);
			}
			if(colors[v2]!=0){
				vForbidden[v1].insert(colors[v2]);
			}
		}
#ifdef DEBUG
		printf("conflict vertex count:%d \n", conflicts.size());
		printf("finsh conflict detect \n");
#endif
		color_done = !found_conflict;
	}
	float elapsedTime = timer.getElapsedTime();
	printf("cost time:%f ms\n", elapsedTime);
}

int main(const int argc, const char*argv[])
{
	const string file_path = argv[1];
	Graph g;
	g.ConstructFromEdge(file_path);
	g.PrintSummary();
#ifdef DEBUG
	printf("construct finish \n");
#endif 

	ColorCount_t max_colors = g.getVertexCount();
	Color_t* colors = new Color_t[max_colors];
	//color g
	ColoringVertexBased(g, colors);
#ifdef DEBUG
	printf("Vertex Coloring finish \n");
#endif 
	CheckGraphCloloring(g, colors);
		
	ColoringEdgeBased(g, colors);
#ifdef DEBUG
	printf("Edge Coloring finish \n");
#endif 
	CheckGraphCloloring(g, colors);
	
	//clean up
	delete[] colors;
	return 0;
}
