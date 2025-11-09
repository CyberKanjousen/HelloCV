//
// Created by cyberkanjousen on 2025/11/7.
//

#include "rclcpp/rclcpp.hpp"
#include "service_114514/srv/add_two_ints.hpp"
#include <memory>

class Server_114514 : public rclcpp::Node {
public:
    Server_114514() : Node("server_114514") {
        auto add =
            [] (const std::shared_ptr<service_114514::srv::AddTwoInts::Request> request,
         std::shared_ptr<service_114514::srv::AddTwoInts::Response> response) -> void {
                response->sum = request->a + request->b;
                RCLCPP_INFO (rclcpp::get_logger("rclcpp"),
                    "Incoming request\na: %ld" " b: %ld",
                    request->a, request->b);
                RCLCPP_INFO (rclcpp::get_logger("rclcpp"),
                    "Sending back response: [%ld]",
                    static_cast<long int>(response->sum));
        };
        service = this->create_service<service_114514::srv::AddTwoInts>("add_two_ints", add);
    }
private:
    rclcpp::Service<service_114514::srv::AddTwoInts>::SharedPtr service;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to add two ints.");
    rclcpp::spin(std::make_shared<Server_114514>());
    rclcpp::shutdown();
    return 0;
}