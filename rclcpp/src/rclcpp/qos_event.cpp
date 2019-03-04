// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rclcpp/qos_event.hpp"


namespace rclcpp
{

/// Get the number of ready events
size_t
QOSEventBase::get_number_of_ready_events()
{
  return 1;
}

/// Add the Waitable to a wait set.
bool
QOSEventBase::add_to_wait_set(rcl_wait_set_t * wait_set)
{
  if (rcl_wait_set_add_event(wait_set, &event_handle_, &wait_set_event_index_) != RCL_RET_OK) {
    RCUTILS_LOG_ERROR_NAMED(
      "rclcpp",
      "Couldn't add event to wait set: %s", rcl_get_error_string().str);
    return false;
  }

  return true;
}

/// Check if the Waitable is ready.
bool
QOSEventBase::is_ready(rcl_wait_set_t * wait_set)
{
  return (wait_set->events[wait_set_event_index_] == &event_handle_);
}

}  // namespace rclcpp