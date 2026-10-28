# Computer Networks Assignment

This project contains two C programs:

1.  **Subnetting Calculator** --- supports Fixed Length Subnet Masking
    (FLSM) and Variable Length Subnet Masking (VLSM).
2.  **Routing Algorithms Simulator** --- supports Distance Vector
    Routing and Link State Routing (Dijkstra's algorithm) for a
    user-entered weighted graph.

------------------------------------------------------------------------

## 1. Subnetting Calculator (`subnetting.c`)

### Code blocks explained

-   **`ipToInt()`**: Converts an IPv4 address from dotted-decimal
    notation into a 32-bit integer so address calculations can be
    performed.
-   **`intToIP()`**: Converts a 32-bit integer back into dotted-decimal
    IPv4 notation.
-   **`displaySubnet()`**: Calculates and displays the network address,
    subnet mask, usable host range, and broadcast address for a subnet.
-   **Input and validation in `main()`**: Asks the user to choose FLSM
    or VLSM, enter the base network IP and CIDR prefix, and validates
    the main inputs.
-   **Network and mask calculation**: Builds the subnet mask from the
    CIDR prefix and aligns the entered address to its network boundary.
-   **FLSM switch case**: Asks for the number of subnets, calculates how
    many bits must be borrowed, assigns the same prefix and block size
    to every subnet, and prints each subnet.
-   **VLSM switch case**: Asks for the host requirement of each subnet,
    calculates the smallest suitable subnet prefix, sorts requirements
    from largest to smallest, and allocates address blocks while
    checking that they fit inside the base network.

### Sample 1 --- FLSM

**Input**

``` text
1
192.168.1.0
24
4
```

Prompts correspond to:

``` text
Enter your choice: 1
Enter Network IP Address: 192.168.1.0
Enter CIDR Prefix (e.g. 24 for /24): 24
Enter Number of Subnets: 4
```

**Output**

``` text
--- Subnet 1 ---
Network Address: 192.168.1.0/26
Subnet Mask: 255.255.255.192
Usable Host Range: 192.168.1.1 - 192.168.1.62
Broadcast Address: 192.168.1.63

--- Subnet 2 ---
Network Address: 192.168.1.64/26
Subnet Mask: 255.255.255.192
Usable Host Range: 192.168.1.65 - 192.168.1.126
Broadcast Address: 192.168.1.127

--- Subnet 3 ---
Network Address: 192.168.1.128/26
Subnet Mask: 255.255.255.192
Usable Host Range: 192.168.1.129 - 192.168.1.190
Broadcast Address: 192.168.1.191

--- Subnet 4 ---
Network Address: 192.168.1.192/26
Subnet Mask: 255.255.255.192
Usable Host Range: 192.168.1.193 - 192.168.1.254
Broadcast Address: 192.168.1.255
```

**Explanation:** A `/24` network is divided into four equal `/26`
subnets. Every subnet has 64 total addresses and 62 conventional usable
host addresses.

### Sample 2 --- VLSM

**Input**

``` text
2
192.168.1.0
24
3
100
50
20
```

Prompts correspond to:

``` text
Enter your choice: 2
Enter Network IP Address: 192.168.1.0
Enter CIDR Prefix (e.g. 24 for /24): 24
Enter Number of Subnets: 3
Enter Required Hosts for Subnet 1: 100
Enter Required Hosts for Subnet 2: 50
Enter Required Hosts for Subnet 3: 20
```

**Output**

``` text
--- Subnet 1 (Required Hosts: 100) ---
Network Address: 192.168.1.0/25
Subnet Mask: 255.255.255.128
Usable Host Range: 192.168.1.1 - 192.168.1.126
Broadcast Address: 192.168.1.127

--- Subnet 2 (Required Hosts: 50) ---
Network Address: 192.168.1.128/26
Subnet Mask: 255.255.255.192
Usable Host Range: 192.168.1.129 - 192.168.1.190
Broadcast Address: 192.168.1.191

--- Subnet 3 (Required Hosts: 20) ---
Network Address: 192.168.1.192/27
Subnet Mask: 255.255.255.224
Usable Host Range: 192.168.1.193 - 192.168.1.222
Broadcast Address: 192.168.1.223
```

**Explanation:** VLSM assigns different-sized subnets based on host
requirements. The largest requirement is allocated first.

------------------------------------------------------------------------

## 2. Routing Algorithms Simulator (`routing.c`)

### Code blocks explained

-   **Graph and router data**: Stores router names and the cost matrix.
    In the interactive version, the user enters the router names and
    edges, including each edge's cost.
-   **`findRouter()`**: Finds the index of a router using its name.
-   **`printDVTable()`**: Displays each router's distance vector for a
    given iteration.
-   **`distanceVector()`**: Initializes costs to direct neighbours,
    exchanges distance vectors, repeatedly updates routes using the
    minimum-cost rule, and stops when the vectors no longer change. It
    then displays the final routing table for every router, including
    destination, cost, and next hop.
-   **`printPath()`**: Prints a route using predecessor information.
-   **`linkState()`**: Implements Dijkstra's algorithm. It selects the
    unvisited router with the lowest tentative cost, updates the costs
    of its neighbours, and displays each iteration. It then prints the
    shortest path and its total cost.
-   **`main()`**: Accepts router names and weighted edges, creates an
    undirected graph, and uses a menu to select Distance Vector or Link
    State routing.

**Input rule:** Enter each edge as `Router1 Router2 Cost`. The graph is
undirected, so an edge entered between `a` and `b` connects both
routers. Edge costs must be non-negative.

### Sample graph used for both routing algorithms

``` text
Routers: a b c d e z
Edges: 9

a b 4
a c 2
b c 1
b d 5
c d 8
c e 10
d e 2
d z 6
e z 3
```

### Sample 3 --- Distance Vector Routing

**Input**

``` text
Enter Number of Routers (maximum 20): 6

Router 1: a
Router 2: b
Router 3: c
Router 4: d
Router 5: e
Router 6: z

Enter Number of Edges: 9
Edge 1: a b 4
Edge 2: a c 2
Edge 3: b c 1
Edge 4: b d 5
Edge 5: c d 8
Edge 6: c e 10
Edge 7: d e 2
Edge 8: d z 6
Edge 9: e z 3

Enter your choice: 1
Enter Source Router: a
Enter Destination Router: z
```

**Output (final result excerpt)**

``` text
========== FINAL ROUTING TABLES ==========

Routing Table for Router a
Destination     Cost            Next Hop
a               0               -
b               3               c
c               2               c
d               8               c
e               10              c
z               13              c

========== DISTANCE VECTOR RESULT ==========
Shortest Path: a -> c -> b -> d -> e -> z
Minimum Cost: 13
```

The program also displays the initial distance vectors and each updated
iteration, followed by final routing tables for routers `a`, `b`, `c`,
`d`, `e`, and `z`.

### Sample 4 --- Link State Routing (Dijkstra)

**Input**

Use the same six routers and nine edges above, then choose option `2`
and enter:

``` text
Enter your choice: 2
Enter Source Router: a
Enter Destination Router: z
```

**Output (illustrative iteration table and final result)**

``` text
========== DIJKSTRA'S ALGORITHM ==========
Source Router: a

Iteration   Selected    a   b   c   d   e   z
1           a           0   4   2   INF INF INF
2           c           0   3   2   10  12  INF
3           b           0   3   2   8   12  INF
4           d           0   3   2   8   10  14
5           e           0   3   2   8   10  13
6           z           0   3   2   8   10  13

========== LINK STATE RESULT ==========
Source: a
Destination: z
Shortest Path: a -> c -> b -> d -> e -> z
Minimum Cost: 13
```

**Explanation:** Dijkstra's algorithm starts from the selected source
router, repeatedly chooses the unvisited router with the smallest known
cost, and updates its neighbours. `INF` means no route has been
discovered at that stage.

------------------------------------------------------------------------

## How to compile and run

Using GCC:

``` bash
gcc subnetting.c -o subnetting
./subnetting

gcc routing.c -o routing
./routing
```

On Windows with GCC installed, run the generated `.exe` files:

``` text
subnetting.exe
routing.exe
```

## Notes

-   The subnetting program expects a valid IPv4 address and CIDR prefix.
    Its current input parsing assumes the address is correctly
    formatted.
-   The FLSM program allocates the same-sized subnet to every requested
    subnet.
-   The VLSM program sorts subnets by descending host requirement and
    allocates larger blocks first.
-   The routing program handles an undirected graph with non-negative
    costs. `-1` is not used for missing edges in the interactive routing
    version; instead, only the edges entered by the user exist.
-   For Distance Vector, the final routing tables are shown for all
    routers. For Dijkstra, the table and final path are computed from
    the selected source.
