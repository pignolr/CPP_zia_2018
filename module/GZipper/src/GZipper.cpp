/*
** EPITECH PROJECT, 2021
** zia
** File description:
** ModuleGZipper.cpp
*/

#include <cstdio>
#include <iostream>
#include <sstream> 
#include <string>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "GZipper.hpp"

dems::CodeStatus ModuleGZipper(dems::Context &context)
{
    auto &request = context.request;
    auto &response = context.response;
    auto &accept_encoding = request.headers->getHeader("Accept-Encoding");
    if (accept_encoding.empty() || accept_encoding.find("gzip") == accept_encoding.npos)
        return dems::CodeStatus::DECLINED;
    else
    {
    	auto &content_encoding = (*response.headers)["content-encoding"];
        if (!content_encoding.empty())
        	content_encoding += ", ";
    	content_encoding += "gzip";
        std::stringstream compressed;
        std::stringstream new_body;
        new_body << response.body;
        boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
        out.push(boost::iostreams::gzip_compressor());
        out.push(new_body);
        boost::iostreams::copy(out, compressed);
        response.body = compressed.str();
    }
    return dems::CodeStatus::OK;
}

extern "C" {
    std::string registerHooks(dems::StageManager &manager)
    {
        manager.request().hookToEnd(50000, "GZipper", ModuleGZipper);
        return "GZipper";
    }
}