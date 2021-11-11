#pragma once

#include <Vertex.h>
#include <Edge.h>

#include <vector>

class StateGraph {
public:
	std::vector<Edge> edges;
	std::vector<Vertex> vertexes;

	void setEdge(Vertex from, Vertex to);
};

