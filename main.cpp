#include "mobilerobot/mobilerobot.cpp"

void get_status(const std::vector<std::unique_ptr<RWA2::MobileRobot>>& robots){
    for (const auto& robot : robots){
        robot->move(10, 45);
        robot->print_status();
    }
}

int main(){
    // Initializing sensors
    auto sensor1 = std::make_unique<RWA2::Sensor>("OSDome");
    auto sensor2 = std::make_unique<RWA2::Sensor>("OS0");
    std::vector<std::unique_ptr<RWA2::Sensor>> sensors;
    sensors.push_back(std::move(sensor1));
    sensors.push_back(std::move(sensor2));

    // initializing battery
    RWA2::Battery battery("Li-ion");

    std::vector<std::unique_ptr<RWA2::MobileRobot>> mobile_robots;

    std::unique_ptr<RWA2::MobileRobot> aerial_ptr =
        std::make_unique<RWA2::AerialRobot>("Erle-Plane",std::move(battery), std::move(sensors));
    
    std::unique_ptr<RWA2::MobileRobot> legged_ptr =
        std::make_unique<RWA2::LeggedRobot>("Spot",std::move(battery), std::move(sensors));

    std::unique_ptr<RWA2::MobileRobot> wheeled_ptr =
        std::make_unique<RWA2::WheeledRobot>("Hiwonder",std::move(battery), std::move(sensors));

    mobile_robots.push_back(std::move(aerial_ptr));
    mobile_robots.push_back(std::move(legged_ptr));
    mobile_robots.push_back(std::move(wheeled_ptr));

    get_status(mobile_robots);
}