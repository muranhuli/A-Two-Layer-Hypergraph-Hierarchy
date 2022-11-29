# Hierarchy-Decomposition-and-Construction-on-Hypergraphs

the source code of Hierarchy Decomposition and Construction on Hypergraphs

## About the dataset

We preprocess all datasets so that the vertices of each hyperedge in the hypergraph are different from each other, and each hyperedge contains at least two vertices

## About how to use

```bash
g++ kCore.cpp -pthread -std=c++11 -o kCore
g++ MeanKSCore.cpp -pthread -std=c++11 -o MeanKSCore
g++ MinKSCore.cpp -pthread -std=c++11 -o MinKSCore
./kCore
./MeanKSCore
./MinKSCore
```

## About the function of the code

kCore.cpp: We compute the KSCore value of each vertex.

MinKSCore.cpp: According to the KSCore value of each vertex, we select the information we need: for each K value, the time required, the number of connected k-core subgraphs, the number of (k,s)-core, the number of hyperedge, the number of vertices.
Note that we are using **MinKSCore**.

MeanKSCore.cpp: According to the KSCore value of each vertex, we select the information we need: for each K value, the time required, the number of connected k-core subgraphs, the number of (k,s)-core, the number of hyperedge, the number of vertices.
Note that we are using **MeanKSCore**.
