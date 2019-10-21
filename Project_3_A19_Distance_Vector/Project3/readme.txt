Author: Katie LIn
3/10/2019
CS 3516

#Distance Vector Routing Protocol

Distance Vector Routing Protocol is a program that implements a
distributed asynchronous distance vector (DV) routing protocol.

-----------------------------------------------------------------
Extra Notes:
I went to the TAs for help, and somehow, for some reason the inital min
costs are not being printed out right in the send packets function even though I 
initialised it like a global variable like Lei Ma told me to do.
------------------------------------------------------------------

Further Instructions: This program must be compiled using the following command:

	gcc -g -Wall -project3.c node0.c node1.c node2.c node3.c -o project3

First open up a terminal using the Linux system and then change the directory
to the directory where this current readme.txt is located. Then type then 
commands stated below:

run make -'make'
run project 3 -'./project3

-----------------------------------------------------------------

In each of the following files, node0.c, node1.c, node2.c, node3.c,
the following routines are executed asynchronously within the emulated environment:


rtinit0():
This routine will be called once at the beginning of the emulation. 
rtinit0() has no arguments. It should initialize the distance table 
in node0 to reflect the direct costs to is neighbors by using 
GetNeighborCosts(). In Figure 1, representing the default configuration,
all links are bi-directional and the costs in both directions are identical. 
After initializing the distance table, and any other data structures 
needed by your node0 routines, it should then send to its directly-connected 
neighbors (in the Figure, 1, 2 and 3) its minimum cost paths to all other
network nodes. This minimum cost information is sent to neighboring nodes 
in a routing packet by calling the routine tolayer2(), as described below.
The format of the routing packet is also described below. 

rtupdate0(struct rtpkt *rcvdpkt):
This routine will be called when node 0 receives a routing packet that was 
sent to it by one of its directly connected neighbors. The parameter
*rcvdpkt is a pointer to the packet that was received rtupdate0() is the 
"heart" of the distance vector algorithm. The values it receives in a routing 
packet from some other node i contain i's current shortest path costs to all 
other network nodes. rtupdate0() uses these received values to update its own 
distance table (as specified by the distance vector algorithm). If its own 
minimum cost to another node changes as a result of the update, node 0 informs its
directly connected neighbors of this change in minimum cost by sending them 
a routing packet.

sendPackets(int currNode, int initialCosts[], struct distance_table *dtPtr, 
struct NeighborCosts *neighborPtr):
This function is in charge of actually sending out the packets when a change in
the minimun cost is identified in rtupdate. rtupdate calls this function once it
identifies the minimum cost. It creates the packet, sets the source id and the destination 
id and then finds the minimun distance for each node and then sets the
minimum costs accordinly.
