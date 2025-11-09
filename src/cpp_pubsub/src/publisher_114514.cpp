//
// Created by cyberkanjousen on 2025/10/30.
//

#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class Server_114514 : public rclcpp::Node {
public:
    Server_114514() : Node("publisher_114514") {
        this->declare_parameter<std::string>("content", "114514");
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic_114514", 10);
        auto time_back =
            [this] () -> void {
                auto message = std_msgs::msg::String();
                message.data = this->get_parameter("content").as_string();
                RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
                this->publisher_->publish(message);
            };
        time_ = this->create_wall_timer(500ms, time_back);
    }
private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr time_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Server_114514>());
    rclcpp::shutdown();
    return 0;
}