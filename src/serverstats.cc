#include "serverstats.h"
#include <glog/logging.h>
#include <ctime>
#include <time.h>
#include <assert.h>

ServerStats* ServerStats::serverstats_ = nullptr;

ServerStats::ServerStats() 
  : qps_old_time_(0), 
    input_kbps_old_time_(0),
    output_kbps_old_time_(0),
    old_total_command_processed_(0),
    current_total_command_processed_(0),
    old_total_input_bytes_(0),
    current_total_input_bytes_(0),
    old_total_output_bytes_(0),
    current_total_output_bytes_(0), 
    total_connections_received_(0),
    server_begin_time_(time(NULL)),
    qps_(0) {
}

ServerStats::~ServerStats() {

}

void ServerStats::Init() {
  if (!serverstats_) {
    serverstats_ = new ServerStats();
    
  }
}

ServerStats* ServerStats::GetCurrent() {
  assert(serverstats_ != nullptr);
  return serverstats_;
}

int ServerStats::GetServerBeginTime() {
  return server_begin_time_;
}

void ServerStats::CalculateQps() {
  time_t now = time(0);
  tm *ltm = localtime(&now);
  qps_current_time_ = (int)ltm->tm_sec;
  if (qps_current_time_ != qps_old_time_) { 
    qps_ =  current_total_command_processed_ - old_total_command_processed_;
    qps_old_time_ = qps_current_time_;
    old_total_command_processed_ = current_total_command_processed_;
  }
}

void ServerStats::CalculateInputKbps() {
  time_t now = time(0);
  tm *ltm = localtime(&now);
  input_kbps_current_time_ = (int)ltm->tm_sec;
  if (input_kbps_current_time_ != input_kbps_old_time_) { 
    input_kbps_ = (current_total_input_bytes_ - old_total_input_bytes_) / 1048576;
    input_kbps_old_time_ = input_kbps_current_time_;
    old_total_input_bytes_ = current_total_input_bytes_;
  }
}

void ServerStats::CalculateOutputKbps() {
  time_t now = time(0);
  tm *ltm = localtime(&now);
  output_kbps_current_time_ = (int)ltm->tm_sec;
  if (output_kbps_current_time_ != output_kbps_old_time_) { 
    output_kbps_ = (current_total_output_bytes_ - old_total_output_bytes_) / 1024;
    output_kbps_old_time_ = output_kbps_current_time_;
    old_total_output_bytes_ = current_total_output_bytes_;
  }
}
void ServerStats::SetConnectClient(int size) {
  connect_client_ = size;
}

int ServerStats::GetQps() {
  return qps_;
}

int ServerStats::GetInputKbps() {
  return input_kbps_;
}

int ServerStats::GetOutputKbps() {
  return output_kbps_;
}

int ServerStats::GetInputBytes() {
  return current_total_input_bytes_;
}

int ServerStats::GetOutputBytes() {
  return current_total_output_bytes_;
}

int ServerStats::GetTotalConnectionsReceived() {
  return total_connections_received_;
}

int ServerStats::GetConnectClient() {
  return connect_client_;
}

int ServerStats::GetTotalCommandsProcessed() {
  return current_total_command_processed_;
}

void ServerStats::AddCommandProcessed() {
  current_total_command_processed_++;
}

void ServerStats::AddInputBytes(int size) {
  current_total_input_bytes_ += size;
}

void ServerStats::AddOutputBytes(int size) {
  current_total_output_bytes_ += size;
}

void ServerStats::AddTotalConnectionsReceived() {
  total_connections_received_++;
}

























