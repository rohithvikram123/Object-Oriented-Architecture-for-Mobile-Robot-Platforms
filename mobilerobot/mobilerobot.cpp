#include "mobilerobot.h"


// MobileRobot Functions
void RWA2::MobileRobot::rotate(double angle){
    orientation_+= angle;
}

void RWA2::MobileRobot::move(double distance, double angle){
    for (int i{0}; i < sensors_.size(); i++){
        sensors_[i]->read_data(5);
        // std::cout << "Data from sensor " << i + 1 << ":\n";
        // std::array<double, 50> data = sensors_[i]->get_data();
        // for (int j = 0; j<50; j++){
        // std::cout << data[j] << "\n";
        // }
    }
}

void RWA2::MobileRobot::print_status(){
    std::cout <<  "Position: (" << position_.first << "," << position_.second << ")\n";
    std::cout << "Orientation: " << orientation_ << "\n";
    std::cout << "Speed: " << speed_ << "\n";
    std::cout << "Battery Current Charge: " << battery_.get_current_charge() << "\n";
}

// Sensor Functions
void RWA2::Sensor::read_data(int unsigned duration){
    std::cout << "Sensor " << model_ << " gathering data for " << duration << " seconds." << "\n";
    sleep(duration);

    for (int i{0};i<50; i++ ){
        data_[i] = (rand() % 31);
    }
}

// Battery Functions
void RWA2::Battery::start_charging(){
    if (is_charging_ == false){
        int required_charge = 100 - current_charge_;
        if(model_ == "Li-ion"){
            int charge_time = required_charge / 2;
            std::cout << "Sensor " << model_ << " is charging." << "\n";
            sleep(charge_time);
            current_charge_ = 100;
        }
        if(model_ == "LiFePO4"){
            int charge_time = required_charge / 4;
            std::cout << "Sensor " << model_ << " is charging." << "\n";
            sleep(charge_time);
            current_charge_ = 100;
        }
        stop_charging();
    }
}

void RWA2::Battery::stop_charging(){
    if (current_charge_ == 100){
        is_charging_ = false;
    }
}

void RWA2::Battery::discharge(double amount){
    current_charge_ -= amount;
}

// Aquatic Robot Functions
void RWA2::AquaticRobot::dive(double depth){
    if (is_diving_ == false){
        is_diving_ = true;
        if (has_fins_ == true){
            std::cout << "The Robot " << model_ << " is diving to depth of " << depth << " meters\n";
            std::this_thread::sleep_for(std::chrono::duration<double>(depth/2));
        }
        else{
            std::cout << "The Robot " << model_ << " is diving to depth of " << depth << " meters\n";
            std::this_thread::sleep_for(std::chrono::duration<double>(depth));
        }
        depth_ = depth;
    }
} 

void RWA2::AquaticRobot::surface(){
    if (is_diving_ == true){
        if (has_fins_ == true){
            std::cout << "The robot " << model_ << " is reaching the surface.\n";
            std::this_thread::sleep_for(std::chrono::duration<double>(depth_/4));
        }
        else{
            std::cout << "The robot " << model_ << " is reaching the surface.\n";
            std::this_thread::sleep_for(std::chrono::duration<double>(depth_/2));
        }
        depth_ = 0.0;
        is_diving_ = false;
    }
}
void RWA2::AquaticRobot::rotate(double angle){
    MobileRobot::rotate(angle);
    std::cout << "AquaticRobot::" << model_ << " rotated " << angle << " degrees.\n";
}

void RWA2::AquaticRobot::print_status(){
    MobileRobot::print_status();
    std::cout << "Has Fins: " << std::boolalpha << has_fins_ << "\n";
    std::cout << "Depth: " << depth_ << " meters\n";
    std::cout << "Is Diving: " << std::boolalpha << is_diving_ << "\n"; 

}

void RWA2::AquaticRobot::move(double distance, double angle){
    if (battery_.get_current_charge() < (distance)){
        battery_.start_charging();
    }
    
    MobileRobot::move(distance, angle);
    rotate(angle);
    dive(distance/2);
    surface();
    // battery_.set_current_charge(battery_.get_current_charge() - distance);
    battery_.discharge(distance);
    std::cout << model_ << " reached a depth of " << distance/2 << " meters and then surfaced.\n";
    print_status();
}



// Aerial Robot Functions

void RWA2::AerialRobot::take_off(double altitude){
    if (is_flying_ == false){
        is_flying_ = true;
        if (has_wings_ == true){
            std::cout << "The Robot " << model_ << " is flying an altitude of " << altitude << " meters\n";
            std::this_thread::sleep_for(std::chrono::duration<double>(altitude/3));
            
        }
        else{
            std::cout << "The Robot " << model_ << " is flying an altitude of " << altitude << " meters\n";
            std::this_thread::sleep_for(std::chrono::duration<double>(altitude/1.5));
        }
        altitude_ = altitude;
    }
} 

void RWA2::AerialRobot::land(){
    if (is_flying_ == true){
        if (has_wings_ == true){
            std::cout << "The robot " << model_ << " is landing.\n";
            std::this_thread::sleep_for(std::chrono::duration<double>(altitude_/4));
        }
        else{
            std::cout << "The robot " << model_ << " is landing.\n";
            std::this_thread::sleep_for(std::chrono::duration<double>(altitude_/2));
        }
        altitude_ = 0.0;
        is_flying_ = false;
    }
}

void RWA2::AerialRobot::rotate(double angle){
    MobileRobot::rotate(angle);
    std::cout << "AerialRobot::" << model_ << " rotated " << angle << " degrees.\n";
}

void RWA2::AerialRobot::print_status(){
    MobileRobot::print_status();
    std::cout << "Has Wings: " << std::boolalpha << has_wings_ << "\n";
    std::cout << "Altitude: " << altitude_ << " meters\n";
    std::cout << "Is Flying: " << std::boolalpha << is_flying_ << "\n"; 
}

void RWA2::AerialRobot::move(double distance, double angle){
    if (battery_.get_current_charge() < (2*distance)){
        battery_.start_charging();
    }
    
    MobileRobot::move(distance, angle);
    take_off(distance/2);
    rotate(angle);
    land();
    // battery_.set_current_charge(battery_.get_current_charge() - (2*distance));
    battery_.discharge(2*distance);
    std::cout << model_ << " reached an altitude of " << distance/2 << " meters and then landed.\n";
    print_status();
}

// Wheeled Robot Functions

void RWA2::WheeledRobot::accelerate(double amount){
    std::cout << "Accelerating the robot " << model_ << "\n";
    while(speed_ < desired_speed_ && (desired_speed_ - speed_) > amount){
        speed_ += amount;
        std::this_thread::sleep_for(std::chrono::duration<double>(0.5));
    }
    std::cout << model_ << " has reached the desired speed of " << desired_speed_ << " m/s\n";
}

void RWA2::WheeledRobot::decelerate(double amount){
    std::cout << "Decelerating the robot " << model_ << "\n";
    while(speed_ > 0 && speed_ > amount){
        speed_ -= amount;
        std::this_thread::sleep_for(std::chrono::duration<double>(0.5));
    }
}

void RWA2::WheeledRobot::brake(){
    speed_ = 0;
}

void RWA2::WheeledRobot::rotate(double angle){
    MobileRobot::rotate(angle);
    std::cout << "WheeledRobot::" << model_ << " rotated " << angle << " degrees.\n";
}

void RWA2::WheeledRobot::print_status(){
    MobileRobot::print_status();
    std::cout << "Number of Wheels: " << number_of_wheels_ << "\n";
    std::cout << "Wheel Diameter: " << wheel_diameter_ << "\n";
    std::cout << "Desired Speed: " << desired_speed_ << "\n";
}

void RWA2::WheeledRobot::move(double distance, double angle){
    if (battery_.get_current_charge() < (distance)){
        battery_.start_charging();
    }
    MobileRobot::move(distance, angle);
    rotate(angle);
    accelerate(2);
    std::this_thread::sleep_for(std::chrono::duration<double>(distance-2));
    decelerate(2);
    brake();
    // battery_.set_current_charge(battery_.get_current_charge() - distance); 
    battery_.discharge(distance);
    std::cout << model_ << " drove " << distance << "m.\n";
    print_status();

}

// Legged Robot Functions

void RWA2::LeggedRobot::kick(){
    std::cout << model_ << " kicks with a strength of " << leg_strength_ <<"\n";
}

void RWA2::LeggedRobot::jump(double amount){
    height_ = amount * leg_strength_;
    std::cout << model_ << " jumps at a height of " << height_ << "cm above the ground\n";
}

void RWA2::LeggedRobot::rotate(double angle){
    MobileRobot::rotate(angle);
    std::cout << "LeggedRobot::" << model_ << " rotated " << angle << " degrees.\n";
}

void RWA2::LeggedRobot::print_status(){
    MobileRobot::print_status();
    std::cout << "Height: " << height_ << "cm\n";
    std::cout << "Leg Strength: " << leg_strength_ << "\n";
    std::cout << "Number of Legs: " << number_of_legs_ << "\n";
}

void RWA2::LeggedRobot::move(double distance, double angle){
    if (battery_.get_current_charge() < ((distance*leg_strength_) + leg_strength_)){
        battery_.start_charging();
    }
    MobileRobot::move(distance, angle);
    rotate(angle);
    jump(distance);
    kick();
    // battery_.set_current_charge(battery_.get_current_charge() - ((distance*leg_strength_) + leg_strength_)); 
    battery_.discharge(((distance*leg_strength_) + leg_strength_));
    print_status();

}