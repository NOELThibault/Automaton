#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <cstddef>
#include <iosfwd>
#include <iostream>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

namespace fa
{
	constexpr char Epsilon = '\0';

	class Automaton
	{
		public:
			/**
			 * Build an empty automaton (no state, no transition).
			 */
			Automaton();

			/**
			 * Tell if an automaton is valid.
			 *
			 * A valid automaton has a non-empty set of states and a non-empty set of symbols
			 */
			bool isValid() const;

			/**
			 * Add a symbol to the automaton
			 *
			 * Epsilon is not a valid symbol.
			 * Returns true if the symbol was effectively added
			 */
			bool addSymbol(char symbol);

			/**
			 * Remove a symbol from the automaton
			 *
			 * Returns true if the symbol was effectively removed
			 */
			bool removeSymbol(char symbol);

			/**
			 * Tell if the symbol is present in the automaton
			 */
			bool hasSymbol(char symbol) const;

			/**
			 * Count the number of symbols
			 */
			std::size_t countSymbols() const;

			/**
			 * Add a state to the automaton.
			 *
			 * By default, a newly added state is not initial and not final.
			 * Returns true if the state was effectively added and false otherwise.
			 */
			bool addState(int state);

			/**
			 * Remove a state from the automaton.
			 *
			 * The transitions involving the state are also removed.
			 * Returns true if the state was effectively removed and false otherwise.
			 */
			bool removeState(int state);

			/**
			 * Tell if the state is present in the automaton.
			 */
			bool hasState(int state) const;

			/**
			 * Compute the number of states.
			 */
			std::size_t countStates() const;

			/**
			 * Set the state initial.
			 */
			void setStateInitial(int state);

			/**
			 * Tell if the state is initial.
			 */
			bool isStateInitial(int state) const;

			/**
			 * Set the state final.
			 */
			void setStateFinal(int state);

			/**
			 * Tell if the state is final.
			 */
			bool isStateFinal(int state) const;

			/**
			 * Add a transition
			 *
			 * Returns true if the transition was effectively added and false otherwise.
			 * If one of the state or the symbol does not exists, the transition is not added.
			 */
			bool addTransition(int from, char alpha, int to);

			/**
			 * Remove a transition
			 *
			 * Returns true if the transition was effectively removed and false otherwise.
			 */
			bool removeTransition(int from, char alpha, int to);

			/**
			 * Tell if a transition is present.
			 */
			bool hasTransition(int from, char alpha, int to) const;

			/**
			 * Compute the number of transitions.
			 */
			std::size_t countTransitions() const;

			/**
			 * Print the automaton in a friendly way
			 */
			void prettyPrint(std::ostream& os) const;

			/**
			 * Print the automaton with respect to the DOT specification
			 */
			// void dotPrint(std::ostream& os) const;

			/**
			 * Tell if the automaton has one or more epsilon-transition
			 */
			bool hasEpsilonTransition() const;

			/**
			 * Tell if the automaton is deterministic
			 */
			bool isDeterministic() const;

			/**
			 * Tell if the automaton is complete
			 */
			bool isComplete() const;

			/**
			 * Make a transition from a set of states with a character.
			 */
			std::set<int> makeTransition(const std::set<int>& origin, char alpha) const;

			/**
			 * Read the string and compute the state set after traversing the automaton
			 */
			std::set<int> readString(const std::string& word) const;

			/**
			 * Tell if the word is in the language accepted by the automaton
			 */
			bool match(const std::string& word) const;

			/**
			 * Remove non-accessible states
			 */
			void removeNonAccessibleStates();

			/**
			 * Remove non-co-accessible states
			 */
			void removeNonCoAccessibleStates();

			/**
			 * Check if the language of the automaton is empty
			 */
			bool isLanguageEmpty() const;

			/**
			 * Tell if the intersection with another automaton is empty
			 */
			bool hasEmptyIntersectionWith(const Automaton& other) const;

			/**
			 * Tell if the langage accepted by the automaton is included in the
			 * language accepted by the other automaton
			 */
			bool isIncludedIn(const Automaton& other) const;

			/**
			 * Create a mirror automaton
			 */
			static Automaton createMirror(const Automaton& automaton);

			/**
			 * Create a complete automaton, if not already complete
			 */
			static Automaton createComplete(const Automaton& automaton);

			/**
			 * Create a complement automaton
			 */
			static Automaton createComplement(const Automaton& automaton);

			/**
			 * Create the intersection of the languages of two automata
			 */
			static Automaton createIntersection(const Automaton& lhs, const Automaton& rhs);

			/**
			 * Create a deterministic automaton, if not already deterministic
			 */
			static Automaton createDeterministic(const Automaton& other);

			/**
			 * Create an equivalent minimal automaton with the Moore algorithm
			 */
			static Automaton createMinimalMoore(const Automaton& other);

			/**
			 * Create an equivalent minimal automaton with the Brzozowski algorithm
			 */
			static Automaton createMinimalBrzozowski(const Automaton& other);


		private:
			// We use unordered_set in order to have a constant amortized complexity on insert, find and remove operations
			// It also assures not having any doubles and ordering is useless to us anyway
			// Worst case scenario ( O( n ) with n being the size of the container ) is when elements have the same hash
			// However, by using char, we should never get any collisions as the hash function to place elements in their bucket is perfectly distributed (the identity function suffices) 
			std::unordered_set< char > alphabet;

			// In the same way, hashing int is also extremely efficient with no collisions
			std::unordered_set< int > states;
			std::unordered_set< int > finalStates;
			std::unordered_set< int > initialStates;
			// We use a set of inital states to be able to model a non-deterministic automaton

			// On modélise les transitions de sorte que chaque sources possède une table de toutes ses destinations avec le symbole associé
			// On utilise donc une map dans une map
			// On choisi d'avoir la destination comme clé de la seconde map afin de potentiellement simplifier la suppression d'états, étant plus courantes que celles de symboles
			// On prendra des unordered_map car le hash d'un int est connu et l'ordre n'importe pas : on a des opérations usuelles de complexité constante
			// Afin de modéliser une relation de transitions, on utilise un unordered_set afin de maper à une destination plusieurs symboles différents
			// On note qu'il n'est pas possible d'insérer plusieurs fois la même transitions
			// States are mapped to a map of its transitions
			// This transitions map a destination to a set of symbol possible to read from the source to the destination
			// This set and both maps are unordered as the keys are int and char
			// This structure makes it impossible for the same transition to appear more than once
			std::unordered_map< int, std::unordered_map< int, std::unordered_set<char> > > transitions;
			// Saves the total count of transitions each change in order to skip calculations
			std::size_t transitionsCount;
	};
}

#endif // AUTOMATON_H
