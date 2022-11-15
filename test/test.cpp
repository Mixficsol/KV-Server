#include <iostream>
#include "glog/logging.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace std;

int main(int argc, char*argv[]) {
  if (!boost::filesystem::exists("config.ini")) {
    std::cerr << "config.ini not exists." << std::endl;
    return -1;
  }
  boost::property_tree::ptree root_node, tag_system;
  boost::property_tree::ini_parser::read_ini("config.ini", root_node);
  tag_system = root_node.get_child("System");
  if (tag_system.count("reboot_cnt") != 1) {
    std::cerr << "reboot_cnt node not exists." << std::endl;
    return -1;
  }
  int cnt = tag_system.get<int>("reboot_cnt");
  std::cout << "reboot_cnt: " << cnt << std::endl;
  return 0;
}
