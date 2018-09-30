# RIOTOS_PingPong_Exp
This experiment performs ping-pong of messages between 2 nodes.

Requirement: 2 virtual nodes or 2 hardware device.

## Steps to Install

1. Please install rdp_udp_pkt on first node/device.
2. Please install snd_udp_pkt on second node/device.

## Expected Execution
node1 acts as a server and node2 acts as a client.

1. node 1 is listening for a udp packet while node2 sends a udp packet to node1.
2. node 2 is listening for a udp packet while node1 sends a udp packet to node2 (acknowledgement).
3. These 2 steps should repeat n times in a loop.

First 2 steps gets executed successfully i.e. first iteration is a success but fails as soon as the 2nd iteration starts.
