#include "kv_io.h"
#include "kv_encode.h"

#include <fstream>
#include <string>

#include <glog/logging.h>

void IO::Flushall() {   // 清空leveldb数据库
  int flag = system("rm -rf /root/Git/KV-Server/leveldb.db");
}

void IO::set(std::string order, std::string key, std::string value) { // 文件set指令
  int maxpos;
  std::ofstream ofs;
  std::string sql = "";
  ofs.open("test.txt", std::ios::app);
  maxpos = ofs.tellp();
  if (maxpos == 0) {
    ofs << "begin: " << std::endl;
  }
  sql = Encode::sqlSplice(sql, order, 1);
  sql = Encode::sqlSplice(sql, key, 1);
  sql = Encode::sqlSplice(sql, value, 0);
  ofs << sql << std::endl;
  ofs.close();
}

std::string IO::Extract(std::string templine, int &position) { //文件中提取字符串中的key，value
  std::string word = "";
  std::string str = "";
  int size;
  for (int i = position; ;i++) {
    if (templine[i] != ' ') {
      word.push_back(templine[i]);
    } else {
      position = i + 1;
      size = atoi(word.c_str());
      str.assign(templine, position, size);
      position = position + size + 1; 
      break;
    }
  }
  return str;
}

std::string IO::get(std::string Key) {  // 文件get指令
  int maxpos;
  char x;
  bool flag = false;
  std::fstream f("test.txt", std::ios::ate);
  std::string order, key, value, line = "";
  if (!f) {
    printf("打开文件失败\n");
  }
  maxpos = f.tellg();
  f.seekg(std::ios::end);
  for (int index = 2; index <= maxpos; index++) {
    f.seekg(-index, std::ios::end);
    f.get(x);
    if (x == 0) {
      continue;
    }
    if (x == '\n') {
      std::string templine = line;
      if (templine.size() > 0 ) {
        int position = 0;
        order = Extract(templine, position);
        key = Extract(templine, position);
        value = Extract(templine, position);
        if (key.compare(Key) == 0 && !value.empty()) {
          flag = true;
          value = Encode::getWord(value);
          break;
        }
      }
      line = "";
    } else {
      line = x + line;
    }
    x = 0;
  }
  if (flag) {
    return value;
  } else {
    return Encode::getWord("not found");
  }
}

std::string IO::flushall() {  // 清空文件数据库
  std::fstream f("test.txt", std::fstream::out | std::ios_base::trunc);
  return Encode::getWord("clear");
}













































