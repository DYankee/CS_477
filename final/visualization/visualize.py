import pandas as pd
import networkx as nx
import plotly.graph_objects as go
import matplotlib.pyplot as plt
from matplotlib.patches import FancyArrowPatch, ArrowStyle
from io import StringIO

# Data as string
data = """city_from,city_to,demand_from_MW,generation_from_MW,demand_to_MW,generation_to_MW,distance_km,capacity_MW
New York,Buffalo,9000,7000,1800,2200,600,2500
New York,Rochester,9000,7000,1500,1800,540,2200
New York,Albany,9000,7000,1200,1500,240,2000
Albany,Syracuse,1200,1500,1300,1600,220,1800
Syracuse,Buffalo,1300,1600,1800,2200,240,2000
Rochester,Buffalo,1500,1800,1800,2200,120,1700
Albany,Rochester,1200,1500,1500,1800,360,1600
Yonkers,New York,1100,900,9000,7000,30,3000
Yonkers,White Plains,1100,900,800,600,25,1200
White Plains,New York,800,600,9000,7000,40,1500
Utica,Syracuse,700,900,1300,1600,90,1400
Utica,Albany,700,900,1200,1500,150,1300
Binghamton,Syracuse,600,800,1300,1600,110,1200
Binghamton,Albany,600,800,1200,1500,220,1100
Ithaca,Binghamton,500,700,600,800,80,900
Ithaca,Syracuse,500,700,1300,1600,90,1000
Troy,Albany,400,600,1200,1500,10,800
Schenectady,Albany,450,650,1200,1500,25,900
Saratoga Springs,Albany,350,500,1200,1500,50,700
Rome,Utica,300,450,700,900,25,600
Watertown,Syracuse,400,700,1300,1600,110,800
Watertown,Utica,400,700,700,900,140,700
Niagara Falls,Buffalo,600,2500,1800,2200,30,2000
Elmira,Binghamton,350,500,600,800,60,600
Jamestown,Buffalo,300,400,1800,2200,110,700"""

# Load data
df = pd.read_csv(StringIO(data))

# Create UNDIRECTED graph for bidirectional power flow
G = nx.Graph()

# Add undirected edges with attributes
for _, row in df.iterrows():
    G.add_edge(
        row['city_from'],
        row['city_to'],
        capacity=row['capacity_MW'],
        distance=row['distance_km']
    )

# Calculate node attributes
node_attrs = {}
for city in G.nodes():
    from_data = df[df['city_from'] == city]
    to_data = df[df['city_to'] == city]
    
    demand = (
        from_data['demand_from_MW'].iloc[0]
        if len(from_data) > 0
        else to_data['demand_to_MW'].iloc[0]
        if len(to_data) > 0
        else 0
    )
    generation = (
        from_data['generation_from_MW'].iloc[0]
        if len(from_data) > 0
        else to_data['generation_to_MW'].iloc[0]
        if len(to_data) > 0
        else 0
    )
    
    node_attrs[city] = {
        'demand': demand,
        'generation': generation,
        'balance': generation - demand
    }

nx.set_node_attributes(G, node_attrs)


# Geographic positions (approximate lat/lon scaled)
# Using relative coordinates that preserve NY State geography
def get_geographic_positions():
    positions = {
        # Downstate/NYC Metro
        'New York': (0.85, 0.10),
        'Yonkers': (0.83, 0.15),
        'White Plains': (0.87, 0.18),
        
        # Capital Region
        'Albany': (0.80, 0.50),
        'Troy': (0.82, 0.52),
        'Schenectady': (0.76, 0.49),
        'Saratoga Springs': (0.78, 0.60),
        
        # Central NY
        'Syracuse': (0.50, 0.55),
        'Utica': (0.65, 0.58),
        'Rome': (0.62, 0.60),
        
        # Western NY
        'Buffalo': (0.05, 0.52),
        'Rochester': (0.20, 0.55),
        'Niagara Falls': (0.02, 0.58),
        
        # Southern Tier
        'Binghamton': (0.65, 0.30),
        'Elmira': (0.52, 0.32),
        'Ithaca': (0.55, 0.40),
        'Jamestown': (0.08, 0.35),
        
        # North Country
        'Watertown': (0.42, 0.75),
    }
    return positions


# Interactive Plotly Visualization with bidirectional arrows
def create_interactive_visualization(G, df):
    pos = get_geographic_positions()
    
    # Create edges with bidirectional arrows
    edge_traces = []
    edge_hover_traces = []
    annotations = []
    
    for edge in G.edges():
        x0, y0 = pos[edge[0]]
        x1, y1 = pos[edge[1]]
        capacity = G.edges[edge]['capacity']
        distance = G.edges[edge]['distance']
        
        # Edge line (no hover on the line itself)
        edge_trace = go.Scatter(
            x=[x0, x1, None],
            y=[y0, y1, None],
            mode='lines',
            line=dict(width=capacity / 200, color='#888'),
            hoverinfo='skip',
            showlegend=False
        )
        edge_traces.append(edge_trace)
        
        # Invisible hover points along the edge for better hover detection
        mid_x = (x0 + x1) / 2
        mid_y = (y0 + y1) / 2
        
        hover_trace = go.Scatter(
            x=[mid_x],
            y=[mid_y],
            mode='markers',
            marker=dict(size=15, color='rgba(0,0,0,0)', opacity=0),
            hoverinfo='text',
            text=f"<b>{edge[0]} ↔ {edge[1]}</b><br>"
                 f"<b>Capacity: {capacity} MW</b><br>"
                 f"Distance: {distance} km<br>"
                 f"Power Flow: Bidirectional",
            showlegend=False
        )
        edge_hover_traces.append(hover_trace)
        
        # Arrow annotation for forward direction (65% along edge)
        arrow_x_fwd = x0 + 0.65 * (x1 - x0)
        arrow_y_fwd = y0 + 0.65 * (y1 - y0)
        
        annotations.append(
            dict(
                x=arrow_x_fwd,
                y=arrow_y_fwd,
                ax=x0 + 0.55 * (x1 - x0),
                ay=y0 + 0.55 * (y1 - y0),
                xref='x',
                yref='y',
                axref='x',
                ayref='y',
                showarrow=True,
                arrowhead=2,
                arrowsize=1.5,
                arrowwidth=2,
                arrowcolor='#555',
            )
        )
        
        # Arrow annotation for backward direction (35% along edge)
        arrow_x_bwd = x0 + 0.35 * (x1 - x0)
        arrow_y_bwd = y0 + 0.35 * (y1 - y0)
        
        annotations.append(
            dict(
                x=arrow_x_bwd,
                y=arrow_y_bwd,
                ax=x0 + 0.45 * (x1 - x0),
                ay=y0 + 0.45 * (y1 - y0),
                xref='x',
                yref='y',
                axref='x',
                ayref='y',
                showarrow=True,
                arrowhead=2,
                arrowsize=1.5,
                arrowwidth=2,
                arrowcolor='#555',
            )
        )
    
    # Create nodes
    node_x, node_y, node_text, node_color, node_size = [], [], [], [], []
    for node in G.nodes():
        x, y = pos[node]
        node_x.append(x)
        node_y.append(y)
        attrs = G.nodes[node]
        node_text.append(
            f"<b>{node}</b><br>"
            f"Demand: {attrs['demand']} MW<br>"
            f"Generation: {attrs['generation']} MW<br>"
            f"Balance: {attrs['balance']:+d} MW"
        )
        node_color.append(attrs['balance'])
        node_size.append(max(20, attrs['demand'] / 150))
    
    node_trace = go.Scatter(
        x=node_x,
        y=node_y,
        mode='markers+text',
        text=[node for node in G.nodes()],
        textposition='top center',
        hoverinfo='text',
        hovertext=node_text,
        marker=dict(
            showscale=True,
            colorscale='RdYlGn',
            color=node_color,
            size=node_size,
            colorbar=dict(
                title="Power Balance (MW)",
                thickness=15,
                xanchor='left'
            ),
            line=dict(width=2, color='white')
        )
    )
    
    fig = go.Figure(
        data=edge_traces + edge_hover_traces + [node_trace],
        layout=go.Layout(
            title='New York State Power Transmission Network<br>'
                  '<sub>Geographically accurate | Bidirectional power flow | '
                  'Hover over lines to see capacity</sub>',
            showlegend=False,
            hovermode='closest',
            xaxis=dict(showgrid=False, zeroline=False, showticklabels=False),
            yaxis=dict(showgrid=False, zeroline=False, showticklabels=False),
            annotations=annotations,
            width=1400,
            height=900
        )
    )
    
    fig.write_html('power_network.html')
    print("Interactive visualization saved as 'power_network.html'")
    return fig


# Static Matplotlib Visualization with bidirectional indication
def create_static_visualization(G, df):
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18, 8))
    
    # Network graph with geographic positions
    pos = get_geographic_positions()
    
    # Draw edges (undirected, so no arrows needed - lines represent bidirectional)
    edges = G.edges()
    capacities = [G.edges[e]['capacity'] for e in edges]
    
    # Draw edges
    nx.draw_networkx_edges(
        G,
        pos,
        width=[c / 300 for c in capacities],
        alpha=0.5,
        ax=ax1,
        edge_color='#888'
    )
    
    # Add edge labels showing capacity
    edge_labels = {(u, v): f"{G.edges[u, v]['capacity']}" 
                   for u, v in G.edges()}
    nx.draw_networkx_edge_labels(
        G, 
        pos, 
        edge_labels, 
        font_size=6, 
        ax=ax1
    )
    
    # Draw nodes colored by power balance
    node_colors = [G.nodes[n]['balance'] for n in G.nodes()]
    node_sizes = [max(100, G.nodes[n]['demand'] / 15) for n in G.nodes()]
    
    nodes = nx.draw_networkx_nodes(
        G,
        pos,
        node_color=node_colors,
        node_size=node_sizes,
        cmap=plt.cm.RdYlGn,
        vmin=min(node_colors),
        vmax=max(node_colors),
        ax=ax1
    )
    
    nx.draw_networkx_labels(G, pos, font_size=8, ax=ax1)
    
    plt.colorbar(nodes, ax=ax1, label='Power Balance (MW)')
    ax1.set_title(
        'Power Transmission Network (Geographically Accurate)\n'
        'Bidirectional | Edge labels show capacity in MW',
        fontsize=11
    )
    ax1.set_aspect('equal')
    ax1.axis('off')
    
    # Bar chart of demand vs generation
    cities = list(G.nodes())
    demands = [G.nodes[c]['demand'] for c in cities]
    generations = [G.nodes[c]['generation'] for c in cities]
    
    x = range(len(cities))
    width = 0.35
    
    ax2.bar(
        [i - width / 2 for i in x],
        demands,
        width,
        label='Demand',
        alpha=0.8
    )
    ax2.bar(
        [i + width / 2 for i in x],
        generations,
        width,
        label='Generation',
        alpha=0.8
    )
    
    ax2.set_xlabel('City')
    ax2.set_ylabel('Power (MW)')
    ax2.set_title('Demand vs Generation by City')
    ax2.set_xticks(x)
    ax2.set_xticklabels(cities, rotation=45, ha='right')
    ax2.legend()
    ax2.grid(axis='y', alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('power_network_static.png', dpi=300, bbox_inches='tight')
    print("Static visualization saved as 'power_network_static.png'")
    plt.show()


# Generate visualizations
create_interactive_visualization(G, df)
create_static_visualization(G, df)

# Print summary statistics
print("\nNetwork Statistics:")
print(f"Total cities: {G.number_of_nodes()}")
print(f"Total transmission lines (bidirectional): {G.number_of_edges()}")
print(f"Total system demand: {sum(n['demand'] for _, n in G.nodes(data=True))} MW")
print(
    f"Total system generation: "
    f"{sum(n['generation'] for _, n in G.nodes(data=True))} MW"
)