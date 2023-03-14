#include "serverstats.h"
#include "kv_conn.h"
#include <glog/logging.h>
#include <ctime>
#include <time.h>
#include <assert.h>
#include <map>
#include <iostream>

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

void ServerStats::Calculate(int size) {
  connect_client_ = size;
  time_t now = time(0);
  tm *ltm = localtime(&now);
  current_time_ = (int)ltm->tm_sec;
  if (current_time_ != old_time_) {
    qps_ =  current_total_command_processed_ - old_total_command_processed_;
    input_kbps_ = (current_total_input_bytes_ - old_total_input_bytes_) / 1048576;
    output_kbps_ = (current_total_output_bytes_ - old_total_output_bytes_) / 1024;
    old_total_command_processed_ = current_total_command_processed_;
    old_total_input_bytes_ = current_total_input_bytes_;
    old_total_output_bytes_ = current_total_output_bytes_;
    old_time_ = current_time_;
  }
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

























