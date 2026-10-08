#pragma once

#include <chrono>  // milliseconds
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "databento/detail/scoped_fd.hpp"  // ScopedFd
#include "databento/ireadable.hpp"
#include "databento/tcp_stats_observer.hpp"

// Forward declare
namespace databento {
class ILogReceiver;
}

namespace databento::detail {
class TcpClient {
 public:
  struct RetryConf {
    std::uint32_t max_attempts{1};
    std::chrono::seconds max_wait{std::chrono::minutes{1}};
    std::chrono::seconds connect_timeout{std::chrono::seconds{10}};
  };

  TcpClient(ILogReceiver* log_receiver, const std::string& gateway, std::uint16_t port);
  TcpClient(ILogReceiver* log_receiver, const std::string& gateway, std::uint16_t port,
            RetryConf retry_conf);

  void WriteAll(std::string_view str);
  void WriteAll(const std::byte* buffer, std::size_t size);
  void ReadExact(std::byte* buffer, std::size_t size);
  IReadable::Result ReadSome(std::byte* buffer, std::size_t max_size);
  // Passing a timeout of 0 will block until data is available of the socket is
  // closed, the same behavior as the Read overload without a timeout.
  IReadable::Result ReadSome(std::byte* buffer, std::size_t max_size,
                             std::chrono::milliseconds timeout);
  // Closes the socket.
  void Close();

  void SetTcpObserver(std::shared_ptr<TcpStatsObserver> o) {
    observer_ = std::move(o);
  };

 private:
  static ScopedFd InitSocket(ILogReceiver* log_receiver, const std::string& gateway,
                             std::uint16_t port, RetryConf retry_conf);

  ScopedFd socket_;
  std::shared_ptr<TcpStatsObserver> observer_;
};
}  // namespace databento::detail
