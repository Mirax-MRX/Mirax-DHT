
#pragma once

#include /utils/port/config.h"

#include /utils/buffer.h"
#include /utils/optional.h"
#include /utils/port/detail/NativeFd.h"
#include /utils/port/detail/PollableFd.h"
#include /utils/port/IPAddress.h"
#include /utils/Slice.h"
#include /utils/Span.h"
#include /utils/Status.h"

#include <memory>

namespace td {
// Udp and errors
namespace detail {
class UdpSocketFdImpl;
class UdpSocketFdImplDeleter {
 public:
  void operator()(UdpSocketFdImpl *impl);
};
}  // namespace detail

struct UdpMessage {
  IPAddress address;
  BufferSlice data;
  Status error;
};

class UdpSocketFd {
 public:
  UdpSocketFd();
  UdpSocketFd(UdpSocketFd &&);
  UdpSocketFd &operator=(UdpSocketFd &&);
  ~UdpSocketFd();

  UdpSocketFd(const UdpSocketFd &) = delete;
  UdpSocketFd &operator=(const UdpSocketFd &) = delete;

  Result<uint32> maximize_snd_buffer(uint32 max_buffer_size = 0);
  Result<uint32> maximize_rcv_buffer(uint32 max_buffer_size = 0);

  static Result<UdpSocketFd> open(const IPAddress &address) TD_WARN_UNUSED_RESULT;

  PollableFdInfo &get_poll_info();
  const PollableFdInfo &get_poll_info() const;
  const NativeFd &get_native_fd() const;

  void close();
  bool empty() const;

  static bool is_critical_read_error(const Status &status);

#if TD_PORT_POSIX
  struct OutboundMessage {
    const IPAddress *to;
    Slice data;
  };
  struct InboundMessage {
    IPAddress *from;
    MutableSlice data;
    Status *error;
  };

  Status send_message(const OutboundMessage &message, bool &is_sent) TD_WARN_UNUSED_RESULT;
  Status receive_message(InboundMessage &message, bool &is_received) TD_WARN_UNUSED_RESULT;

  Status send_messages(Span<OutboundMessage> messages, size_t &count) TD_WARN_UNUSED_RESULT;
  Status receive_messages(MutableSpan<InboundMessage> messages, size_t &count) TD_WARN_UNUSED_RESULT;
#elif TD_PORT_WINDOWS
  Result<optional<UdpMessage> > receive();

  void send(UdpMessage message);

  Status flush_send();
#endif

 private:
  static constexpr uint32 DEFAULT_UDP_MAX_SND_BUFFER_SIZE = (1 << 24);
  static constexpr uint32 DEFAULT_UDP_MAX_RCV_BUFFER_SIZE = (1 << 24);
  std::unique_ptr<detail::UdpSocketFdImpl, detail::UdpSocketFdImplDeleter> impl_;
  explicit UdpSocketFd(unique_ptr<detail::UdpSocketFdImpl> impl);
};

}  // namespace td
