#include "gtest/gtest.h"
#include "Automaton.h"

TEST( AutomatonAddSymbol, Default )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
}
TEST( AutomatonAddSymbol, ImplicitConversion )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 69 ) ); // (char)69 => 'E'
}
TEST( AutomatonAddSymbol, Epsilon )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( fa::Epsilon ) );
}
TEST( AutomatonAddSymbol, IndirectEpsilon )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( '\0' ) );
}
TEST( AutomatonAddSymbol, ConversionIndirectEpsilon )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 0 ) );
}
TEST( AutomatonAddSymbol, Multiple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.addSymbol( fa::Epsilon ) );
}
TEST( AutomatonAddSymbol, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_FALSE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( fa::Epsilon ) );
}
TEST( AutomatonAddSymbol, Special )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( 'π' ) );
	EXPECT_FALSE( fa.addSymbol( 'シ' ) );
	EXPECT_FALSE( fa.addSymbol( 'é' ) );
	EXPECT_TRUE( fa.addSymbol( '(' ) );
}
TEST( AutomatonAddSymbol, NoGraph )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( '\n' ) );
	EXPECT_FALSE( fa.addSymbol( '\t' ) );
	EXPECT_FALSE( fa.addSymbol( ' ' ) );
	EXPECT_FALSE( fa.addSymbol( '\b' ) );
}

TEST( AutomatonCountSymbol, Default )
{
	fa::Automaton fa;
	EXPECT_EQ( fa.countSymbols(), 0 );
}
TEST( AutomatonCountSymbol, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'B' ) );
	EXPECT_EQ( fa.countSymbols(), 2 );
}
TEST( AutomatonCountSymbol, Epsilon )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( fa::Epsilon ) );
	EXPECT_EQ( fa.countSymbols(), 1 );
}
TEST( AutomatonCountSymbol, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_FALSE( fa.addSymbol( 'a' ) );
	EXPECT_EQ( fa.countSymbols(), 1 );
}
TEST( AutomatonCountSymbol, NoGraph )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( '\n' ) );
	EXPECT_FALSE( fa.addSymbol( '\t' ) );
	EXPECT_FALSE( fa.addSymbol( ' ' ) );
	EXPECT_FALSE( fa.addSymbol( '\b' ) );
	EXPECT_TRUE( fa.addSymbol( '\0' ) ); // Epsilon
	EXPECT_EQ( fa.countSymbols(), 1 );
}
TEST( AutomatonCountSymbol, Special )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.addSymbol( 'π' ) );
	EXPECT_EQ( fa.countSymbols(), 0 );
}

TEST( AutomatonRemoveSymbol, Default )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_TRUE( fa.removeSymbol( 'a' ) );
	EXPECT_EQ( fa.countSymbols(), 1 );
}
TEST( AutomatonRemoveSymbol, NoSymbol )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.removeSymbol( 'a' ) );
	EXPECT_EQ( fa.countSymbols(), 0 );
}
TEST( AutomatonRemoveSymbol, SymbolNotFound )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 'b' ) );
	EXPECT_FALSE( fa.removeSymbol( 'x' ) );
	EXPECT_EQ( fa.countSymbols(), 2 );
}
TEST( AutomatonRemoveSymbol, SpecialSymbol )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 's' ) );
	EXPECT_FALSE( fa.removeSymbol( 'シ' ) );
	EXPECT_EQ( fa.countSymbols(), 1 );
}

TEST( AutomatonHasSymbol, Default )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( 'a' ) );
	EXPECT_TRUE( fa.addSymbol( 's' ) );
	EXPECT_TRUE( fa.hasSymbol( 's' ) );
}
TEST( AutomatonHasSymbol, NoSymbol )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.hasSymbol( 'a' ) );
}
TEST( AutomatonHasSymbol, SymbolNotFound )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addSymbol( fa::Epsilon ) );
	EXPECT_FALSE( fa.hasSymbol( 'a' ) );
}
TEST( AutomatonHasSymbol, Special )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.hasSymbol( 'シ' ) );
}

TEST( AutomatonAddState, Default )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
}
TEST( AutomatonAddState, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_FALSE( fa.addState( 0 ) );
}
TEST( AutomatonAddState, Multiple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 10 ) );
}
TEST( AutomatonAddState, Negative )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( INT32_MIN ) );
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
	EXPECT_TRUE( fa.hasState( 0 ) );
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
	EXPECT_TRUE( fa.addState( 10 ) );
	EXPECT_TRUE( fa.hasState( 0 ) );
	EXPECT_TRUE( fa.hasState( 0 ) );
	EXPECT_TRUE( fa.hasState( 10 ) );
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

TEST( AutomatonCountState, Default )
{
	fa::Automaton fa;
	EXPECT_EQ( fa.countStates(), 0 );
}
TEST( AutomatonCountState, Simple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 10 ) );
	EXPECT_TRUE( fa.addState( 100 ) );
	EXPECT_EQ( fa.countStates(), 3 );
}
TEST( AutomatonCountState, RemoveDefault )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.removeState( 42 ) );
	EXPECT_EQ( fa.countStates(), 0 );
}
TEST( AutomatonCountState, Double )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_FALSE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 100 ) );
	EXPECT_EQ( fa.countStates(), 2 );
}
TEST( AutomatonCountState, RemoveSimple )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 0 ) );
	EXPECT_TRUE( fa.addState( 10 ) );
	EXPECT_TRUE( fa.addState( 100 ) );
	EXPECT_TRUE( fa.removeState( 0 ) );
	EXPECT_EQ( fa.countStates(), 2 );
}
TEST( AutomatonCountState, RemoveNone )
{
	fa::Automaton fa;
	EXPECT_TRUE( fa.addState( 1 ) );
	EXPECT_TRUE( fa.addState( 10 ) );
	EXPECT_TRUE( fa.addState( 100 ) );
	EXPECT_FALSE( fa.removeState( 0 ) );
	EXPECT_EQ( fa.countStates(), 3 );
}


TEST( AutomatonIsValid, Default )
{
	fa::Automaton fa;
	EXPECT_FALSE( fa.isValid() );
}
TEST( AutomatonIsValid, WithLetterEmptyLanguage )
{
	fa::Automaton fa;
	fa.addState( 0 );
	fa.addSymbol( 'a' );
	EXPECT_TRUE( fa.isValid() );
}
TEST( AutomatonIsValid, NoLetterEmptyLanguage )
{
	fa::Automaton fa;
	fa.addState( 0 );
	EXPECT_FALSE( fa.isValid() );
}

int main( int argc, char ** argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
