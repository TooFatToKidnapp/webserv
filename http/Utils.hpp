/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabtaim <ylabtaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:03:15 by ylabtaim          #+#    #+#             */
/*   Updated: 2022/12/09 15:26:07 by ylabtaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "codes.hpp"

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ft_trim(const std::string &s);
std::string ReasonPhrase(int code);
std::vector<std::string> ft_split(const std::string &str, const std::string &del);
std::string	getDate();
int getFileLength(const std::string & filename);
std::string	getMediaType(const std::string &subtype);
int		pathIsFile(const std::string& path);