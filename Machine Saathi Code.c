#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_VERTICES 50
#define MAX_NAME_LEN 50
#define MIN_CRITICALITY 1
#define MAX_CRITICALITY 5

// Component health status 
typedef enum {
    STATUS_OPERATIONAL,
    STATUS_FAULTY
} ComponentStatus;

// Adjacency list node 
typedef struct AdjListNode {
    int destIndex;
    struct AdjListNode *next;
} AdjListNode;

// Component information 
typedef struct {
    char name[MAX_NAME_LEN];
    ComponentStatus status;
    int criticality;
    AdjListNode *head;
} ComponentVertex;

// Machine graph 
typedef struct {
    ComponentVertex vertices[MAX_VERTICES];
    int numVertices;
    char machineName[MAX_NAME_LEN];
} MachineGraph;

// Circular queue for BFS 
typedef struct {
    int items[MAX_VERTICES];
    int front;
    int rear;
    int count;
} Queue;


// INPUT FUNCTIONS

void clearInputBuffer(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF);
}

void readString(char *str, int size)
{
    if (fgets(str, size, stdin) != NULL) {
        size_t len = strcspn(str, "\n");

        if (str[len] == '\n') {
            str[len] = '\0';
        } else {
            clearInputBuffer();
        }
    } else {
        str[0] = '\0';
    }
}


// QUEUE FUNCTIONS

void initQueue(Queue *q)
{
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

bool isQueueEmpty(const Queue *q)
{
    return q->count == 0;
}

bool isQueueFull(const Queue *q)
{
    return q->count == MAX_VERTICES;
}

bool enqueue(Queue *q, int value)
{
    if (isQueueFull(q))
        return false;

    q->rear = (q->rear + 1) % MAX_VERTICES;
    q->items[q->rear] = value;
    q->count++;

    return true;
}

int dequeue(Queue *q)
{
    int value;

    if (isQueueEmpty(q))
        return -1;

    value = q->items[q->front];
    q->front = (q->front + 1) % MAX_VERTICES;
    q->count--;

    return value;
}

//UTILITY FUNCTIONS

int caseInsensitiveCompare(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        int diff = tolower((unsigned char)*s1)
                 - tolower((unsigned char)*s2);

        if (diff != 0)
            return diff;

        s1++;
        s2++;
    }

    return tolower((unsigned char)*s1)
         - tolower((unsigned char)*s2);
}

const char *getStatusString(ComponentStatus status)
{
    if (status == STATUS_FAULTY)
        return "FAULTY";

    return "OPERATIONAL";
}

int findComponentIndex(const MachineGraph *graph, const char *name)
{
    int i;

    for (i = 0; i < graph->numVertices; i++) {
        if (caseInsensitiveCompare(
                graph->vertices[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}


//GRAPH FUNCTIONS

void initGraph(MachineGraph *graph, const char *machineName)
{
    int i;

    graph->numVertices = 0;

    strncpy(graph->machineName, machineName, MAX_NAME_LEN - 1);
    graph->machineName[MAX_NAME_LEN - 1] = '\0';

    for (i = 0; i < MAX_VERTICES; i++) {
        graph->vertices[i].name[0] = '\0';
        graph->vertices[i].status = STATUS_OPERATIONAL;
        graph->vertices[i].criticality = 1;
        graph->vertices[i].head = NULL;
    }
}

void freeGraph(MachineGraph *graph)
{
    int i;

    for (i = 0; i < graph->numVertices; i++) {
        AdjListNode *current = graph->vertices[i].head;

        while (current != NULL) {
            AdjListNode *temp = current;
            current = current->next;
            free(temp);
        }

        graph->vertices[i].head = NULL;
    }

    graph->numVertices = 0;
}

int addComponent(MachineGraph *graph,
                 const char *name,
                 int criticality)
{
    int index;

    if (graph->numVertices >= MAX_VERTICES) {
        printf("\nError: Maximum of %d components allowed.\n",
               MAX_VERTICES);
        return -1;
    }

    if (strlen(name) == 0) {
        printf("\nError: Component name cannot be empty.\n");
        return -1;
    }

    if (findComponentIndex(graph, name) != -1) {
        printf("\nError: Component already exists.\n");
        return -1;
    }

    if (criticality < MIN_CRITICALITY ||
        criticality > MAX_CRITICALITY) {

        printf("Invalid criticality. Default value 3 used.\n");
        criticality = 3;
    }

    index = graph->numVertices;

    strncpy(graph->vertices[index].name,
            name,
            MAX_NAME_LEN - 1);

    graph->vertices[index].name[MAX_NAME_LEN - 1] = '\0';
    graph->vertices[index].status = STATUS_OPERATIONAL;
    graph->vertices[index].criticality = criticality;
    graph->vertices[index].head = NULL;

    graph->numVertices++;

    return index;
}

bool addDependency(MachineGraph *graph,
                    const char *sourceName,
                    const char *destinationName)
{
    int source;
    int destination;
    AdjListNode *current;
    AdjListNode *newNode;

    source = findComponentIndex(graph, sourceName);
    destination = findComponentIndex(graph, destinationName);

    if (source == -1 || destination == -1) {
        printf("\nError: One or both components were not found.\n");
        return false;
    }

    if (source == destination) {
        printf("\nError: A component cannot depend on itself.\n");
        return false;
    }

    current = graph->vertices[source].head;

    while (current != NULL) {
        if (current->destIndex == destination) {
            printf("\nError: Dependency already exists.\n");
            return false;
        }

        current = current->next;
    }

    newNode = (AdjListNode *)malloc(sizeof(AdjListNode));

    if (newNode == NULL) {
        printf("\nError: Memory allocation failed.\n");
        return false;
    }

    newNode->destIndex = destination;
    newNode->next = graph->vertices[source].head;

    graph->vertices[source].head = newNode;

    return true;
}


//SAMPLE MACHINE

void loadSampleMachine(MachineGraph *graph)
{
    freeGraph(graph);

    initGraph(graph, "Sample Industrial Air Compressor");

    addComponent(graph, "Bearing", 3);
    addComponent(graph, "Cooling Fan", 2);
    addComponent(graph, "Cooling System", 3);
    addComponent(graph, "Motor", 5);
    addComponent(graph, "Shaft", 4);
    addComponent(graph, "Control Unit", 5);
    addComponent(graph, "Compressor Unit", 5);
    addComponent(graph, "Pressure Sensor", 2);

    
// A -> B means B depends on A

    addDependency(graph, "Bearing", "Motor");
    addDependency(graph, "Cooling Fan", "Cooling System");
    addDependency(graph, "Cooling System", "Motor");
    addDependency(graph, "Cooling System", "Compressor Unit");
    addDependency(graph, "Motor", "Shaft");
    addDependency(graph, "Motor", "Control Unit");
    addDependency(graph, "Shaft", "Compressor Unit");
    addDependency(graph, "Control Unit", "Compressor Unit");
    addDependency(graph, "Compressor Unit", "Pressure Sensor");
}


// DISPLAY FUNCTIONS
void displayMachineStatus(const MachineGraph *graph)
{
    int i;

    printf("\n------------------------------------------------------------\n");
    printf("MACHINE HEALTH REPORT\n");
    printf("Machine: %s\n", graph->machineName);
    printf("------------------------------------------------------------\n");

    printf("%-4s %-25s %-12s %-15s\n",
           "ID", "Component", "Criticality", "Status");

    printf("------------------------------------------------------------\n");

    for (i = 0; i < graph->numVertices; i++) {
        printf("%-4d %-25s %-12d %-15s\n",
               i + 1,
               graph->vertices[i].name,
               graph->vertices[i].criticality,
               getStatusString(graph->vertices[i].status));
    }

    printf("------------------------------------------------------------\n");
}

void displayAdjacencyList(const MachineGraph *graph)
{
    int i;

    printf("\n------------------------------------------------------------\n");
    printf("MACHINE DEPENDENCY GRAPH\n");
    printf("------------------------------------------------------------\n");

    for (i = 0; i < graph->numVertices; i++) {

        AdjListNode *current =
            graph->vertices[i].head;

        printf("%s (Criticality %d) -> ",
               graph->vertices[i].name,
               graph->vertices[i].criticality);

        if (current == NULL) {
            printf("No downstream components");
        }

        while (current != NULL) {
            printf("%s",
                   graph->vertices[current->destIndex].name);

            if (current->next != NULL)
                printf(" -> ");

            current = current->next;
        }

        printf("\n");
    }

    printf("------------------------------------------------------------\n");
}


//BFS FAILURE IMPACT ANALYSIS

void analyzeFailureBFS(const MachineGraph *graph, int startIndex)
{
    bool visited[MAX_VERTICES] = { false };
    int level[MAX_VERTICES];
    Queue queue;

    int i;
    int currentIndex;
    int totalAffected = 0;
    int criticalAffected = 0;
    int maxLevel = 0;
    int impactScore = 0;

    for (i = 0; i < MAX_VERTICES; i++)
        level[i] = -1;

    initQueue(&queue);

    visited[startIndex] = true;
    level[startIndex] = 0;

    enqueue(&queue, startIndex);

    while (!isQueueEmpty(&queue)) {

        currentIndex = dequeue(&queue);

        if (currentIndex != startIndex) {
            totalAffected++;

            impactScore +=
                graph->vertices[currentIndex].criticality;

            if (graph->vertices[currentIndex].criticality >= 4)
                criticalAffected++;
        }

        AdjListNode *current =
            graph->vertices[currentIndex].head;

        while (current != NULL) {

            int nextIndex = current->destIndex;

            if (!visited[nextIndex]) {

                visited[nextIndex] = true;

                level[nextIndex] =
                    level[currentIndex] + 1;

                if (level[nextIndex] > maxLevel)
                    maxLevel = level[nextIndex];

                enqueue(&queue, nextIndex);
            }

            current = current->next;
        }
    }

    printf("BFS FAILURE IMPACT ANALYSIS\n");
    printf("------------------------------------------------------------\n");

    printf("Failed Component: %s\n",
           graph->vertices[startIndex].name);

    printf("Criticality: %d\n",
           graph->vertices[startIndex].criticality);

    printf("\nAffected Components:\n");

    for (i = 1; i <= maxLevel; i++) {

        printf("\nLevel %d:\n", i);

        for (int j = 0; j < graph->numVertices; j++) {

            if (level[j] == i) {

                printf("  %s - AT RISK - Criticality %d\n",
                       graph->vertices[j].name,
                       graph->vertices[j].criticality);
            }
        }
    }

    if (totalAffected == 0) {
        printf("\nNo downstream components are affected.\n");
    }

    printf("\n------------------------------------------------------------\n");
    printf("Total Components At Risk : %d\n", totalAffected);
    printf("High Criticality At Risk : %d\n", criticalAffected);
    printf("Maximum Impact Level     : %d\n", maxLevel);
    printf("Cumulative Impact Score  : %d\n", impactScore);
    printf("------------------------------------------------------------\n");
}


//   DFS FAILURE PATH ANALYSIS
void dfsTrace(const MachineGraph *graph,
              int current,
              bool visited[],
              int path[],
              int pathLength,
              int *pathCount)
{
    AdjListNode *edge;
    bool hasNext = false;

    visited[current] = true;
    path[pathLength] = current;
    pathLength++;

    edge = graph->vertices[current].head;

    while (edge != NULL) {
        int next = edge->destIndex;

        if (!visited[next]) {
            hasNext = true;

            dfsTrace(graph,
                     next,
                     visited,
                     path,
                     pathLength,
                     pathCount);
        }

        edge = edge->next;
    }

    // Print path if it reaches a terminal node or has no unvisited downstream nodes
    if (!hasNext && pathLength > 1) {
        int i;

        (*pathCount)++;

        printf("Path %d: ", *pathCount);

        for (i = 0; i < pathLength; i++) {
            printf("%s", graph->vertices[path[i]].name);

            if (i < pathLength - 1) {
                printf(" -> ");
            }
        }

        printf("\n");
    }

    visited[current] = false;
}

void traceFailureDFS(const MachineGraph *graph,
                     int startIndex)
{
    bool visited[MAX_VERTICES] = { false };
    int path[MAX_VERTICES];
    int pathCount = 0;

    printf("\n------------------------------------------------------------\n");
    printf("DFS FAILURE PROPAGATION PATHS\n");
    printf("------------------------------------------------------------\n");

    printf("Starting Component: %s\n\n",
           graph->vertices[startIndex].name);

    dfsTrace(graph,
             startIndex,
             visited,
             path,
             0,
             &pathCount);

    if (pathCount == 0) {
        printf("No downstream propagation path found.\n");
    }

    printf("------------------------------------------------------------\n");
}



//UPSTREAM DEPENDENCY ANALYSIS
void findUpstreamDependencies(const MachineGraph *graph,
                              const char *targetName)
{
    int targetIndex;
    int i;
    int count = 0;

    targetIndex =
        findComponentIndex(graph, targetName);

    if (targetIndex == -1) {
        printf("\nComponent not found.\n");
        return;
    }

    printf("\n------------------------------------------------------------\n");
    printf("UPSTREAM DEPENDENCY ANALYSIS\n");
    printf("------------------------------------------------------------\n");

    printf("Target Component: %s\n\n",
           graph->vertices[targetIndex].name);

    printf("Components directly feeding this component:\n");

    for (i = 0; i < graph->numVertices; i++) {

        AdjListNode *current =
            graph->vertices[i].head;

        while (current != NULL) {

            if (current->destIndex == targetIndex) {

                printf("  %s - Criticality %d\n",
                       graph->vertices[i].name,
                       graph->vertices[i].criticality);

                count++;
                break;
            }

            current = current->next;
        }
    }

    if (count == 0) {
        printf("  No upstream components found.\n");
    }

    printf("------------------------------------------------------------\n");
}


// COMPONENT SEARCH
void searchComponent(const MachineGraph *graph)
{
    char name[MAX_NAME_LEN];
    int index;

    printf("\nEnter component name: ");
    readString(name, MAX_NAME_LEN);

    index = findComponentIndex(graph, name);

    if (index == -1) {
        printf("\nComponent not found.\n");
        return;
    }

    printf("\n------------------------------------------------------------\n");
    printf("COMPONENT INFORMATION\n");
    printf("------------------------------------------------------------\n");

    printf("Name        : %s\n",
           graph->vertices[index].name);

    printf("Criticality : %d\n",
           graph->vertices[index].criticality);

    printf("Status      : %s\n",
           getStatusString(graph->vertices[index].status));

    printf("------------------------------------------------------------\n");
}


//RESET MACHINE
void resetMachine(MachineGraph *graph)
{
    int i;

    for (i = 0; i < graph->numVertices; i++) {
        graph->vertices[i].status =
            STATUS_OPERATIONAL;
    }

    printf("\nAll components have been reset to OPERATIONAL.\n");
}



// ADD COMPONENT
void addNewComponent(MachineGraph *graph)
{
    char name[MAX_NAME_LEN];
    int criticality;

    printf("\nEnter component name: ");
    readString(name, MAX_NAME_LEN);

    printf("Enter criticality (1-5): ");

    if (scanf("%d", &criticality) != 1) {
        clearInputBuffer();
        printf("Invalid input. Criticality set to 3.\n");
        criticality = 3;
    } else {
        clearInputBuffer();
    }

    if (addComponent(graph, name, criticality) != -1) {
        printf("\nComponent '%s' added successfully.\n",
               name);
    }
}


//   ADD DEPENDENCY
void addNewDependency(MachineGraph *graph)
{
    char source[MAX_NAME_LEN];
    char destination[MAX_NAME_LEN];

    printf("\nEnter source component: ");
    readString(source, MAX_NAME_LEN);

    printf("Enter dependent component: ");
    readString(destination, MAX_NAME_LEN);

    if (addDependency(graph, source, destination)) {

        printf("\nDependency added:\n");
        printf("%s -> %s\n",
               source,
               destination);
    }
}


//  CREATE CUSTOM MACHINE
void createCustomMachine(MachineGraph *graph)
{
    char machineName[MAX_NAME_LEN];
    int numberOfComponents;
    int i;

    freeGraph(graph);

    printf("\nEnter machine name: ");
    readString(machineName, MAX_NAME_LEN);

    if (strlen(machineName) == 0) {
        strcpy(machineName, "Custom Industrial Machine");
    }

    initGraph(graph, machineName);

    printf("\nHow many components do you want to add? ");
    
    if (scanf("%d", &numberOfComponents) != 1) {
        clearInputBuffer();
        numberOfComponents = 0;
    } else {
        clearInputBuffer();
    }

    if (numberOfComponents < 0)
        numberOfComponents = 0;

    if (numberOfComponents > MAX_VERTICES)
        numberOfComponents = MAX_VERTICES;

    for (i = 0; i < numberOfComponents; i++) {

        printf("\nComponent %d\n", i + 1);

        addNewComponent(graph);
    }

    printf("\nCustom machine created successfully.\n");
}


//   MACHINE CONFIGURATION

void selectMachineConfiguration(MachineGraph *graph)
{
    int choice;

    printf("\n------------------------------------------------------------\n");
    printf("MACHINE CONFIGURATION\n");
    printf("------------------------------------------------------------\n");
    printf("1. Load Sample Air Compressor\n");
    printf("2. Create Custom Machine\n");
    printf("------------------------------------------------------------\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        choice = 1;
    } else {
        clearInputBuffer();
    }

    if (choice == 2) {
        createCustomMachine(graph);
    }
    else {
        loadSampleMachine(graph);
        printf("\nSample air compressor loaded successfully.\n");
    }
}


// REGISTER FAULT
void registerFault(MachineGraph *graph)
{
    char name[MAX_NAME_LEN];
    int index;

    printf("\nEnter faulty component name: ");
    readString(name, MAX_NAME_LEN);

    index = findComponentIndex(graph, name);

    if (index == -1) {
        printf("\nComponent not found.\n");
        return;
    }

    graph->vertices[index].status = STATUS_FAULTY;

    printf("\nFault registered for '%s'.\n",
           graph->vertices[index].name);

    printf("\nRunning BFS impact analysis...\n");

    analyzeFailureBFS(graph, index);

    printf("\nRunning DFS propagation analysis...\n");

    traceFailureDFS(graph, index);
}


int main(void)
{
    MachineGraph machine;
    int choice;


    initGraph(&machine, "Unconfigured Machine");

    printf("\n------------------------------------------------------------\n");
    printf("MACHINE SAATHI \n");
    printf("Industrial Machine Dependency and Fault Analyzer\n");
    printf("------------------------------------------------------------\n");

    selectMachineConfiguration(&machine);

    while (1) {

    printf("\n------------------------------------------------------------\n");
    printf("MACHINE SAATHI\n");
    printf("Machine: %s\n", machine.machineName);
    printf("------------------------------------------------------------\n");

    printf("1. Display Machine Health\n");
    printf("2. View Dependency Graph\n");
    printf("3. Register Component Fault\n");
    printf("4. BFS Fault Impact Analysis\n");
    printf("5. DFS Fault Propagation Paths\n");
    printf("6. Upstream Dependency Analysis\n");
    printf("7. Search Component\n");
    printf("8. Add New Component\n");
    printf("9. Add Dependency\n");
    printf("10. Reset Machine Health\n");
    printf("11. Change Machine Configuration\n");
    printf("12. Exit\n");

    printf("------------------------------------------------------------\n");
    printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\nInvalid input. Please enter a number.\n");
            continue;
        }

        clearInputBuffer();

        switch (choice) {

            case 1:
                displayMachineStatus(&machine);
                break;

            case 2:
                displayAdjacencyList(&machine);
                break;

            case 3:
                registerFault(&machine);
                break;

            case 4:
            {
                char name[MAX_NAME_LEN];
                int index;

                printf("\nEnter component name: ");
                readString(name, MAX_NAME_LEN);

                index =
                    findComponentIndex(&machine, name);

                if (index == -1) {
                    printf("\nComponent not found.\n");
                }
                else {
                    analyzeFailureBFS(&machine, index);
                }

                break;
            }

            case 5:
            {
                char name[MAX_NAME_LEN];
                int index;

                printf("\nEnter component name: ");
                readString(name, MAX_NAME_LEN);

                index =
                    findComponentIndex(&machine, name);

                if (index == -1) {
                    printf("\nComponent not found.\n");
                }
                else {
                    traceFailureDFS(&machine, index);
                }

                break;
            }

            case 6:
            {
                char name[MAX_NAME_LEN];

                printf("\nEnter component name: ");
                readString(name, MAX_NAME_LEN);

                findUpstreamDependencies(
                    &machine,
                    name);

                break;
            }

            case 7:
                searchComponent(&machine);
                break;

            case 8:
                addNewComponent(&machine);
                break;

            case 9:
                addNewDependency(&machine);
                break;

            case 10:
                resetMachine(&machine);
                break;

            case 11:
                selectMachineConfiguration(&machine);
                break;

            case 12:
                freeGraph(&machine);

                printf("\nMemory released successfully.\n");
                printf("Machine Saathi closed.\n");

                return 0;

            default:
                printf("\nInvalid choice. Please select 1-12.\n");
        }
    }

    return 0;
}
