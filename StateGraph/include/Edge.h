#pragma once

#include <Vertex.h>
#include <string>
#include <memory>

class Edge {
public:
	std::shared_ptr<Vertex> from;
	std::shared_ptr<Vertex> to;

};

