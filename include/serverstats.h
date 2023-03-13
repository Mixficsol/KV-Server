#ifndef __ServerStats_H_
#define __ServerStats_H_

#include <string>

static const std::string Version = "1.0.0";
static const std::string Multiplexing_Api = "epoll";

class ServerStats {
  public:
   ServerStats();
   ~ServerStats();

   static void Init();
   static ServerStats* GetCurrent();
   void GetQps();
   int GetInputBytes();
   int GetOutputBytes();
   void AddCommandProcessed();
   void AddInputBytes(int size);
   void AddOutputBytes(int size);
  

  protected:

  private:
   static ServerStats* serverstats_;
   ServerStats(ServerStats& se);
   void operator =(const ServerStats& se);
   int old_total_command_processed_; // 旧条目
   int current_total_command_processed_; // 新条目
   int old_total_input_bytes_; // 旧读取字节
   int current_total_input_bytes_; // 新读取字节
   int old_total_output_bytes_; // 旧写出字节
   int current_total_output_bytes_; // 新写出字节
   int old_time_ = 0; // 旧时间
   int current_time_; // 新时间
};




#endif // __ServerStats_H_
