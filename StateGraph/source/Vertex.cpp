//
// Created by josef on 05.11.21.
//

#include "Vertex.h"

#include <utility>

Vertex::Vertex(std::string newName) : name{std::move(newName)} {}
