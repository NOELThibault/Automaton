#include "Automaton.h"
#include <iterator>
#include <assert.h>
#include <vector>
#include <utility>

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
		// Filter out all non-ASCII and non-graphable characters, Epsilon included
		if( !isascii( symbol ) || !isgraph( symbol ) ) // O( 1 )
			return false;

		return alphabet.insert( symbol ).second; // O( 1 ) since our buckets has to be of size 1
	}

	// O( m ) with m being the total number of transitions
	bool Automaton::removeSymbol( char symbol )
	{
		if( !alphabet.erase( symbol ) ) // O( 1 ) since our buckets has to be of size 1
			return false;

		// We will save all the transitions to remove before actually removing them in order to not mess with the number of loop and risk infinite loops or segfaults
		// There is a high probability that a solutions with only 1 loop exists
		std::vector< std::pair< int, char > > toRemove;
	
		// There is a nested loop but it only iterates once through all the pairs of (source, letter) present in our transitions
		for( auto [ source, transi ] : transitions )
		{
			for( auto [ letter, dests ] : transi )
			{
				if( letter == symbol ) // O( 1 )
				{
					toRemove.push_back( std::pair{ source, letter } ); // O( 1 ) amortized (there is no data in the documentation about the worst case)
				}
			}
		}
		for( auto [ source, letter ] : toRemove )
		{
			// There are possibly multiple transitions from source with letter
			transitionsCount -= transitions[ source ][ letter ].size(); // O( 1 ) since our buckets are of size 1
			transitions[ source ].erase( letter ); // O( 1 )
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

	// O( n ) with n being the number of (source, letter) pairs existing in the transitions ,which equals the number total of transitions in the case of a deterministic automaton
	bool Automaton::removeState( int state )
	{
		if( !states.erase( state ) ) // O( 1 )
			return false;

		initialStates.erase( state ); // O( 1 )
		finalStates.erase( state ); // O( 1 )
	
		transitionsCount -= transitions.erase( state ); // O( 1 )
	
		// We save all the (source, letter) that have state as one of their destinations
		std::vector< std::pair< int, char > > toRemove;
		for( auto [ source, transi ] : transitions )
		{
			for( auto [ letter, dests ] : transi )
			{
				if( dests.count( state ) ) // O( 1 )
					toRemove.push_back( std::pair{ source, letter } ); // O( 1 )
			}
		}
		for( auto [ source, letter ] : toRemove )
		{
			transitions[ source ][ letter ].erase( state ); // O( 1 )
			transitionsCount--;
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
		if( states.count( state ) ) // O( 1 )
			initialStates.insert( state ); // O( 1 )
		// The set data structure ensures we don't need to check if the state is already in
	}

	// O( 1 )
	bool Automaton::isStateInitial( int state ) const
	{
		return initialStates.count( state );
	}

	// O( 1 )
	void Automaton::setStateFinal( int state )
	{
		if( states.count( state ) ) // O( 1 )
			finalStates.insert( state ); // O( 1 )
	}

	// O( 1 )
	bool Automaton::isStateFinal( int state ) const
	{
		return finalStates.count( state );
	}

	// O( 1 )
	bool Automaton::addTransition( int from, char alpha, int to )
	{
		// Filter out invalid parameters ( Epsilon is a valid character, even if it cannot be in the alphabet )
		if( !hasState( from ) || !hasState( to ) || ( !hasSymbol( alpha ) && alpha != fa::Epsilon ) ) // O( 1 )
			return false;

		transitionsCount++;
		// The [] operator inserts elements if they are not already in
		return transitions[ from ][ alpha ].insert( to ).second; // O( 1 ) even in worst case since the buckets are of size 1
	}

	// O( 1 )
	bool Automaton::removeTransition( int from, char alpha, int to )
	{
		auto iterFrom = transitions.find( from ); // O( 1 )
		if( iterFrom == transitions.end() ) // O( 1 )
			return false;

		auto iterLetter = iterFrom->second.find( alpha ); // O( 1 )
		if( iterLetter == iterFrom->second.end() ) // O( 1 )
			return false;
		
		bool res = iterLetter->second.erase( to ); // O( 1 )
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
		
		auto transi = iterFrom->second;
		auto iterLetter = transi.find( alpha ); // O( 1 )
		if( iterLetter == transi.end() ) // O( 1 )
			return false;
		
		std::unordered_set< int > dests = iterLetter->second;
		return dests.count( to ); // O( 1 )
	}

	// O( 1 )
	std::size_t Automaton::countTransitions() const
	{
		return transitionsCount;
	}

	// O( n + m + l ) with n the number of transitions, m the number of states and l the size of the alphabet
	void Automaton::prettyPrint( std::ostream & os ) const
	{
		os << "Alphabet : {  ";
		for( char letter : alphabet )
		{
			os << letter << "  ";
		}
		os << "}\n";

		os << "States : ";
		for( int state : states )
		{
			os << state << "  ";
		}
		os << "\nInital states : ";
		for( int state : initialStates )
		{
			os << state << "  ";
		}
		os << "\nFinal states : ";
		for( int state : finalStates )
		{
			os << state << "  ";
		}

		os << "\nTransitions :\n";
		for( auto [ source, transi ] : transitions )
		{
			for( auto [ letter, destinations ] : transi )
			{
				for( int dest : destinations )
				{
					os << "\t" << source << " -> " << dest << " with ";
					if( letter == fa::Epsilon )
						os << "Epsilon\n";
					else
						os << letter << '\n';
				}
			}
		}
		os << std::endl;
	}

	void Automaton::dotPrint( std::ostream & os ) const
	{
	}

		for( auto [ source, transi ] : transitions )
		{
			for( auto [ destination, letters ] : transi )
			{
				for( char alpha : letters )
				{
					os << "\t" << source << " -> " << destination << " with " << alpha << "\n";
				}
			}
		}
	}
}