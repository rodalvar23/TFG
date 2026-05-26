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
class Manipulator {
public:
    void init_motors(RoboArm *arm, Var_motors *var_m, dynamixel::PortHandler *portHandler, dynamixel::PacketHandler *packetHandler);
    void set_Mode(char mode); // 'v' for Velocity, 'p' for Position
    
    // Command Interfaces
    void set_Velocity_raw(target_velocity vel);
    void set_Position_raw(target_position pos);
    void set_max_Velocity(max_velocity vel_max);
    void set_max_Position(max_position pos_max);
    
    // Telemetry Interfaces
    void read_position();
    void read_velocity();
    void read_temperature();
    void read_current();
    
    // Configuration
    void set_torque_state(bool state);
    void get_torque_state();
    void getIDS(uint8_t *ids[8]);
    void setIDS(uint8_t ids[8]);
};
