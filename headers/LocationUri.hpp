/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUri.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <aabdou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:56:00 by aabdou            #+#    #+#             */
/*   Updated: 2022/11/04 17:58:33 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONURI_HPP
#define LOCATIONURI_HPP

#include "./LocationContext.hpp"
#include "./Uri.hpp"

class LocationUri {
	private:



	public:
		LocationUri();
		~LocationUri();
		LocationUri(const LocationUri &obj);
		LocationUri &operator=(LocationUri const &obj);



}




#endif
