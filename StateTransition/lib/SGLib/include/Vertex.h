#pragma once

#include <string>

class Vertex {
public:
	explicit Vertex(std::string newName) : name{std::move(newName)} {};
	const std::string name;
};

