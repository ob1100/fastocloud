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

#include "server/daemon/commands_info/service/prepare_info.h"

#include <string>

#include <common/file_system/file_system.h>
#include <common/file_system/file_system_utils.h>
#include <common/file_system/string_path_utils.h>

#include "base/utils.h"

#define OK_RESULT "OK"

#define PREPARE_SERVICE_INFO_FEEDBACK_DIRECTORY_FIELD "feedback_directory"
#define PREPARE_SERVICE_INFO_TIMESHIFTS_DIRECTORY_FIELD "timeshifts_directory"
#define PREPARE_SERVICE_INFO_HLS_DIRECTORY_FIELD "hls_directory"
#define PREPARE_SERVICE_INFO_VODS_DIRECTORY_FIELD "vods_directory"
#define PREPARE_SERVICE_INFO_CODS_DIRECTORY_FIELD "cods_directory"
#define PREPARE_SERVICE_INFO_PROXY_DIRECTORY_FIELD "proxy_directory"

#define SAVE_DIRECTORY_FIELD_PATH "path"
#define SAVE_DIRECTORY_FIELD_CONTENT "content"
#define SAVE_DIRECTORY_FIELD_RESULT "result"
#define SAVE_DIRECTORY_FIELD_ERROR "error"

namespace fastocloud {
namespace server {
namespace service {

namespace {
json_object* MakeDirectoryStateResponce(const DirectoryState& dir) {
  json_object* obj_dir = json_object_new_object();

  json_object* obj = json_object_new_object();
  const std::string path_str = dir.dir.GetPath();
  json_object_object_add(obj, SAVE_DIRECTORY_FIELD_PATH, json_object_new_string(path_str.c_str()));
  if (dir.is_valid) {
    json_object_object_add(obj, SAVE_DIRECTORY_FIELD_RESULT, json_object_new_string(OK_RESULT));
    json_object* jcontent = json_object_new_array();
    if (dir.content) {
      const auto content = *dir.content;
      for (const common::file_system::ascii_file_string_path& file : content) {
        const auto path = file.GetPath();
        json_object_array_add(jcontent, json_object_new_string(path.c_str()));
      }
    }
    json_object_object_add(obj, SAVE_DIRECTORY_FIELD_CONTENT, jcontent);
  } else {
    json_object_object_add(obj, SAVE_DIRECTORY_FIELD_ERROR, json_object_new_string(dir.error_str.c_str()));
  }

  json_object_object_add(obj_dir, dir.key.c_str(), obj);
  return obj_dir;
}
}  // namespace

PrepareInfo::PrepareInfo()
    : base_class(),
      feedback_directory_(),
      timeshifts_directory_(),
      hls_directory_(),
      vods_directory_(),
      cods_directory_(),
      proxy_directory_() {}

std::string PrepareInfo::GetFeedbackDirectory() const {
  return feedback_directory_;
}

std::string PrepareInfo::GetTimeshiftsDirectory() const {
  return timeshifts_directory_;
}

std::string PrepareInfo::GetHlsDirectory() const {
  return hls_directory_;
}

std::string PrepareInfo::GetVodsDirectory() const {
  return vods_directory_;
}

std::string PrepareInfo::GetCodsDirectory() const {
  return cods_directory_;
}

std::string PrepareInfo::GetProxyDirectory() const {
  return proxy_directory_;
}

common::Error PrepareInfo::SerializeFields(json_object* out) const {
  json_object_object_add(out, PREPARE_SERVICE_INFO_FEEDBACK_DIRECTORY_FIELD,
                         json_object_new_string(feedback_directory_.c_str()));
  json_object_object_add(out, PREPARE_SERVICE_INFO_TIMESHIFTS_DIRECTORY_FIELD,
                         json_object_new_string(timeshifts_directory_.c_str()));
  json_object_object_add(out, PREPARE_SERVICE_INFO_HLS_DIRECTORY_FIELD, json_object_new_string(hls_directory_.c_str()));
  json_object_object_add(out, PREPARE_SERVICE_INFO_VODS_DIRECTORY_FIELD,
                         json_object_new_string(vods_directory_.c_str()));
  json_object_object_add(out, PREPARE_SERVICE_INFO_CODS_DIRECTORY_FIELD,
                         json_object_new_string(cods_directory_.c_str()));
  json_object_object_add(out, PREPARE_SERVICE_INFO_PROXY_DIRECTORY_FIELD,
                         json_object_new_string(proxy_directory_.c_str()));
  return common::Error();
}

common::Error PrepareInfo::DoDeSerialize(json_object* serialized) {
  PrepareInfo inf;
  json_object* jfeedback_directory = nullptr;
  json_bool jfeedback_directory_exists =
      json_object_object_get_ex(serialized, PREPARE_SERVICE_INFO_FEEDBACK_DIRECTORY_FIELD, &jfeedback_directory);
  if (jfeedback_directory_exists) {
    inf.feedback_directory_ = json_object_get_string(jfeedback_directory);
  }

  json_object* jtimeshifts_directory = nullptr;
  json_bool jtimeshifts_directory_exists =
      json_object_object_get_ex(serialized, PREPARE_SERVICE_INFO_TIMESHIFTS_DIRECTORY_FIELD, &jtimeshifts_directory);
  if (jtimeshifts_directory_exists) {
    inf.timeshifts_directory_ = json_object_get_string(jtimeshifts_directory);
  }

  json_object* jhls_directory = nullptr;
  json_bool jhls_directory_exists =
      json_object_object_get_ex(serialized, PREPARE_SERVICE_INFO_HLS_DIRECTORY_FIELD, &jhls_directory);
  if (jhls_directory_exists) {
    inf.hls_directory_ = json_object_get_string(jhls_directory);
  }

  json_object* jvods_directory = nullptr;
  json_bool jvods_directory_exists =
      json_object_object_get_ex(serialized, PREPARE_SERVICE_INFO_VODS_DIRECTORY_FIELD, &jvods_directory);
  if (jvods_directory_exists) {
    inf.vods_directory_ = json_object_get_string(jvods_directory);
  }

  json_object* jcods_directory = nullptr;
  json_bool jcods_directory_exists =
      json_object_object_get_ex(serialized, PREPARE_SERVICE_INFO_CODS_DIRECTORY_FIELD, &jcods_directory);
  if (jcods_directory_exists) {
    inf.cods_directory_ = json_object_get_string(jcods_directory);
  }

  json_object* jproxy_directory = nullptr;
  json_bool jproxy_directory_exists =
      json_object_object_get_ex(serialized, PREPARE_SERVICE_INFO_PROXY_DIRECTORY_FIELD, &jproxy_directory);
  if (jproxy_directory_exists) {
    inf.proxy_directory_ = json_object_get_string(jproxy_directory);
  }

  *this = inf;
  return common::Error();
}

DirectoryState::DirectoryState(const std::string& dir_str, const char* k)
    : key(k), dir(), content(), is_valid(false), error_str() {
  if (dir_str.empty()) {
    error_str = "Invalid input.";
    return;
  }

  dir = common::file_system::ascii_directory_string_path(dir_str);
  const std::string dir_path = dir.GetPath();
  common::ErrnoError errn = CreateAndCheckDir(dir_path);
  if (errn) {
    error_str = errn->GetDescription();
    return;
  }

  is_valid = true;
}

void DirectoryState::LoadContent() {
  if (is_valid) {
    content = common::file_system::ScanFolder(dir, "*.*", true);
  }
}

Directories::Directories(const PrepareInfo& sinf)
    : feedback_dir(sinf.GetFeedbackDirectory(), PREPARE_SERVICE_INFO_FEEDBACK_DIRECTORY_FIELD),
      timeshift_dir(sinf.GetTimeshiftsDirectory(), PREPARE_SERVICE_INFO_TIMESHIFTS_DIRECTORY_FIELD),
      hls_dir(sinf.GetHlsDirectory(), PREPARE_SERVICE_INFO_HLS_DIRECTORY_FIELD),
      vods_dir(sinf.GetVodsDirectory(), PREPARE_SERVICE_INFO_VODS_DIRECTORY_FIELD),
      cods_dir(sinf.GetCodsDirectory(), PREPARE_SERVICE_INFO_CODS_DIRECTORY_FIELD),
      proxy_dir(sinf.GetProxyDirectory(), PREPARE_SERVICE_INFO_PROXY_DIRECTORY_FIELD) {}

std::string MakeDirectoryResponce(const Directories& dirs) {
  json_object* obj = json_object_new_array();
  json_object_array_add(obj, MakeDirectoryStateResponce(dirs.feedback_dir));
  json_object_array_add(obj, MakeDirectoryStateResponce(dirs.timeshift_dir));
  json_object_array_add(obj, MakeDirectoryStateResponce(dirs.hls_dir));
  json_object_array_add(obj, MakeDirectoryStateResponce(dirs.vods_dir));
  json_object_array_add(obj, MakeDirectoryStateResponce(dirs.cods_dir));
  json_object_array_add(obj, MakeDirectoryStateResponce(dirs.proxy_dir));
  std::string obj_str = json_object_get_string(obj);
  json_object_put(obj);
  return obj_str;
}

}  // namespace service
}  // namespace server
}  // namespace fastocloud
