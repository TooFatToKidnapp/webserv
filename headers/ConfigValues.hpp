/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValues.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 12:02:07 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/31 15:15:36 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// store all the vals in server block

#ifndef CONFIGVALUES
#define CONFIGVALUES

#include "./index.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>


class ConfigValues {
	protected:
		bool _Root;
		bool _Index;
		bool _ClientMaxBodySize;
		bool _ErrorPage;
		bool _AutoIndex;
		bool _ReturnValue;

		std::string _RootPath;
		std::vector<std::string> _IndexList;
		size_t _ClientMaxSize;
		std::map<int, std::string> _ErrorPages;
		bool _AutoIndexStatus;

		// add return direcory config !!



	public:
	ConfigValues();
	~ConfigValues();
	ConfigValues(const ConfigValues &obj);
	ConfigValues &operator=(const ConfigValues& obj);


};




#endif
