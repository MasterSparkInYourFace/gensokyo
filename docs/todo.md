# TODO / Notes

- define protocol between gensokyonet daemon and client for issuing requests
- decide on protocols between gensokyonet nodes, managed by the daemon
	- support for one-time messages as well as connections
	- personally in favor of a simplified version of IPv6
- define how to expose these protocols to gensokyonet clients
- implement these protocols
- implement the daemon
- implement non-hardcoded types for nodes
- **code running on nodes**
	- daemon actually runs the code
	- code can do whatever it wants on the node
	- code registered locally via plugins, associated with a new node type
	- allows for extending gensokyonet functionality without the need to alter core
		- example: node that runs an embedded programming language