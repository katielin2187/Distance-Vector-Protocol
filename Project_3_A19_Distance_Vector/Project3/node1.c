#include <stdio.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;

struct distance_table {
    int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt1;
struct NeighborCosts   *neighbor1;
int minCosts[MAX_NODES];  //this is the minimun cost vector being kept track of
int currNode1 = 1;

/* students to write the following two routines, and maybe some others */

void printdt1(int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr);
void rtupdate1(struct RoutePacket *rcvdpkt);
void sendPackets1(int *initialCosts, struct distance_table *dtPtr , struct NeighborCosts *neighborPtr);

void rtinit1() {
    //make sure to
    // send packets only connected to the router
     //the current node being looked at
    neighbor1 = getNeighborCosts(currNode1); //gets the retun value for given neighbor
    int p=0;
    printf("Initial min costs:");
    for(p=0; p<MAX_NODES; p++) {
        minCosts[p]=INFINITY;
        printf("%d ", neighbor1->NodeCosts[p]);
    }
    struct NeighborCosts* neighbor_table1 = neighbor1;
    struct distance_table *dt1ptr; //distance table pointer
    dt1ptr = &dt1;

    int k=0;
    int w=0;

    for(k=0;k<MAX_NODES;k++){
        for(w=0; w<MAX_NODES;w++){
            dt1ptr->costs[k][w] = INFINITY;  //set to infinity
        }
    }


    int i=0; //length of node costs
    int j = 0;

    printf("\nLet me see what bullshit is in here node1:");
    for(i=0; i < neighbor1->NodesInNetwork; i++){
        //put neighbors cost into distance table
        if(i != currNode1){
            for(j=0; j < neighbor1->NodesInNetwork; j++){
                if(i==j){
                    //the i at the end can also be j
                    dt1ptr->costs[i][j] = neighbor1->NodeCosts[i];
                    minCosts[i] = neighbor1->NodeCosts[i];
                    printf(" %d",minCosts[i]);
                }
            }
        }else if(i==currNode1){
            dt1ptr->costs[i][j] = 0;
            minCosts[i] = 0;
            printf(" %d",minCosts[i]);
        }
    }

    puts("\n");


//	prindt0(currNode, neighbor2, dt0ptr);

    //need to print, check tracelevel, if bigger than , print out dv table
    if(TraceLevel>0){
        printdt1(currNode1, neighbor1, dt1ptr);
    }


    sendPackets1(minCosts, dt1ptr, neighbor1);
    printf("At time t=%f, rtinit%d() called.\n", clocktime, currNode1);
//send that you have this setting to the other nodes
////make function to make the routing packets and send to layer2
//call send packets
}


void rtupdate1( struct RoutePacket *rcvdpkt ) {

    struct NeighborCosts *neighbor_table1 = neighbor1;
    neighbor1 = getNeighborCosts(currNode1); //gets the retun value for given neighbor
    struct distance_table *dt1ptr; //distance table pointer
    dt1ptr = &dt1;

    int i = 0;
    int k = 0;
    int w = 0;

    int rcvd_id = rcvdpkt->sourceid;
    int update = NO;

    for (i = 0; i < MAX_NODES; i++) {
        //find the current minimun cost and compare it with the previous minimun cost
        int calculatedCost = dt1ptr->costs[rcvd_id][rcvd_id] + rcvdpkt->mincost[i];
        int prevCost = dt1ptr->costs[i][rcvd_id];

        if (calculatedCost < prevCost) {
            dt1ptr->costs[i][rcvd_id] = calculatedCost;
            update = YES;
        }
    }


    if (update == YES) {
        neighbor_table1 = getNeighborCosts(currNode1);
        int newMinCosts[MAX_NODES];

        for(k=0; k<neighbor_table1->NodesInNetwork; k++){
            newMinCosts[k] = neighbor_table1->NodeCosts[i];
        }

        for (k = 0; k < MAX_NODES; k++) {
            int min = INFINITY;
            for (w = 0; w < MAX_NODES; w++) {
                int tempCost = dt1ptr->costs[k][w];
                if (tempCost < min) {
                    min = tempCost;
                }
                minCosts[k] = min;
            }
        }

        if (TraceLevel > 0) {
            printdt1(currNode1, neighbor1, dt1ptr);
        }

        sendPackets1(minCosts, dt1ptr, neighbor_table1);

        int p=0;
        printf("\nCurrent min costs in neighbor1:");
        for(p=0; p<MAX_NODES; p++) {
            printf("%d ", neighbor1->NodeCosts[p]);
        }

        int f=0;
        printf("Current minCosts global1:");
        for(f=0; f<MAX_NODES; f++) {
            printf("%d ", minCosts[f]);
        }
        puts("\n");
    }

    printf("At time t=%f, rtupdate%d() called.\n", clocktime, currNode1);
}
void sendPackets1(int *initialCosts, struct distance_table *dtPtr , struct NeighborCosts *neighborPtr){

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
        if(i!=currNode1 && neighborPtr->NodeCosts[i]!= INFINITY) {
            newPacket.sourceid = currNode1;
            newPacket.destid = i;

            printf("At time: %f, node %d is sending a packet to node %d with:", clocktime, currNode1, i);

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
void printdt1(int MyNodeNumber, struct NeighborCosts *neighbor,
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

