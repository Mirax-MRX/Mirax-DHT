
#pragma once

#include /utils/common.h"
#include /utils/Slice.h"
#include /utils/Status.h"
#include /utils/StringBuilder.h"

namespace td {

class HttpUrl {
 public:
  enum class Protocol { Http, Https } protocol_ = Protocol::Http;
  string userinfo_;
  string host_;
  bool is_ipv6_ = false;
  int specified_port_ = 0;
  int port_ = 0;
  string query_;

  string get_url() const;

  HttpUrl(Protocol protocol, string userinfo, string host, bool is_ipv6, int specified_port, int port, string query)
      : protocol_(protocol)
      , userinfo_(std::move(userinfo))
      , host_(std::move(host))
      , is_ipv6_(is_ipv6)
      , specified_port_(specified_port)
      , port_(port)
      , query_(std::move(query)) {
  }
};

Result<HttpUrl> parse_url(Slice url,
                          HttpUrl::Protocol default_protocol = HttpUrl::Protocol::Http) TD_WARN_UNUSED_RESULT;

StringBuilder &operator<<(StringBuilder &sb, const HttpUrl &url);

string get_url_query_file_name(const string &query);

string get_url_file_name(Slice url);

}  // namespace td
