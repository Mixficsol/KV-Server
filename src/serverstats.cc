#include "serverstats.h"
#include <glog/logging.h>
#include <ctime>
#include <assert.h>

ServerStats* ServerStats::serverstats_ = nullptr;

ServerStats::ServerStats() 
  : old_time_(0), 
    old_total_command_processed_(0),
    current_total_command_processed_(0),
    old_total_input_bytes_(0),
    current_total_input_bytes_(0),
    old_total_output_bytes_(0),
    current_total_output_bytes_(0), 
    total_connections_received_(0),
    qps_(0) {
}

ServerStats::~ServerStats() {

}

void ServerStats::Init() {
  if (!serverstats_) {
    serverstats_ = new ServerStats();
  }
}

void ServerStats::InitTime() {
  tm Tm;
  time_t now = time(0);
  char* Begin_time = ctime(&now);
  begin_time = Begin_time;
  LOG(INFO) << begin_time;
  strptime(begin_time, "%Y-%m-%d %H:%M:%S", &Tm);
  server_begin_time = mktime(&Tm);
}

char* ServerStats::GetBeginTime() {
  return begin_time;
}

ServerStats* ServerStats::GetCurrent() {
  assert(serverstats_ != nullptr);
  return serverstats_;
}

void ServerStats::CalculateQps() {
  time_t now = time(0);
  tm *ltm = localtime(&now);
  current_time_ = (int)ltm->tm_sec;
  if (current_time_ != old_time_) { 
    qps_ =  current_total_command_processed_ - old_total_command_processed_;
    old_time_ = current_time_;
    old_total_command_processed_ = current_total_command_processed_;
  }
}

int ServerStats::GetQps() {
  return qps_;
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

























