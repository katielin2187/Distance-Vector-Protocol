#include <stdio.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;

struct distance_table {
    int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt2;
struct NeighborCosts   *neighbor2;
int minCosts[MAX_NODES];  //this is the minimun cost vector being kept track of
int currNode2 = 2;

/* students to write the following two routines, and maybe some others */

void printdt2(int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr);
void rtupdate2(struct RoutePacket *rcvdpkt);
void sendPackets2(int *initialCosts, struct distance_table *dtPtr , struct NeighborCosts *neighborPtr);

void rtinit2() {
    //make sure to
    // send packets only connected to the router
    //the current node being looked at
    neighbor2 = getNeighborCosts(currNode2); //gets the retun value for given neighbor
    int p=0;
    printf("Initial min costs:");
    for(p=0; p<MAX_NODES; p++) {
        minCosts[p]=INFINITY;
        printf("%d ", neighbor2->NodeCosts[p]);
    }
    struct NeighborCosts* neighbor_table2 = neighbor2;
    struct distance_table *dt2ptr; //distance table pointer
    dt2ptr = &dt2;

    int k=0;
    int w=0;

    for(k=0;k<MAX_NODES;k++){
        for(w=0; w<MAX_NODES;w++){
            dt2ptr->costs[k][w] = INFINITY;  //set to infinity
        }
    }


    int i=0; //length of node costs
    int j = 0;

    printf("\nLet me see what bullshit is in here node2:");
    for(i=0; i < neighbor2->NodesInNetwork; i++){
        //put neighbors cost into distance table
        if(i != currNode2){
            for(j=0; j < neighbor2->NodesInNetwork; j++){
                if(i==j){
                    //the i at the end can also be j
                    dt2ptr->costs[i][j] = neighbor2->NodeCosts[i];
                    minCosts[i] = neighbor2->NodeCosts[i];
                    printf(" %d",minCosts[i]);
                }
            }
        }else if(i==currNode2){
            dt2ptr->costs[i][j] = 0;
            minCosts[i] = 0;
            printf(" %d",minCosts[i]);
        }
    }

    puts("\n");

//	prindt0(currNode, neighbor2, dt2ptr);

    //need to print, check tracelevel, if bigger than , print out dv table
    if(TraceLevel>0){
        printdt2(currNode2, neighbor2, dt2ptr);
    }

    if(TraceLevel>0){
        printdt2(currNode2, neighbor2, dt2ptr);
    }
    sendPackets2(minCosts, dt2ptr, neighbor2);
    printf("At time t=%f, rtinit%d() called.\n", clocktime, currNode2);

//send that you have this setting to the other nodes
////make function to make the routing packets and send to layer2
//call send packets
}


void rtupdate2( struct RoutePacket *rcvdpkt) {
    struct NeighborCosts *neighbor_table2 = neighbor2;
    neighbor2 = getNeighborCosts(currNode2); //gets the retun value for given neighbor
    struct distance_table *dt2ptr; //distance table pointer
    dt2ptr = &dt2;

    int i = 0;
    int k = 0;
    int w = 0;

    int rcvd_id = rcvdpkt->sourceid;
    int update = NO;

    for (i = 0; i < MAX_NODES; i++) {
        //find the current minimun cost and compare it with the previous minimun cost
        int calculatedCost = dt2ptr->costs[rcvd_id][rcvd_id] + rcvdpkt->mincost[i];
        int prevCost = dt2ptr->costs[i][rcvd_id];

        if (calculatedCost < prevCost) {
            dt2ptr->costs[i][rcvd_id] = calculatedCost;
            update = YES;
        }
    }


    if (update == YES) {
        neighbor_table2 = getNeighborCosts(currNode2);
        int newMinCosts[MAX_NODES];

        for(k=0; k<neighbor_table2->NodesInNetwork; k++){
            newMinCosts[k] = neighbor_table2->NodeCosts[i];
        }

        for (k = 0; k < MAX_NODES; k++) {
            int min = INFINITY;
            for (w = 0; w < MAX_NODES; w++) {
                int tempCost = dt2ptr->costs[k][w];
                if (tempCost < min) {
                    min = tempCost;
                }
                minCosts[k] = min;
            }
        }

        if (TraceLevel > 0) {
            printdt2(currNode2, neighbor2, dt2ptr);
        }

        sendPackets2(minCosts, dt2ptr, neighbor_table2);

        int p=0;
        printf("\nCurrent min costs in neighbor2:");
        for(p=0; p<MAX_NODES; p++) {
            printf("%d ", neighbor2->NodeCosts[p]);
        }

        int f=0;
        printf("Current minCosts global2:");
        for(f=0; f<MAX_NODES; f++) {
            printf("%d ", minCosts[f]);
        }
        puts("\n");
    }

    printf("At time t=%f, rtupdate%d() called.\n", clocktime, currNode2);
}
void sendPackets2(int *initialCosts, struct distance_table *dtPtr , struct NeighborCosts *neighborPtr){

//every time you send the ruting packets, print the nodes, print the router you are sending to
//
//ex) print sending from node0 to node 1
////At time, node is sending to other node with minimun costs: values x y z

    int i=0;
    int j=0;
    //int currMinCost = 0;
    struct RoutePacket newPacket;

    //newPacket = (struct RoutePacket*)malloc(sizeof(struct RoutePacket));

    for(i=0; i<neighborPtr->NodesInNetwork; i++){		//get the neighbors
        if(i!=currNode2 && neighborPtr->NodeCosts[i]!= INFINITY) {
            newPacket.sourceid = currNode2;
            newPacket.destid = i;

            printf("At time: %f, node %d is sending a packet to node %d with:", clocktime, currNode2, i);

            //se up the new packet, put min in min costs
            for(j=0; j<MAX_NODES; j++){
                newPacket.mincost[j] = minCosts[j];
                printf(" %d", newPacket.mincost[j]);
            }
            printf("\n");
            toLayer2(newPacket);

        }

    }

}
/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt2(int MyNodeNumber, struct NeighborCosts *neighbor,
              struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt0

