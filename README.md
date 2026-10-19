# Machine Saathi: Graph-Based Machine Failure Impact and Dependency Analyzer

## Overview

Machine Saathi is a C-based project designed to analyze machine component dependencies and identify the potential impact of a component failure. It represents an industrial machine as a directed graph, where vertices represent machine components and directed edges represent dependencies between them. The project uses graph traversal algorithms to understand how a failure in one component may affect other connected components.

The current implementation demonstrates how data structures and graph algorithms can support basic industrial fault analysis and dependency tracking.

## Problem Statement

Industrial machines consist of interconnected components, where the failure of one component may affect other dependent components. Identifying these dependencies manually can make fault analysis difficult, especially when machines contain multiple interconnected parts. Machine Saathi addresses this problem by representing component dependencies as a graph and analyzing the possible downstream impact of a reported failure.

## Objectives

The primary objective of Machine Saathi is to demonstrate how graph-based analysis can help identify components that may be affected by a machine failure. The project aims to represent machine dependencies efficiently using adjacency lists, apply Breadth-First Search (BFS) to analyze failure impact levels, and use Depth-First Search (DFS) to trace dependency paths. It also incorporates component status and criticality ratings to provide a more informative analysis of potential failure impact.

## Technologies and Data Structures

The project is implemented in the C programming language and can be compiled using GCC. Visual Studio Code can be used as the development environment.

The implementation uses a directed graph with adjacency lists to represent machine dependencies. Singly linked lists store the outgoing connections of each component, while arrays maintain component information, traversal states, and path details. A circular queue supports level-order traversal in BFS, and recursion is used to implement DFS and explore dependency paths.

## Key Features

### Component and Dependency Management

Machine Saathi represents an industrial machine as a collection of components connected through directed dependencies. Each component has a name, an operational or faulty status, and a predefined criticality rating ranging from 1 to 5.

### Failure Registration

Users can select a machine component and register it as faulty. The system updates the selected component's status and automatically performs BFS-based impact analysis and DFS-based dependency path tracing.

### BFS-Based Failure Impact Analysis

Breadth-First Search explores reachable downstream components level by level. The analysis reports the components that may be affected, their dependency levels, the number of affected components, the number of highly critical affected components, and the calculated impact score.

### DFS-Based Dependency Path Tracing

Depth-First Search explores dependency paths from the selected component. The recursive implementation uses backtracking to explore alternative paths and display the dependency routes identified by the algorithm.

### Component Status and Criticality Analysis

The project distinguishes between the component reported as faulty and downstream components that are considered at risk. Criticality ratings help estimate the importance of the affected components and calculate an overall impact score.

### Upstream Dependency Identification

The system can identify the direct upstream dependencies of a selected component by examining the graph connections. This helps show which components directly support the operation of another component.

## How It Works

The program initializes the machine graph and registers its components and dependencies. When a component failure is reported, the selected component is marked as faulty. BFS then identifies reachable downstream components and organizes them by dependency level, while DFS explores possible dependency paths from the faulty component. The results provide a structured view of the potential impact of the failure.

For the current implementation, an edge from component A to component B indicates that component B depends on component A. Therefore, a failure in A may affect B.

## Algorithms and Complexity

Breadth-First Search is used for downstream impact analysis. Its time complexity is O(V + E), where V represents the number of vertices and E represents the number of edges. Its auxiliary space complexity is O(V).

Depth-First Search is used to explore dependency paths. A standard traversal takes O(V + E) time, but enumerating all possible dependency paths can require exponential time in the worst case, depending on the graph structure. The graph and adjacency-list representation require O(V + E) storage.

## Current Limitations

Machine Saathi currently performs rule-based analysis using predefined machine components, dependencies, statuses, and criticality ratings. It does not collect live sensor readings, predict failures using machine learning, or automatically detect physical faults. Components identified by the analysis are classified as potentially at risk; they are not automatically marked as faulty.

## Future Goals

Future development will focus on integrating IoT sensors to collect machine parameters such as vibration, temperature, and current. These readings could support continuous condition monitoring and help identify abnormal operating conditions.

The project could be extended with machine learning models trained on historical machine data to estimate failure risk and support predictive maintenance. A graphical dashboard could display the machine dependency graph, highlight faulty and at-risk components, and visualize BFS levels and DFS paths.

Additional improvements could include severity-based prioritization, configurable criticality ratings, historical fault logging, maintenance recommendations, and support for larger industrial dependency networks. Integration with a digital twin could eventually enable the system to represent machine components and their changing operational conditions more closely.

These enhancements would build on the existing graph-based foundation and move the project toward a more comprehensive industrial fault analysis and maintenance support system.

## Conclusion

Machine Saathi demonstrates the application of fundamental data structures and graph traversal algorithms to an industrial machine failure analysis problem. By combining adjacency lists, linked lists, a circular queue, arrays, recursion, BFS, and DFS, the project provides a structured way to examine component dependencies and potential failure propagation. Its current implementation serves as a foundation for future development in industrial monitoring, fault diagnosis, and predictive maintenance.

## References

The project is based on fundamental concepts of directed graphs, adjacency-list representation, Breadth-First Search, Depth-First Search, linked lists, queues, recursion, and algorithmic complexity.
