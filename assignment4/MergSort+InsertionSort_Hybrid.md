# Assignment4

## Problem1

https://sedgewick.io/wp-content/uploads/2022/04/Algs05-Mergesort.pdf till till slide #19
Why is it to your advantage to do insertion sort when the subarray sizes are <7 (let say we do 8)? And why is it not to your advantage to do insertion sort once the subarray size reaches 16? Can you do the analysis (time complexity + space complexity, assuming creating of a single subarray is cost of 1). Merge+ insertion sort hybrid: Larger N 8, divide into half as usual, but do insertion sort for size of 8, then from size 8 to N (regular merge stage), use the merge sort.  

Zoom in:  

Merge sort only (size 8): Comparisons + Creating subarray (from 8-4-2-1-2-4-8) : 3*N[from size 2-4-8] + (N/4+ N/2+N+N/2+N/4+N/8] ~= 3N + 2.6N = 5.6N. Insertion (size 8) : Sort the sub array of size 8: (64/2)*(N/8) = 4N.  
4N <5.6N.  

Now can you do the case of 16, via analysis similar to the above, (3 cases, pure merge sort only, merge sort after size 16 (including), but do insertion sort at size 8, case do insertion at size 16 + then merge sort)such that insertion sort loses its advantage?  

---

### Case 1: Pure merge Sort

Merge sort (size 16): Comparisons + Creating sub array (from 16-8-4-2-1-2-4-8-16) :  

#### Comparisons from size 2-4-8-16

$(4*N) = 4N$

#### Creating sub arrays

$(\frac{N}{8}+\frac{N}{4}+\frac{N}{2}+N+\frac{N}{2}+\frac{N}{4}+\frac{N}{8}+\frac{N}{16}) = 2.8125N \approx{2.8N} $

#### Total Cost of Comparisons Plus Sub Array Creation

$ 4N + 2.8N = 6.8N $

---

### Case 2: Pure Insertion Sort

Insertion Sort (size 16) :  

#### Sort the sub array of size 16

$ \frac{16^2}{2}*\frac{N}{8} = 128 * \frac{N}{16} = 8N $

#### Total cost of pure insertion sort

$ 8N $

---

### Case 3: Hybrid - Insertion Sort at Size 8 Merge sort for size 8->16

Split 16 into two 8s insertion sort them. Then merge back to 16

#### Split 16->8

$ \frac{N}{8} $

#### Sort the sub arrays of size 8

$ \frac{8^2}{2}*\frac{N}{8} = 32 * \frac{N}{8} = 4N $

#### Merge 8->16

comparisons: $N$
Subarray creation: $\frac{N}{16}$

#### Total Cost

$4N+N+\frac{N}{8}+\frac{N}{16} = 5.19N \approx{5.2N}$

---

### Explanation

| Case | Cost |
| :--- | :--- |
| Merge Sort | $6.8N$ |
| Insertion Sort | $8N$ |
| Hybrid Sort | $5.2N$ |

As you can see from the example cases the hybrid version has the best performance with a cost of $5.2N$. This is because we are able to take advantage of insertion sorts better performance for arrays of size less than 8 ($4N$ vs $5.6N$). We then use Merge sort for arrays larger than 8 again taking advantage of the algorithms improved performance ($6.8N$ vs $8N$). The combination of these two algorithms lead to a cost of $5.2N$ for an array of size 16.

## Problem 2

Given a tree of high h, root is at level 0, assume it is a balanced tree such that each node has k branches. Eg. there would be k nodes in level 1, and k^2 nodes in level 2, etc. We are interested to find a node T at location (m,t) where m is the mth level and t is the tth node of that level counting from the left. So, what is the cost of BFS and what is the cost of DFS (DFS prefers leftmost first) to find node T.

### BFS

BFS traverses the tree level by level from one side to the other(typically left to right). This means that to reach the point $(m,t)$ we will visit every node in levels $0 through m - 1$ plus the first $t$ nodes of level $m$. This gives us the equation $1 + k + k^2 + ... + k^{m-1} + t = \frac{k^{m}-1}{k-1}+t$

This shows that the cost of using BFS is dependent on $m$ and $t$

### DFS

DFS on the other hand traverses the tree by following each branch as far down as possible before moving on to the next. This means that for DFS to reach $(m,t)$ we need to visit all nodes to the left of $t$ regardless of their height. The number of nodes in a fully balanced tree of height $H$ is: $ S(H)=\frac{k^{H+1}-1}{k-1} $

We can represent the path to node $T$ by converting the 0-indexed position of the node $(t-1)$ into a base-$k$ representation of length $m$. We will call these $d_1,d_2,...,d_m$ where $0 <= d_i < k$.  
$ t-1 = d_{1}k^{m-1}+d_{2}k^{m-2}+...+d_{m}k^{0} $

The cost can then be calculated by adding the cost visiting the $(m+1)$ nodes along the direct path to $T$  
To the cost of fully exploring all subtrees to the left of that path $\sum_{i=1}^{m} d_i (\frac{k^{h-i+1} - 1}{k - 1})$

This gives us a total cost of $(m + 1) + \sum_{i=1}^{m} d_i (\frac{k^{h-i+1} - 1}{k - 1})$

### Analysis

Based of the calculated costs of the two searches we can see that the performance of BFS is directly related to the size of $m$ and $t$. While DFS on the other hand depends on $m,t$ and the height of the tree $h$. This means that if the height of the tree is taller than the node $T$ is deep, DFS will waste time checking nodes BFS would not.
