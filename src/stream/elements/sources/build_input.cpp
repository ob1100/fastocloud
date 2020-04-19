/*  Copyright (C) 2014-2020 FastoGT. All right reserved.
    This file is part of fastocloud.
    fastocloud is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    fastocloud is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with fastocloud.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stream/elements/sources/build_input.h"

#include <string>

#include "stream/elements/sources/filesrc.h"
#include "stream/elements/sources/httpsrc.h"
#include "stream/elements/sources/rtmpsrc.h"
#include "stream/elements/sources/srtsrc.h"
#include "stream/elements/sources/tcpsrc.h"
#include "stream/elements/sources/udpsrc.h"

namespace {
const char kFFmpegUA[] = "Lavf/58.27.103";
const char kVLC[] = "VLC/3.0.1 LibVLC/3.0.1";
const char kWinkUA[] = "WINK";
const char kChromeUA[] = "Chrome/60.0.3112.113 Safari/537.36";
const char kMozillaUA[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64)";
const char kSafariUA[] = "AppleWebKit/537.36 (KHTML, like Gecko)";
}  // namespace

namespace fastocloud {
namespace stream {
namespace elements {
namespace sources {

Element* make_src(const InputUri& uri, element_id_t input_id, gint timeout_secs) {
  common::uri::Url url = uri.GetInput();
  common::uri::Url::scheme scheme = url.GetScheme();
  if (scheme == common::uri::Url::file) {
    const common::uri::Upath upath = url.GetPath();
    return make_file_src(upath.GetPath(), input_id);
  } else if (scheme == common::uri::Url::http || scheme == common::uri::Url::https) {
    common::Optional<std::string> agent;
    const auto ua = uri.GetUserAgent();
    if (ua) {
      InputUri::UserAgent uagent = *ua;
      if (uagent == InputUri::VLC) {
        agent = std::string(kVLC);
      } else if (uagent == InputUri::FFMPEG) {
        agent = std::string(kFFmpegUA);
      } else if (uagent == InputUri::WINK) {
        agent = std::string(kWinkUA);
      } else if (uagent == InputUri::CHROME) {
        agent = std::string(kChromeUA);
      } else if (uagent == InputUri::MOZILLA) {
        agent = std::string(kMozillaUA);
      } else if (uagent == InputUri::SAFARI) {
        agent = std::string(kSafariUA);
      }
    }

    return make_http_src(url.GetUrl(), agent, uri.GetHttpProxyUrl(), timeout_secs, input_id);
  } else if (scheme == common::uri::Url::udp) {
    // udp://localhost:8080
    std::string host_str = url.GetHost();
    common::net::HostAndPort host;
    if (!common::ConvertFromString(host_str, &host)) {
      NOTREACHED() << "Unknown input url: " << host_str;
      return nullptr;
    }
    return make_udp_src(host, uri.GetMulticastIface(), input_id);
  } else if (scheme == common::uri::Url::rtmp) {
    return make_rtmp_src(url.GetUrl(), timeout_secs, input_id);
  } else if (scheme == common::uri::Url::tcp) {
    // tcp://localhost:8080
    std::string host_str = url.GetHost();
    common::net::HostAndPort host;
    if (!common::ConvertFromString(host_str, &host)) {
      NOTREACHED() << "Unknown input url: " << host_str;
      return nullptr;
    }
    return make_tcp_server_src(host, input_id);
  } else if (scheme == common::uri::Url::srt) {
    return make_srt_src(url.GetUrl(), input_id);
  }

  NOTREACHED() << "Unknown input url: " << url.GetUrl();
  return nullptr;
}

}  // namespace sources
}  // namespace elements
}  // namespace stream
}  // namespace fastocloud
