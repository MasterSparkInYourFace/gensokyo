# TODO / Notes

- define protocol between gensokyonet daemon and client for issuing requests
- decide on protocols between gensokyonet nodes, managed by the daemon
	- support for one-time messages as well as connections
	- personally in favor of a simplified version of IPv6
- define how to expose these protocols to gensokyonet clients
- implement these protocols
- implement the daemon
- implement non-hardcoded types for nodes
- code running on nodes
	- daemon executes node code
	- code is unrestricted
	- implemented via plugins which provide a unique name and node type
	- allows for extending gensokyonet functionality without the need to alter core
- freezing a network on file
	- store map of node type names -> type ids
	- store the nodes themselves (address, name, type id, persistent data)
	- persistent data optionally provided by nodes on shutdown, passed to nodes on wakeup