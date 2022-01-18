#pragma once

#include <Vertex.hpp>
#include <string>
#include <memory>

namespace bringauto {

class Edge {
public:
	std::shared_ptr<Vertex> from;
	std::shared_ptr<Vertex> to;

};

}