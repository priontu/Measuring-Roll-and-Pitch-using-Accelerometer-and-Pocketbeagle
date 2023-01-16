#include <iostream>
#include <unistd.h>
#include <fstream>
#include <thread>
#include "ADXL345.h"
#include "LED.h"

using namespace std;
using namespace exploringBB;

class accelero{

    private:
        LED led[4] = {LED(0), LED(1), LED(2), LED(3)};
        int num_leds = 4;
        ADXL345 sensor = ADXL345(1, 0x53);

        float zero_level_pitch;
        float zero_level_roll;

        float pitch;
        float roll;

    public:
        accelero(){
            sensor.setResolution(ADXL345::NORMAL);
            sensor.setRange(ADXL345::PLUSMINUS_4_G);

            remove_all_trigger();


            validate_sensor_state(sensor);

            zero_level_pitch = sensor.getPitch();
            zero_level_roll = sensor.getRoll();

            pitch = 0;
            roll = 0;

        }

        void sleep(float time){
            usleep(time * 1000000);
        }

        void validate_sensor_state(ADXL345 &sensor){
            int sensor_state = sensor.readSensorState();

            if (sensor_state == -1){
                cout << "Sensor State is -1 because device ID (of accelerometer) is incorrect." << endl;
                exit(0);
            }
        }

        void remove_all_trigger(){
            for (int i = 0; i < num_leds; i++){
                led[i].removeTrigger();
            }
        }

        void blink_pattern(int i, float sleep_time){
            led[i].turnOn();
            this -> sleep(sleep_time);
            led[i].turnOff();
            this -> sleep(sleep_time);
        }

        // void eval_pitch(){
        //     int i = 1;
        //     int j = 2;
        //     int value = pitch;

        //     while (true){
        //         value = pitch;
        //         if (value <= 30 && value >= 10 ){
        //             remove_all_trigger();
        //             this->blink_pattern(i, 1);
        //         }

        //         if (value > 30){
        //             remove_all_trigger();
        //             this -> blink_pattern(i, 0.005);
        //         }

        //         if (value <= -10 && value >= -30){
        //             remove_all_trigger();
        //             this -> blink_pattern(j, 1);
        //         }

        //         if (value < -30){
        //             remove_all_trigger();
        //             this -> blink_pattern(j, 0.005);
        //         }

        //         if (value > -10 && value < 10){
        //             remove_all_trigger();
        //         }    
        //     }
        // }

        // void eval_roll(){
        //     int i = 0;
        //     int j = 3;
        //     int value = roll;

        //     while (true){
        //         value = roll;
        //         if (value <= 30 && value >= 10 ){
        //             remove_all_trigger();
        //             this->blink_pattern(i, 1);
        //         }

        //         if (value > 30){
        //             remove_all_trigger();
        //             this -> blink_pattern(i, 0.005);
        //         }

        //         if (value <= -10 && value >= -30){
        //             remove_all_trigger();
        //             this -> blink_pattern(j, 1);
        //         }

        //         if (value < -30){
        //             remove_all_trigger();
        //             this -> blink_pattern(j, 0.005);
        //         }

        //         if (value > -10 && value < 10){
        //             remove_all_trigger();
        //         }    
        //     }
        // }


        void evaluate_param(string param, float &value){    //General function for reaxting to changes in pitch and roll.
                int i, j;
                if (param == "pitch"){
                    i = 1;
                    j = 2;
                }
                
                if (param == "roll"){
                    i = 0;
                    j = 3;
                }

                float small_blink_time = 0.5;
                float large_blink_time = 1.5;

                while (true){
                    if (value <= 30 && value >= 10 ){
                        remove_all_trigger();
                        this->blink_pattern(i, large_blink_time);
                    }

                    if (value > 30){
                        remove_all_trigger();
                        this -> blink_pattern(i, small_blink_time);
                    }

                    if (value <= -10 && value >= -30){
                        remove_all_trigger();
                        this -> blink_pattern(j, large_blink_time);
                    }

                    if (value < -30){
                        remove_all_trigger();
                        this -> blink_pattern(j, small_blink_time);
                    }

                    if (value > -10 && value < 10){
                        remove_all_trigger();
                    }    
                }
        }

        void eval_pitch(){  // pitch value fed to general function for reaction on pitch.
            evaluate_param("pitch", pitch);
        }

        void eval_roll(){   //roll value fed to general function for reaction on roll.
            evaluate_param("roll", roll);
        }

        void read_accelerometer(){  // This thread continuously reads pitch and roll values.
            
            while (true){
                validate_sensor_state(sensor);

                usleep(1000000);

                pitch = sensor.getPitch() - zero_level_pitch;
                roll = sensor.getRoll() - zero_level_roll;

                cout << "Pitch: " << pitch << "Roll: " << roll << "\r" << flush;

                // evaluate_param("pitch", pitch);
                // evaluate_param("roll", roll);
                
            }
        }

        void run_threads(){
            thread th1 = thread(&accelero::read_accelerometer, this);
            thread th2 = thread(&accelero::eval_pitch, this);
            thread th3 = thread(&accelero::eval_roll, this);

            th1.join();
            th2.join();
            th3.join();
        }
};

int main(){
    accelero item;
    item.remove_all_trigger();
    item.run_threads();
}