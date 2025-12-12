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
TEST( AurtomatonAddState, Negative )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addState( -1 ) );
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

TEST( AutomatonIntegratedEpsilon, Default )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );

	EXPECT_FALSE( fa.hasTransition( 0, fa::Epsilon, 1 ) );
	EXPECT_FALSE( fa.hasEpsilonTransition() );
	EXPECT_EQ( fa.countTransitions(), 0u );

	EXPECT_TRUE( fa.addTransition( 0, fa::Epsilon, 1 ) );

	EXPECT_TRUE( fa.hasTransition( 0, fa::Epsilon, 1 ) );
	EXPECT_TRUE( fa.hasEpsilonTransition() );
	EXPECT_EQ( fa.countTransitions(), 1u );

	EXPECT_TRUE( fa.removeTransition( 0, fa::Epsilon, 1 ) );

	EXPECT_FALSE( fa.hasTransition( 0, fa::Epsilon, 1 ) );
	EXPECT_FALSE( fa.hasEpsilonTransition() );
	EXPECT_EQ( fa.countTransitions(), 0u );
}

TEST( AutomatonIsComplete, EmptyLanguage )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	fa.setStateInitial( 0 );
	EXPECT_FALSE( fa.isComplete() );
}
TEST( AutomatonIsComplete, aStar )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	fa.setStateFinal( 0 );
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.isComplete() );
}
TEST( AutomatonIsComplete, Full4 )
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
	EXPECT_TRUE( fa.addTransition( 1, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 1 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 3 ) );

	EXPECT_FALSE( fa.isComplete() );
}
TEST( AutomatonIsComplete, Full4Complete )
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
	EXPECT_TRUE( fa.addTransition( 0, 'b', 3 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 2 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 3 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 1 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 3 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'b', 1 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'b', 2 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 3 ) );

	EXPECT_TRUE( fa.isComplete() );
}

TEST( AutomatonCreateComplete, EmptyLanguage )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	fa.setStateInitial( 0 );
	EXPECT_FALSE( fa.isComplete() );
	EXPECT_EQ( fa.countTransitions(), (std::size_t)0 );

	fa::Automaton complete = fa::Automaton::createComplete( fa );
	EXPECT_TRUE( complete.isComplete() );

	EXPECT_EQ( complete.countSymbols(), (std::size_t)1 );
	EXPECT_TRUE( complete.hasSymbol( 'a' ) );

	EXPECT_EQ( complete.countStates(), (std::size_t)2 );

	EXPECT_EQ( complete.countTransitions(), (std::size_t)2 );
}
TEST( AutomatonCreateComplete, Complete )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 1 ) );

	EXPECT_TRUE( fa.isComplete() );

	fa::Automaton copy = fa::Automaton::createComplete( fa );
	EXPECT_TRUE( copy.isComplete() );

	EXPECT_EQ( copy.countStates(), (std::size_t)2 );
	EXPECT_TRUE( copy.hasSymbol( 'a' ) );
	EXPECT_TRUE( copy.hasSymbol( 'b' ) );
	EXPECT_EQ( copy.countSymbols(), (std::size_t)2 );
	EXPECT_EQ( copy.countTransitions(), (std::size_t)4 );
}

TEST( AutomatonCreateComplement, EmptyLanguage )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
}

TEST( AutomatonCreateMirror, EmptyLanguage )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );

	fa::Automaton m = fa::Automaton::createMirror( fa );
	EXPECT_TRUE( m.hasSymbol( 'a' ) );
	EXPECT_EQ( m.countSymbols(), (std::size_t)1 );
	EXPECT_EQ( m.countStates(), (std::size_t)1 );
	EXPECT_EQ( m.countTransitions(), (std::size_t)0 );
}
TEST( AutomatonCreateMirror, UnityLanguage )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );

	fa::Automaton m = fa::Automaton::createMirror( fa );
	EXPECT_TRUE( m.hasSymbol( 'a' ) );
	EXPECT_EQ( m.countSymbols(), (std::size_t)1 );
	EXPECT_TRUE( m.hasState( 0 ) );
	EXPECT_EQ( m.countStates(), (std::size_t)1 );
	EXPECT_TRUE( m.isStateFinal( 0 ) );
	EXPECT_TRUE( m.isStateInitial( 0 ) );
	EXPECT_EQ( m.countTransitions(), (std::size_t)0 );
}
TEST( AutomatonCreateMirror, abStar )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );

	fa::Automaton m = fa::Automaton::createMirror( fa );
	EXPECT_TRUE( m.hasState( 0 ) );
	EXPECT_TRUE( m.hasState( 1 ) );
	EXPECT_EQ( m.countStates(), (std::size_t)2 );
	EXPECT_TRUE( m.hasSymbol( 'a' ) );
	EXPECT_TRUE( m.hasSymbol( 'b' ) );
	EXPECT_EQ( m.countSymbols(), (std::size_t)2 );
	EXPECT_TRUE( m.hasTransition( 1, 'a', 0 ) );
	EXPECT_TRUE( m.hasTransition( 0, 'b', 1 ) );
	EXPECT_FALSE( m.hasTransition( 0, 'a', 1 ) );
	EXPECT_FALSE( m.hasTransition( 1, 'b', 0 ) );
	EXPECT_EQ( m.countTransitions(), (std::size_t)2 );
}

TEST( AutomatonMakeTransition, Deterministic )
{
	fa::Automaton fa; // (ab)*
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );

	std::set< int > source;
	source.insert( 0 );
	std::set< int > deriv = fa.makeTransition( source, 'a' );
	EXPECT_TRUE( deriv.count( 1 ) );
	EXPECT_EQ( deriv.size(), (std::size_t)1 );
}
TEST( AutomatonMakeTransition, NonDeterministic )
{
	fa::Automaton fa; // (ab)*(a + ab) = (ab)*a(b + Eps)
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	EXPECT_TRUE( fa.addState( 3 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 2 );
	fa.setStateFinal( 3 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 3 ) );

	std::set< int > source;
	source.insert( 0 );
	std::set< int > deriv = fa.makeTransition( source, 'a' );
	EXPECT_TRUE( deriv.count( 1 ) );
	EXPECT_TRUE( deriv.count( 2 ) );
	EXPECT_EQ( deriv.size(), (std::size_t)2 );
}
TEST( AutomatonMakeTransition, Parallel )
{
	fa::Automaton fa; // a(ba)*a(a + Eps)
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	EXPECT_TRUE( fa.addState( 3 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 2 );
	fa.setStateFinal( 3 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'a', 3 ) );

	std::set< int > source;
	source.insert( 0 );
	source.insert( 1 );
	source.insert( 2 );
	std::set< int > deriv = fa.makeTransition( source, 'a' );
	EXPECT_TRUE( deriv.count( 1 ) );
	EXPECT_TRUE( deriv.count( 2 ) );
	EXPECT_TRUE( deriv.count( 3 ) );
	EXPECT_EQ( deriv.size(), (std::size_t)3 );
}
TEST( AutomatonMakeTransition, NotFound )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );

	std::set< int > source;
	source.insert( 0 );
	source.insert( 1 );
	source.insert( 2 );
	std::set< int > deriv = fa.makeTransition( source, 'a' );
	EXPECT_EQ( deriv.size(), (std::size_t)0 );
}

TEST( AutomatonReadString, Deterministic )
{
	fa::Automaton fa; // (ab)*
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );

	std::set< int > accepted = fa.readString( "abababab" );
	EXPECT_TRUE( accepted.count( 0 ) );
	EXPECT_EQ( accepted.size(), (std::size_t)1 );

	std::set< int > na = fa.readString( "baba" );
	EXPECT_TRUE( na.empty() );
}
TEST( AutomatonReadString, NonDeterministicSingleInitial )
{
	fa::Automaton fa; // acc + ac + b
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	EXPECT_TRUE( fa.addState( 3 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 3 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addSymbol( 'c' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 3 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'c', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'c', 3 ) );

	std::set< int > s = fa.readString( "ac" );
	EXPECT_TRUE( s.count( 2 ) );
	EXPECT_TRUE( s.count( 3 ) );
	EXPECT_EQ( s.size(), (std::size_t)2 );
}
TEST( AutomatonReadString, NonDeterministicMultipleInitials )
{
	fa::Automaton fa; // aa + aa
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	EXPECT_TRUE( fa.addState( 3 ) );
	EXPECT_TRUE( fa.addState( 4 ) );
	EXPECT_TRUE( fa.addState( 5 ) );
	fa.setStateInitial( 0 );
	fa.setStateInitial( 1 );
	fa.setStateFinal( 4 );
	fa.setStateFinal( 5 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'a', 4 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 3 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 5 ) );

	std::set< int > s = fa.readString( "aa" );
	EXPECT_TRUE( s.count( 4 ) );
	EXPECT_TRUE( s.count( 5 ) );
	EXPECT_EQ( s.size(), (std::size_t)2 );
}
TEST( AutomatonReadString, Epsilon )
{
	fa::Automaton fa; // (ab)*
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );

	std::set< int > s = fa.readString( "" );
	EXPECT_TRUE( s.count( 0 ) );
	EXPECT_EQ( s.size(), (std::size_t)1 );
}
TEST( AutomatonReadString, EmptyLanguage )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );

	std::set< int > s = fa.readString( "not found" );
	EXPECT_TRUE( s.empty() );
}
TEST( AutomatonReadString, NoInitial )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );

	std::set< int > s = fa.readString( "a" );
	EXPECT_TRUE( s.empty() );
}

TEST( AutomatonMatch, Single )
{
	fa::Automaton fa; // (ab)*
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );

	EXPECT_TRUE( fa.match( "" ) );
	EXPECT_TRUE( fa.match( "ab" ) );
	EXPECT_TRUE( fa.match( "ababababab" ) );
	EXPECT_FALSE( fa.match( "a" ) );
	EXPECT_FALSE( fa.match( "ba" ) );
	EXPECT_FALSE( fa.match( "bababababab" ) );
	EXPECT_FALSE( fa.match( "not defined letters" ) );
}
TEST( AutomatonMatch, Multiple )
{
	fa::Automaton fa; // acc + ac + b
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	EXPECT_TRUE( fa.addState( 3 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 3 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addSymbol( 'c' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 3 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'c', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'c', 3 ) );

	EXPECT_TRUE( fa.match( "b" ) );
	EXPECT_TRUE( fa.match( "ac" ) );
	EXPECT_TRUE( fa.match( "acc" ) );
	EXPECT_FALSE( fa.match( "" ) );
	EXPECT_FALSE( fa.match( "a" ) );
	EXPECT_FALSE( fa.match( "acb" ) );
	EXPECT_FALSE( fa.match( "accac" ) );
}
TEST( AutomatonMatch, EmptyLanguage )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );

	EXPECT_FALSE( fa.match( "" ) );
	EXPECT_FALSE( fa.match( "a" ) );
	EXPECT_FALSE( fa.match( "aaaaaa" ) );
}
TEST( AutomatonMatch, UnityLanguage )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );

	EXPECT_TRUE( fa.match( "" ) );
	EXPECT_FALSE( fa.match( "a" ) );
	EXPECT_FALSE( fa.match( "aaaaaa" ) );
}
TEST( AutomatonMatch, NoInitial )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );

	EXPECT_FALSE( fa.match( "" ) );
	EXPECT_FALSE( fa.match( "ab" ) );
	EXPECT_FALSE( fa.match( "ababababab" ) );
	EXPECT_FALSE( fa.match( "a" ) );
	EXPECT_FALSE( fa.match( "ba" ) );
	EXPECT_FALSE( fa.match( "bababababab" ) );
	EXPECT_FALSE( fa.match( "not defined letters" ) );
}
TEST( AutomatonMatch, NoFinal )
{	
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );

	EXPECT_FALSE( fa.match( "" ) );
	EXPECT_FALSE( fa.match( "ab" ) );
	EXPECT_FALSE( fa.match( "ababababab" ) );
	EXPECT_FALSE( fa.match( "a" ) );
	EXPECT_FALSE( fa.match( "ba" ) );
	EXPECT_FALSE( fa.match( "bababababab" ) );
	EXPECT_FALSE( fa.match( "not defined letters" ) );
}

TEST( AutomatonIntegratedCreateComplete, abStar )
{
	fa::Automaton fa; // (ab)*
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );
	
	EXPECT_FALSE( fa.isComplete() );
	EXPECT_TRUE( fa.match( "" ) );
	EXPECT_TRUE( fa.match( "abab" ) );
	EXPECT_FALSE( fa.match( "babab" ) );

	fa::Automaton c = fa::Automaton::createComplete( fa );
	EXPECT_TRUE( c.isComplete() );
	EXPECT_EQ( c.countStates(), (std::size_t)3 );
	EXPECT_EQ( c.countTransitions(), (std::size_t)6 );

	EXPECT_TRUE( c.match( "" ) );
	EXPECT_TRUE( c.match( "abab" ) );
	EXPECT_FALSE( c.match( "babab" ) );
}

TEST( AutomatonIntegratedCreateComplement, EndsWithB )
{
	fa::Automaton fa; // (a + b)*b
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 1 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 1 ) );

	EXPECT_TRUE( fa.match( "b" ) );
	EXPECT_TRUE( fa.match( "abab" ) );
	EXPECT_TRUE( fa.match( "babab" ) );
	EXPECT_FALSE( fa.match( "aba" ) );
	EXPECT_FALSE( fa.match( "a" ) );
	EXPECT_FALSE( fa.match( "" ) );

	fa::Automaton c = fa::Automaton::createComplement( fa ); // Eps + (a + b)*a

	EXPECT_FALSE( c.match( "b" ) );
	EXPECT_FALSE( c.match( "abab" ) );
	EXPECT_FALSE( c.match( "babab" ) );
	EXPECT_TRUE( c.match( "aba" ) );
	EXPECT_TRUE( c.match( "a" ) );
	EXPECT_TRUE( c.match( "" ) );
}

TEST( AutomatonIntegratedCreateMirror, AnBm )
{
	fa::Automaton fa; // a^n b^m with n > 0 and m > 0
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 2 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 2 ) );

	EXPECT_TRUE( fa.match( "ab" ) );
	EXPECT_TRUE( fa.match( "aaaabbbb" ) );
	EXPECT_TRUE( fa.match( "aaabbbb" ) );
	EXPECT_FALSE( fa.match( "" ) );
	EXPECT_FALSE( fa.match( "a" ) );
	EXPECT_FALSE( fa.match( "b" ) );
	EXPECT_FALSE( fa.match( "aba" ) );

	fa::Automaton m = fa::Automaton::createMirror( fa );
	EXPECT_FALSE( m.match( "ab" ) );
	EXPECT_FALSE( m.match( "aaaabbbb" ) );
	EXPECT_FALSE( m.match( "" ) );
	EXPECT_TRUE( m.match( "ba" ) );
	EXPECT_TRUE( m.match( "bbbaaaa" ) );
}

TEST( AutomatonIsEmpty, EmptyMin )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	fa.setStateInitial( 0 );

	EXPECT_TRUE( fa.isLanguageEmpty() );
	EXPECT_FALSE( fa.match( "" ) );
	EXPECT_FALSE( fa.match( "a" ) );
}
TEST( AutomatonIsEmpty, Empty )
{
	fa::Automaton fa; // Would be (ab)* or a(ba)* if there was a final state
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 0 ) );

	EXPECT_TRUE( fa.isLanguageEmpty() );
	EXPECT_FALSE( fa.match( "" ) );
	EXPECT_FALSE( fa.match( "ab" ) );
	EXPECT_FALSE( fa.match( "aba" ) );
}
TEST( AutomatonIsEmpty, Unity )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );

	EXPECT_FALSE( fa.isLanguageEmpty() );
	EXPECT_TRUE( fa.match( "" ) );
	EXPECT_FALSE( fa.match( "a" ) );
}
TEST( AutomatonIsEmpty, Deterministic )
{
	fa::Automaton fa; // a^n b^m with n > 0 and m >= 0
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 1 );
	fa.setStateFinal( 2 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 2 ) );

	EXPECT_FALSE( fa.isLanguageEmpty() );
}
TEST( AutomatonIsEmpty, NonDeterministic )
{
	fa::Automaton fa; // (a + b)*b
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 1 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 1 ) );

	EXPECT_FALSE( fa.isLanguageEmpty() );
}

TEST( AutomatonRemoveNonAccessible, NoInitial )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );

	fa.removeNonAccessibleStates();

	EXPECT_TRUE( fa.isValid() );

	EXPECT_EQ( fa.countStates(), (std::size_t)1 );

	EXPECT_EQ( fa.countTransitions(), (std::size_t)0 );

	EXPECT_TRUE( fa.hasSymbol( 'a' ) );
	EXPECT_EQ( fa.countSymbols(), (std::size_t)1 );
}
TEST( AutomatonRemoveNonAccessible, FullAccessible )
{
	fa::Automaton fa; // (a + b)*b
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 1 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 1 ) );

	fa.removeNonAccessibleStates();

	EXPECT_EQ( fa.countStates(), (std::size_t)2 );
	EXPECT_TRUE( fa.hasState( 0 ) );
	EXPECT_TRUE( fa.hasState( 1 ) );
	EXPECT_EQ( fa.countTransitions(), (std::size_t)3 );
	EXPECT_TRUE( fa.hasTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.hasTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( fa.hasTransition( 0, 'b', 1 ) );
}
TEST( AutomatonRemoveNonAccessible, Simple )
{
	fa::Automaton fa; // (a + b)*b
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	EXPECT_TRUE( fa.addState( 3 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 1 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 1 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 2 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 3 ) );

	fa.removeNonAccessibleStates();

	EXPECT_TRUE( fa.hasState( 2 ) );
	EXPECT_FALSE( fa.hasState( 3 ) );
	EXPECT_EQ( fa.countStates(), (std::size_t)3 );

	EXPECT_TRUE( fa.hasTransition( 1, 'b', 2 ) );
	EXPECT_FALSE( fa.hasTransition( 3, 'a', 3 ) );
	EXPECT_EQ( fa.countTransitions(), (std::size_t)4 );
}

TEST( AutomatonRemoveNonCoaccessible, Empty )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );

	fa.removeNonCoAccessibleStates();

	EXPECT_TRUE( fa.isValid() );
	EXPECT_EQ( fa.countStates(), (std::size_t)1 );
	EXPECT_FALSE( fa.hasTransition( 0, 'a', 0 ) );
	EXPECT_EQ( fa.countTransitions(), (std::size_t)0 );
}
TEST( AutomatonRemoveNonCoaccessible, FullCoaccessible )
{
	fa::Automaton fa; // (a + b)*b
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 1 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 1 ) );

	fa.removeNonCoAccessibleStates();

	EXPECT_TRUE( fa.hasState( 0 ) );
	EXPECT_TRUE( fa.hasState( 1 ) );
	EXPECT_EQ( fa.countStates(), (std::size_t)2 );

	EXPECT_TRUE( fa.hasTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.hasTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( fa.hasTransition( 0, 'b', 1 ) );
	EXPECT_EQ( fa.countTransitions(), (std::size_t)3 );

	EXPECT_FALSE( fa.match( "" ) );
	EXPECT_TRUE( fa.match( "b" ) );
	EXPECT_TRUE( fa.match( "abbabab" ) );
}

TEST( AutomatonCreateIntersection, UnityToEmpty )
{
	fa::Automaton u;
	EXPECT_TRUE( u.addState( 0 ) );
	EXPECT_TRUE( u.addSymbol( 'c' ) );
	u.setStateInitial( 0 );
	u.setStateFinal( 0 );

	fa::Automaton fa; // (a + b)*b
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 1 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 1 ) );

	fa::Automaton inter = fa::Automaton::createIntersection( fa, u );

	EXPECT_TRUE( inter.isLanguageEmpty() );
}
TEST( AutomatonCreateIntersection, UnityToUnity )
{
	fa::Automaton u; // Unity language
	EXPECT_TRUE( u.addState( 0 ) );
	EXPECT_TRUE( u.addSymbol( 'c' ) );
	u.setStateInitial( 0 );
	u.setStateFinal( 0 );

	fa::Automaton fa; // Sigma*
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );

	fa::Automaton inter = fa::Automaton::createIntersection( fa, u );

	EXPECT_FALSE( inter.isLanguageEmpty() );
	EXPECT_TRUE( inter.match( "" ) );
	EXPECT_FALSE( inter.match( "a" ) );
	EXPECT_FALSE( inter.match( "b" ) );
}
TEST( AutomatonCreateIntersection, StartWithAEndsWithB )
{
	fa::Automaton b; // (a + b)*b
	EXPECT_TRUE( b.addState( 0 ) );
	EXPECT_TRUE( b.addState( 1 ) );
	b.setStateInitial( 0 );
	b.setStateFinal( 1 );
	EXPECT_TRUE( b.addSymbol( 'a' ) );
	EXPECT_TRUE( b.addSymbol( 'b' ) );
	EXPECT_TRUE( b.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( b.addTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( b.addTransition( 0, 'b', 1 ) );

	fa::Automaton a; // a(a + b + c)*
	EXPECT_TRUE( a.addState( 0 ) );
	EXPECT_TRUE( a.addState( 1 ) );
	a.setStateInitial( 0 );
	a.setStateFinal( 1 );
	EXPECT_TRUE( a.addSymbol( 'a' ) );
	EXPECT_TRUE( a.addSymbol( 'b' ) );
	EXPECT_TRUE( a.addSymbol( 'c' ) );
	EXPECT_TRUE( a.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'b', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'c', 1 ) );

	fa::Automaton ab = fa::Automaton::createIntersection( a, b );

	EXPECT_FALSE( ab.isLanguageEmpty() );
	EXPECT_TRUE( ab.match( "ab" ) );
	EXPECT_TRUE( ab.match( "aabbabab" ) );
	EXPECT_FALSE( ab.match( "" ) );
	EXPECT_FALSE( ab.match( "a" ) );
	EXPECT_FALSE( ab.match( "b" ) );
	EXPECT_FALSE( ab.match( "baaa" ) );
	EXPECT_FALSE( ab.match( "bab" ) );
	EXPECT_FALSE( ab.match( "aaaaa" ) );
}

TEST( AutomatonHasEmptyIntersection, UnityToEmpty )
{
	fa::Automaton u;
	EXPECT_TRUE( u.addState( 0 ) );
	EXPECT_TRUE( u.addSymbol( 'c' ) );
	u.setStateInitial( 0 );
	u.setStateFinal( 0 );

	fa::Automaton fa; // (a + b)*b
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 1 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 1 ) );

	EXPECT_TRUE( fa.hasEmptyIntersectionWith( u ) );
	EXPECT_TRUE( u.hasEmptyIntersectionWith( fa ) );
}
TEST( AutomatonHasEmptyIntersection, UnityToUnity )
{
	fa::Automaton u; // Unity language
	EXPECT_TRUE( u.addState( 0 ) );
	EXPECT_TRUE( u.addSymbol( 'c' ) );
	u.setStateInitial( 0 );
	u.setStateFinal( 0 );

	fa::Automaton fa; // Sigma*
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );

	EXPECT_FALSE( fa.hasEmptyIntersectionWith( u ) );
	EXPECT_FALSE( u.hasEmptyIntersectionWith( fa ) );
}
TEST( AutomatonHasEmptyIntersection, StartWithAEndsWithB )
{
	fa::Automaton b; // (a + b)*b
	EXPECT_TRUE( b.addState( 0 ) );
	EXPECT_TRUE( b.addState( 1 ) );
	b.setStateInitial( 0 );
	b.setStateFinal( 1 );
	EXPECT_TRUE( b.addSymbol( 'a' ) );
	EXPECT_TRUE( b.addSymbol( 'b' ) );
	EXPECT_TRUE( b.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( b.addTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( b.addTransition( 0, 'b', 1 ) );

	fa::Automaton a; // a(a + b + c)*
	EXPECT_TRUE( a.addState( 0 ) );
	EXPECT_TRUE( a.addState( 1 ) );
	a.setStateInitial( 0 );
	a.setStateFinal( 1 );
	EXPECT_TRUE( a.addSymbol( 'a' ) );
	EXPECT_TRUE( a.addSymbol( 'b' ) );
	EXPECT_TRUE( a.addSymbol( 'c' ) );
	EXPECT_TRUE( a.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'b', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'c', 1 ) );

	EXPECT_FALSE( a.hasEmptyIntersectionWith( b ) );
	EXPECT_FALSE( b.hasEmptyIntersectionWith( a ) );
}

TEST( AutomatonCreateDeterministic, Empty )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );

	fa::Automaton d = fa::Automaton::createDeterministic( fa );

	EXPECT_TRUE( d.isValid() );
}
TEST( AutomatonCreateDeterministic, NonDeterministic )
{
	fa::Automaton fa; // (a + b)*b
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 1 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 1 ) );

	fa::Automaton d = fa::Automaton::createDeterministic( fa );

	EXPECT_EQ( d.countSymbols(), (std::size_t)2 );
	EXPECT_TRUE( d.hasSymbol( 'a' ) );
	EXPECT_TRUE( d.hasSymbol( 'b' ) );

	EXPECT_TRUE( d.match( "b" ) );
	EXPECT_TRUE( d.match( "abbabab" ) );
	EXPECT_FALSE( d.match( "" ) );
	EXPECT_FALSE( d.match( "a" ) );
	EXPECT_FALSE( d.match( "baaa" ) );
}
TEST( AutomatonCreateDeterministic, AlreadyDeterministic )
{
	fa::Automaton a; // a(a + b + c)*
	EXPECT_TRUE( a.addState( 0 ) );
	EXPECT_TRUE( a.addState( 1 ) );
	a.setStateInitial( 0 );
	a.setStateFinal( 1 );
	EXPECT_TRUE( a.addSymbol( 'a' ) );
	EXPECT_TRUE( a.addSymbol( 'b' ) );
	EXPECT_TRUE( a.addSymbol( 'c' ) );
	EXPECT_TRUE( a.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'b', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'c', 1 ) );

	fa::Automaton d = fa::Automaton::createDeterministic( a );

	EXPECT_EQ( d.countSymbols(), (std::size_t)3 );
	EXPECT_TRUE( d.hasSymbol( 'a' ) );
	EXPECT_TRUE( d.hasSymbol( 'b' ) );
	EXPECT_TRUE( d.hasSymbol( 'c' ) );

	EXPECT_TRUE( d.match( "a" ) );
	EXPECT_TRUE( d.match( "abababab" ) );
	EXPECT_TRUE( d.match( "ababababcccc" ) );
	EXPECT_TRUE( d.match( "acccc" ) );
	EXPECT_FALSE( d.match( "babababcccc" ) );
	EXPECT_FALSE( d.match( "ccccbbabab" ) );
	EXPECT_FALSE( d.match( "" ) );
	EXPECT_FALSE( d.match( "b" ) );
	EXPECT_FALSE( d.match( "c" ) );
}

TEST( AutomatonIsIncludedIn, Empty )
{
	fa::Automaton empty;
	EXPECT_TRUE( empty.addState( 0 ) );
	EXPECT_TRUE( empty.addState( 1 ) );
	empty.setStateInitial( 0 );
	EXPECT_TRUE( empty.addSymbol( 'a' ) );
	EXPECT_TRUE( empty.addTransition( 0, 'a', 0 ) );

	fa::Automaton a; // a(a + b + c)*
	EXPECT_TRUE( a.addState( 0 ) );
	EXPECT_TRUE( a.addState( 1 ) );
	a.setStateInitial( 0 );
	a.setStateFinal( 1 );
	EXPECT_TRUE( a.addSymbol( 'a' ) );
	EXPECT_TRUE( a.addSymbol( 'b' ) );
	EXPECT_TRUE( a.addSymbol( 'c' ) );
	EXPECT_TRUE( a.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'b', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'c', 1 ) );

	fa::Automaton fa; // Sigma*
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( fa.addTransition( 0, 'b', 0 ) );

	fa::Automaton u; // Unity language
	EXPECT_TRUE( u.addState( 0 ) );
	EXPECT_TRUE( u.addSymbol( 'c' ) );
	u.setStateInitial( 0 );
	u.setStateFinal( 0 );

	EXPECT_FALSE( empty.isIncludedIn( u ) );
	EXPECT_FALSE( empty.isIncludedIn( a ) );
	EXPECT_TRUE( empty.isIncludedIn( fa ) );
	EXPECT_TRUE( empty.isIncludedIn( empty ) );
}
TEST( AutomatonIsIncludedIn, UnityEqual )
{
	fa::Automaton u; // Unity language
	EXPECT_TRUE( u.addState( 0 ) );
	EXPECT_TRUE( u.addSymbol( 'c' ) );
	u.setStateInitial( 0 );
	u.setStateFinal( 0 );

	fa::Automaton fa; // Unity language but bigger
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	for( uint i = 0; i < 2; i++ )
	{
		EXPECT_TRUE( fa.addState( i ) );
		fa.setStateFinal( i );
	}
	fa.setStateInitial( 1 );

	EXPECT_TRUE( fa.isIncludedIn( u ) );
	EXPECT_TRUE( u.isIncludedIn( fa ) );
}
TEST( AutomatonIsIncludedIn, SigmaEqual )
{
	fa::Automaton s; // Sigma*
	EXPECT_TRUE( s.addState( 0 ) );
	EXPECT_TRUE( s.addSymbol( 'a' ) );
	EXPECT_TRUE( s.addSymbol( 'b' ) );
	s.setStateInitial( 0 );
	s.setStateFinal( 0 );
	EXPECT_TRUE( s.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( s.addTransition( 0, 'b', 0 ) );

	fa::Automaton fa; // Sigma* but bigger
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	fa.setStateInitial( 0 );
	fa.setStateFinal( 0 );
	for( uint i = 1; i < 3; i++ )
	{
		EXPECT_TRUE( fa.addState( i ) );
		EXPECT_TRUE( fa.addTransition( i - 1, 'a', i ) );
		EXPECT_TRUE( fa.addTransition( i - 1, 'b', i ) );
	}
	fa.setStateFinal( 1 );
	EXPECT_TRUE( fa.addTransition( 2, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 2 ) );

	EXPECT_TRUE( fa.isIncludedIn( s ) );
	EXPECT_TRUE( s.isIncludedIn( fa ) );
}
TEST( AutomatonIsIncludedIn, StartWithAEndsWithB )
{
	fa::Automaton b; // (a + b)*b
	EXPECT_TRUE( b.addState( 0 ) );
	EXPECT_TRUE( b.addState( 1 ) );
	b.setStateInitial( 0 );
	b.setStateFinal( 1 );
	EXPECT_TRUE( b.addSymbol( 'a' ) );
	EXPECT_TRUE( b.addSymbol( 'b' ) );
	EXPECT_TRUE( b.addTransition( 0, 'a', 0 ) );
	EXPECT_TRUE( b.addTransition( 0, 'b', 0 ) );
	EXPECT_TRUE( b.addTransition( 0, 'b', 1 ) );

	fa::Automaton a; // a(a + b + c)*
	EXPECT_TRUE( a.addState( 0 ) );
	EXPECT_TRUE( a.addState( 1 ) );
	a.setStateInitial( 0 );
	a.setStateFinal( 1 );
	EXPECT_TRUE( a.addSymbol( 'a' ) );
	EXPECT_TRUE( a.addSymbol( 'b' ) );
	EXPECT_TRUE( a.addSymbol( 'c' ) );
	EXPECT_TRUE( a.addTransition( 0, 'a', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'a', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'b', 1 ) );
	EXPECT_TRUE( a.addTransition( 1, 'c', 1 ) );

	fa::Automaton i = fa::Automaton::createIntersection( a, b );

	EXPECT_FALSE( a.isIncludedIn( b ) );
	EXPECT_FALSE( b.isIncludedIn( a ) );
	EXPECT_FALSE( a.isIncludedIn( i ) );
	EXPECT_FALSE( b.isIncludedIn( i ) );
	EXPECT_TRUE( i.isIncludedIn( b ) );
	EXPECT_TRUE( i.isIncludedIn( a ) );
}

TEST( AutomatonCreateMinimalMoore, Unity )
{
	fa::Automaton fa; // Unity language but bigger
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	for( uint i = 1; i < 3; i++ )
	{
		EXPECT_TRUE( fa.addState( i ) );
		EXPECT_TRUE( fa.addTransition( i, 'a', i - 1 ) );
		EXPECT_TRUE( fa.addTransition( i, 'b', i - 1 ) );
		fa.setStateFinal( i );
	}
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.addTransition( 2, 'a', 0 ) ); // => non-deterministic

	fa::Automaton m = fa::Automaton::createMinimalMoore( fa );
	
	EXPECT_TRUE( m.hasSymbol( 'a' ) && m.hasSymbol( 'b' ) && m.countSymbols() == (std::size_t)2 );
	EXPECT_EQ( m.countStates(), (std::size_t)1 );
	EXPECT_TRUE( m.isDeterministic() );
	EXPECT_TRUE( m.isComplete() );
}
TEST( AutomatonCreateMinimalMoore, TL41 )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	EXPECT_TRUE( fa.addState( 3 ) );
	EXPECT_TRUE( fa.addState( 4 ) );
	EXPECT_TRUE( fa.addState( 5 ) );
	EXPECT_TRUE( fa.addState( 6 ) );
	EXPECT_TRUE( fa.addState( 7 ) );
	fa.setStateFinal( 5 );
	fa.setStateFinal( 7 );
	fa.setStateInitial( 1 );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 4 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'a', 5 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 4 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 7 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'b', 4 ) );
	EXPECT_TRUE( fa.addTransition( 4, 'a', 6 ) );
	EXPECT_TRUE( fa.addTransition( 4, 'b', 5 ) );
	EXPECT_TRUE( fa.addTransition( 5, 'b', 6 ) );
	EXPECT_TRUE( fa.addTransition( 6, 'a', 3 ) );
	EXPECT_TRUE( fa.addTransition( 6, 'b', 4 ) );
	EXPECT_TRUE( fa.addTransition( 7, 'b', 3 ) );

	fa::Automaton m = fa::Automaton::createMinimalMoore( fa );
	EXPECT_TRUE( m.hasSymbol( 'a' ) && m.hasSymbol( 'b' ) && m.countSymbols() == (std::size_t)2 );
	EXPECT_EQ( m.countStates(), (std::size_t)5 );
	EXPECT_TRUE( m.isDeterministic() );
	EXPECT_TRUE( m.isComplete() );
}
TEST( AutomatonCreateMinimalBrzozowski, Unity )
{
	fa::Automaton fa; // Unity language but bigger
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addState( 0 ) );
	for( uint i = 1; i < 3; i++ )
	{
		EXPECT_TRUE( fa.addState( i ) );
		EXPECT_TRUE( fa.addTransition( i, 'a', i - 1 ) );
		EXPECT_TRUE( fa.addTransition( i, 'b', i - 1 ) );
		fa.setStateFinal( i );
	}
	fa.setStateInitial( 0 );
	EXPECT_TRUE( fa.addTransition( 2, 'a', 0 ) ); // => non-deterministic

	fa::Automaton m = fa::Automaton::createMinimalBrzozowski( fa );
	
	EXPECT_TRUE( m.hasSymbol( 'a' ) && m.hasSymbol( 'b' ) && m.countSymbols() == (std::size_t)2 );
	EXPECT_EQ( m.countStates(), (std::size_t)1 );
	EXPECT_TRUE( m.isDeterministic() );
	EXPECT_TRUE( m.isComplete() );
}
TEST( AutomatonCreateMinimalBrzozowski, TL41 )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 2 ) );
	EXPECT_TRUE( fa.addState( 3 ) );
	EXPECT_TRUE( fa.addState( 4 ) );
	EXPECT_TRUE( fa.addState( 5 ) );
	EXPECT_TRUE( fa.addState( 6 ) );
	EXPECT_TRUE( fa.addState( 7 ) );
	fa.setStateFinal( 5 );
	fa.setStateFinal( 7 );
	fa.setStateInitial( 1 );
	EXPECT_TRUE( fa.addTransition( 1, 'a', 2 ) );
	EXPECT_TRUE( fa.addTransition( 1, 'b', 4 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'a', 5 ) );
	EXPECT_TRUE( fa.addTransition( 2, 'b', 4 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'a', 7 ) );
	EXPECT_TRUE( fa.addTransition( 3, 'b', 4 ) );
	EXPECT_TRUE( fa.addTransition( 4, 'a', 6 ) );
	EXPECT_TRUE( fa.addTransition( 4, 'b', 5 ) );
	EXPECT_TRUE( fa.addTransition( 5, 'b', 6 ) );
	EXPECT_TRUE( fa.addTransition( 6, 'a', 3 ) );
	EXPECT_TRUE( fa.addTransition( 6, 'b', 4 ) );
	EXPECT_TRUE( fa.addTransition( 7, 'b', 3 ) );

	fa::Automaton m = fa::Automaton::createMinimalBrzozowski( fa );
	EXPECT_TRUE( m.hasSymbol( 'a' ) && m.hasSymbol( 'b' ) && m.countSymbols() == (std::size_t)2 );
	EXPECT_EQ( m.countStates(), (std::size_t)5 );
	EXPECT_TRUE( m.isDeterministic() );
	EXPECT_TRUE( m.isComplete() );
}

int main( int argc, char ** argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
