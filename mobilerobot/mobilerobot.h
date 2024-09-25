#ifndef __MOBILEROBOT__H__
#define __MOBILEROBOT__H__

#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <iostream>
#include <array>
#include <unistd.h>
#include <cmath>
#include <thread>

namespace RWA2{

    // Composition relationship between Battery and MobileRobot
    class Battery{
        public:
            // Constructor
            Battery(std::string model, int current_charge=100, bool is_charging=false): model_{model}, current_charge_{current_charge}, is_charging_{is_charging}{
                std::cout << "Class Battery is initialized" << "\n";
            }

            // methods
            void start_charging();          // starts charging
            void discharge(double amount);      // reduces the current_charge_ attribute according to the amount
            
        private:
            // attributes
            std::string model_;     // Battery model name
            int current_charge_;    // current charge of the battery
            bool is_charging_;  

            // methods
            void stop_charging();   // stops charging


    }; // class Battery

    // Aggregation relationship between Sensor and MobileRobot
    class Sensor{
        public:         
            // Constructor
            Sensor(std::string model): model_{model}{ 
                std::cout << "Class Sensor is initialized" << "\n";
            }

            // methods
            void read_data(int unsigned duration);  // To gather data emulating sensor data acquisition
            std::array<double, 50> get_data(){return data_;}    // getter for accessing the gathered sensor data

        private:
            // attributes
            std::string model_;     // Sensor model name
            std::array<double,50> data_;    //  gathered data

    };  // Class Sensor
    
    // Parent class (base class)
    class MobileRobot{
        public:
            // Constructor
            MobileRobot(std::string model,RWA2::Battery battery, std::vector<std::unique_ptr<RWA2::Sensor>>&& sensors, std::pair<double, double> position= std::make_pair(0.0,0.0), double orientation=0.0 )
            : model_{model}, battery_{std::move(battery)}, sensors_{std::move(sensors)},position_{position}, orientation_{orientation}{
                std::cout << "Class MobileRobot initialized" << "\n";
            }

            // methods
            virtual void move(double distance, double angle);   // move the vehicle (overriden according to the child classes)
            virtual void print_status();    // prints all the attributes of the class

        protected:
            // attributes
            std::pair<double,double> position_; 
            double orientation_;
            double speed_= 0.0;
            RWA2::Battery battery_;
            std::vector<std::unique_ptr<RWA2::Sensor>> sensors_;    // Sensors used 
            std::string model_;
            
            // methods
            virtual void rotate(double angle);      // rotate the robot


    };  // class MobileRobot

    class AquaticRobot: public MobileRobot{
        
        public:

            AquaticRobot(std::string model,RWA2::Battery battery, std::vector<std::unique_ptr<RWA2::Sensor>>&& sensors, std::pair<double, double> position= std::make_pair(0.0,0.0), double orientation=0.0 ): MobileRobot(model, std::move(battery), std::move(sensors), position, orientation){
                if (model_ == "SoFi"){
                    has_fins_ = true;
                }
                else
                    has_fins_ = false;   
            }   // Constructor

            // methods
            void move(double distance, double angle);
            void print_status();
        
        protected:
            // methods
            void rotate(double angle);
        
        private:
            // attributes
            bool has_fins_;
            double depth_=0.0;
            bool is_diving_=false;

            // methods
            void dive(double depth);
            void surface();

    };

    class AerialRobot: public MobileRobot{
        public:

            AerialRobot(std::string model,RWA2::Battery battery, std::vector<std::unique_ptr<RWA2::Sensor>>&& sensors, std::pair<double, double> position= std::make_pair(0.0,0.0), double orientation=0.0 ): MobileRobot(model, std::move(battery), std::move(sensors), position, orientation){
                if (model_ == "Erle-Plane"){
                    has_wings_ = true;
                }
                else
                    has_wings_ = false;   
            }   // Constructor


            // methods
            void move(double distance, double angle);
            void print_status();
        
        protected:
            // methods
            void rotate(double angle);
        
        private:
            // attributes
            bool has_wings_;
            double altitude_=0.0;
            bool is_flying_=false;

            // methods
            void take_off(double altitude);
            void land();
    };

    class WheeledRobot: public MobileRobot{
        public:

            WheeledRobot(std::string model,RWA2::Battery battery, std::vector<std::unique_ptr<RWA2::Sensor>>&& sensors, std::pair<double, double> position= std::make_pair(0.0,0.0), double orientation=0.0 ): MobileRobot(model, std::move(battery), std::move(sensors), position, orientation){
                if (model_ == "Turtlebot"){
                    number_of_wheels_ = 2;
                }
                if (model_ == "Hiwonder"){
                    number_of_wheels_ = 4;
                }
                if (model_ == "Dagu"){
                    number_of_wheels_ = 6;
                }

            }

            // methods
            void move(double distance, double angle);
            void print_status();
        
        protected:
            // methods
            void rotate(double angle);

        private:
            // attributes
            int number_of_wheels_=2;
            double wheel_diameter_=20;
            double desired_speed_=20;

            // methods
            void accelerate(double amount);
            void decelerate(double amount);
            void brake();
    };

    class LeggedRobot: public MobileRobot{
        public:

            LeggedRobot(std::string model,RWA2::Battery battery, std::vector<std::unique_ptr<RWA2::Sensor>>&& sensors, std::pair<double, double> position= std::make_pair(0.0,0.0), double orientation=0.0 ): MobileRobot(model, std::move(battery), std::move(sensors), position, orientation){
                if (model_ == "Atlas"){
                    number_of_legs_ = 2;
                }
                if (model_ == "Spot"){
                    number_of_legs_ = 4;
                }
                if (model == "Mx-Phoenix"){
                    number_of_legs_ = 6;
                }
            }

            void move(double distance, double angle);
            void print_status();
        
        protected:
            void rotate(double angle);
        
        private:
            // attributes
            double height_;
            int leg_strength_=5;
            int number_of_legs_=2;

            // methods
            void kick();
            void jump(double amount);  
    };

} // namespace RWA2



#endif