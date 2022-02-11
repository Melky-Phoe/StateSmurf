#pragma once
#include <state_smurf/diagram/StateDiagram.hpp>
/**
 *  This file serves to define StateDiagram using DiagramSmurf,
 *  Evaluator finds circuits in diagram.
 *  Evaluating compares Transitions between aggregated circuits, rather than single transitions.
 **/

namespace state_smurf {
	diagram::StateDiagram createDiagram() {
		/**
		 * Insert Any StateDiagram here
		 */
		state_smurf::diagram::StateDiagram stateDiagram;
		/// Creating Vertexes, they are accessible by name.
		auto connected = stateDiagram.addVertex("Connected");
		auto carStatusSent = stateDiagram.addVertex("CarStatusSent");
		auto carCommandReceived = stateDiagram.addVertex("CarCommandReceived");
		auto startReceived = stateDiagram.addVertex("StartReceived");
		auto stopReceived = stateDiagram.addVertex("StopReceived");
		auto idle = stateDiagram.addVertex("Idle");
		auto drive = stateDiagram.addVertex("Drive");
		auto inStop = stateDiagram.addVertex("InStop");
		
		stateDiagram.setStartVertex(idle);
		/// Setting Edges, from each state to every state it can transition
		stateDiagram.setEdge(connected, carStatusSent);
		
		stateDiagram.setEdge(carStatusSent, carCommandReceived);
		stateDiagram.setEdge(carCommandReceived, stopReceived);
		stateDiagram.setEdge(carCommandReceived, startReceived);
		
		stateDiagram.setEdge(stopReceived, idle);
		stateDiagram.setEdge(startReceived, idle);
		stateDiagram.setEdge(startReceived, drive);
		stateDiagram.setEdge(startReceived, inStop);
		
		stateDiagram.setEdge(idle, connected);
		stateDiagram.setEdge(idle, carStatusSent);
		stateDiagram.setEdge(drive, carStatusSent);
		stateDiagram.setEdge(drive, inStop);
		stateDiagram.setEdge(inStop, carStatusSent);
		return stateDiagram;
	}
}
