#pragma once

#include <string>

namespace bringauto {

class Vertex {
public:
	explicit Vertex(std::string newName) : name{std::move(newName)} {};
	const std::string name;
};

}