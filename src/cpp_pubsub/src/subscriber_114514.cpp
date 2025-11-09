//
// Created by cyberkanjousen on 2025/11/5.
//

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Subscriber_114514 : public rclcpp::Node {
public:
    Subscriber_114514() : Node("subscriber_114514") {
        auto topic_114514_callback = [this](std_msgs::msg::String::UniquePtr msg) -> void {
            RCLCPP_INFO(this->get_logger(), "%s", msg->data.c_str());
        };
        subscription_ = this->create_subscription<std_msgs::msg::String>("topic_114514", rclcpp::SensorDataQoS(), topic_114514_callback);
    }
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Subscriber_114514>());
    rclcpp::shutdown();
    return 0;
}