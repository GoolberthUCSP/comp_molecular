import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from graphviz import Digraph
from random import choices

protein_alpha = ['A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y', '-']
alpha_idx = {protein_alpha[i]: i for i in range(len(protein_alpha))}

def preprocess(data):
    data = data.replace('\t', '')
    data = data.replace(' ', '')
    data = data.split('\n')
    try:
        cols = len(data[0])
        for i in range(1, len(data)):
            assert len(data[i]) == cols
    except AssertionError:
        raise ValueError('The sequences must have the same length')
    
    data = np.array([[x for x in y] for y in data])
    rows, cols = data.shape
    
    for col in range(cols - 1, -1, -1):
        tmp = data[:, col]
        n_hyphens = np.count_nonzero(tmp == '-')
        # Delete the actual column if it has more than the 50% of hyphens
        if n_hyphens >= int(0.5 * rows):
            data = np.delete(data, col, axis=1) # Delete the column
    return data

def calc_score(data):
    rows, cols = data.shape
    alpha_size = len(protein_alpha)
    propensity = np.zeros((alpha_size, cols))
    for j in range(cols):
        tmp = data[:, j]
        for i in range(alpha_size):
            propensity[i, j] = np.count_nonzero(tmp == protein_alpha[i])
    # Zero frequence
    b = .00001
    propensity = ((propensity + b) / (rows + alpha_size * b)) / (1 / alpha_size)
    score = np.log10(propensity)
    return score

def HMM_profile_analisys(data, score):
    rows, cols = data.shape
    graph = nx.DiGraph(comment='HMM Profile Analysis')
    
    # Add nodes
    graph.add_node('BEG')
    graph.add_node('END')
    graph.add_node('I0')
    for col in range(cols):
        graph.add_node(f'M{col+1}')  # Match state
        graph.add_node(f'I{col+1}')  # Insertion state
        graph.add_node(f'D{col+1}')  # Deletion state
        # TODO: Add probabilities to the Match states
    # Add edges
    for col in range(cols + 1):
        if col == 0:
            graph.add_edge('BEG', 'I0')
            graph.add_edge('BEG', 'M1')
            graph.add_edge('BEG', 'D1')

            graph.add_edge('I0', 'I0')
            graph.add_edge('I0', 'M1')
            graph.add_edge('I0', 'D1')
        elif col == cols:
            graph.add_edge(f'M{col}', 'END')
            graph.add_edge(f'M{col}', f'I{col}')

            graph.add_edge(f'I{col}', 'END')
            graph.add_edge(f'I{col}', f'I{col}')
            graph.add_edge(f'I{col}', f'D{col}')

            graph.add_edge(f'D{col}', 'END')
        else:
            graph.add_edge(f'M{col}', f'M{col+1}')
            graph.add_edge(f'M{col}', f'D{col+1}')
            graph.add_edge(f'M{col}', f'I{col}')

            graph.add_edge(f'I{col}', f'I{col}')
            graph.add_edge(f'I{col}', f'D{col}')
            graph.add_edge(f'I{col}', f'D{col+1}')
            graph.add_edge(f'I{col}', f'M{col+1}')
            
            graph.add_edge(f'D{col}', f'D{col+1}')
            graph.add_edge(f'D{col}', f'M{col+1}')
        # TODO: Add probabilities to the edges
    return graph

def plot_graph(graph, graphviz=False):
    if graphviz:
        dot = Digraph(comment='HMM Profile Analysis')
        for node in graph.nodes:
            dot.node(node)
        for edge in graph.edges:
            dot.edge(edge[0], edge[1])
        dot.render('HMM Profile Analysis', format='png', view=True, cleanup=True)
    else:
        pos = nx.spring_layout(graph)  # Positions for all nodes
        nx.draw(graph, pos, with_labels=True, node_color='lightblue', node_size=2000, font_size=10)
        edge_labels = nx.get_edge_attributes(graph, 'weight')
        nx.draw_networkx_edge_labels(graph, pos, edge_labels=edge_labels)
        plt.title("DiGraph with Weighted Edges")
        plt.show()

def generate_sequence(graph : nx.DiGraph):
    current = 'BEG'
    sequence = str()
    while current != 'END':
        next_states = list(graph.successors(current))
        if current[0] == 'M':
            pass
        elif current[0] == 'I':
            pass
        else:
            pass

if __name__ == '__main__':
    input = """VGA--HAGEY
                V----NVDEV
                VEA--DVAGH
                VKG------D
                VYS--TYETS
                FNA--NIPKH
                IAGADNGAGY"""
    preprocessed = preprocess(input)
    score = calc_score(preprocessed)
    graph = HMM_profile_analisys(preprocessed, score)
    plot_graph(graph)