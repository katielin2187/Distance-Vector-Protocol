#include <stdio.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;

struct distance_table {
    int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt3;
struct NeighborCosts   *neighbor3;
int minCosts[MAX_NODES];  //this is the minimun cost vector being kept track of
int currNode3 = 3;

/* students to write the following two routines, and maybe some others */

void printdt3(int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr);
void rtupdate3(struct RoutePacket *rcvdpkt);
void sendPackets3(int *initialCosts, struct distance_table *dtPtr , struct NeighborCosts *neighborPtr );

void rtinit3() {
    //make sure to
    // send packets only connected to the router
    //the current node being looked at
    neighbor3 = getNeighborCosts(currNode3); //gets the retun value for given neighbor
    int p=0;

    printf("Initial min costs:");
    for(p=0; p<MAX_NODES; p++) {
        minCosts[p]=INFINITY;
        printf("%d ", neighbor3->NodeCosts[p]);
    }

    //struct NeighborCosts* neighbor_table3 = neighbor3;
    struct distance_table *dt3ptr; //distance table pointer
    dt3ptr = &dt3;

    int k=0;
    int w=0;

    for(k=0;k<MAX_NODES;k++){
        for(w=0; w<MAX_NODES;w++){
            dt3ptr->costs[k][w] = INFINITY;  //set to infinity
        }
    }


    int i=0; //length of node costs
    int j = 0;

    printf("\nLet me see what bullshit is in here node3:");
    for(i=0; i < neighbor3->NodesInNetwork; i++){
        //put neighbors cost into distance table
        if(i != currNode3){
            for(j=0; j < neighbor3->NodesInNetwork; j++){
                if(i==j){
                    //the i at the end can also be j
                    dt3ptr->costs[i][j] = neighbor3->NodeCosts[i];
                    minCosts[i] = neighbor3->NodeCosts[i];
                    printf(" %d",minCosts[i]);
                }
            }
        } else if(i==currNode3){
            dt3ptr->costs[i][j] = 0;
            minCosts[i] = 0;
            printf(" %d",minCosts[i]);
        }

    }

    puts("\n");

//	prindt0(currNode, neighbor2, dt2ptr);

    //need to print, check tracelevel, if bigger than ,    print out dv table
    if(TraceLevel>0){
        printdt3(currNode3, neighbor3, dt3ptr);
    }


    sendPackets3(minCosts, dt3ptr, neighbor3);
    printf("At time t=%f, rtinit%d() called.\n", clocktime, currNode3);
//send that you have this setting to the other nodes
////make function to make the routing packets and send to layer2
//call send packets
}


void rtupdate3( struct RoutePacket *rcvdpkt) {

    struct NeighborCosts *neighbor_table3 = neighbor3;
    neighbor3 = getNeighborCosts(currNode3); //gets the retun value for given neighbor
    struct distance_table *dt3ptr; //distance table pointer
    dt3ptr = &dt3;

    int i = 0;
    int k = 0;
    int w = 0;

    int rcvd_id = rcvdpkt->sourceid;
    int update = NO;

    for (i = 0; i < MAX_NODES; i++) {
        //find the current minimun cost and compare it with the previous minimun cost
        int calculatedCost = dt3ptr->costs[rcvd_id][rcvd_id] + rcvdpkt->mincost[i];
        int prevCost = dt3ptr->costs[i][rcvd_id];

        if (calculatedCost < prevCost) {
            dt3ptr->costs[i][rcvd_id] = calculatedCost;
            update = YES;
        }
    }


    if (update == YES) {
        neighbor_table3 = getNeighborCosts(currNode3);
        int newMinCosts[MAX_NODES];

        for(k=0; k<neighbor_table3->NodesInNetwork; k++){
            newMinCosts[k] = neighbor_table3->NodeCosts[i];
        }

        for (k = 0; k < MAX_NODES; k++) {
            int min = INFINITY;
            for (w = 0; w < MAX_NODES; w++) {
                int tempCost = dt3ptr->costs[k][w];
                if (tempCost < min) {
                    min = tempCost;
                }
                minCosts[k] = min;
            }
        }

        if (TraceLevel > 0) {
            printdt3(currNode3, neighbor3, dt3ptr);
        }

        sendPackets3(minCosts, dt3ptr, neighbor_table3);

        int p=0;
        printf("\nCurrent min costs in neighbor3:");
        for(p=0; p<MAX_NODES; p++) {
            printf("%d ", neighbor3->NodeCosts[p]);
        }

        int f=0;
        printf("Current minCosts global3:");
        for(f=0; f<MAX_NODES; f++) {
            printf("%d ", minCosts[f]);
        }
        puts("\n");
    }

    printf("At time t=%f, rtupdate%d() called.\n", clocktime, currNode3);
}
void sendPackets3(int *initialCosts, struct distance_table *dtPtr , struct NeighborCosts *neighborPtr){

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
        if(i!=currNode3 && neighborPtr->NodeCosts[i]!= INFINITY) {
            newPacket.sourceid = currNode3;
            newPacket.destid = i;

            printf("At time: %f, node %d is sending a packet to node %d with:", clocktime, currNode3, i);

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
void printdt3(int MyNodeNumber, struct NeighborCosts *neighbor,
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

