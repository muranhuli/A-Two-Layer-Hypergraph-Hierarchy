# Hierarchy-Decomposition-and-Construction-on-Hypergraphs

the source code of Hierarchy Decomposition and Construction on Hypergraphs

## About the dataset

We preprocess all datasets so that the vertices of each hyperedge in the hypergraph are different from each other, and each hyperedge contains at least two vertices

NFI: This is a processed dataset for our experiments with 446 vertices and 3651 hyperedges.

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

## A simple running demo

There is a data set NFI, we need to compute MinKSCore.

Our operations are all implemented on MinKSCore.

1. We need to input the path of the file at line 290 and the name of the file NFI at line 364.

2. `g++ MinKSCore.cpp -pthread -std=c++11 -o MinKSCore`

3. `./MinKSCore`

4. We will obtain a folder (result) containing the experimental results at the min.cpp file, and find the file corresponding to the NFI result in it, which is the experimental result of NFI.

5. In step four, the files found contain the information we need: for each K value, the time required, the number of connected k-core subgraphs, the number of (k,s)-core, the number of hyperedge, the number of vertices.
