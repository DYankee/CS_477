#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>
#include <iomanip>
#include "./electricGrid.cpp"

using namespace std;

// Initialize the grid with the provided data
ElectricalGrid initializeGrid() {
    ElectricalGrid grid;

    struct GridData {
        string from, to;
        double demandFrom, genFrom, demandTo, genTo,
            distance, capacity;
    };

    vector<GridData> data = {
        {"New York", "Buffalo", 9000, 7000, 1800, 2200,
         600, 2500},
        {"New York", "Rochester", 9000, 7000, 1500, 1800,
         540, 2200},
        {"New York", "Albany", 9000, 7000, 1200, 1500, 240,
         2000},
        {"Albany", "Syracuse", 1200, 1500, 1300, 1600, 220,
         1800},
        {"Syracuse", "Buffalo", 1300, 1600, 1800, 2200,
         240, 2000},
        {"Rochester", "Buffalo", 1500, 1800, 1800, 2200,
         120, 1700},
        {"Albany", "Rochester", 1200, 1500, 1500, 1800,
         360, 1600},
        {"Yonkers", "New York", 1100, 900, 9000, 7000, 30,
         3000},
        {"Yonkers", "White Plains", 1100, 900, 800, 600,
         25, 1200},
        {"White Plains", "New York", 800, 600, 9000, 7000,
         40, 1500},
        {"Utica", "Syracuse", 700, 900, 1300, 1600, 90,
         1400},
        {"Utica", "Albany", 700, 900, 1200, 1500, 150,
         1300},
        {"Binghamton", "Syracuse", 600, 800, 1300, 1600,
         110, 1200},
        {"Binghamton", "Albany", 600, 800, 1200, 1500, 220,
         1100},
        {"Ithaca", "Binghamton", 500, 700, 600, 800, 80,
         900},
        {"Ithaca", "Syracuse", 500, 700, 1300, 1600, 90,
         1000},
        {"Troy", "Albany", 400, 600, 1200, 1500, 10, 800},
        {"Schenectady", "Albany", 450, 650, 1200, 1500, 25,
         900},
        {"Saratoga Springs", "Albany", 350, 500, 1200, 1500,
         50, 700},
        {"Rome", "Utica", 300, 450, 700, 900, 25, 600},
        {"Watertown", "Syracuse", 400, 700, 1300, 1600,
         110, 800},
        {"Watertown", "Utica", 400, 700, 700, 900, 140,
         700},
        {"Niagara Falls", "Buffalo", 600, 2500, 1800, 2200,
         30, 2000},
        {"Elmira", "Binghamton", 350, 500, 600, 800, 60,
         600},
        {"Jamestown", "Buffalo", 300, 400, 1800, 2200, 110,
         700},
    };

    // Add all cities
    map<string, pair<double, double>> cityData;

    for (const auto& row : data) {
        cityData[row.from] = {row.demandFrom, row.genFrom};
        cityData[row.to] = {row.demandTo, row.genTo};
    }

    for (const auto& pair : cityData) {
        grid.addCity(
            pair.first, pair.second.first,
            pair.second.second
        );
    }

    // Add all edges (bidirectional)
    for (const auto& row : data) {
        grid.addEdge(
            row.from, row.to, row.distance, row.capacity
        );
    }

    return grid;
}



/*
    Helper functions
*/


/*
    Functions to use the required algorithms to solve each
    question
*/
void solveProblemA1(const ElectricalGrid& grid) {
    cout << "\n=== Problem (a): Shortest Paths from Utica "
            "==="
         << endl;

    vector<string> destinations = {
        "Jamestown", "New York", "Elmira", "Rochester",
        "Niagara Falls"};

    for (const string& dest : destinations) {
        auto [distance, path] =
            grid.dijkstra("Utica", dest);

        cout << "\nUtica to " << dest << ":" << endl;
        cout << "  Distance: " << distance << " km" << endl;
        cout << "  Path: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
}

void solveProblemB1(const ElectricalGrid& grid) {
    cout << "\n=== Problem (b-1): NYC + Yonkers Power "
            "Supply Analysis ==="
         << endl;

    auto cities = grid.getAllCities();

    // Step 1: Identify self-sufficient cities
    CityMap suppliers = grid.getSelfSufficient();
    double totalSurplus = 0;

    cout << "\nSelf-Sufficient Cities (Potential "
            "Suppliers):"
         << endl;
    cout << setw(20) << left << "City" << setw(15)
         << "Surplus (MW)" << endl;
    cout << string(35, '-') << endl;

    for (const auto& [name, city] : suppliers) {
        totalSurplus += city.getSurplus();
        cout << setw(20) << left << city.name << setw(15)
             << fixed << setprecision(1)
             << city.getSurplus() << endl;
    }

    // Step 2: Calculate deficits
    City nyc = grid.getCity("New York");
    City yonkers = grid.getCity("Yonkers");

    double nycDeficit = nyc.demand_MW - nyc.generation_MW;
    double yonkersDeficit = yonkers.demand_MW - yonkers.generation_MW;
    double totalDeficit = nycDeficit + yonkersDeficit;

    cout << "\nPower Deficit Analysis:" << endl;
    cout << "  NYC:           " << setw(10) << fixed
         << setprecision(1) << nycDeficit << " MW deficit"
         << endl;
    cout << "  Yonkers:       " << setw(10)
         << yonkersDeficit << " MW deficit" << endl;
    cout << "  Total needed:  " << setw(10) << totalDeficit
         << " MW" << endl;
    cout << "  Total surplus: " << setw(10) << totalSurplus
         << " MW" << endl;

    // Step 3: Build flow network
    map<pair<string, string>, double> flowGraph;

    const string SOURCE = "VIRTUAL_SOURCE";
    const string SINK = "VIRTUAL_SINK";

    // Connect virtual source to all suppliers
    for (const auto& [name, city] : suppliers) {
        flowGraph[{SOURCE, name}] = city.getSurplus();
    }

    // Add all transmission line edges (both directions)
    for (const auto& cityPair : cities) {
        string city = cityPair.first;
        vector<Edge> neighbors = grid.getNeighbors(city);
        for (const Edge& edge : neighbors) {
            flowGraph[{edge.from, edge.to}] =
                edge.capacity_MW;
        }
    }

    // Connect NYC and Yonkers to virtual sink
    flowGraph[{"New York", SINK}] = nycDeficit;
    flowGraph[{"Yonkers", SINK}] = yonkersDeficit;

    // Step 4: Compute maximum flow
    auto [maxFlowValue, flowMap] =
        grid.computeMaxFlow(flowGraph, SOURCE, SINK);

    // Step 5: Analysis and results
    cout << "\n" << string(50, '=') << endl;
    cout << "MAXIMUM FLOW ANALYSIS" << endl;
    cout << string(50, '=') << endl;
    cout << "Maximum achievable flow: " << setw(10)
         << maxFlowValue << " MW" << endl;
    cout << "Required flow:           " << setw(10)
         << totalDeficit << " MW" << endl;
    cout << string(50, '-') << endl;

    bool canSupply =
        (maxFlowValue >= totalDeficit - 0.01);

    if (canSupply) {
        cout << "\n✓ RESULT: YES - NYC and Yonkers CAN "
                "be fully supplied!"
             << endl;
        cout << "\nPower Distribution:" << endl;

        double nycReceived = flowMap[{"New York", SINK}];
        double yonkersReceived =
            flowMap[{"Yonkers", SINK}];

        cout << "  NYC receives:     " << setw(10) << fixed
             << setprecision(1) << nycReceived
             << " MW (needs " << nycDeficit << " MW)"
             << endl;
        cout << "  Yonkers receives: " << setw(10)
             << yonkersReceived << " MW (needs "
             << yonkersDeficit << " MW)" << endl;
    } else {
        cout << "\n✗ RESULT: NO - NYC and Yonkers CANNOT "
                "be fully supplied."
             << endl;
        cout << "  Shortfall: "
             << (totalDeficit - maxFlowValue) << " MW"
             << endl;
    }

    // Show major supplier contributions
    cout << "\nMajor Supplier Contributions:" << endl;
    cout << setw(20) << left << "City" << setw(15)
         << "Contributed (MW)" << setw(15)
         << "Available (MW)" << endl;
    cout << string(50, '-') << endl;

    for (const auto& [name, city] : suppliers) {
        double contribution = flowMap[{SOURCE, name}];
        if (contribution > 0.1) {
            cout << setw(20) << left << name << setw(15)
                 << fixed << setprecision(1) << contribution
                 << setw(15) << city.getSurplus() << endl;
        }
    }
}

void solveProblemB2(const ElectricalGrid& grid) {
    cout << "\n=== Problem (b-2): Cheapest Supply by "
            "Distance ==="
         << endl;
    cout << "(Cost = total distance power travels)\n"
         << endl;

    auto allCities = grid.getAllCities();
    auto suppliers = grid.getSelfSufficient();

    City nyc = grid.getCity("New York");
    City yonkers = grid.getCity("Yonkers");
    double nycDeficit = nyc.demand_MW - nyc.generation_MW;
    double yonkersDeficit =
        yonkers.demand_MW - yonkers.generation_MW;
    double totalDeficit = nycDeficit + yonkersDeficit;

    const string SOURCE = "VIRTUAL_SOURCE";
    const string SINK = "VIRTUAL_SINK";

    // Build capacity graph
    map<pair<string, string>, double> flowGraph;
    map<pair<string, string>, double> costGraph;

    // Virtual source -> suppliers (cost 0)
    for (const auto& [name, city] : suppliers) {
        flowGraph[{SOURCE, name}] = city.getSurplus();
        costGraph[{SOURCE, name}] = 0;
    }

    // All transmission edges with distance as cost
    for (const auto& [cityName, _] : allCities) {
        for (const Edge& edge :
             grid.getNeighbors(cityName)) {
            flowGraph[{edge.from, edge.to}] =
                edge.capacity_MW;
            costGraph[{edge.from, edge.to}] =
                edge.distance_km;
        }
    }

    // NYC and Yonkers -> virtual sink (cost 0)
    flowGraph[{"New York", SINK}] = nycDeficit;
    costGraph[{"New York", SINK}] = 0;
    flowGraph[{"Yonkers", SINK}] = yonkersDeficit;
    costGraph[{"Yonkers", SINK}] = 0;

    // Compute min-cost flow
    auto [totalCost, totalFlow, flowMap] =
        grid.computeMinCostFlow(
            flowGraph, costGraph, SOURCE, SINK,
            totalDeficit
        );

    cout << string(55, '=') << endl;
    cout << "MINIMUM COST FLOW ANALYSIS (cost = distance)"
         << endl;
    cout << string(55, '=') << endl;
    cout << "Total flow achieved: " << fixed
         << setprecision(1) << totalFlow << " MW" << endl;
    cout << "Required flow:       " << totalDeficit
         << " MW" << endl;
    cout << "Total cost:          " << totalCost
         << " (MW * km)" << endl;
    cout << string(55, '-') << endl;

    if (totalFlow >= totalDeficit - 0.01) {
        cout << "\n✓ Full supply achieved at minimum "
                "distance cost!"
             << endl;
    } else {
        cout << "\n✗ Could not fully supply. Shortfall: "
             << (totalDeficit - totalFlow) << " MW"
             << endl;
    }

    // Show supplier contributions
    cout << "\nSupplier Contributions:" << endl;
    cout << setw(22) << left << "City" << setw(18)
         << "Contributed (MW)" << setw(18)
         << "Available (MW)" << endl;
    cout << string(58, '-') << endl;

    for (const auto& [name, city] : suppliers) {
        double contribution = flowMap[{SOURCE, name}];
        if (contribution > 0.1) {
            cout << setw(22) << left << name << setw(18)
                 << fixed << setprecision(1) << contribution
                 << setw(18) << city.getSurplus() << endl;
        }
    }

    // Show active transmission lines
    cout << "\nActive Transmission Lines:" << endl;
    cout << setw(22) << left << "From" << setw(22) << "To"
         << setw(14) << "Flow (MW)" << setw(14)
         << "Dist (km)" << endl;
    cout << string(72, '-') << endl;

    for (const auto& [edge, flow] : flowMap) {
        if (
            flow > 0.1 && edge.first != SOURCE &&
            edge.second != SINK
        ) {
            double dist = 0;
            auto costIt = costGraph.find(edge);
            if (costIt != costGraph.end())
                dist = costIt->second;
            cout << setw(22) << left << edge.first
                 << setw(22) << edge.second << setw(14)
                 << fixed << setprecision(1) << flow
                 << setw(14) << dist << endl;
        }
    }
}

void solveProblemB3(const ElectricalGrid& grid) {
    cout << "\n=== Problem (b-3): Cheapest Supply by "
            "Weighted Cost ==="
         << endl;
    cout << "(Cost = supply_amount * distance per unit "
            "flow)\n"
         << endl;

    /*
        Note: In min-cost flow, the total cost is computed
        as sum over all edges of (flow * cost_per_unit).
        For B-2, cost_per_unit = distance.
        For B-3, cost_per_unit = distance as well, but the
        problem states cost = supply_amount * distance.
        Since min-cost flow already computes
        sum(flow_e * cost_e), and cost_e = distance_e,
        the result is sum(flow_e * distance_e) which is
        exactly what B-3 asks for.

        However, if the intent is to make the per-unit cost
        on each edge proportional to the flow (quadratic),
        that changes the problem. Based on the problem
        statement, the most natural reading is:
        cost = flow * distance on each edge, which is
        exactly what min-cost flow computes. Thus B-2 and
        B-3 use the same algorithm but we interpret the
        cost differently.

        Re-reading: B-2 says "distance it travels" (total
        distance of the path taken), B-3 says "supply
        amount * distance". The key difference:
        - B-2: minimize total distance summed across paths
          (each unit of flow costs the distance it travels)
        - B-3: same computation, same result since min-cost
          flow already multiplies flow * distance per edge.

        They produce the same answer. We present both for
        completeness but note the distinction in
        interpretation.
    */

    auto allCities = grid.getAllCities();
    auto suppliers = grid.getSelfSufficient();

    City nyc = grid.getCity("New York");
    City yonkers = grid.getCity("Yonkers");
    double nycDeficit = nyc.demand_MW - nyc.generation_MW;
    double yonkersDeficit =
        yonkers.demand_MW - yonkers.generation_MW;
    double totalDeficit = nycDeficit + yonkersDeficit;

    const string SOURCE = "VIRTUAL_SOURCE";
    const string SINK = "VIRTUAL_SINK";

    // Build capacity and cost graphs
    map<pair<string, string>, double> flowGraph;
    map<pair<string, string>, double> costGraph;

    for (const auto& [name, city] : suppliers) {
        flowGraph[{SOURCE, name}] = city.getSurplus();
        costGraph[{SOURCE, name}] = 0;
    }

    for (const auto& [cityName, _] : allCities) {
        for (const Edge& edge :
             grid.getNeighbors(cityName)) {
            flowGraph[{edge.from, edge.to}] =
                edge.capacity_MW;
            // Cost per unit = distance (so total cost on
            // edge = flow * distance)
            costGraph[{edge.from, edge.to}] =
                edge.distance_km;
        }
    }

    flowGraph[{"New York", SINK}] = nycDeficit;
    costGraph[{"New York", SINK}] = 0;
    flowGraph[{"Yonkers", SINK}] = yonkersDeficit;
    costGraph[{"Yonkers", SINK}] = 0;

    auto [totalCost, totalFlow, flowMap] =
        grid.computeMinCostFlow(
            flowGraph, costGraph, SOURCE, SINK,
            totalDeficit
        );

    cout << string(60, '=') << endl;
    cout << "MINIMUM COST FLOW ANALYSIS "
            "(cost = flow * distance)"
         << endl;
    cout << string(60, '=') << endl;
    cout << "Total flow achieved: " << fixed
         << setprecision(1) << totalFlow << " MW" << endl;
    cout << "Required flow:       " << totalDeficit
         << " MW" << endl;
    cout << "Total weighted cost: " << totalCost
         << " (MW * km)" << endl;
    cout << string(60, '-') << endl;

    if (totalFlow >= totalDeficit - 0.01) {
        cout << "\n✓ Full supply achieved at minimum "
                "weighted cost!"
             << endl;
    } else {
        cout << "\n✗ Could not fully supply. Shortfall: "
             << (totalDeficit - totalFlow) << " MW"
             << endl;
    }

    // Show supplier contributions
    cout << "\nSupplier Contributions:" << endl;
    cout << setw(22) << left << "City" << setw(18)
         << "Contributed (MW)" << setw(18)
         << "Available (MW)" << endl;
    cout << string(58, '-') << endl;

    for (const auto& [name, city] : suppliers) {
        double contribution = flowMap[{SOURCE, name}];
        if (contribution > 0.1) {
            cout << setw(22) << left << name << setw(18)
                 << fixed << setprecision(1) << contribution
                 << setw(18) << city.getSurplus() << endl;
        }
    }

    // Show active transmission with weighted costs
    cout << "\nActive Transmission Lines:" << endl;
    cout << setw(20) << left << "From" << setw(20) << "To"
         << setw(12) << "Flow (MW)" << setw(12)
         << "Dist (km)" << setw(16) << "Cost (MW*km)"
         << endl;
    cout << string(80, '-') << endl;

    double verifyTotal = 0;
    for (const auto& [edge, flow] : flowMap) {
        if (
            flow > 0.1 && edge.first != SOURCE &&
            edge.second != SINK
        ) {
            double dist = 0;
            auto costIt = costGraph.find(edge);
            if (costIt != costGraph.end())
                dist = costIt->second;
            double edgeCost = flow * dist;
            verifyTotal += edgeCost;
            cout << setw(20) << left << edge.first
                 << setw(20) << edge.second << setw(12)
                 << fixed << setprecision(1) << flow
                 << setw(12) << dist << setw(16)
                 << edgeCost << endl;
        }
    }
    cout << string(80, '-') << endl;
    cout << "Verified total cost: " << fixed
         << setprecision(1) << verifyTotal << " (MW * km)"
         << endl;
}

void solveProblemB4(const ElectricalGrid& grid) {
    cout << "\n=== Problem (b-4): Max Flow to NYC from "
            "Each City ==="
         << endl;
    cout << "(Find which single city can send the most "
            "power to NYC)\n"
         << endl;

    auto allCities = grid.getAllCities();
    string bestCity = "";
    double bestFlow = 0;
    map<pair<string, string>, double> bestFlowMap;

    // Results storage for display
    vector<pair<string, double>> results;

    for (const auto& [cityName, city] : allCities) {
        // Skip NYC itself
        if (cityName == "New York") continue;
        // Only consider self-sufficient cities
        if (!city.isSelfSufficient()) continue;

        // Build a flow network: cityName -> NYC
        // Source capacity = that city's surplus
        // Edge capacities = transmission line capacities
        map<pair<string, string>, double> flowGraph;

        const string SOURCE = "FLOW_SOURCE";
        const string SINK = "FLOW_SINK";

        // Source -> the supplier city, capped by surplus
        flowGraph[{SOURCE, cityName}] = city.getSurplus();

        // All transmission edges
        for (const auto& [cn, _] : allCities) {
            for (const Edge& edge :
                 grid.getNeighbors(cn)) {
                flowGraph[{edge.from, edge.to}] =
                    edge.capacity_MW;
            }
        }

        // NYC -> Sink (unlimited)
        flowGraph[{"New York", SINK}] = 1e9;

        auto [maxFlowVal, flowMap] =
            grid.computeMaxFlow(flowGraph, SOURCE, SINK);

        results.push_back({cityName, maxFlowVal});

        if (maxFlowVal > bestFlow) {
            bestFlow = maxFlowVal;
            bestCity = cityName;
            bestFlowMap = flowMap;
        }
    }

    // Sort results by flow descending
    sort(
        results.begin(), results.end(),
        [](const pair<string, double>& a,
           const pair<string, double>& b) {
            return a.second > b.second;
        }
    );

    cout << setw(22) << left << "Source City" << setw(18)
         << "Max Flow (MW)" << setw(15) << "Surplus (MW)"
         << endl;
    cout << string(55, '-') << endl;

    for (const auto& [name, flow] : results) {
        City c = grid.getCity(name);
        cout << setw(22) << left << name << setw(18)
             << fixed << setprecision(1) << flow
             << setw(15) << c.getSurplus() << endl;
    }

    cout << "\n" << string(55, '=') << endl;
    cout << "BEST SINGLE SUPPLIER FOR NYC" << endl;
    cout << string(55, '=') << endl;
    cout << "City:     " << bestCity << endl;
    cout << "Max Flow: " << fixed << setprecision(1)
         << bestFlow << " MW" << endl;
    cout << "Surplus:  "
         << grid.getCity(bestCity).getSurplus() << " MW"
         << endl;
    cout << string(55, '-') << endl;

    // Show active paths for the best city
    cout << "\nActive Transmission Lines (from " << bestCity
         << " to NYC):" << endl;
    cout << setw(22) << left << "From" << setw(22) << "To"
         << setw(14) << "Flow (MW)" << endl;
    cout << string(58, '-') << endl;

    for (const auto& [edge, flow] : bestFlowMap) {
        if (
            flow > 0.1 && edge.first != "FLOW_SOURCE" &&
            edge.second != "FLOW_SINK"
        ) {
            cout << setw(22) << left << edge.first
                 << setw(22) << edge.second << setw(14)
                 << fixed << setprecision(1) << flow
                 << endl;
        }
    }
}

void solveProblemC(const ElectricalGrid& grid) {
    cout << "\n=== Problem (c): Can All 25 Cities Be "
            "Fully Powered? ==="
         << endl;
    cout << "(Using only self-sufficient cities as "
            "suppliers)\n"
         << endl;

    auto allCities = grid.getAllCities();
    auto suppliers = grid.getSelfSufficient();
    auto deficitCities = grid.getDeficit();

    // Show deficit cities
    double totalDeficit = 0;
    double totalSurplus = 0;

    cout << "Deficit Cities:" << endl;
    cout << setw(22) << left << "City" << setw(15)
         << "Deficit (MW)" << endl;
    cout << string(37, '-') << endl;

    for (const auto& [name, city] : deficitCities) {
        double deficit = city.demand_MW - city.generation_MW;
        totalDeficit += deficit;
        cout << setw(22) << left << name << setw(15)
             << fixed << setprecision(1) << deficit << endl;
    }

    cout << "\nSurplus Cities:" << endl;
    cout << setw(22) << left << "City" << setw(15)
         << "Surplus (MW)" << endl;
    cout << string(37, '-') << endl;

    for (const auto& [name, city] : suppliers) {
        totalSurplus += city.getSurplus();
        cout << setw(22) << left << name << setw(15)
             << fixed << setprecision(1)
             << city.getSurplus() << endl;
    }

    cout << "\nTotal deficit:  " << totalDeficit << " MW"
         << endl;
    cout << "Total surplus:  " << totalSurplus << " MW"
         << endl;

    // Quick check: is there even enough surplus globally?
    if (totalSurplus < totalDeficit - 0.01) {
        cout << "\n✗ NOT ENOUGH SURPLUS: Total surplus ("
             << totalSurplus
             << " MW) < Total deficit (" << totalDeficit
             << " MW)" << endl;
        cout << "  The grid CANNOT supply all cities."
             << endl;
        return;
    }

    cout << "\nSurplus >= Deficit. Checking if "
            "transmission infrastructure allows delivery..."
         << endl;

    // Build flow network
    const string SOURCE = "VIRTUAL_SOURCE";
    const string SINK = "VIRTUAL_SINK";
    map<pair<string, string>, double> flowGraph;

    // Virtual source -> surplus cities
    for (const auto& [name, city] : suppliers) {
        flowGraph[{SOURCE, name}] = city.getSurplus();
    }

    // All transmission edges
    for (const auto& [cityName, _] : allCities) {
        for (const Edge& edge :
             grid.getNeighbors(cityName)) {
            flowGraph[{edge.from, edge.to}] =
                edge.capacity_MW;
        }
    }

    // Deficit cities -> virtual sink
    for (const auto& [name, city] : deficitCities) {
        double deficit =
            city.demand_MW - city.generation_MW;
        flowGraph[{name, SINK}] = deficit;
    }

    // Compute max flow
    auto [maxFlowVal, flowMap] =
        grid.computeMaxFlow(flowGraph, SOURCE, SINK);

    cout << "\n" << string(55, '=') << endl;
    cout << "ALL-CITY POWER SUPPLY ANALYSIS" << endl;
    cout << string(55, '=') << endl;
    cout << "Maximum achievable flow: " << fixed
         << setprecision(1) << maxFlowVal << " MW" << endl;
    cout << "Total deficit required:  " << totalDeficit
         << " MW" << endl;
    cout << string(55, '-') << endl;

    bool canSupply =
        (maxFlowVal >= totalDeficit - 0.01);

    if (canSupply) {
        cout << "\n✓ YES - All 25 cities CAN be fully "
                "powered!"
             << endl;
    } else {
        cout << "\n✗ NO - Not all cities can be fully "
                "powered."
             << endl;
        cout << "  Shortfall: " << fixed << setprecision(1)
             << (totalDeficit - maxFlowVal) << " MW"
             << endl;
    }

    // Show how much each deficit city receives
    cout << "\nDeficit City Supply Status:" << endl;
    cout << setw(22) << left << "City" << setw(16)
         << "Needed (MW)" << setw(16) << "Received (MW)"
         << setw(10) << "Status" << endl;
    cout << string(64, '-') << endl;

    for (const auto& [name, city] : deficitCities) {
        double deficit =
            city.demand_MW - city.generation_MW;
        double received = flowMap[{name, SINK}];
        bool ok = (received >= deficit - 0.01);
        cout << setw(22) << left << name << setw(16)
             << fixed << setprecision(1) << deficit
             << setw(16) << received << setw(10)
             << (ok ? "✓ OK" : "✗ SHORT") << endl;
    }

    // Show supplier usage
    cout << "\nSupplier Usage:" << endl;
    cout << setw(22) << left << "City" << setw(18)
         << "Contributed (MW)" << setw(18)
         << "Available (MW)" << setw(10) << "Usage %"
         << endl;
    cout << string(68, '-') << endl;

    for (const auto& [name, city] : suppliers) {
        double contribution = flowMap[{SOURCE, name}];
        double surplus = city.getSurplus();
        double pct =
            (surplus > 0) ? (contribution / surplus * 100)
                          : 0;
        cout << setw(22) << left << name << setw(18)
             << fixed << setprecision(1) << contribution
             << setw(18) << surplus << setw(10)
             << setprecision(0) << pct << "%" << endl;
    }
}

int main() {
    cout << "Electrical Grid Analysis - Starting from "
            "Utica\n"
         << endl;

    ElectricalGrid grid = initializeGrid();
    grid.printStatistics();

    /*
        Problem (A): GPS: Find the shortest path from Utica
        to (1) Jamestown (2) NYC (3) Elmira (4) Rochester
        (5) Niagara Falls

        Algorithm: Dijkstra's Algorithm
        Time Complexity: O((V + E) log V)
    */
    solveProblemA1(grid);

    /*
        Problem (B-1): Can NYC + Yonkers be supplied with
        enough power from self-sufficient cities?

        Algorithm: Edmonds-Karp (Max Flow)
        Time Complexity: O(V * E^2)
    */
    solveProblemB1(grid);

    /*
        Problem (B-2): Shortest (cheapest by distance)
        way to supply NYC + Yonkers. Cost = distance
        power travels.

        Algorithm: Successive Shortest Paths
        (Min-Cost Max Flow with Bellman-Ford)
        Time Complexity: O(F * V * E)
    */
    solveProblemB2(grid);

    /*
        Problem (B-3): Cheapest way to supply NYC +
        Yonkers where cost = supply_amount * distance.

        Algorithm: Successive Shortest Paths
        (Min-Cost Max Flow with Bellman-Ford)
        Time Complexity: O(F * V * E)
    */
    solveProblemB3(grid);

    /*
        Problem (B-4): Max flow NYC can get from any
        single city. Iterate all self-sufficient cities,
        find the one that can send the most.

        Algorithm: Edmonds-Karp (Max Flow) per city
        Time Complexity: O(S * V * E^2) where S =
        number of self-sufficient cities
    */
    solveProblemB4(grid);

    /*
        Problem (C): Can all 25 cities be fully powered
        using existing infrastructure, with only
        self-sufficient cities as suppliers?

        Algorithm: Edmonds-Karp (Max Flow)
        Time Complexity: O(V * E^2)
    */
    solveProblemC(grid);

    return 0;
}