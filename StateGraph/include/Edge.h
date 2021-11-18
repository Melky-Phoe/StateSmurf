#pragma once

#include <Vertex.h>
#include <string>
#include <memory>

namespace bringauto {

class Edge {
public:
	std::shared_ptr<Vertex> from;
	std::shared_ptr<Vertex> to;

};

}