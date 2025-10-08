#include "gtest/gtest.h"
#include "Automaton.h"

TEST( AutomatonIsValid, Default )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.isValid() );
}
TEST( AutomatonIsValid, Simple )
{
	fa::Automaton fa;
	fa.addState( 0 );
	fa.addSymbol( 'a' );
	EXPECT_TRUE( fa.isValid() );
}
TEST( AutomatonIsValid, NoLetter )
{
	fa::Automaton fa;
	fa.addState( 0 );
	EXPECT_FALSE( fa.isValid() );
}
TEST( AutomatonIsValid, NoState )
{
	fa::Automaton fa;
	fa.addSymbol( 'a' );
	EXPECT_FALSE( fa.isValid() );
}

TEST( AutomatonAddSymbol, Default )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
}
TEST( AutomatonAddSymbol, Epsilon )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( fa::Epsilon ) );
}
TEST( AutomatonAddSymbol, IndirectEpsilon )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( '\0' ) ); // Epsilon
}
TEST( AutomatonAddSymbol, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_FALSE( fa.addSymbol( 'a' ) );
}
TEST( AutomatonAddSymbol, Special )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( (char)50089 ) ); // (char)50089 => 'é'
	EXPECT_TRUE( fa.addSymbol( '(' ) ); // '() is an ASCII character and is graphable so it is valid
}
TEST( AutomatonAddSymbol, NoGraph )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( '\n' ) );
	EXPECT_FALSE( fa.addSymbol( '\t' ) );
	EXPECT_FALSE( fa.addSymbol( ' ' ) ); // Space is NOT graphable
	EXPECT_FALSE( fa.addSymbol( '\b' ) );
}

TEST( AutomatonRemoveSymbol, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.removeSymbol( 'a' ) );
	EXPECT_EQ( fa.countSymbols(), (std::size_t)1 );
}
TEST( AutomatonRemoveSymbol, Default )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.removeSymbol( 'a' ) );
	EXPECT_EQ( fa.countSymbols(), (std::size_t)0 );
}
TEST( AutomatonRemoveSymbol, SymbolNotFound )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_FALSE( fa.removeSymbol( 'x' ) );
	EXPECT_EQ( fa.countSymbols(), (std::size_t)2 );
}
TEST( AutomatonRemoveSymbol, SpecialSymbol )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_FALSE( fa.addSymbol( (char)50080 ) ); // (char)50080 => 'à'
	EXPECT_FALSE( fa.removeSymbol( (char)50080 ) ); // (char)50080 => 'à'
	EXPECT_EQ( fa.countSymbols(), (std::size_t)1 );
}

TEST( AutomatonHasSymbol, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 's' ) );
	EXPECT_TRUE( fa.hasSymbol( 's' ) );
}
TEST( AutomatonHasSymbol, Default )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.hasSymbol( 'a' ) );
}
TEST( AutomatonHasSymbol, SymbolNotFound )
{
	fa::Automaton fa;
	EXPECT_TRUE( 'A' );
	EXPECT_FALSE( fa.hasSymbol( 'a' ) );
}
TEST( AutomatonHasSymbol, Special )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( (char)50089 ) ); // (char)50089 => 'é'
	EXPECT_FALSE( fa.hasSymbol( (char)50089 ) ); // (char)50089 => 'é'
}
TEST( AutomatonHasSymbol, Epsilon )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( '\0' ) );
	EXPECT_FALSE( fa.hasSymbol( fa::Epsilon ) );
}
TEST( AutomatonHasSymbol, CorrectConversion )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 69 ) );
	EXPECT_TRUE( fa.hasSymbol( 'E' ) ); // (char)69 => 'E'
}

TEST( AutomatonCountSymbol, Default )
{
	fa::Automaton fa;
	EXPECT_EQ( fa.countSymbols(), (std::size_t)0 );
}
TEST( AutomatonCountSymbol, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'B' ) );
	EXPECT_EQ( fa.countSymbols(), (std::size_t)2 );
}
TEST( AutomatonCountSymbol, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_FALSE( fa.addSymbol( 'a' ) );
	EXPECT_EQ( fa.countSymbols(), (std::size_t)1 );
}
TEST( AutomatonCountSymbol, NoGraph )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( '\n' ) );
	EXPECT_FALSE( fa.addSymbol( '\t' ) );
	EXPECT_FALSE( fa.addSymbol( ' ' ) );
	EXPECT_FALSE( fa.addSymbol( '\b' ) );
	EXPECT_FALSE( fa.addSymbol( '\0' ) );
	EXPECT_EQ( fa.countSymbols(), (std::size_t)0 );
}
TEST( AutomatonCountSymbol, Special )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( (char)50080 ) ); // (char)50080 => 'à'
	EXPECT_EQ( fa.countSymbols(), (std::size_t)0 );
}

TEST( AutomatonAddState, Default )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 10 ) );
}
TEST( AutomatonAddState, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_FALSE( fa.addState( 0 ) );
}

TEST( AutomatonRemoveState, Default )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.removeState( 0 ) );
}
TEST( AutomatonRemoveState, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.removeState( 0 ) );
	EXPECT_FALSE( fa.hasState( 0 ) );
}
TEST( AutomatonRemoveState, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.removeState( 0 ) );
	EXPECT_FALSE( fa.removeState( 0 ) );
	EXPECT_FALSE( fa.hasState( 0 ) );
}

TEST( AutomatonHasState, Default )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.hasState( 0 ) );
}
TEST( AutomatonHasState, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 10 ) );
	EXPECT_TRUE( fa.hasState( 0 ) );
	EXPECT_TRUE( fa.hasState( 10 ) );
}
TEST( AutomatonHasState, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_FALSE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.hasState( 0 ) );
}
TEST( AutomatonHasState, Multiple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.hasState( 0 ) );
	EXPECT_TRUE( fa.hasState( 0 ) );
}
TEST( AutomatonHasState, NotFound )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 10 ) );
	EXPECT_FALSE( fa.hasState( 5 ) );
}

TEST( AutomatonCountState, Default )
{
	fa::Automaton fa;
	EXPECT_EQ( fa.countStates(), (std::size_t)0 );
}
TEST( AutomatonCountState, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 10 ) );
	EXPECT_TRUE( fa.addState( 100 ) );
	EXPECT_EQ( fa.countStates(), (std::size_t)3 );
}
TEST( AutomatonCountState, RemoveDefault )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.removeState( 42 ) );
	EXPECT_EQ( fa.countStates(), (std::size_t)0 );
}
TEST( AutomatonCountState, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_FALSE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 100 ) );
	EXPECT_EQ( fa.countStates(), (std::size_t)2 );
}
TEST( AutomatonCountState, RemoveSimple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 10 ) );
	EXPECT_TRUE( fa.addState( 100 ) );
	EXPECT_TRUE( fa.removeState( 0 ) );
	EXPECT_EQ( fa.countStates(), (std::size_t)2 );
}
TEST( AutomatonCountState, RemoveNone )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 10 ) );
	EXPECT_TRUE( fa.addState( 100 ) );
	EXPECT_FALSE( fa.removeState( 0 ) );
	EXPECT_EQ( fa.countStates(), (std::size_t)3 );
}

TEST( AutomatonInitalState, Default )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.isStateInitial( 0 ) );
}
TEST( AutomatonInitalState, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateInitial( 1 );
	EXPECT_TRUE( fa.isStateInitial( 0 ) );
	EXPECT_TRUE( fa.isStateInitial( 1 ) );
}
TEST( AutomatonInitialState, NotFound )
{
	fa::Automaton fa;
	fa.setStateInitial( 0 );
	fa.setStateInitial( 1 );
	EXPECT_FALSE( fa.hasState( 0 ) );
	EXPECT_FALSE( fa.hasState( 1 ) );
	EXPECT_FALSE( fa.isStateInitial( 0 ) );
	EXPECT_FALSE( fa.isStateInitial( 1 ) );
}
TEST( AutomatonInitalState, NotInitial )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	EXPECT_FALSE( fa.isStateInitial( 1 ) );
}
TEST( AutomatonInitalState, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateInitial( 0 );
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.isStateInitial( 0 ) );
	EXPECT_TRUE( fa.isStateInitial( 0 ) );
}

TEST( AutomatonFinalState, Default )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.isStateFinal( 0 ) );
}
TEST( AutomatonFinalState, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateFinal( 0 );
	fa.setStateFinal( 1 );
	EXPECT_TRUE( fa.hasState( 0 ) );
	EXPECT_TRUE( fa.hasState( 1 ) );
	EXPECT_TRUE( fa.isStateFinal( 0 ) );
	EXPECT_TRUE( fa.isStateFinal( 1 ) );
}
TEST( AutomatonFinalState, NotFound )
{
	fa::Automaton fa;
	fa.setStateFinal( 0 );
	fa.setStateFinal( 1 );
	EXPECT_FALSE( fa.hasState( 0 ) );
	EXPECT_FALSE( fa.hasState( 1 ) );
	EXPECT_FALSE( fa.isStateFinal( 0 ) );
	EXPECT_FALSE( fa.isStateFinal( 1 ) );
}
TEST( AutomatonFinalState, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateFinal( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.isStateFinal( 0 ) );
	EXPECT_TRUE( fa.isStateFinal( 0 ) );
}
TEST( AutomatonFinalState, Initial )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateFinal( 0 );
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.isStateFinal( 0 ) );
	EXPECT_TRUE( fa.isStateInitial( 0 ) );
}

TEST( AutomatonAddTransition, Default )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addTransition( 0, 'a', 1 ) );
}
TEST( AutomatonAddTransition, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );
}
TEST( AutomatonAddTransition, SimpleCycle )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
}
TEST( AutomatonAddTransition, ComplexCycle )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'a', 0 ) );
}
TEST( AutomatonAddTransition, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 1 ) );
	EXPECT_FALSE( fa.addTransition( 0, 'a', 1 ) );
}
TEST( AutomatonAddTransition, NoLetter )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_FALSE( fa.addTransition( 0, 'a', 1 ) );
}
TEST( AutomatonAddTransition, NoDestination )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_FALSE( fa.addTransition( 0, 'a', 1 ) );
}
TEST( AutomatonAddTransition, NoSource )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_FALSE( fa.addTransition( 1, 'a', 0 ) );
}
TEST( AutomatonAddTransition, OnlyLetter )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_FALSE( fa.addTransition( 0, 'a', 1 ) );
}
TEST( AutomatonAddTransition, OnlyDestination )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_FALSE( fa.addTransition( 1, 'a', 0 ) );
}
TEST( AutomatonAddTransition, OnlySource )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_FALSE( fa.addTransition( 0, 'a', 1 ) );
}
TEST( AutomatonAddTransition, Epsilon )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, fa::Epsilon, 1 ) );
}
TEST( AutomatonAddTransition, AlreadySource )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );
}
TEST( AutomatonAddTransition, NonDeterministic )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 2 ) );
}

TEST( AutomatonRemoveTransition, Default )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.removeTransition( 0, 'a', 1 ) );
}
TEST( AutomatonRemoveTransition, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.removeTransition( 0, 'a', 1 ) );
	EXPECT_FALSE( fa.removeTransition( 0, 'a', 1 ) );
}
TEST( AutomatonRemoveTransition, OnlySource )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_FALSE( fa.removeTransition( 0, 'a', 1 ) );
}
TEST( AutomatonRemoveTransition, OnlyDestination )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_FALSE( fa.removeTransition( 0, 'a', 1 ) );
}
TEST( AutomatonRemoveTransition, OnlyLetter )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_FALSE( fa.removeTransition( 0, 'a', 1 ) );
}
TEST( AutomatonRemoveTransition, NoSource )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_FALSE( fa.removeTransition( 0, 'a', 1 ) );
}
TEST( AutomatonRemoveTransition, NoDestination )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_FALSE( fa.removeTransition( 0, 'a', 1 ) );
}
TEST( AutomatonRemoveTransition, NoLetter )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_FALSE( fa.removeTransition( 0, 'a', 1 ) );
}
TEST( AutomatonRemoveTransition, SimpleCycle )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.removeTransition( 0, 'a', 0 ) );
}

TEST( AutomatonHasTransition, Default )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.hasTransition( 0, 'a', 1 ) );
}
TEST( AutomatonHasTransition, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.hasTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.hasTransition( 0, 'a', 1 ) );
}
TEST( AutomatonHasTransition, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 1 ) );
	EXPECT_TRUE( fa.hasTransition( 0, 'b', 1 ) );
}
TEST( AutomatonHasTransition, SimpleCycle )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.hasTransition( 0, 'a', 0 ) );
}
TEST( AutomatonHasTransition, Removed )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.removeTransition( 0, 'a', 1 ) );
	EXPECT_FALSE( fa.hasTransition( 0, 'a', 1 ) );
}

TEST( AutomatonCountTransitions, Default )
{
	fa::Automaton fa;
	EXPECT_EQ( fa.countTransitions(), (std::size_t)0 );
}
TEST( AutomatonCountTransitions, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 0 ) );
	EXPECT_EQ( fa.countTransitions(), (std::size_t)2 );
}
TEST( AutomatonCountTransitions, Remove )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 0 ) );
	EXPECT_TRUE( fa.removeTransition( 0, 'a', 1 ) );
	EXPECT_EQ( fa.countTransitions(), (std::size_t)1 );
}
TEST( AutomatonCountTransitions, RemoveNone )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 0 ) );
	EXPECT_FALSE( fa.removeTransition( 0, 'a', 1 ) );
	EXPECT_EQ( fa.countTransitions(), (std::size_t)1 );
}
TEST( AutomatonCountTransitions, AddNone )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addTransition( 1, 'a', 0 ) );
	EXPECT_EQ( fa.countTransitions(), (std::size_t)0 );
}

TEST( AutomatonPrettyPrint, Default )
{
	fa::Automaton fa;
	fa.prettyPrint( std::cout );
}
TEST( AutomatonPrettyPrint, IterA )
{
	fa::Automaton fa;
	fa.addSymbol( 'a' );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	fa.addTransition( 0, 'a', 0 );
	fa.prettyPrint( std::cout );
}
TEST( AutomatonPrettyPrint, Full4 )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	EXPECT_TRUE( fa.addState( 3 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 3 );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 2 ) );
	EXPECT_TRUE( fa.addTransition( 0, fa::Epsilon, 3 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 2 ) );
	EXPECT_TRUE( fa.addTransition( 1, fa::Epsilon, 3 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 1 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, fa::Epsilon, 3 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 3, fa::Epsilon, 2 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 3 ) );
	fa.prettyPrint( std::cout );
}

TEST( AutomatonIntegratedRemoval, Letter )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, fa::Epsilon, 0 ) );
	EXPECT_TRUE( fa.removeSymbol( 'a' ) );
	EXPECT_FALSE( fa.hasTransition( 0, 'a', 0 ) );
	EXPECT_FALSE( fa.hasTransition( 0, 'a', 1 ) );
	EXPECT_FALSE( fa.hasTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( fa.hasTransition( 1, fa::Epsilon, 0 ) );
	EXPECT_EQ( fa.countTransitions(), (std::size_t)1 );
}
TEST( AutomatonIntegratedRemoval, State )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 10 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 10 ) );
	EXPECT_TRUE( fa.addTransition( 10, 'a', 10 ) );
	EXPECT_TRUE( fa.removeState( 1 ) );
	EXPECT_FALSE( fa.hasTransition( 0, 'a', 1 ) );
	EXPECT_FALSE( fa.hasTransition( 1, 'a', 10 ) );
	EXPECT_TRUE( fa.hasTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.hasTransition( 10, 'a', 10 ) );
	EXPECT_EQ( fa.countTransitions(), (std::size_t)2 );
}


TEST( AutomatonHasEpsilonTransition, Default )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_FALSE( fa.hasEpsilonTransition() );
}
TEST( AutomatonHasEpsilonTransition, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, fa::Epsilon, 1 ) );
	EXPECT_TRUE( fa.hasEpsilonTransition() );
	EXPECT_TRUE( fa.hasEpsilonTransition() );
}
TEST( AutomatonHasEpsilonTransition, None )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_FALSE( fa.hasEpsilonTransition() );
}

}

int main( int argc, char ** argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
