import numpy as np
from networkx import MultiGraph

def complement(string: str):
    complements = str.maketrans('ATCG', 'TAGC')
    return string[::-1].translate(complements)

def overlaps(string1 : str, string2 : str):
    overlaps_ = []
    for i in range(1, min(len(string1), len(string2)) + 1):
        if string1[-i:] == string2[:i]:
            overlaps_.append(i)
    return overlaps_

def create_graph(strings : list):
    graph = MultiGraph()
    size = len(strings)
    complements = [complement(string_) for string_ in strings]
    # Adding strings as nodes
    for string_, complement_ in zip(strings, complements):
        graph.add_node(string_)
        graph.add_node(complement_)
    # Calculating overlaps and adding as edges
    for node1 in strings:
        for node2 in strings:
            if node1 != node2:
                overlaps_ = overlaps(node1, node2)
                for key, overlap_ in enumerate(overlaps_):
                    graph.add_edge(node1, node2, key=key, weight=overlap_)
        
        for node2 in complements:
            if node1 != complement(node2):
                overlaps_ = overlaps(node1, node2)
                for key, overlap_ in enumerate(overlaps_):
                    graph.add_edge(node1, node2, key=key, weight=overlap_)
                overlaps_ = overlaps(node2, node1)
                for key, overlap_ in enumerate(overlaps_):
                    graph.add_edge(node2, node1, key=key, weight=overlap_)
    return graph

def shortest_common_superstring(strings : list, min_t: int = 1e9, desired_length: int = 0, threshold: int = 2):
    graph = create_graph(strings)
    all_paths = []
    for node in graph.nodes:
        all_paths += hamiltonian_paths(graph, node, min_t=min_t)
    # return all_paths
    
    results = []
    for path in all_paths:
        if len(path) != (len(strings) * 2 - 1):
            continue
        scs = path.pop(0)
        while len(path) > 0:
            weight = path.pop(0)
            string = path.pop(0)
            scs = scs + string[:weight]
        results.append(scs)

    if desired_length > 0:
        results = [result for result in results if desired_length - threshold <= len(result) <= desired_length + threshold]
    return results

def hamiltonian_paths(graph: MultiGraph, current_node: str, path: list = [], visited: set = set(), min_t: int = 1e9, all_paths: list = []):
    visited.add(current_node)
    visited.add(complement(current_node))
    path.append(current_node)

    if len(visited) == len(graph.nodes):
        all_paths.append(path[:])
    else:
        neighbors = graph.neighbors(current_node)
        for neighbor in neighbors:
            if neighbor not in visited:
                for key, edge_data in graph[current_node][neighbor].items():
                    weight = edge_data["weight"]
                    if weight >= min_t:
                        hamiltonian_paths(graph, neighbor, path + [weight], visited, min_t, all_paths)
                        

    visited.remove(current_node)
    visited.remove(complement(current_node))
    path.pop()

    return all_paths


def main():
    sequences = [
        "ATCCGTTGAAGCCGCGGGC",
        "TTAACTCGAGG",
        "TTAAGTACTGCCCG",
        "ATCTGTGTCGGG",
        "CGACTCCCGACACA",
        "CACAGATCCGTTGAAGCCGCGGG",
        "CTCGAGTTAAGTA",
        "CGCGGGCAGTACTT"
    ]
    ans = shortest_common_superstring(sequences, min_t=3, desired_length=55, threshold=1)
    for i in ans:
        print(len(i), ": ", i)

if __name__ == "__main__":
    main()