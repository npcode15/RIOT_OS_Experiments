# RIOTOS_PingPong_Exp
This experiment performs ping-pong of messages between 2 nodes.

## Requirement 
2 virtual nodes or 2 hardware device.

## Steps to Install

1. Please install server on first node/device.
2. Please install client on second node/device.

## Expected Execution
node1 acts as a server whereas node2 acts as a client.

1. node 1 is listening for a mac packet while node2 sends a mac packet to node1.
2. node 2 is listening for a mac packet while node1 sends a mac packet to node2 (acknowledgement).
3. These 2 steps should repeat n times in a loop.