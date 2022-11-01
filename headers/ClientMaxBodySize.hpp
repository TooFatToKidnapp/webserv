/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientMaxBodySize.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:33:50 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/01 16:03:25 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTMAXBODYSIZE_HPP
#define CLIENTMAXBODYSIZE_HPP

#include <string>
#include <stdexcept>
#include <cctype> // std::isdigit
#include "./../src/utils.cpp"

// https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
// if client_max_body_size is smaller than the file size trying to be uploaded it gives a:
// 413 error (Request Entity too large)

class ClientMaxBodySize {
	private:
		size_t _val;
	public:
		ClientMaxBodySize() : _val(1) {}
		ClientMaxBodySize(std::string val) {
			size_t i = 0;
			size_t max = 0;
			size_t tmp;
			if (val.compare("") == 0)
				throw std::invalid_argument("Error: Missing Client Max Body Size");
			while (val[i]) {
				if (std::isdigit(val[i]) == 0 && val[i] != 'm' && val[i] != 'M')
					throw std::invalid_argument("Error: invalid Client Max Body Size");
				if (std::isdigit(val[i]) == 1 && max == 1)
					throw  std::invalid_argument("Error: invalid Client Max Body Size");
				else if (val[i] == 'm' || val[i] == 'M') {
					if (max == 1)
						throw std::invalid_argument("Error: invalid Client Max Body Size");
					max = 1;
				}
				i++;
			}
			tmp = StringToSize_T(val.substr(0, i));
			if (tmp > 2147483647)
				throw std::invalid_argument("Error: Max Body Size Too Large");
			_val = tmp;
		}
		~ClientMaxBodySize() {};
		size_t GetVal() const {
			return _val;
		}
};

#endif
