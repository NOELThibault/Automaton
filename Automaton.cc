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

	// O( m ) with m being the number of state source of a transition, every state in the worst case
	bool Automaton::hasEpsilonTransition() const
	{
		assert( isValid() );

		// Loop on every source
		for( auto [ source, transi ] : transitions )
		{
			if( transi.find( fa::Epsilon ) != transi.end() ) // O( 1 )
				return true;
		}
		return false;
	}

	// O( n ) with n being the number of (source, letter) pairs existing in the transitions, which equals the number total of transitions in the case of a deterministic automaton
	bool Automaton::isDeterministic() const
	{
		assert( isValid() );

		if( initialStates.size() != 1 ) // O( 1 )
			return false;
	
		for( auto [ source, transi ] : transitions )
		{
			// For each sources, check if there is an Epsilon transition or if there are multiple destinations
			for( auto [ letter, dests ] : transi )
			{
				if( letter == fa::Epsilon || dests.size() > 1 ) // O( 1 )
					return false;
			}
		}
		return true;
	}

	// O( n ) with n being the total number of transitions
	bool Automaton::isComplete() const
	{
		assert( isValid() );

		// Skip heavy calculations if we know the automaton doesn't have enough transitions
		if( transitionsCount < states.size() * alphabet.size() ) // O( 1 )
			return false;
		
		// Loop all transitions
		for( auto [ source, transi ] : transitions )
		{
			// For each source, check if the number of letters departing from source is equal to the size of the alphabet ( + 1 if there is Epsilon )
			if( transi.size() != alphabet.size() + transi.count( fa::Epsilon ) ) // O( 1 )
				return false;
		}
		return true;
	}

	// O( n * l ) with n being the number states and l being the size of the alphabet
	Automaton Automaton::createComplete( const Automaton & automaton )
	{
		assert( automaton.isValid() );
		Automaton res = automaton;

		if( res.isComplete() ) // O( n ) with n being the total number of transitions
			return res;

		// We select a new number for the added state
		int stateNumber = automaton.countStates() + 1;
		// We suppose there is less than INT32_MAX states in the automaton ( the loop would be infinite )
		while( !res.addState( stateNumber ) )
			stateNumber++;

		for( int source : automaton.states )
		{
			auto transi = res.transitions.find( source );
			// If the state is source to no transitions
			if( transi == res.transitions.end() )
			{
				for( char letter : automaton.alphabet )
				{
					res.transitions[ source ][ letter ].insert( stateNumber ); // O( 1 )
					res.transitionsCount++;
				}
			}
			else if( transi->second.size() != automaton.alphabet.size() + transi->second.count( fa::Epsilon ) ) // O( 1 )
			{
				// If the source state isn't complete, we map all missing letters to the new state
				for( char letter : automaton.alphabet )
				{
					if( !transi->second.count( letter ) ) // O( 1 )
					{
						// The [] operator inserts elements if necessary
						transi->second[ letter ].insert( stateNumber ); // O( 1 )
						res.transitionsCount++;
					}
				}
			}
		}
		// Don't forget to make the new state loop on itself
		for( char letter : res.alphabet )
		{
			res.transitions[ stateNumber ][ letter ].insert( stateNumber ); // O( 1 )
			res.transitionsCount++;
		}
		return res;
	}

	// O( n + m ) with n being the total number of states and m the number of transitions
	Automaton Automaton::createComplement( const Automaton & automaton )
	{
		assert( automaton.isValid() );
		Automaton res = automaton;

		if( res.isDeterministic() && res.isComplete() ) // O( n ) with n being the number of transitions
		{
			for( int state : automaton.states )
			{
				if( automaton.isStateFinal( state ) ) // O( 1 )
					res.finalStates.erase( state ); // O( 1 )
				else
					res.setStateFinal( state ); // O( 1 )
			}
		}
		return res;
	}

	// O( n + l ) with n being the number of transitions and l being the number of initial and final states, every state in the worst case
	Automaton Automaton::createMirror( const Automaton & automaton )
	{
		assert( automaton.isValid() );
		Automaton res = automaton;

		for( int state : automaton.initialStates )
		{
			if( !automaton.isStateFinal( state ) ) // O( 1 )
			{
				res.setStateFinal( state ); // O( 1 )
				res.initialStates.erase( state ); // O( 1 )
			}
		}
		for( int state : automaton.finalStates )
		{
			if( !automaton.isStateInitial( state ) ) // O( 1 )
			{
				res.setStateInitial( state ); // O( 1 )
				res.finalStates.erase( state ); // O( 1 )
			}
		}

		for( auto [ source, transi ] : automaton.transitions )
		{
			for( auto [ letter, dests ] : transi )
			{
				for( int destination : dests )
				{
					res.removeTransition( source, letter, destination ); // O( 1 )
					res.addTransition( destination, letter, source ); // O( 1 )
				}
			}
		}
		return res;
	}

	// O( n * log n ) with n being the number of transitions from origin
	std::set< int > Automaton::makeTransition( const std::set< int > & origin, char alpha ) const
	{
		assert( isValid() );
		std::set< int > res;

		for( int state : origin )
		{
			auto iterState = transitions.find( state ); // O( 1 )
			if( iterState == transitions.end() ) // O( 1 )
			continue;
			
			auto iterAlpha = iterState->second.find( alpha ); // O( 1 )
			if( iterAlpha == iterState->second.end() ) // O( 1 )
				continue;

			// Loop on all transitions from the origin using alpha
			for( int dest : iterAlpha->second )
				res.insert( dest ); // O( log n ) with n being the number of resulting derivations, which equals the number of transitions from origin using alpha
		}
		return res;
	}

	// O( l * n * log n ) with l being the length of the word and n the number of transitions
	std::set< int > Automaton::readString( const std::string & word ) const
	{
		assert( isValid() );
		std::set< int > res;
		// We can't convert unordered_set to set so we need to insert all the intial states ourselves
		res.insert( initialStates.begin(), initialStates.end() ); // O( log n ) with n being the number of initial states

		for( char alpha : word )
		{
			res = makeTransition( res, alpha ); // O( n * log n ) with n being the number of transitions from res
		}
		return res;
	}

	// O( l * n * log n ) with l being the length of the word and n the number of transitions
	bool Automaton::match( const std::string & word ) const
	{
		std::set< int > s = readString( word );

		for( int state : s )
		{
			if( isStateFinal( state ) ) // O( 1 )
				return true;
		}
		return false;
	}
}