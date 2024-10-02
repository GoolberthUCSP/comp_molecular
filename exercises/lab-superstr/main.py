from networkx import MultiDiGraph
from graphviz import Digraph

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
    graph = MultiDiGraph()
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
                for overlap_ in overlaps_:
                    graph.add_edge(node1, node2, weight=overlap_)
        
        for node2 in complements:
            if node2 != complement(node1):
                overlaps_ = overlaps(node1, node2)
                for overlap_ in overlaps_:
                    graph.add_edge(node1, node2, weight=overlap_)
                overlaps_ = overlaps(node2, node1)
                for overlap_ in overlaps_:
                    graph.add_edge(node2, node1, weight=overlap_)
    return graph

def shortest_common_superstring(strings : list, min_t: int = 1e9, desired_length: int = 0, max_threshold: int = 0):
    graph = create_graph(strings)
    all_paths = []
    for node in graph.nodes:
        all_paths += hamiltonian_paths(graph, node, min_t=min_t)
    
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
    results_ = []
    if desired_length > 0:
        for i in range(max_threshold):
            results_ = [result for result in results if desired_length - i <= len(result) <= desired_length + i]
            if len(results_) > 0:
                break
    results_.sort(key=len)

    draw_graph(graph=graph)
    return results_

def hamiltonian_paths(graph: MultiDiGraph, current_node: str, path: list = [], visited: set = set(), min_t: int = 1e9, all_paths: list = []):
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

def draw_graph(graph: MultiDiGraph):
    dot = Digraph(strict=True)
    dot.attr(rankdir='RL')
    dot.attr('node', shape='box')
    for node in graph.nodes:
        dot.node(node)
    for node1, node2, data in graph.edges(data=True):
        dot.edge(node1, node2, label=str(data['weight']))
    dot.render(filename="tmp", cleanup=True)

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
    ans = shortest_common_superstring(sequences, min_t=2, desired_length=55, max_threshold=3)
    for i in ans:
        print(len(i), ": ", i)

if __name__ == "__main__":
    main()