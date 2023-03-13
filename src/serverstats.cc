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
    current_total_output_bytes_(0) {
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

void ServerStats::GetQps() {
  time_t now = time(0);
  tm *ltm = localtime(&now);
  current_time_ = (int)ltm->tm_sec;
  if (current_time_ != old_time_) { 
    LOG(INFO) << "QPS: " << current_total_command_processed_ - old_total_command_processed_;
    old_time_ = current_time_;
    old_total_command_processed_ = current_total_command_processed_;
  }
}

int ServerStats::GetInputBytes() {
  return current_total_input_bytes_;
}

int ServerStats::GetOutputBytes() {
  return current_total_output_bytes_;
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



























