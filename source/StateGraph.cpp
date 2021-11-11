//
// Created by josef on 05.11.21.
//

#include "StateGraph.h"

void StateGraph::setEdge(Vertex from, Vertex to) {
	Edge edge{};
	edge.from = from;
	edge.to = to;

	edges.push_back(edge);

}
