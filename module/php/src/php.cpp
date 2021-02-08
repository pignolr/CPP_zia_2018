/*
** EPITECH PROJECT, 2021
** zia
** File description:
** php.c
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <experimental/filesystem>
#include "api/Stage.hpp"
#include <iostream>
#include <ctime>
#include <cstring>
#include <sstream>

#define PROG "/usr/bin/php"
extern char **environ;

std::vector<std::string>  GetEnvForPhp(dems::Context &context)
{
    std::vector<std::string> tmp_env(environ, environ+std::strlen((char *)environ));
    std::stringstream ss;

    auto &firstLine = std::get<dems::header::Request>(context.request.firstLine);
    ss << "PHP_SELF=" << firstLine.path;
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    ss << "REQUEST_URI=" << firstLine.path;
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    try  {
        auto &confServer = std::get<dems::config::ConfigObject>(context.config["server"].v);
        try {
            auto &ip = std::get<std::string>(context.config["ip"].v);
            ss << "SERVER_ADDR=" << ip;
            tmp_env.push_back(ss.str());
            ss.str(std::string());
        } catch (std::exception &) {}
        try {
            auto &port = std::get<long long>(context.config["port"].v);
            ss << "SERVER_PORT=" << port;
            tmp_env.push_back(ss.str());
            ss.str(std::string());
        } catch (std::exception &) {}
        try {
            auto &root = std::get<std::string>(context.config["root"].v);
            ss << "DOCUMENT_ROOT=" << root;
            tmp_env.push_back(ss.str());
            ss.str(std::string());
        } catch (std::exception &) {}
    } catch (std::exception &) {}

    tmp_env.push_back("SERVER_SOFTWARE=ZIA");
    tmp_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    ss << "REQUEST_METHOD=" <<  firstLine.method;
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    ss << "REQUEST_TIME=" <<  std::time(0);
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    ss << "HTTP_ACCEPT=" << context.request.headers->getHeader("Accept");
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    ss << "HTTP_ACCEPT_CHARSET=" <<  context.request.headers->getHeader("Accept-Charset");
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    ss << "HTTP_ACCEPT_ENCODING=" <<  context.request.headers->getHeader("Accept-Encoding");
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    ss << "HTTP_ACCEPT_LANGUAGE=" <<  context.request.headers->getHeader("Accept-Language");
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    ss << "HTTP_CONNECTION=" <<  context.request.headers->getHeader("Connection");
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    ss << "HTTP_HOST=" <<  context.request.headers->getHeader("Host");
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    ss << "HTTP_REFERER=" <<  context.request.headers->getHeader("Referer");
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    ss << "HTTP_USER_AGENT=" <<  context.request.headers->getHeader("User_Agent");
    tmp_env.push_back(ss.str());
    ss.str(std::string());
    try {
    if (std::get<bool>(context.config["is_SSL"].v))
        tmp_env.push_back("HTTPS=true");
    }catch(std::exception&){}

    return tmp_env;
}

std::string     GetFilePath(dems::Context &context)
{
    namespace fs = std::experimental::filesystem;
    std::string requestFile;
    fs::path path;
    try {
        auto &requestFirstLine = std::get<dems::header::Request>(context.request.firstLine);
        requestFile = requestFirstLine.path;
        auto &serverConfig = std::get<dems::config::ConfigObject>(context.config["server"].v);
        fs::path root = std::get<std::string>(serverConfig["root"].v);
        if (root.is_absolute()) {
            path = root;
        } else {
            path = fs::current_path();
            path /= root;
        }
        if (!fs::exists(path))
            path = fs::current_path();
    }
    catch (std::exception &) {
        path = fs::current_path();
    }
    path += requestFile;
    if (!fs::is_regular_file(path)) {
        path /= "index.html";
        if (!fs::exists(path) || !fs::is_regular_file(path))
            return "";
    }
    return path;
}

int phpCaller(dems::Context &context)
{
    int pipefd[2];
    pipe(pipefd);
    dems::config::ConfigObject phpConf;
    std::string phpPath = PROG;
    std::string phpIniPath;

try  {
    auto &confModule = std::get<dems::config::ConfigObject>(context.config["modules"].v);
    phpConf = std::get<dems::config::ConfigObject>(context.config["PHP"].v);
    try {
        phpPath = std::get<std::string>(context.config["EXE"].v);
    } catch (std::exception &) {}
} catch (std::exception &) {}

    auto file = GetFilePath(context);
    char* a[] = { phpPath.data(), file.data(), nullptr };

    auto tmp_env = GetEnvForPhp(context);

    std::vector<char*> new_env;
    new_env.reserve(tmp_env.size());

    for(size_t i = 0; i < tmp_env.size(); ++i)
        new_env.push_back(const_cast<char*>(tmp_env[i].c_str()));
    new_env.push_back(nullptr);

    int forkId = fork();
    if (forkId == -1)
    {
        printf("Fork failed\n");
        return (1);
    }
    else if (forkId == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        dup2(pipefd[1], 2);
        close(pipefd[1]);
        if (execve(phpPath.c_str(), a, new_env.data()) == -1)
          {
             std::cerr << "Execve failed: " << strerror(errno) << std::endl;
             exit(1);
          }
    }
    else
    {
      uint8_t buffer[1024];

      close(pipefd[1]);
      int size = read(pipefd[0], buffer, sizeof(buffer));
      context.response.body.clear();
      while (size > 0)
      {
          for (int i = 0; i < size; ++i)
            context.response.body.push_back(buffer[i]);
         size = read(pipefd[0], buffer, sizeof(buffer));
      }
      if (!context.response.body.empty())
          context.response.firstLine = dems::header::Response{ "HTTP/1.1", "200", "OK" };
    }
}

dems::CodeStatus ModulePhp(dems::Context &context)
{
    phpCaller(context);
    return dems::CodeStatus::OK;
}

extern "C" {
std::string registerHooks(dems::StageManager &manager)
{
    manager.request().hookToMiddle(0, "php module", ModulePhp);
    return "PHP";
}
}
