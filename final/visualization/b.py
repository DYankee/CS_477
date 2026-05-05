import pandas as pd
from collections import defaultdict, deque

# Load data
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

df = pd.read_csv(pd.io.common.StringIO(data))

# Step 1: Calculate surplus for each city
cities = set(df['city_from'].unique()) | set(df['city_to'].unique())
city_surplus = {}

for city in cities:
    from_rows = df[df['city_from'] == city]
    to_rows = df[df['city_to'] == city]
    
    if not from_rows.empty:
        demand = from_rows.iloc[0]['demand_from_MW']
        generation = from_rows.iloc[0]['generation_from_MW']
    elif not to_rows.empty:
        demand = to_rows.iloc[0]['demand_to_MW']
        generation = to_rows.iloc[0]['generation_to_MW']
    else:
        continue
    
    city_surplus[city] = generation - demand

print("=" * 70)
print("STEP 1: Calculate Net Supply/Deficit for Each City")
print("=" * 70)
print(f"{'City':<20} {'Generation':>12} {'Demand':>12} {'Surplus':>12}")
print("-" * 70)
for city, surplus in sorted(city_surplus.items(), 
                            key=lambda x: x[1], reverse=True):
    gen = 0
    dem = 0
    from_rows = df[df['city_from'] == city]
    to_rows = df[df['city_to'] == city]
    if not from_rows.empty:
        gen = from_rows.iloc[0]['generation_from_MW']
        dem = from_rows.iloc[0]['demand_from_MW']
    elif not to_rows.empty:
        gen = to_rows.iloc[0]['generation_to_MW']
        dem = to_rows.iloc[0]['demand_to_MW']
    
    print(f"{city:<20} {gen:>12} {dem:>12} {surplus:>+12}")

print("\n" + "=" * 70)
print("STEP 2: Identify NYC + Yonkers Deficit")
print("=" * 70)
nyc_deficit = abs(city_surplus['New York'])
yonkers_deficit = abs(city_surplus['Yonkers'])
total_deficit = nyc_deficit + yonkers_deficit
print(f"NYC deficit: {nyc_deficit} MW")
print(f"Yonkers deficit: {yonkers_deficit} MW")
print(f"TOTAL NEEDED: {total_deficit} MW")

# Step 3: Build bidirectional graph with capacities
graph = defaultdict(list)
for _, row in df.iterrows():
    graph[row['city_from']].append(
        (row['city_to'], row['capacity_MW'])
    )
    graph[row['city_to']].append(
        (row['city_from'], row['capacity_MW'])
    )

print("\n" + "=" * 70)
print("STEP 3: Analyze Direct Connections to NYC")
print("=" * 70)

targets = ['New York', 'Yonkers']
direct_supply = 0

for target in targets:
    print(f"\nDirect connections to {target}:")
    print(f"{'From City':<20} {'Surplus':>12} {'Capacity':>12} "
          f"{'Can Supply':>12}")
    print("-" * 70)
    
    for neighbor, capacity in graph[target]:
        surplus = city_surplus.get(neighbor, 0)
        if surplus > 0:
            can_supply = min(surplus, capacity)
            direct_supply += can_supply
            print(f"{neighbor:<20} {surplus:>12} {capacity:>12} "
                  f"{can_supply:>12}")

print(f"\nTotal from direct connections: {direct_supply} MW")

# Step 4: Analyze indirect paths (1-hop through intermediaries)
print("\n" + "=" * 70)
print("STEP 4: Analyze Indirect Paths (Through Intermediaries)")
print("=" * 70)

# Cities directly connected to NYC
nyc_neighbors = set(n for n, _ in graph['New York'])
indirect_supply = 0

# Find cities 2 hops away from NYC that have surplus
for intermediate in nyc_neighbors:
    if city_surplus.get(intermediate, 0) >= 0:  # Self-sufficient
        for supplier, cap_to_intermediate in graph[intermediate]:
            if supplier not in nyc_neighbors and supplier != 'New York':
                supplier_surplus = city_surplus.get(supplier, 0)
                if supplier_surplus > 0:
                    # Find capacity from intermediate to NYC
                    cap_to_nyc = next(
                        (c for n, c in graph[intermediate] 
                         if n == 'New York'), 0
                    )
                    
                    flow = min(supplier_surplus, cap_to_intermediate, 
                              cap_to_nyc)
                    if flow > 0:
                        print(f"{supplier} → {intermediate} → NYC: "
                              f"{flow} MW (limited by "
                              f"min({supplier_surplus}, {cap_to_intermediate}, "
                              f"{cap_to_nyc}))")
                        indirect_supply += flow

print(f"\nAdditional from indirect paths: {indirect_supply} MW")

print("\n" + "=" * 70)
print("FINAL ANSWER")
print("=" * 70)
total_available = direct_supply + indirect_supply
print(f"Total power needed: {total_deficit} MW")
print(f"Total power available: {total_available} MW")
print(f"Difference: {total_available - total_deficit:+} MW")

if total_available >= total_deficit:
    print("\n✓ YES - NYC + Yonkers CAN be supplied with enough power!")
else:
    print("\n✗ NO - Insufficient power available")
    print(f"  Shortfall: {total_deficit - total_available} MW")