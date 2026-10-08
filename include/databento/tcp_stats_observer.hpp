#pragma once

#include <chrono>

namespace databento {

class TcpStatsObserver {
 public:
  struct Metrics {
    std::chrono::nanoseconds kernelToUserLatency;
    std::chrono::nanoseconds rttEstimate;
    std::uint64_t bytesUnread = 0;
    std::uint64_t rcvSpace = 0;
    std::uint64_t rcvSsthresh = 0;
  };

  virtual ~TcpStatsObserver() = default;

  virtual void observe(Metrics const& m) = 0;
};

}  // namespace databento
