#ifndef __CONF_H_
#define __CONF_H_

#include <string>

static const uint32_t PORT = 1024;
static char* HOST = (char*)"127.0.0.1";
static const std::string DB_PATH = "./db";
static const std::string PASSWORD = "BuildTree";
static const std::string LOG_PATH = "./log";
static const char* DB_Delete_Sentence = "rm -rf /root/Git/KV-Server/db";
#endif // --CONF_H_
