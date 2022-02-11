#include <state_smurf/log_evaluator/CircuitFinder.hpp>
#include <iostream>

namespace state_smurf::log_evaluator {

    CircuitFinder::CircuitFinder(diagram::StateDiagram stateDiagram) : adjacencyMap{stateDiagram.createAdjacencyMap()} {
        numberOfVertexes = adjacencyMap.size();
        blocked = static_cast<bool *>(calloc(sizeof(bool), numberOfVertexes));
		blockMap.resize(numberOfVertexes);
        if (blocked == nullptr) {
            exit(1); // exception
        }
        for (auto & it : adjacencyMap) {
            keys.push_back(it.first);
        }
		startingVertexes = stateDiagram.getStartVertexes();
    }
	
	std::vector<std::vector<std::string>>  CircuitFinder::find() {
		for (int i = 0; i < startingVertexes.size(); ++i) {
			if (!adjacencyMap.empty()) {
				for (int j = 0; j < numberOfVertexes; j++) {
					blockMap[j].clear();
					blocked[j] = false;
				}
				startVertex = startingVertexes[i];
				circuit(startVertex);
				
				for (const auto& adjacentVertex : adjacencyMap[startVertex]) {
					startingVertexes.push_back(adjacentVertex);
				}
				adjacencyMap.erase(startVertex);
				for (auto & it : adjacencyMap) {
					auto it2 = std::find(it.second.begin(), it.second.end(), startVertex);
					if (it2 != it.second.end()) {
						it.second.erase(it2);
					}
				}
			} else {
				break;
			}
		}
		return circuits;
	}
	
    void CircuitFinder::unblock(const std::shared_ptr<diagram::Vertex>& vertex) {
        long vertexIndex = getVertexIndex(keys, vertex);
        blocked[vertexIndex] = false;
	    for (auto it = blockMap[vertexIndex].begin(); it != blockMap[vertexIndex].end();) {
		    auto blockedVertexIndex = it - blockMap[vertexIndex].begin();
		    it = blockMap[vertexIndex].erase(it);
		    if (blocked[blockedVertexIndex]) {
			    unblock(blockMap[vertexIndex][blockedVertexIndex]);
		    }
	    }
    }

    long CircuitFinder::getVertexIndex(const std::vector<std::shared_ptr<diagram::Vertex>>& vertexVector,
                                      const std::shared_ptr<diagram::Vertex>& vertex) {
        auto it = std::find(vertexVector.begin(), vertexVector.end(), vertex);
        if (it != vertexVector.end()) {
            long index = it - vertexVector.begin();
            return index;
        } else {
            return -2;
        }
    }

    bool CircuitFinder::circuit(const std::shared_ptr<diagram::Vertex>& vertex) {
        bool found = false;
        long currVertexIndex = getVertexIndex(keys, vertex);
        stack.push_back(vertex);
        blocked[currVertexIndex] = true;

        for (const auto& nextVertex : adjacencyMap[vertex]) {
            if (nextVertex == startVertex) {
				std::vector<std::string> newCircuit;
                for (const auto& circuitVertex : stack) {
                    // save circuit
                    //std::cout << circuitVertex->getName() << " ";
					newCircuit.push_back(circuitVertex->getName());
                }
                //std::cout << std::endl; // oddelovac DEBUG
				circuits.push_back(newCircuit);
                found = true;
            } else if (!blocked[getVertexIndex(keys, nextVertex)]) {
                if (circuit(nextVertex)) {
                    found = true;
                }
            }
        }
        if (found) {
            unblock(vertex);
        } else {
            for (auto blockVertex : adjacencyMap[vertex]) {
				auto blockedVector = blockMap[getVertexIndex(keys, blockVertex)];
	            if (std::find(blockedVector.begin(), blockedVector.end(), vertex) == blockedVector.end()) {
		            blockMap[getVertexIndex(keys, blockVertex)].push_back(vertex);
				}
            }
        }
		stack.pop_back();
        return found;
    }
}

/*
 Johnson v Jave
public final class CircuitFinding {

    final Stack<Integer> stack = new Stack<Integer>();
    final Map<Integer, List<Integer>> a;
    final List<List<Integer>> b;
    final boolean[] blocked;
    final int n;
    Integer s;

public static void main(String[] args) {
        List<List<Integer>> a = new ArrayList<List<Integer>>();
        a.add(new ArrayList<Integer>(Arrays.asList(1, 2)));
        a.add(new ArrayList<Integer>(Arrays.asList(0, 2)));
        a.add(new ArrayList<Integer>(Arrays.asList(0, 1)));
        CircuitFinding cf = new CircuitFinding(a);
        cf.find();
    }

    **
     * @param a adjacency structure of strong component K with least vertex in
     * subgraph of G induced by {s, s + 1, n};
     *
public CircuitFinding(List<List<Integer>> A) {
        this.a = new HashMap<Integer, List<Integer>>(A.size());
        for (int i = 0; i < A.size(); i++) {
            this.a.put(i, new ArrayList<Integer>());
            for (int j : A.get(i)) {
                this.a.get(i).add(j);
            }
        }
        // toto by mela byt ta adjacency kterou uz mam
        n = a.size();
        blocked = new boolean[n];
        b = new ArrayList<List<Integer>>();
        for (int i = 0; i < n; i++) {
            b.add(new ArrayList<Integer>());
        }
    }

private void unblock(int u) {
        blocked[u] = false;
        List<Integer> list = b.get(u);
        for (int w : list) {
            //delete w from B(u);
            list.remove(Integer.valueOf(w));
            if (blocked[w]) {
                unblock(w);
            }
        }
    }

private boolean circuit(int v) {
        boolean f = false;
        stack.push(v);
        blocked[v] = true;
        L1:
        for (int w : a.get(v)) {
            if (w == s) {
                //output circuit composed of stack followed by s;
                for (int i : stack) {
                    System.out.print(i + " ");
                }
                System.out.println(s);
                f = true;
            } else if (!blocked[w]) {
                if (circuit(w)) {
                    f = true;
                }
            }
        }
        L2:
        if (f) {
            unblock(v);
        } else {
            for (int w : a.get(v)) {
                //if (v∉B(w)) put v on B(w);
                if (!b.get(w).contains(v)) {
                    b.get(w).add(v);
                }
            }
        }
        v = stack.pop();
        return f;
    }

public void find() {
        s = 0;
        while (s < n) {
            if (!a.isEmpty()) {
                //s := least vertex in V;
                L3:
                for (int i : a.keySet()) {
                    b.get(i).clear();
                    blocked[i] = false;
                }
                circuit(s);
                a.remove(s);
                for (Integer j : a.keySet()) {
                    if (a.get(j).contains(s)) {
                        a.get(j).remove(s);
                    }
                }
                s++;
            } else {
                s = n;
            }
        }
    }
}

 */