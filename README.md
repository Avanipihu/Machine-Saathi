# Machine Saathi

**Intelligent Failure Propagation and Impact Analysis for Legacy Industrial Machines**

Machine Saathi models an interconnected industrial machine as a **directed graph**, where machine components are represented as vertices and their dependencies as directed edges. When a component failure is reported, the system uses **Breadth-First Search (BFS)** to analyze the level-wise impact on dependent components and **Depth-First Search (DFS)** to trace possible failure-propagation paths.

The project demonstrates a practical application of **Graph Data Structures, BFS, and DFS** in industrial maintenance and forms a foundational module for the broader Machine Saathi concept of making legacy machinery more intelligent and easier to maintain.

### Core Features

* Directed graph representation of machine dependencies
* Adjacency-list implementation
* Component and dependency management
* Failure reporting
* BFS-based failure impact analysis
* DFS-based failure propagation tracing
* Technician-oriented maintenance interface
* Component status tracking

> **Scope:** This project performs graph-based dependency and impact analysis. It does not claim to perform sensor-based fault diagnosis or predictive maintenance.

**Tech Stack:** C • Graph Data Structure • BFS • DFS • Adjacency List
