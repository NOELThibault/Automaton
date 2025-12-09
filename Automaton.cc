#include "Automaton.h"
#include <iterator>
#include <assert.h>
#include <vector>
#include <utility>
#include <queue>

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

	// void Automaton::dotPrint( std::ostream & os ) const
	// {
	// }

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
		int stateNumber = -__INT32_MAX__;
		// We suppose there is less than 2 * INT32_MAX states in the automaton
		while( stateNumber < __INT32_MAX__ && !res.addState( stateNumber ) )
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

	Automaton Automaton::createComplement( const Automaton & automaton )
	{
		assert( automaton.isValid() );
		Automaton res = fa::Automaton::createComplete ( fa::Automaton::createDeterministic( automaton ) );

		for( int state : automaton.states )
		{
			if( automaton.isStateFinal( state ) ) // O( 1 )
				res.finalStates.erase( state ); // O( 1 )
			else
				res.setStateFinal( state ); // O( 1 )
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
		assert( isValid() );
		std::set< int > s = readString( word );

		for( int state : s )
		{
			if( isStateFinal( state ) ) // O( 1 )
				return true;
		}
		return false;
	}

	// O( n ) with n being the number of states
	void Automaton::depthSearch( std::unordered_map< int, bool > & visit, int state ) const
	{
		// Suppose the automaton is valid, state is in the automaton and visits has information on all states of the automaton
		visit[ state ] = true; // O( 1 )
		auto iter = transitions.find( state ); // O( 1 )
		// If the state isn't part of the transitions map then we don't need to search further
		if( iter == transitions.end() ) // O( 1 )
			return;

		for( auto [ letter, adjacents ] : iter->second )
		{
			for( int adj : adjacents )
			{
				if( !visit[ adj ] ) // O( 1 )
					depthSearch( visit, adj );
			}
		}
	}

	// O ( n² ) with n being the number of states
	bool Automaton::isLanguageEmpty() const
	{
		assert( isValid() );

		for( int origin : initialStates )
		{
			std::unordered_map< int, bool > visit;
			for( int state : states )
				visit[ state ] = false; // O( 1 )

			depthSearch( visit, origin ); // O( n ) with n being the number of states
			for( int f : finalStates )
			{
				if( visit[ f ] ) // O( 1 )
					return false;
			}
		}
		return true;
	}

	// O( n² + n * m ) with n being the number of states and m the number of transitions
	void Automaton::removeNonAccessibleStates()
	{
		assert( isValid() );

		std::unordered_map< int, bool > visit;
		for( int state : states )
			visit[ state ] = false; // O( 1 )

		for( int origin : initialStates )
		{
			depthSearch( visit, origin ); // O( n ) with n being the number of states
		}

		for( auto [ state, accessible ] : visit )
		{
			if( !accessible )
				removeState( state ); // O( m ) with m being the number of transitions
		}
	}

	// O( n² + n * m ) with n being the number of states and m the number of transitions
	void Automaton::removeNonCoAccessibleStates()
	{
		assert( isValid() );

		std::vector< int > toRemove;
		for( int state : states )
		{
			std::unordered_map< int, bool > visit;
			for( int state : states )
				visit[ state ] = false; // O( 1 )

			depthSearch( visit, state ); // O( n ) with n being the number of states

			bool coaccessible = false;
			for( int f : finalStates )
			{
				if( visit[ f ] ) // O( 1 )
				{
					coaccessible = true;
					break;
				}
			}
			if( !coaccessible )
				toRemove.push_back( state ); // O( 1 )
		}

		for( int state : toRemove )
			removeState( state ); // O( m ) with m being the number of transitions
	}

	// O( l + ) with l being the number of symbol in both automata and 
	Automaton Automaton::createIntersection( const Automaton & lhs, const Automaton & rhs )
	{
		assert( lhs.isValid() && rhs.isValid() );

		Automaton res;

		// The alphabet of the resulting automaton is the intersection of the 2 orignal alphabets
		for( char symbol : lhs.alphabet )
		{
			if( rhs.hasSymbol( symbol ) ) // O( 1 )
				res.addSymbol( symbol ); // O( 1 )
		}
		for( char symbol : rhs.alphabet )
		{
			if( lhs.hasSymbol( symbol ) ) // O( 1 )
				// No need to check if the symbol is already inserted thanks to the insert method of std::unordered_set
				res.addSymbol( symbol ); // O( 1 )
		}
		// If the intersection of both alphabets is empty, we add a letter to keep the validity of the result
		// We can add any letter we want since the letter has no importance for the automaton (no transitions)
		if( res.countSymbols() == 0 ) // O( 1 )
			res.addSymbol( 'a' ); // O( 1 )


		std::queue< std::tuple<int, int, int> > q; // State numbers following : (la, ra, res)
		int newState = 0;
		// Initial states of res
		for( int initL : lhs.initialStates )
		{
			for( int initR : rhs.initialStates )
			{
				res.addState( newState ); // O( 1 )
				q.push( std::tuple{ initL, initR, newState } );
				if( lhs.isStateFinal( initL ) && rhs.isStateFinal( initR ) ) // O( 1 )
					res.setStateFinal( newState ); // O( 1 )
				newState++;
			}
		}
		// No initial states means no intersection between initial states of lhs and rhs
		if( res.countStates() == 0 ) // O( 1 )
		{
			// We add a state to keep the validity of the automaton
			res.addState( 0 ); // O( 1 )
			return res;
		}
		// Run through synchronized transitions
		// while( !q.empty() )
		// {
		// 	std::tuple<int, int, int> current = q.front(); // O( 1 )
		// 	q.pop();
		// 	int lstate = std::get<0>( current );
		// 	int rstate = std::get<1>( current );
		// 	int state = std::get<2>( current );

		// 	// Skip loop if there are no transitions from lstate or rstate
		// 	if( lhs.transitions.count( lstate ) && rhs.transitions.count( rstate ) ) // O( 1 )
		// 	{
		// 		for( char letter : res.alphabet )
		// 		{
		// 			if( lhs.transitions.at( lstate ).count( letter ) && rhs.transitions.at( rstate ).count( letter ) ) // O( 1 )
		// 			{
		// 				int l = *lhs.transitions.at( lstate ).at( letter ).begin();
		// 				int r = *rhs.transitions.at( rstate ).at( letter ).begin();
		// 				if( res.addState( newState ) ) // O( 1 )
		// 				{
		// 					if( lhs.isStateFinal( l ) && rhs.isStateFinal( r ) ) // O( 1 )
		// 						res.setStateFinal( newState ); // O( 1 )
		// 					q.push( std::tuple{ l, r, newState } );
		// 					newState++;
		// 				}
		// 				res.addTransition( state, letter, newState ); // O( 1 )
		// 			}
		// 		}
		// 	}
		// }
		return res;
	}

	bool Automaton::hasEmptyIntersectionWith( const Automaton & other ) const
	{
		assert( isValid() && other.isValid() );
		return createIntersection( *this, other ).isLanguageEmpty();
	}

	// Suppose other has no Epsilon transition
	Automaton Automaton::createDeterministic( const Automaton & other )
	{
		assert( other.isValid() );
		Automaton res;

		res.alphabet = other.alphabet;
		res.addState( 0 ); // O( 1 )
		if( other.initialStates.size() == 0 ) // O( 1 )
			return res;

		std::queue< std::unordered_set< int > > q;
		q.push( other.initialStates ); // O( 1 )
		std::map< std::unordered_set< int >, int > stateMapping;
		stateMapping[ other.initialStates ] = 0; // O( 1 ) since the map is empty
	
		while( !q.empty() )
		{
			std::unordered_set< int > current = q.front(); // O( 1 )
			q.pop();
			int stateNumber = stateMapping[ current ]; // O( log n ) with n being the number of states in res

			// Check if current contains a final state
			for( int s : current )
			{
				if( other.isStateFinal( s ) ) // O( 1 )
				{
					res.setStateFinal( stateNumber ); // O( 1 )
					break;
				}
			}

			for( char letter : other.alphabet )
			{
				std::unordered_set< int > dests;
				// Compute the set of destinations from current using letter
				for( int s : current )
				{
					auto iterState = other.transitions.find( s ); // O( 1 )
					if( iterState == other.transitions.end() ) // O( 1 )
						continue;
					
					auto iterLetter = iterState->second.find( letter ); // O( 1 )
					if( iterLetter == iterState->second.end() ) // O( 1 )
						continue;

					// Loop on all transitions from the origin using letter
					for( int dest : iterLetter->second )
						dests.insert( dest ); // O( 1 )
				}
				if( dests.empty() ) // O( 1 )
					continue;

				// If this set of destinations is new, we add it to the automaton
				if( stateMapping.find( dests ) == stateMapping.end() ) // O( log n ) with n being the number of states in res
				{
					int newStateNumber = res.countStates(); // O( 1 )
					res.addState( newStateNumber ); // O( 1 )
					stateMapping[ dests ] = newStateNumber; // O( 1 )
					q.push( dests ); // O( 1 )
				}
				// Add the transition from stateNumber to the destination state
				res.addTransition( stateNumber, letter, stateMapping[ dests ] ); // O( 1 )
			}
		}
		return res;
	}

	bool Automaton::isIncludedIn( const Automaton & other ) const
	{
		assert( isValid() && other.isValid() );
		return hasEmptyIntersectionWith( fa::Automaton::createComplement( other ) );
	}
	
	Automaton Automaton::createMinimalMoore( const Automaton & other )
	{
		assert( other.isValid() );
		Automaton res = fa::Automaton::createDeterministic( other );
		return res;
	}

	Automaton Automaton::createMinimalBrzozowski( const Automaton & other )
	{
		assert( other.isValid() );
		Automaton res = fa::Automaton::createDeterministic( fa::Automaton::createMirror( fa::Automaton::createDeterministic( fa::Automaton::createMirror( other ) ) ) );
		return res;
	}
}