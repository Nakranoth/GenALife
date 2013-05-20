GenALife
========

Generic Artificial Life Proof of Concept

TODO:
	Agency:
		Connection handling: Single server port with callback function. Accepts a connection, reads sizeof(Agent) bytes into new Agent's memory space.
			Multiple client destinations. Select at random for any clone or movement.  Decrement Agent's energy, Send agent, then delete locally if it moved.
