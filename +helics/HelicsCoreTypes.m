classdef HelicsCoreTypes < uint64
	%{
	pick a core type depending on compile configuration usually either ZMQ if available or TCP

	Attributes:
		HELICS_CORE_TYPE_DEFAULT: value:0	a default core type that will default to something available
		HELICS_CORE_TYPE_ZMQ: value:1	use the Zero MQ networking protocol
		HELICS_CORE_TYPE_MPI: value:2	use MPI for operation on a parallel cluster
		HELICS_CORE_TYPE_TEST: value:3	use the Test core if all federates are in the same process
		HELICS_CORE_TYPE_INTERPROCESS: value:4	interprocess uses memory mapped files to transfer data (for use when all federates are on the same machine
		HELICS_CORE_TYPE_IPC: value:5	interprocess uses memory mapped files to transfer data (for use when all federates are on the same machine ipc is the same as /ref HELICS_CORE_TYPE_interprocess
		HELICS_CORE_TYPE_TCP: value:6	use a generic TCP protocol message stream to send messages
		HELICS_CORE_TYPE_UDP: value:7	use UDP packets to send the data
		HELICS_CORE_TYPE_ZMQ_SS: value:10	single socket version of ZMQ core usually for high fed count on the same system
		HELICS_CORE_TYPE_NNG: value:9	for using the nanomsg communications
		HELICS_CORE_TYPE_TCP_SS: value:11	a single socket version of the TCP core for more easily handling firewalls
		HELICS_CORE_TYPE_HTTP: value:12	a core type using http for communication
		HELICS_CORE_TYPE_WEBSOCKET: value:14	a core using websockets for communication
		HELICS_CORE_TYPE_INPROC: value:18	an in process core type for handling communications in shared memory it is pretty similar to the test core but stripped from the "test" components
		HELICS_CORE_TYPE_NULL: value:66	an explicit core type that is recognized but explicitly doesn't exist, for testing and a few other assorted reasons
		HELICS_CORE_TYPE_EMPTY: value:77	an explicit core type exists but does nothing but return empty values or sink calls
	%}
	enumeration
		HELICS_CORE_TYPE_DEFAULT (0)
		HELICS_CORE_TYPE_ZMQ (1)
		HELICS_CORE_TYPE_MPI (2)
		HELICS_CORE_TYPE_TEST (3)
		HELICS_CORE_TYPE_INTERPROCESS (4)
		HELICS_CORE_TYPE_IPC (5)
		HELICS_CORE_TYPE_TCP (6)
		HELICS_CORE_TYPE_UDP (7)
		HELICS_CORE_TYPE_ZMQ_SS (10)
		HELICS_CORE_TYPE_NNG (9)
		HELICS_CORE_TYPE_TCP_SS (11)
		HELICS_CORE_TYPE_HTTP (12)
		HELICS_CORE_TYPE_WEBSOCKET (14)
		HELICS_CORE_TYPE_INPROC (18)
		HELICS_CORE_TYPE_NULL (66)
		HELICS_CORE_TYPE_EMPTY (77)
	end
end