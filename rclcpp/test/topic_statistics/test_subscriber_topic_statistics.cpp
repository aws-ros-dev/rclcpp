// Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "libstatistics_collector/moving_average_statistics/types.hpp"

#include "metrics_statistics_msgs/msg/metrics_message.hpp"

#include "rclcpp/create_publisher.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/topic_statistics/subscriber_topic_statistics.hpp"

#include "test_msgs/msg/empty.hpp"


namespace
{
constexpr const char kTestNodeName[]{"test_sub_stats_node"};
constexpr const char kTestSubStatsTopic[]{"/test_sub_stats_topic"};
constexpr const char kTestTopicStatisticsTopic[]{"/test_topic_statistics_topic"};
constexpr const uint16_t kNoSamples = 0;
}  // namespace

using test_msgs::msg::Empty;
using metrics_statistics_msgs::msg::MetricsMessage;
using rclcpp::topic_statistics::SubscriberTopicStatistics;

/**
 * Empty subscriber node: used to create subscriber topic statistics requirements
 */
class EmptySubscriber : public rclcpp::Node
{
public:
  EmptySubscriber(const std::string & name, const std::string & topic)
  : Node(name)
  {
    auto callback = [this](Empty::UniquePtr msg) {
        this->ReceiveMessage(*msg);
      };
    subscription_ = create_subscription<Empty,
        std::function<void(Empty::UniquePtr)>>(
      topic,
      0 /*history_depth*/,
      callback);
  }

private:
  void ReceiveMessage(const Empty & msg) const
  {
    (void) msg;
  }

  rclcpp::Subscription<Empty>::SharedPtr subscription_;
};

/**
 * Test fixture to bring up and teardown rclcpp
 */
class TestSubscriberTopicStatisticsFixture : public ::testing::Test
{
protected:
  void SetUp()
  {
    rclcpp::init(0, nullptr);
  }

  void TearDown()
  {
    rclcpp::shutdown();
  }
};

TEST_F(TestSubscriberTopicStatisticsFixture, TestManualConstruction)
{
  auto empty_subscriber = std::make_shared<EmptySubscriber>(
    kTestNodeName,
    kTestSubStatsTopic);

  // manually create publisher tied to the node
  auto topic_stats_publisher =
    empty_subscriber->create_publisher<MetricsMessage>(
    kTestTopicStatisticsTopic,
    10);

  // construct the instance
  auto sub_topic_stats = std::make_unique<SubscriberTopicStatistics<Empty>>(
    empty_subscriber->get_name(),
    topic_stats_publisher);

  using libstatistics_collector::moving_average_statistics::StatisticData;

  // expect no data has been collected / no samples received
  for (const auto & data : sub_topic_stats->GetCurrentCollectorData()) {
    EXPECT_TRUE(std::isnan(data.average));
    EXPECT_TRUE(std::isnan(data.min));
    EXPECT_TRUE(std::isnan(data.max));
    EXPECT_TRUE(std::isnan(data.standard_deviation));
    EXPECT_EQ(kNoSamples, data.sample_count);
  }
}