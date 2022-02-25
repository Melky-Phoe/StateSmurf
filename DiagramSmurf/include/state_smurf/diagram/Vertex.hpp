#pragma once

#include <string>

namespace state_smurf::diagram {
	/**
	 * Class specifying Vertexes in StateDiagram
	 * Every Vertex is one state, in which StateDiagram's current state can be
	 * Vertexes can be addressed by name or pointer
	 */
	class Vertex {
	public:
		explicit Vertex(std::string newName) : name_{std::move(newName)} {};
		
		std::string getName() { return name_; }
	
	private:
		const std::string name_;
	};
	
}