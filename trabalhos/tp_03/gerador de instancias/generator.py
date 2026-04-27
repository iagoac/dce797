#!/usr/bin/env python3

import networkx as nx # Import networkx for graph operations
import sys  # Import sys for command line arguments and standard output
import random # Import random for random number generation
import math # Import math for computing the Euclidean distance between nodes
import secrets # Import secrets for secure random seed generation

def udg_r_graph(n, radius) -> nx.Graph:
    """
    Generate a random geometric graph (UDG) with n nodes randomly placed in a 100x100 plane.
    Edges are created between nodes that are within the specified radius.
    This is different from the built-in networkx random_geometric_graph function,
    as it places nodes randomly instead of uniformly.

    Args:
        n (int): Number of nodes.
        radius (float): Distance threshold for edge creation.

    Returns:
        networkx.Graph: Generated UDG graph.
    """

    # Generates an empty graph with n nodes
    G = nx.empty_graph(n)

    # Randomly place nodes in [0,100] x [0,100] plane
    for i in range(n):
            x = random.random()
            y = random.random()
            G.nodes[i]['pos'] = (x, y)  
    
    # Add edges based on the radius threshold
    for i in range(n):
        for j in range(i + 1, n):
            if (math.dist(G.nodes[i]['pos'], G.nodes[j]['pos']) <= radius):
                G.add_edge(i, j)

    return G

GRAPH_GENERATORS = {
    "complete": {
        "function": nx.complete_graph,
        "args": ["n"],
        "types": ["int"],
        "meanings": ["number of nodes"]
    },
    "erdos": {
        "function": nx.erdos_renyi_graph,
        "args": ["n", "p"],
        "types": ["int", "float"],
        "meanings": ["number of nodes", "probability of edge creation"]
    },
    "watts": {
        "function": lambda n, k, p: nx.connected_watts_strogatz_graph(n, k, p, 100),
        "args": ["n", "k", "p"],
        "types": ["int", "int", "float"],
        "meanings": ["number of nodes", "each node is joined with its k nearest neighbors in a ring topology", "probability of rewiring each edge"]
    },
    "barabasi": {
        "function": nx.barabasi_albert_graph,
        "args": ["n", "m"],
        "types": ["int", "int"],
        "meanings": ["number of nodes", "number of edges to attach from a new node to existing nodes"]  
    },
    "turan": {
        "function": nx.turan_graph,
        "args": ["n", "r"],
        "types": ["int", "int"],
        "meanings": ["number of nodes", "number of partitions"]
    },
    "powerlaw": {
        "function": nx.powerlaw_cluster_graph,
        "args": ["n", "m", "p"],
        "types": ["int", "int", "float"],
        "meanings": ["number of nodes", "number of edges to attach from a new node to existing nodes", "probability of forming a triangle after adding a random edge"]
    },
    "regular": {
        "function": lambda n, d: nx.random_regular_graph(d, n),
        "args": ["n", "d"],
        "types": ["int", "int"],
        "meanings": ["number of nodes", "degree of each node"]
    },
    "udg_u": {
        "function": lambda n, radius: nx.random_geometric_graph(n, radius/100, 2),
        "args": ["n", "radius"],
        "types": ["int", "float"],
        "meanings": ["number of nodes", "radius threshold value"]
    },
    "udg_r": {
        "function": udg_r_graph,
        "args": ["n", "radius"],
        "types": ["int", "float"],
        "meanings": ["number of nodes", "radius threshold value"]
    }
}


def verify_argument_errors(graph_topology, args):
    """
    Verify if the provided arguments match the expected types for the given graph topology.
    
    Args:
        graph_topology (str): Type of graph to generate.
        args (list): List of arguments provided for graph generation.
    """
    generator_info = GRAPH_GENERATORS[graph_topology]
    required_types = generator_info["types"]
    
    for i, arg in enumerate(args):
        expected_type = required_types[i + 1]  # Skip 'n'
        arg_name = generator_info["args"][i + 1]
        if expected_type == "int":
            if not (isinstance(arg, float) and arg.is_integer()):
                print(f"Error: Argument '{arg_name}' must be an integer, got {arg}")
                sys.exit(1)
        # For float, no additional check needed as it's already a number

    
def generate_graph_topology(graph_topology, n, seed, *args):
    """
    Generate different types of graphs based on the graph_topology using networkx.
    
    Args:
        graph_topology (str): Type of graph to generate.
        n (int): Number of nodes.
        seed (float): Seed for the random number generator.
        *args: Additional arguments for the graph generator.
    """
    random.seed(seed)

    generator_info = GRAPH_GENERATORS[graph_topology]
    required_args = generator_info["args"]
    required_types = generator_info["types"]
    
    # The first argument is always n, so we expect len(required_args) - 1 additional arguments
    if len(args) < len(required_args) - 1:
        expected = [f"{arg} ({typ})" for arg, typ in zip(required_args, required_types)]
        expected_meanings = [f"{arg}: {meaning}" for arg, meaning in zip(required_args, generator_info["meanings"])]
        print(f"Wrong arguments given for graph type '{graph_topology}'. Expected: {', '.join(expected)}\n\nwhere\n")
        for meaning in expected_meanings:
            print(f"  - {meaning}")
        sys.exit(1)

    # Verify argument types
    verify_argument_errors(graph_topology, args)

    # Prepare the arguments for the function call
    kwargs = {'n': n}
    for i, arg_name in enumerate(required_args[1:]):
        arg_value = args[i]
        if isinstance(arg_value, float) and arg_value.is_integer():
            kwargs[arg_name] = int(arg_value)
        else:
            kwargs[arg_name] = arg_value
        
    return generator_info["function"](**kwargs)
        
        
def generate_graph_costs(G, n, seed):
    """
    Generate a complete undirected graph with n nodes using networkx.
    Nodes are placed randomly in a 100x100 plane, and each edge has:
    - w1, w2, w3: random integers 1-100
    - euclidean: Euclidean distance (rounded to 3 decimals)
    - class: random integer 1-10

    Args:
        G (networkx.Graph): The graph to which costs will be added.
        n (int): Number of nodes.
        seed (float): Seed for the random number generator.

    Returns:
        networkx.Graph: Complete undirected graph with multiple edge attributes.
    """

    random.seed(seed)

    # Generate random positions in [0,100] x [0,100] plane
    # If graph has no positions, generate them
    if 'pos' not in G.nodes[0]:
        for i in range(n):
            x = random.random() * 100
            y = random.random() * 100
            G.nodes[i]['pos'] = (x, y)  
    # Else, if graph already has positions, scale them to [0,100]
    # Only for Uniform UDG graphs
    else:
        for i in range(n):
            x, y = G.nodes[i]['pos']
            G.nodes[i]['pos'] = (100*x, 100*y)

    # Generates a class for every node
    for i in range(n):
        G.nodes[i]['class'] = random.randint(1, 10)

    # Set edge weights as Euclidean distances and additional random attributes
    for u, v in G.edges():
        G[u][v]['w1'] = random.randint(1, 100)
    
    return G

def print_graph(G):
    """
    Print the graph G in a readable format.
    
    Args:
        G (networkx.Graph): The graph to print.
    """
    # Print the number of nodes and number of edges
    print(G.number_of_nodes(), G.number_of_edges())

    # Manually flush the buffer to ensure immediate display
    # This is necessary because nx.write_edgelist uses a buffered writer
    # Without this, the write_edgelist output may be written before the previous prints
    sys.stdout.flush() 

    # Print the edge list with attributes
    nx.write_edgelist(G, sys.stdout.buffer, data=['w1'])  # Output edgelist to terminal

def main():
    # Verify the minimum number of command line arguments is provided (at least graph type and number of nodes)
    if len(sys.argv) < 3:
        print("Error: Not enough arguments provided.")
        print("Usage: ./generator.py <graph_topology> <number_of_nodes> [additional_arguments...]")      
        valid_cases = ", ".join(GRAPH_GENERATORS.keys())
        print(f"Valid graph topologies are: {valid_cases}")
        sys.exit(1)    

    # Get the graph type from command line argument
    graph_topology = sys.argv[1]

    # Verify if the graph type is valid
    if graph_topology not in GRAPH_GENERATORS:
        valid_cases = ", ".join(GRAPH_GENERATORS.keys())
        print(f"Unknown graph type: {graph_topology}. Valid options are: {valid_cases}")
        sys.exit(1)

    # Get number of nodes from command line argument
    n = int(sys.argv[2])
    
    # Get additional arguments
    additional_args = [float(arg) for arg in sys.argv[3:]]
    
    # Generate the desired connected graph and print it on stdout
    while True:
        # Sets the seed for random number generation using a secure random integer
        seed = secrets.token_hex(32)
        
        G = generate_graph_topology(graph_topology, n, seed, *additional_args)
        if nx.is_connected(G):
            G = generate_graph_costs(G, n, seed)
            break
    
    # Print the graph on stdout
    print_graph(G)

if __name__ == "__main__":
    main()