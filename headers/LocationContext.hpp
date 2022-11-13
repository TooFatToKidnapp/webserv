/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationContext.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 09:46:40 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/13 17:36:30 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONTEXT_HPP
#define LOCATIONCONTEXT_HPP

#include "./CheckMethods.hpp"
#include "./ConfigValues.hpp"
#include "./LocationUri.hpp"
#include "./ParseCGI.hpp"
#include <iostream>
#include <exception>


class LocationContext : virtual public ConfigValues {
	protected:
		bool _Uri;
		bool _Cgi;
		bool AllowedMethods;
		bool Alias;
		LocationUri _LocationUri;
		ParseCGI _CGI;
		CheckMethods _Methods;
		std::string _Alias;


		// overlodes
		void SetValue(int const directive, std::string input);
		int IsDirective(std::string const directive);
		bool IsSet(std::string val);


	public:
		LocationContext();
		LocationContext(std::string data);
		LocationContext &operator=(const LocationContext &obj);
		virtual ~LocationContext();
		LocationContext(const LocationContext &obj);


		LocationUri GetLocationUri() const;
		std::string GetAlias() const;
		CheckMethods GetMethods() const;

		virtual bool IsSet(std::string str);





};






#endif
