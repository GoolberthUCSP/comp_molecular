from math import log
from graphviz import Graph
from utils import *

class PhylogeneticTree:
    def __init__(self, name, sequences):
        self.name = name
        self.sequences = sequences
    
    def calc_distance(self, a, b):
        solution = pairwise_alignment(a, b)
        seq1, seq2 = solution.first, solution.second
        num_differences = 0
        for i in range(len(seq1)):
            if seq1[i] != seq2[i]:
                num_differences += 1
                
        return -.75 * log(max(0.0001, 1 - (4/3) * (num_differences / len(seq1))))
    
    def get(self, i, j):
        if (i, j) in self.dist_mat:
            return self.dist_mat[(i, j)]
        elif (j, i) in self.dist_mat:
            return self.dist_mat[(j, i)]
        else:
            return 0

    def nodes(self):
        nodes = set()
        for key, val in self.dist_mat.items():
            nodes.add(key[0])
            nodes.add(key[1])
            
        return list(nodes)
    
    def gen_dist_mtx(self):
        self.dist_mat = {}
        for i in range(len(self.sequences)):
            for j in range(i+1, len(self.sequences)):
                self.dist_mat[(i+1, j+1)] = self.calc_distance(self.sequences[i], self.sequences[j])

    def distance(self, a, b):
        raise NotImplementedError('abstract class')
        
    def update(self, node, pair):
        raise NotImplementedError('abstract class')
        
    def plot(self):
        dot = Graph(self.name)
        dot.attr(rankdir='LR')
        dot.attr('graph', fontname='Courier')
        dot.attr('node', fontname='Courier', fontsize='7pt', shape='plain')
        dot.attr('edge', fontname='Courier', fontsize='7pt')
        dot.attr('node', shape='plain')
        
        self.gen_dist_mtx()
        diff_mat = {}
        for i in range(len(self.sequences)):
            dot.node(str(i+1), label=f'{i+1}: {self.sequences[i]}')
            diff_mat[i+1] = 0
            
        nodes_ord = ' '.join(map(str, range(1, len(self.sequences)+1)))
        dot.body.append(f'\t{{ rank=same; {nodes_ord} }}\n')
        dot.attr('node', shape='point')
        
        while len(self.dist_mat.keys()) > 0:
            min_pair = None
            dist_pair = 10**18
            nodes = self.nodes()
            for i in range(len(nodes)):
                for j in range(i+1, len(nodes)):
                    if self.distance(nodes[i], nodes[j]) < dist_pair:
                        min_pair = (nodes[i], nodes[j])
                        dist_pair = self.distance(nodes[i], nodes[j])
        
            nodes.remove(min_pair[0])
            nodes.remove(min_pair[1])
            for node in nodes:
                self.dist_mat[(node, min_pair)] = self.new_distance(node, min_pair)
                    
            #graficar aristas
            diff_mat[min_pair] = self.get(*min_pair) / 2
            dot.node(str(min_pair))
            dot.edge(str(min_pair[0]), str(min_pair), label=f'{diff_mat[min_pair]- diff_mat[min_pair[0]]:.2f}')
            dot.edge(str(min_pair[1]), str(min_pair), label=f'{diff_mat[min_pair] - diff_mat[min_pair[1]]:.2f}')
            
            #eliminar los pares de la matriz de distancias
            for key in list(self.dist_mat.keys()):
                if key[0] in min_pair or key[1] in min_pair:
                    del self.dist_mat[key]
            
        dot.render(directory='.', view=True, filename=self.name, format='png')

class UPGMATree(PhylogeneticTree):
    def __init__(self, name, sequences):
        super().__init__(name, sequences)
    
    def distance(self, a, b):
        return self.get(a, b)
        
    def new_distance(self, node, pair):
        return (self.get(node, pair[0]) + self.get(node, pair[1])) / 2

class NeighborJoiningTree(PhylogeneticTree):
    def __init__(self, name, sequences):
        super().__init__(name, sequences)
    
    def distance(self, a, b):
        nodes = self.nodes()
        a_mean_dist, b_mean_dist = 0, 0
        for node in nodes:
            if node != a:
                a_mean_dist += self.get(a, node)
            if node != b:
                b_mean_dist += self.get(b, node)
        
        a_mean_dist /= len(nodes) - 1
        b_mean_dist /= len(nodes) - 1
        return self.get(a, b) - a_mean_dist - b_mean_dist
        
    def new_distance(self, node, pair):
        return (self.get(node, pair[0]) + self.get(node, pair[1]) - self.get(pair[0], pair[1])) / 2

def main():
    sequences = ['ATTGCCATT', 
                 'ATGGCCATT', 
                 'ATCCAATTTT', 
                 'ATCTTCTT', 
                 'ACTGACC']
    tree = UPGMATree('test-upgma', sequences)
    tree.plot()
    tree = NeighborJoiningTree('test-neighbor', sequences)
    tree.plot()
    sequences = ['concha sifuentes', 'flores melendez', 'pino zavala', 'quispe neira', 'salas ticona', 'tupac valdivia', 'villanueva borda']
    tree = UPGMATree('lastname-upgma', sequences)
    tree.plot()
    tree = NeighborJoiningTree('lastname-nj', sequences)
    tree.plot()