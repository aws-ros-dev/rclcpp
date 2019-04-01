// Copyright 2019 Open Source Robotics Foundation, Inc.
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

#ifndef RCLCPP__PUBLISHER_OPTIONS_HPP_
#define RCLCPP__PUBLISHER_OPTIONS_HPP_

#include <memory>
#include <string>
#include <vector>

#include "rclcpp/qos_event.hpp"
#include "rclcpp/visibility_control.hpp"

namespace rclcpp
{

/// Contains callbacks for various types of events a Publisher can receive from the middleware
struct PublisherEventCallbacks
{
  QOSDeadlineOfferedCallbackType deadline_callback;
  QOSLivelinessLostCallbackType liveliness_callback;
};

/// Structure containing configuration options for Publishers/Subscribers
template<typename Alloc = std::allocator<void>>
struct PublisherOptions
{
  PublisherEventCallbacks event_callbacks;
  rmw_qos_profile_t qos_profile = rmw_qos_profile_default;
  std::shared_ptr<Alloc> allocator = nullptr;
};

}  // namespace rclcpp

#endif  // RCLCPP__PUBLISHER_OPTIONS_HPP_
