/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationContext.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 09:49:38 by aabdou            #+#    #+#             */
/*   Updated: 2022/10/29 09:52:08 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../headers/LocationContext.hpp"


LocationContext::LocationContext() {

}

LocationContext::~LocationContext() {

}

LocationContext::LocationContext(const LocationContext &obj) {
	*this = obj;
}

LocationContext &LocationContext::operator=(const LocationContext &obj) {
	if (this != &obj) {

	}
	return *this;
}
