#pragma once

#include <string>
#include <utility>

namespace bringauto {
/**
 * Class specifying Vertexes in StateGraph
 * Every Vertex is one state, in which StateGraph's current state can be
 * Vertexes can be addressed by name or pointer
 */
class Vertex {
public:
	explicit Vertex(std::string newName) : name{std::move(newName)} {};
    std::string getName() { return name; }
private:
	const std::string name;
};

}