# Problem Set 6

## Question 1

<https://sedgewick.io/wp-content/uploads/2022/04/Algs15-ShortestPaths.pdf> there on slide #2, we use Dijkstra's algorithm to find the shortest path from source node 0. let us change the weight to unit weight (that is 1) for all edges. Please compare the performance of finding shortest path using BFS v.s. Dijkstra's algorithm.

### Response

``` md
Run 1: BFS = 2.34263 ms, Dijkstra = 7.82079 ms
Run 2: BFS = 1.12205 ms, Dijkstra = 4.03489 ms
Run 3: BFS = 0.872029 ms, Dijkstra = 3.90135 ms
Run 4: BFS = 0.864654 ms, Dijkstra = 3.88851 ms
Run 5: BFS = 0.864965 ms, Dijkstra = 3.90657 ms

--- Averages over 5 runs ---
BFS Time:      1.21327 ms
Dijkstra Time: 4.71042 ms
Difference: Dijkstra is approx 3.88243x slower than BFS due to Priority Queue overhead.
```

As shown by result from the test above we can see that BFS is almost 4 times faster then Dijkstra's when used on a graph where all edges have the same weight. This makes sense because of the overhead added by the priority queue needed for Dijkstra's. On a weighted graph this overhead is worth the cost to reduce the number of edges checked, But on an unweighted graph or a graph where all edges have the same weight the added cost provides no benefit because Dijkstra's will check the same number of edges as BFS.

### Code

Code for Question 2 can be found in q1.cpp

## Question 2

<https://sedgewick.io/wp-content/uploads/2022/04/Algs15-ShortestPaths.pdf> on slide #40, the algorithm takes advantage of the DAG graph such that its complexity is a bit cheaper than Dijkstra's. Hence, please explain why the directed acyclic graph property helps with the this algorithm that Dijkstra's overlooks?

### Response

