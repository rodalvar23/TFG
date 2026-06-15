# Manipulator Control Library

A lightweight, high-performance C++ control library designed for a heterogeneous 8-DOF robotic manipulator. This library provides a unified interface to synchronously control and monitor an arm assembly comprising **Rozum Servo Drives** (base/position stages) and **Dynamixel Actuators** (claw/orientation stages). 

By focusing on direct joint-space velocity and position control, this architecture avoids heavy kinematic overhead, making it ideal for direct sequencing, teleoperation, and responsive real-time joint manipulation.

---

## Key Features

- **Heterogeneous Hardware Integration:** Seamless abstraction layer combining CAN-bus Rozum actuators and serial-based Dynamixel servos under a single `Manipulator` interface.
- **Dual Control Modes:** Native support for both Raw Velocity Control (`set_Velocity_raw`) and Raw Position Control (`set_Position_raw`).
- **Synchronous Dynamixel Communication:** Utilizes the Robotis `GroupSyncWrite` and `GroupSyncRead` protocols to command and query multiple Dynamixel servos simultaneously, minimizing latency and maximizing bus efficiency.
- **Hardware-Level Optimization:** Implements parameter caching for Rozum servos (`rr_param_cache`) to optimize continuous telemetry queries and prevent CAN-bus saturation.
- **Embedded Safety Fail-safes:** Built-in software position limiters and velocity clamping routines guard against over-travel, mechanical collisions, and current surges.
- **Comprehensive Telemetry:** Real-time monitoring of joint positions, velocities, temperatures, and electrical current consumption.

---

## Hardware Architecture & Mapping

The library orchestrates 8 independent degrees of freedom, partitioned into position and orientation groups:

### 1. Main Arm Base (Position Joints)
Driven by **Rozum Servo Drives** via a USB-to-CAN interface (`/dev/rozum_api`).
- **Motor 1 (ID: 123):** Base / Shoulder Joint
- **Motor 2 (ID: 124):** Elbow Joint
- **Motor 3 (ID: 125):** Forearm / Wrist Pitch

### 2. Robotic Claw & Wrist (Orientation Joints)
Driven by **Dynamixel Actuators** using the Robotis Protocol 2.0 via a shared serial bus (configured at 57,600 bps).
- **Motor 4 (ID: 3):** Wrist Roll
- **Motor 5 (ID: 1):** Wrist Traslation
- **Motor 6 (ID: 5):** Wrist Roll (Only Claw)
- **Motor 7 (ID: 2):** Claw Gripper Left
- **Motor 8 (ID: 12):** Claw Gripper Right

---

## Code Architecture Overview

### Core Class: `Manipulator`
The primary controller class managing hardware handlers, initialization sequences, command distributions, and telemetry collection loops.

```cpp
// Principal control and telemetry interface
class Manipulator
{   

    private:
    std::thread hilo_dynamixel;
    std::mutex mtx_sincronizacion;
    std::condition_variable cv_iniciar_tarea_;
    std::condition_variable cv_tarea_terminada_;
    
    // Aquí guardamos la orden actual
    tarea_dynamixel comando_actual_ = tarea_dynamixel::SLEEPING;
    bool tarea_completada_ = false;

    void gestor_tareas();

    //Variables publicas
    public:
        RoboArm arm;
        Var_motors var_m;
        dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler("/dev/u2d2_dyn"); // your dxl port name;
        dynamixel::PacketHandler *packetHandler  = dynamixel::PacketHandler::getPacketHandler(2.0); //protocol version;
        vec_motors max_pos,vel,pos;
        
        public:
        //Constructor
        Manipulator() = default;
        //Destructor
        ~Manipulator()
        {
            
        }
        //Funciones
        void init_motors(RoboArm *arm,Var_motors *var_m,dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler);
        void set_max_Velocity(vec_motors vel_max);
        void set_max_Position(vec_motors pos_max);
        void set_velocity_claw();
        void set_velocity_raw();
        void set_position_claw();
        void set_position_raw();
        void set_Mode(char mode);
        void getIDS(int ids[8]);
        void setIDS(int ids[8]);
        void set_torque_state(bool state);
        void get_torque_state();
        void full_manual_mode();
        void read_temperature();
        void read_velocity();
        void read_position();
        void read_current();
        void init();
        void deinit();

  };
