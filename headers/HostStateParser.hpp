/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HostStateParser.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 22:18:20 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/09 22:57:58 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOSTSTATEPARSER_HPP
#define HOSTSTATEPARSER_HPP

#include "./UriParser.hpp"
#include "./utils.hpp"


template<class T>
class HostStatePrser {
	public:
		HostStatePrser(T StartState, T EndState) :
		_StartState(StartState),
		_EndState(EndState),
		_Cur

		~HostStatePrser();
		HostStatePrser &operator=(const HostStatePrser &obj);
		HostStatePrser(const HostStatePrser &obj);



	protected:
		T _StartState;
		T _EndState;
		T _CurrentState;
		std::string _Buf;
		std::string _Input;
		size_t _Index;
		bool _SkipChar;
};



#endif
