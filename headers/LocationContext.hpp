/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationContext.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 09:46:40 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/12 13:55:57 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONTEXT_HPP
#define LOCATIONCONTEXT_HPP

#include "./ConfigValues.hpp"
#include "./LocationUri.hpp"
#include <iostream>

class LocationContext : virtual public ConfigValues {
	protected:
		bool _Uri;
		bool _Cgi;
		bool AllowedMethods;
		bool Alias;
		LocationUri _LocationUri;


		// overlodes
		void SetValue(int const directive, std::string input);
		int IsDirective(std::string const directive);
		bool IsSet(std::string val);


	public:
		LocationContext();
		LocationContext(std::string data);
		~LocationContext();
		LocationContext(const LocationContext &obj);
		LocationContext &operator=(const LocationContext &obj);

		LocationUri GetLocationUri() const;
};






#endif
