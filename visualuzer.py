import graphviz

# Function to read states, accepting states, and transitions from a file
def read_nfa(file_path):
    states = set()
    accepting_states = set()
    transitions = []

    with open(file_path, 'r') as file:
        for line in file:
            parts = line.strip().split(',')
            if len(parts) == 3:
                src, dest, label = parts
                states.add(src)
                states.add(dest)
                transitions.append((src, dest, label))
            elif line.startswith('Accepting States:'):
                accepting_states = set(line.split(':')[1].strip().split(' '))
            elif line.startswith('States:'):
                states = set(line.split(':')[1].strip().split(' '))


    return states, accepting_states, transitions

# Read the NFA from the file
file_path = 'nfa.txt'
states, accepting_states, transitions = read_nfa(file_path)

# Create a directed graph for the automaton
dot = graphviz.Digraph(format='png', engine='dot')
dot.attr(rankdir='LR', size='30,30', dpi='1080', splines='true', nodesep='1', ranksep='2')  # Adjust size, resolution, and layout
# Add states to the graph
for state in states:
    if state in accepting_states:
        dot.node(state, state, shape='doublecircle', style='filled', color='lightgrey')  # Accepting state
    else:
        dot.node(state, state, shape='circle')

# Add transitions to the graph
for src, dest, label in transitions:
    dot.edge(src, dest, label)

# Save and render the graph
output_path = "./automaton"
dot.render(output_path, format="png", cleanup=True)
output_path + ".png"