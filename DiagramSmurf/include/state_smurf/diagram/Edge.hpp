#pragma once

#include <state_smurf/diagram/Vertex.hpp>
#include <string>
#include <memory>
#include <utility>

namespace state_smurf::diagram {

/**
 * Class specifying oriented edges in StateGraphs
 * Edges define valid transitions in state graph
 * If Edge is defined from vertex A to vertex B, it is possible to change state from A to B,
 * for reverse transition there must be defined edge from B to A
 * From = initial Vertex
 * To = target Vertex
 */
class Edge {
public:
    Edge(std::shared_ptr<Vertex> newFrom, std::shared_ptr<Vertex> newTo)
         : from{std::move(newFrom)}, to{std::move(newTo)} {};

    /**
     * @return pointer to initial Vertex of Edge
     */
    std::shared_ptr<Vertex> getFrom() { return from; }
    /**
     * @return pointer to target Vertex of Edge
     */
    std::shared_ptr<Vertex> getTo() { return to; }
private:
	std::shared_ptr<Vertex> from;
	std::shared_ptr<Vertex> to;

};

}