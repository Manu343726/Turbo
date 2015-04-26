
#ifndef LEFT_HPP
#define LEFT_HPP

#include "let_expressions.hpp"

namespace tml
{
	template<typename... ARGS>
	struct let
	{
		template<typename Var, typename Val>
		struct pair {};

		template<typename ARGS, typename PAIRS>
		struct parse_args;

		template<typename Var, typename Val, typename... TAIL,
		         typename... Pairs>
		struct parse_args<tml::list<Var, Val, TAIL...>, tml::list<Pairs...>>
		{
			using _pairs = tml::list<Pairs...,pair<Var,Val>>;
			
			using pairs = typename parse_args<tml::list<TAIL...>, _pairs>::pairs; 
			using body = typename parse_args<tml::list<TAIL...>, _pairs>::body
		};

		template<typename Pairs, typename Body>
		struct parse_args<tml::list<Body>, Pairs>
		{
			using pairs = Pairs;
			using body = Body;
		};


		template<typename Pairs, typename Body>
		struct process;

		template<typename Var, typename Val, typename... Tail, typename Body>
		struct process<tml::list<pair<Var,Val>, Tail...>, Body>
		{
			using body = typename impl::let_impl_high<Var,Val,Body>::result;
			using result = typename process<tml::list<Tail...>, body>::result;
		};

		using parser = parse_args<tml::list<ARGS...>,tml::empty_list>;

		using result = typename process<typename parser::pairs, typename parser::result>::result;
	};
}

#endif