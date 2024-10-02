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

def shortest_common_superstring(strings : list, min_t: int = 1e9, desired_length: int = 0):
    graph = create_graph(strings)
    all_paths = []
    global_size = sum(len(string) for string in strings)

    for node in graph.nodes:
        hamiltonian_paths(graph, node, min_t=min_t, all_paths=all_paths)
    
    results = []
    formatted_results = []
    def format(path : list, global_size : int):
        n_strings = (len(path) + 1) // 2
        overlap_sum = sum(path[i] for i in range(1, len(path), 2))
        result_size = global_size - overlap_sum
        result = path.pop(0)
        t_size = len(result)
        t_overlap = 0
        result = result.ljust(result_size, '-')
        for _ in range(n_strings - 1):
            t_overlap += path.pop(0)
            string = path.pop(0)
            x = len(string)
            string = string.rjust(t_size - t_overlap + x, '-')
            string = string.ljust(result_size, '-')
            t_size += x
            result += ('\n' + string)
        return result
    # Formatting results: SCS and formatted SCS
    for path in all_paths:
        formatted_results.append(format(path[:], global_size= global_size))
        # print(path)
        scs = path.pop(0)
        while len(path) > 0:
            overlap = path.pop(0)
            string = path.pop(0)
            scs += string[overlap:]
        results.append(scs)
    results_ = []
    formatted_results_ = []
    # Filtering results
    if desired_length > 0:
        for i in range(desired_length):
            for i in range(len(results)):
                if desired_length - i <= len(results[i]) <= desired_length + i:
                    results_.append(results[i])
                    formatted_results_.append(formatted_results[i])
                if len(results_) > 0:
                    break

    draw_graph(graph=graph)
    return results_, formatted_results_

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
                for _, edge_data in graph[current_node][neighbor].items():
                    weight = edge_data["weight"]
                    if weight >= min_t:
                        hamiltonian_paths(graph, neighbor, path + [weight], visited, min_t, all_paths)

    visited.remove(current_node)
    visited.remove(complement(current_node))
    path.pop()

def draw_graph(graph: MultiDiGraph):
    dot = Digraph()
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
    results, formatted = shortest_common_superstring(sequences, min_t=2, 
                                                     desired_length=55)
    for result, formatted_ in zip(results, formatted):
        print(f"SIZE: {len(result)} \n{formatted_}")
        print("_" * len(result))
        print(result)
        
if __name__ == "__main__":
    main()