/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValues.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 12:02:07 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/01 14:02:27 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// store all the vals in server block

#ifndef CONFIGVALUES
#define CONFIGVALUES

#include "./Index.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>

// cant creat this obj , can only inharit its atributes

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


		//setters
		void SetRoot(std::string val);
		void SetIndex(std::string val);
		void SetCmbs(std::string val);
		void SetErrorPage(std::string val);
		void SetAutoIndexDir(std::string val);
		// add set retuen func

		ConfigValues();
		virtual ~ConfigValues();
		ConfigValues(const ConfigValues &obj);
		ConfigValues &operator=(const ConfigValues& obj);

	public:

		virtual std::string GetRoot() const;
		virtual std::vector<std::string> GetIndex() const;
		virtual size_t GetCmbs() const;
		virtual std::map<int, std::string> GetErrorPage() const;
		virtual bool GetAutoIndexDir() const;
		// add get return func
		virtual bool HasErrorPage() const ;
		virtual bool IsSet(std::string directive);
};




#endif
