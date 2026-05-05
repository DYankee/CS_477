# CS477 Final

Attached with this file are two c++ files electricGrid.cpp and final.cpp.

the first, electricGrid.cpp contains a class I created to represent the given data and provide the algorithms to traverse, and work with it.  

The second, final.cpp contains the solutions to each question along with comments documenting the steps taken to achieve the answer.

## Compiling

To compile the program I used the following command: `g++ -O2 final.cpp -o grid`

The program can then be run with `./grid`

## Program output

``` log

Electrical Grid Analysis - Starting from Utica


=== Electrical Grid Statistics ===
Total cities: 18
Self-sufficient cities: 15
Total surplus from self-sufficient cities: 5050 MW
==================================


=== Problem (a): Shortest Paths from Utica ===

Utica to Jamestown:
  Distance: 440 km
  Path: Utica -> Syracuse -> Buffalo -> Jamestown

Utica to New York:
  Distance: 390 km
  Path: Utica -> Albany -> New York

Utica to Elmira:
  Distance: 260 km
  Path: Utica -> Syracuse -> Binghamton -> Elmira

Utica to Rochester:
  Distance: 450 km
  Path: Utica -> Syracuse -> Buffalo -> Rochester

Utica to Niagara Falls:
  Distance: 360 km
  Path: Utica -> Syracuse -> Buffalo -> Niagara Falls

=== Problem (b-1): NYC + Yonkers Power Supply Analysis ===

Self-Sufficient Cities (Potential Suppliers):
City                Surplus (MW)
-----------------------------------
Albany              300.0
Binghamton          200.0
Buffalo             400.0
Elmira              150.0
Ithaca              200.0
Jamestown           100.0
Niagara Falls       1900.0
Rochester           300.0
Rome                150.0
Saratoga Springs    150.0
Schenectady         200.0
Syracuse            300.0
Troy                200.0
Utica               200.0
Watertown           300.0

Power Deficit Analysis:
  NYC:           2000.0     MW deficit
  Yonkers:       200.0      MW deficit
  Total needed:  2200.0     MW
  Total surplus: 5050.0     MW

==================================================
MAXIMUM FLOW ANALYSIS
==================================================
Maximum achievable flow: 2200.0     MW
Required flow:           2200.0     MW
--------------------------------------------------

✓ RESULT: YES - NYC and Yonkers CAN be fully supplied!

Power Distribution:
  NYC receives:     2000.0     MW (needs 2000.0 MW)
  Yonkers receives: 200.0      MW (needs 200.0 MW)

Major Supplier Contributions:
City                Contributed (MW)Available (MW)
--------------------------------------------------
Albany              300.0          300.0
Binghamton          200.0          200.0
Buffalo             400.0          400.0
Jamestown           100.0          100.0
Niagara Falls       900.0          1900.0
Rochester           300.0          300.0

=== Problem (b-2): Cheapest Supply by Distance ===
(Cost = total distance power travels)

=======================================================
MINIMUM COST FLOW ANALYSIS (cost = distance)
=======================================================
Total flow achieved: 2200.0 MW
Required flow:       2200.0 MW
Total cost:          860250.0 (MW * km)
-------------------------------------------------------

✓ Full supply achieved at minimum distance cost!

Supplier Contributions:
City                  Contributed (MW)  Available (MW)
----------------------------------------------------------
Albany                300.0             300.0
Binghamton            200.0             200.0
Elmira                150.0             150.0
Rochester             200.0             300.0
Rome                  150.0             150.0
Saratoga Springs      150.0             150.0
Schenectady           200.0             200.0
Syracuse              300.0             300.0
Troy                  200.0             200.0
Utica                 200.0             200.0
Watertown             150.0             300.0

Active Transmission Lines:
From                  To                    Flow (MW)     Dist (km)
------------------------------------------------------------------------
Albany                New York              2000.0        240.0
Binghamton            Albany                350.0         220.0
Elmira                Binghamton            150.0         60.0
New York              Yonkers               200.0         30.0
Rochester             New York              200.0         540.0
Rome                  Utica                 150.0         25.0
Saratoga Springs      Albany                150.0         50.0
Schenectady           Albany                200.0         25.0
Syracuse              Albany                300.0         220.0
Troy                  Albany                200.0         10.0
Utica                 Albany                500.0         150.0
Watertown             Utica                 150.0         140.0

=== Problem (b-3): Cheapest Supply by Weighted Cost ===
(Cost = supply_amount * distance per unit flow)

============================================================
MINIMUM COST FLOW ANALYSIS (cost = flow * distance)
============================================================
Total flow achieved: 2200.0 MW
Required flow:       2200.0 MW
Total weighted cost: 860250.0 (MW * km)
------------------------------------------------------------

✓ Full supply achieved at minimum weighted cost!

Supplier Contributions:
City                  Contributed (MW)  Available (MW)
----------------------------------------------------------
Albany                300.0             300.0
Binghamton            200.0             200.0
Elmira                150.0             150.0
Rochester             200.0             300.0
Rome                  150.0             150.0
Saratoga Springs      150.0             150.0
Schenectady           200.0             200.0
Syracuse              300.0             300.0
Troy                  200.0             200.0
Utica                 200.0             200.0
Watertown             150.0             300.0

Active Transmission Lines:
From                To                  Flow (MW)   Dist (km)   Cost (MW*km)
--------------------------------------------------------------------------------
Albany              New York            2000.0      240.0       480000.0
Binghamton          Albany              350.0       220.0       77000.0
Elmira              Binghamton          150.0       60.0        9000.0
New York            Yonkers             200.0       30.0        6000.0
Rochester           New York            200.0       540.0       108000.0
Rome                Utica               150.0       25.0        3750.0
Saratoga Springs    Albany              150.0       50.0        7500.0
Schenectady         Albany              200.0       25.0        5000.0
Syracuse            Albany              300.0       220.0       66000.0
Troy                Albany              200.0       10.0        2000.0
Utica               Albany              500.0       150.0       75000.0
Watertown           Utica               150.0       140.0       21000.0
--------------------------------------------------------------------------------
Verified total cost: 860250.0 (MW * km)

=== Problem (b-4): Max Flow to NYC from Each City ===
(Find which single city can send the most power to NYC)

Source City           Max Flow (MW)     Surplus (MW)
-------------------------------------------------------
Niagara Falls         1900.0            1900.0
Buffalo               400.0             400.0
Albany                300.0             300.0
Rochester             300.0             300.0
Syracuse              300.0             300.0
Watertown             300.0             300.0
Binghamton            200.0             200.0
Ithaca                200.0             200.0
Schenectady           200.0             200.0
Troy                  200.0             200.0
Utica                 200.0             200.0
Elmira                150.0             150.0
Rome                  150.0             150.0
Saratoga Springs      150.0             150.0
Jamestown             100.0             100.0

=======================================================
BEST SINGLE SUPPLIER FOR NYC
=======================================================
City:     Niagara Falls
Max Flow: 1900.0 MW
Surplus:  1900.0 MW
-------------------------------------------------------

Active Transmission Lines (from Niagara Falls to NYC):
From                  To                    Flow (MW)
----------------------------------------------------------
Buffalo               New York              1900.0
Niagara Falls         Buffalo               1900.0

=== Problem (c): Can All 25 Cities Be Fully Powered? ===
(Using only self-sufficient cities as suppliers)

Deficit Cities:
City                  Deficit (MW)
-------------------------------------
New York              2000.0
White Plains          200.0
Yonkers               200.0

Surplus Cities:
City                  Surplus (MW)
-------------------------------------
Albany                300.0
Binghamton            200.0
Buffalo               400.0
Elmira                150.0
Ithaca                200.0
Jamestown             100.0
Niagara Falls         1900.0
Rochester             300.0
Rome                  150.0
Saratoga Springs      150.0
Schenectady           200.0
Syracuse              300.0
Troy                  200.0
Utica                 200.0
Watertown             300.0

Total deficit:  2400.0 MW
Total surplus:  5050.0 MW

Surplus >= Deficit. Checking if transmission infrastructure allows delivery...

=======================================================
ALL-CITY POWER SUPPLY ANALYSIS
=======================================================
Maximum achievable flow: 2400.0 MW
Total deficit required:  2400.0 MW
-------------------------------------------------------

✓ YES - All 25 cities CAN be fully powered!

Deficit City Supply Status:
City                  Needed (MW)     Received (MW)   Status
----------------------------------------------------------------
New York              2000.0          2000.0          ✓ OK
White Plains          200.0           200.0           ✓ OK
Yonkers               200.0           200.0           ✓ OK

Supplier Usage:
City                  Contributed (MW)  Available (MW)    Usage %
--------------------------------------------------------------------
Albany                300.0             300.0             100       %
Binghamton            200.0             200.0             100       %
Buffalo               400.0             400.0             100       %
Elmira                0.0               150.0             0         %
Ithaca                0.0               200.0             0         %
Jamestown             100.0             100.0             100       %
Niagara Falls         1100.0            1900.0            58        %
Rochester             300.0             300.0             100       %
Rome                  0.0               150.0             0         %
Saratoga Springs      0.0               150.0             0         %
Schenectady           0.0               200.0             0         %
Syracuse              0.0               300.0             0         %
Troy                  0.0               200.0             0         %
Utica                 0.0               200.0             0         %
Watertown             0.0               300.0             0         %
```