#ifndef __ServerStats_H_
#define __ServerStats_H_

#include <string>
#include <ctime>

static const std::string Version = "1.0.0";
static const std::string Multiplexing_Api = "epoll";

class ServerStats {
  public:
   ServerStats();
   ~ServerStats();

   static void Init();
   void InitTime();
   static ServerStats* GetCurrent();
   void CalculateQps();
   void CalculateInputKbps();
   void CalculateOutputKbps();
   void SetConnectClient(int size);
   int GetQps();
   int GetInputKbps();
   int GetOutputKbps();
   int GetInputBytes();
   int GetOutputBytes();
   int GetServerBeginTime();
   int GetConnectClient();
   int GetTotalConnectionsReceived();
   int GetTotalCommandsProcessed();
   void AddCommandProcessed();
   void AddInputBytes(int size);
   void AddOutputBytes(int size);
   void AddTotalConnectionsReceived();
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
   int qps_old_time_; // 旧时间
   int qps_current_time_; // 新时间
   int input_kbps_old_time_;
   int input_kbps_current_time_;
   int output_kbps_old_time_;
   int output_kbps_current_time_;
   int qps_;
   int total_connections_received_; // 总共的连接数
   int input_kbps_;
   int output_kbps_;
   int server_begin_time_;
   int connect_client_;
};




#endif // __ServerStats_H_
