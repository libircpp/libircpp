#IRC TODO 

##Next Steps 
 * Transition to message_parser and use switch instead of phoenix based parser.cpp
 * Add modes (in production)

##Bugs

##Considerations and future ideas
 * Add a lower level API
 	+ Stateless
	+ High level built on top
 	+ Could have C binds?
	+ Isn't this just connection + parser?
 * Is hostmask a better name than prefix
 * Encapsulate irc::message, may have repocussion for BOOST_FUSION_ADAPT_STRU
 * Header only option
 	+ Header and non-headeronly modes
	+ Using ASIO rather than Boost.ASIO (Which uses Boost.System)
	+ CPP MACROS to optionally include .cpp/.ipp
	+ CPP MACROS to INLINE function (stopping multiple symbols)
 * Propper debugging/logging system
 * remove all direct invokations of cout/cerr (this is library)

