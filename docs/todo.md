# TODO / Notes

- define protocol between gensokyonet daemon and client for issuing requests
- decide on protocols between gensokyonet nodes, marshaled by the daemon
	- support for one-time messages as well as connections
	- personally in favor of a simplified version of IPv6
- define how to expose these protocols to gensokyonet clients
- implement these protocols
- implement the daemon
- possibility to define custom node protocols with dynamic library plugins?
	- secondary, non-hardcoded way of identifying a node protocol
- **external code running on nodes?**
	- daemon actually runs the code
	- code can do whatever it wants on the node
	- native code registered locally via dynamic libraries
	- perl code registered locally or remotely from strings
	- uses gensokyo net protocols to communicate