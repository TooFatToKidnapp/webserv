/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationContext.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 09:46:40 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/04 17:55:21 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONTEXT_HPP
#define LOCATIONCONTEXT_HPP

#include "./ConfigValues.hpp"
#include <iostream>

class LocationContext : virtual public ConfigValues {
	protected:
		bool _Uri;
		bool _Cgi;
		bool AllowedMethods;
		bool Alias;



		// overlodes
		void SetValue(int const directive, std::string input);
		int IsDirective(std::string const directive);
		bool IsSet(std::string val);


	public:
		LocationContext();
		~LocationContext();
		LocationContext(const LocationContext &obj);
		LocationContext &operator=(const LocationContext &obj);


};






#endif
