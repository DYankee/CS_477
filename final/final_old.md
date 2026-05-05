# CS477 Final

## (a) Electrical Grids

Given the following data

| City (From) | City (To) | Demand (From) MW | Gen (From) MW | Demand (To) MW | Gen (To) MW | Distance (km) | Capacity (MW) |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| New York | Buffalo | 9000 | 7000 | 1800 | 2200 | 600 | 2500 |
| New York | Rochester | 9000 | 7000 | 1500 | 1800 | 540 | 2200 |
| New York | Albany | 9000 | 7000 | 1200 | 1500 | 240 | 2000 |
| Albany | Syracuse | 1200 | 1500 | 1300 | 1600 | 220 | 1800 |
| Syracuse | Buffalo | 1300 | 1600 | 1800 | 2200 | 240 | 2000 |
| Rochester | Buffalo | 1500 | 1800 | 1800 | 2200 | 120 | 1700 |
| Albany | Rochester | 1200 | 1500 | 1500 | 1800 | 360 | 1600 |
| Yonkers | New York | 1100 | 900 | 9000 | 7000 | 30 | 3000 |
| Yonkers | White Plains | 1100 | 900 | 800 | 600 | 25 | 1200 |
| White Plains | New York | 800 | 600 | 9000 | 7000 | 40 | 1500 |
| Utica | Syracuse | 700 | 900 | 1300 | 1600 | 90 | 1400 |
| Utica | Albany | 700 | 900 | 1200 | 1500 | 150 | 1300 |
| Binghamton | Syracuse | 600 | 800 | 1300 | 1600 | 110 | 1200 |
| Binghamton | Albany | 600 | 800 | 1200 | 1500 | 220 | 1100 |
| Ithaca | Binghamton | 500 | 700 | 600 | 800 | 80 | 900 |
| Ithaca | Syracuse | 500 | 700 | 1300 | 1600 | 90 | 1000 |
| Troy | Albany | 400 | 600 | 1200 | 1500 | 10 | 800 |
| Schenectady | Albany | 450 | 650 | 1200 | 1500 | 25 | 900 |
| Saratoga Springs | Albany | 350 | 500 | 1200 | 1500 | 50 | 700 |
| Rome | Utica | 300 | 450 | 700 | 900 | 25 | 600 |
| Watertown | Syracuse | 400 | 700 | 1300 | 1600 | 110 | 800 |
| Watertown | Utica | 400 | 700 | 700 | 900 | 140 | 700 |
| Niagara Falls | Buffalo | 600 | 2500 | 1800 | 2200 | 30 | 2000 |
| Elmira | Binghamton | 350 | 500 | 600 | 800 | 60 | 600 |
| Jamestown | Buffalo | 300 | 400 | 1800 | 2200 | 110 | 700 |

Assume the edges works for both power transmission (one way) and cars (both ways)  
Do the following: Assume you live in Utica  
Before you implement any solution, please briefly describe your algorithm. 

## Part A

GPS: Find the shortest path from Utica to (1) James Town (2) NYC (3) Elmira (4) Rochester (5) Niagra Falls:  

### Algorithm: Dijkstra's

For part A we can find the shortest paths using Dijkstra's Algorithm. The algorithm works as follows:

1. Initialize the algorithm
    - Mark all cities as unvisited and set their distance to infinity.
    - Set utica as the starting node and set its distance to 0.
2. Relax neighbors
    - Consider all neighbors of the current city and calculate their total distance when traveling through the current node.
    - Compare the current shortest distance and the new distance and keep the smaller of the two.
3. Select next node
    - After relaxing all neighboring nodes, mark the current city as visited.
    - Select the unvisited city with the smallest total distance as the new current node.
4. Repeat
    - Repeat steps 2 and 3 until the destination city is reached or all cities have been visited.

**Work Shown for A-1** Since the process is the same for A1-5 I omitted the work.

#### A-1: Utica to James town

``` txt
Iter 1:
    Init:
        Current Node: Utica
    Relax neighbors:
        Utica -> Syracuse = 90
        Utica -> Albany = 150
        Utica -> Watertown = 140
        Utica -> Rome = 25
    Distances:
        - x Utica = 0
        - Syracuse = 90
        - Albany = 150
        - Watertown = 140
        - Rome = 25
    Select Next Node:
        Rome = 25

Iter 2:
    Current Node:
        Rome = 25
    Relax Neighbors:
        None
    Distances:
        - x Utica = 0
        -   Syracuse = 90
        -   Albany = 150
        -   Watertown = 140
        - x Rome = 25
    Select Next Node:
        Syracuse

Iter 3:
    Current Node:
        Syracuse = 90
    Relax Neighbors:
        Syracuse -> Buffalo = 240 (330)
        Syracuse -> Albany = 220 (310)
        Syracuse -> Utica = 90 (180)
        Syracuse -> Watertown = 110 (200)
    Distances:
        - x Utica = 0
        - x Syracuse = 90
        -   Albany = 150
        -   Watertown = 140
        - x Rome = 25
        -   Buffalo = 330
    Select Next Node:
        Watertown = 140

Iter 4:
    Current Node:
        Watertown = 140
    Relax Neighbors:
       Watertown -> Syracuse = 110 (250)
       Watertown -> Utica = 140 (280)
    Distances:
        - x Utica = 0
        - x Syracuse = 90
        -   Albany = 150
        - x Watertown = 140
        - x Rome = 25
        -   Buffalo = 330
    Select Next Node:
        Albany = 150

Iter 5:
    Current Node:
        Albany = 150
    Relax Neighbors:
       Albany -> New York = 240 (390)
       Albany -> Syracuse = 220 (370)
       Albany -> Rochester = 360 (510)
       Albany -> Utica = 150 (300)
       Albany -> Binghamton 220 (370)
       Albany -> Troy 10 (160)
       Albany -> Schenectady 25 (175)
       Albany -> Saratoga Springs 50 (200)
    Distances:
        - x Utica = 0
        - x Syracuse = 90
        - x Albany = 150
        - x Watertown = 140
        - x Rome = 25
        -   Buffalo = 330
        -   New York = 390
        -   Rochester = 510
        -   Binghamton = 370
        -   Troy = 160
        -   Schenectady = 175
        -   Saratoga Springs = 200
    Select Next Node:
        Troy = 160

Iter 6,7,8:
    Iterations 6(Troy),7(Schenectady), and 8(Saratoga Springs) all have no neighbors to relax so to save space their steps have been omitted

    Distances:
        - x Utica = 0
        - x Syracuse = 90
        - x Albany = 150
        - x Watertown = 140
        - x Rome = 25
        -   Buffalo = 330
        -   New York = 390
        -   Rochester = 510
        -   Binghamton = 370
        - x Troy = 160
        - x Schenectady = 175
        - x Saratoga Springs = 200
    Select Next Node: Buffalo = 330

Iter 9:
    Current Node:
        Buffalo = 330
    Relax Neighbors
        Buffalo -> New York = 600 (930) 
        Buffalo -> Syracuse = 240 (570)
        Buffalo -> Rochester = 120 (450)
        Buffalo -> Jamestown = 110 (440)
     Distances:
        - x Utica = 0
        - x Syracuse = 90
        - x Albany = 150
        - x Watertown = 140
        - x Rome = 25
        - x Buffalo = 330
        -   New York = 390
        -   Rochester = 510
        -   Binghamton = 370
        - x Troy = 160
        - x Schenectady = 175
        - x Saratoga Springs = 200
        -   Rochester = 450
        -   Jamestown = 440
    
    Jamestown was found so we stop iterating
```

Shortest path to jamestown: 440km
Route: Utica -> Syracuse -> Buffalo -> Jamestown

#### A-2: Utica to NYC

Shortest path to NYC: 390km  
Route: Utica -> Albany -> New York  

#### A-3: Utica to Elmira

Shortest path to Elmira: 260km  
Route: Utica -> Syracuse -> Binghamton -> Elmira  

#### A-4: Utica to Rochester

Shortest path to Rochester: 450km  
Route: Utica -> Syracuse -> Buffalo -> Rochester  

#### A-5: Utica to Niagara Falls

Shortest path to Niagara Falls: 360km  
Route: Utica -> Syracuse -> Buffalo -> Niagara Falls  

## Part B

Net Power Surplus/Deficit

| City | Surplus/Deficit (MW) | City | Surplus/Deficit (MW) |
| :--- | :--- | :--- | :--- |
| New York | -2000 | Troy | +200 |
| Yonkers | -200 | Schenectady | +200 |
| White Plains | -200 | Saratoga | +150 |
| Niagara Falls | +1900 | Rome | +150 |
| Buffalo | +400 | Watertown | +300 |
| Rochester | +300 | Elmira | +150 |
| Albany | +300 | Jamestown | +100 |
| Syracuse | +300 | Binghamton | +200 |
| Utica | +200 | Ithaca | +200 |

### B-1

Can NYC + Yonker be supplied with enough power from other cities while those cities are those who can be self sufficient first (white plains can't be suppliers because their consumption is greater than generation)?



### B-2

What's the shortest (cheapest;  distance it travels (among all feasible ways to supply, find the cheapest) ) way to supply NYC + Yonker ? What cities and how much each of them is contributing?


### B-3

What's the shortest (cheapest; the cost the (supply amount to other city* distance it travels) ) way to supply NYC + Yonker ? What cities and how much each of them is contributing?  


### B-4

What the max flow (with respect to power it can supply with constraint by the capacity) NYC can get , and from what city? (max of max flow, iterate all other cities)

**Algorithm: Ford-Fulkerson**  

## Part C

Can those 25 cities, using existing infrastructure, be without fear of having no power?  
