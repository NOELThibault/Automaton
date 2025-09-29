#include "Automaton.h"
#include <iterator>

namespace fa
{
	// O( 1 )
	Automaton::Automaton()
	{
		transitionsCount = 0;
	}

	// O( 1 )
	bool Automaton::isValid() const
	{
		return !alphabet.empty() && !states.empty();
	}

	// O( 1 )
	bool Automaton::addSymbol( char symbol )
	{
		// Filter out all non-ASCII and non-graphable characters except Epsilon
		if( ( !isascii( symbol ) || !isgraph( symbol ) ) && symbol != fa::Epsilon ) // O( 1 )
			return false;

		return alphabet.insert( symbol ).second; // O( 1 )
	}

	// O( m ) with m being the total number of transitions
	bool Automaton::removeSymbol( char symbol )
	{
		if( !alphabet.erase( symbol ) ) // O( 1 ) since erase(symbol) is a O( count(symbol) ) and symbol is unique in the set
			return false;

		// There is a nested loop but it only iterates once through all the transitions
		for( auto [ source, transi ] : transitions )
		{
			for( auto [ dest, s ] : transi )
			{
				if( s.erase( symbol ) ) // O( 1 )
				{
					transitionsCount--;
					// Clean up structure if there us an empty container
					if( s.empty() ) // O( 1 )
					{
						transi.erase( dest ); // O( 1 ) since erase( dest ) is a O( count(dest) ) and dest is a unique key in the map
						if( transi.empty() ) // O( 1 )
							transitions.erase( source ); // O( 1 ) for the same reason
					}
				}
			}
		}
		return true;
	}

	// O( 1 ) since symbol is unique in the set
	bool Automaton::hasSymbol( char symbol ) const
	{
		return alphabet.count( symbol );
	}

	// O( 1 )
	std::size_t Automaton::countSymbols() const
	{
		return alphabet.size();
	}

	// O( 1 )
	bool Automaton::addState( int state )
	{
		return states.insert( state ).second;
	}

	// O( n ) with n equals the number of states being the source of a transitions, all of them in the worst case 
	bool Automaton::removeState( int state )
	{
		if( !states.erase( state ) ) // O( 1 )
			return false;

		initialStates.erase( state ); // O( 1 )
		finalStates.erase( state ); // O( 1 )
		// Loop an all the sources of a transition
		for( auto [ source, transi ] : transitions )
		{
			if( transi.erase( state ) ) // O( 1 )
			{
				transitionsCount--;
				// Clean up the structure if there is an empty container
				if( transi.empty() ) // O( 1 )
					transitions.erase( source ); // O( 1 ) 
			}
		}
		return true;
	}

	// O( 1 ) since state is unique in the map
	bool Automaton::hasState( int state ) const
	{
		return states.count( state );
	}

	// O( 1 )
	std::size_t Automaton::countStates() const
	{
		return states.size();
	}

	// O( 1 )
	void Automaton::setStateInitial( int state )
	{
		initialStates.insert( state );
	}

	// O( 1 )
	bool Automaton::isStateInitial( int state ) const
	{
		return initialStates.count( state );
	}

	// O( 1 )
	void Automaton::setStateFinal( int state )
	{
		finalStates.insert( state );
	}

	// O( 1 )
	bool Automaton::isStateFinal( int state ) const
	{
		return finalStates.count( state );
	}

	// O( 1 )
	bool Automaton::addTransition( int from, char alpha, int to )
	{
		// Filter out invalid parameters
		if( !hasState( from ) || !hasState( to ) || !hasSymbol( alpha ) ) // O( 1 )
		{
			return false;
		}

		auto iterFrom = transitions.find( from ); // O( 1 ) since from is unique in the map
		if( iterFrom == transitions.end() ) // O( 1 )
		{
			std::unordered_set<char> s;
			s.insert( alpha ); // O( 1 )

			std::unordered_map< int, std::unordered_set<char> > transi;
			transi.insert( std::pair{ to, s } ); // O( 1 )

			transitions.insert( std::pair{ from, transi } ); // O( 1 )
		}
		else
		{
			auto iterTo = iterFrom->second.find( to ); // O( 1 )
			if( iterTo == iterFrom->second.end() ) // O( 1 )
			{
				std::unordered_set<char> s;
				s.insert( alpha ); // O( 1 )

				iterFrom->second.insert( std::pair{ to, s } ); //O( 1 )
			}
			else
			{
				iterTo->second.insert( alpha ); // O( 1 )
			}
		}
		transitionsCount++;
		return true;
	}

	// O( 1 )
	bool Automaton::removeTransition( int from, char alpha, int to )
	{
		auto iterFrom = transitions.find( from ); // O( 1 )
		if( iterFrom == transitions.end() ) // O( 1 )
			return false;

		auto iterTo = iterFrom->second.find( to ); // O( 1 )
		if( iterTo == iterFrom->second.end() ) // O( 1 )
			return false;
		
		bool res = iterTo->second.erase( alpha ); // O( 1 )
		// Clean up the structure if there are empty containers
		if( iterTo->second.empty() )
			iterFrom->second.erase( iterTo->first ); // O( 1 )
		if( iterFrom->second.empty() )
			transitions.erase( iterFrom->first ); // O( 1 )
		
		if( res )
			transitionsCount--;		
		return res;
	}

	// O( 1 )
	bool Automaton::hasTransition( int from, char alpha, int to ) const
	{
		auto iterFrom = transitions.find( from ); // O( 1 )
		if( iterFrom == transitions.end() ) // O( 1 )
			return false;
	
		auto iterTo = iterFrom->second.find( to ); // O( 1 )
		if( iterTo == iterFrom->second.end() ) // O( 1 )
			return false;
		
		return iterTo->second.count( alpha ); // O( 1 )
	}

	// O( 1 )
	std::size_t Automaton::countTransitions() const
	{
		return transitionsCount;
	}
}