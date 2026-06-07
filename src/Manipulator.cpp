#include "../include/robotic_arm.h"








/**
 * @brief Populates a float array with specific motor data based on the selected mode.
 * @param var Pointer to the structure containing all motor variables.
 * @param vec Output array to store the requested data.
 * @param select Char indicating the data type: 'v' (velocity), 'p' (position), 't' (temperature), 'c' (current).
 */
void fill(Var_motors *var, vec_motors vec, char select)
{
  switch (select)
  {
  case 'v':
    vec[0] = var->v.motor_1;
    vec[1] = var->v.motor_2;
    vec[2] = var->v.motor_3;
    vec[3] = var->v.motor_4;
    vec[4] = var->v.motor_5;
    vec[5] = var->v.motor_6;
    vec[6] = var->v.motor_7;
    vec[7] = var->v.motor_8;
    break;
  case 'p':
    vec[0] = var->p.motor_1;
    vec[1] = var->p.motor_2;
    vec[2] = var->p.motor_3;
    vec[3] = var->p.motor_4;
    vec[4] = var->p.motor_5;
    vec[5] = var->p.motor_6;
    vec[6] = var->p.motor_7;
    vec[7] = var->p.motor_8;
    break;
  case 't':
    vec[0] = var->t.motor_1;
    vec[1] = var->t.motor_2;
    vec[2] = var->t.motor_3;
    vec[3] = var->t.motor_4;
    vec[4] = var->t.motor_5;
    vec[5] = var->t.motor_6;
    vec[6] = var->t.motor_7;
    vec[7] = var->t.motor_8;
    break;
  case 'c':
    vec[0] = var->c.motor_1;
    vec[1] = var->c.motor_2;
    vec[2] = var->c.motor_3;
    vec[3] = var->c.motor_4;
    vec[4] = var->c.motor_5;
    vec[5] = var->c.motor_6;
    vec[6] = var->c.motor_7;
    vec[7] = var->c.motor_8;
    break;
  default:
    break;
  }
}


/**
 * @brief Sets the maximum allowable velocity for all motors.
 * Applies clamping to ensure values stay within safe hardware limits for both 
 * Rozum arm motors (0-2) and Dynamixel claw motors (3-7).
 * @param vel_max Array containing the target maximum velocities for the 8 motors.
 */
void Manipulator :: set_max_Velocity(vec_motors vel_max)
{
  result res_claw;
  rr_ret_status_t res_arm_1;
  rr_ret_status_t res_arm_2;
  rr_ret_status_t res_arm_3;
  float conversion = 1/0.229;
  for (int i = 0; i < 8; i++)
  {
    if(i < 3) //Arm motors limits
    {
      if(vel_max[i] > 55)
      {
        vel_max[i] = 55;
      }
      else if(vel_max[i] < -55)
      {
        vel_max[i] = -55;
      }
    }
    else //Claw motors limits
    {
      if(vel_max[i] > 233)
      {
        vel_max[i] = 233;
      }
      else if(vel_max[i] < -233)
      {
        vel_max[i] = -233;
      }
    }
  }
  
  //Sets limits for Rozum motors
  res_arm_1 = rr_set_max_velocity(arm.motor_1,vel_max[0]*60);
  if(res_arm_1 != RET_OK)
  {
    fprintf(stderr,"Fail to set max velocity in motor 1");
    exit(1);
  }
  rr_set_max_velocity(arm.motor_2,vel_max[1]*60);
  if(res_arm_2 != RET_OK)
  {
    fprintf(stderr,"Fail to set max velocity in motor 2");
    exit(1);
  }
  rr_set_max_velocity(arm.motor_3,vel_max[2]*60);
  if(res_arm_3 != RET_OK)
  {
    fprintf(stderr,"Fail to set max velocity in motor 3");
    exit(1);
  }

  //Sets limits for Dynamixel motors
  res_claw[0] = packetHandler->write4ByteTxRx(portHandler,ID_3, VELOCITY_LIMIT, (int)vel_max[3]*conversion);
  if(res_claw[0] != 0)
  {
    fprintf(stderr,"Fail to set max velocity in motor 4");
    exit(1);
  }
  
  res_claw[1] = packetHandler->write4ByteTxRx(portHandler,ID_1, VELOCITY_LIMIT, (int)vel_max[4]*conversion);
  if(res_claw[1] != 0)
  {
    fprintf(stderr,"Fail to set max velocity in motor 5");
    exit(1);
  }

  res_claw[2] = packetHandler->write4ByteTxRx(portHandler,ID_5, VELOCITY_LIMIT, (int)vel_max[5]*conversion);
  if(res_claw[2] != 0)
  {
    fprintf(stderr,"Fail to set max velocity in motor 6");
    exit(1);
  }

  res_claw[3] = packetHandler->write4ByteTxRx(portHandler,ID_2, VELOCITY_LIMIT, (int)vel_max[6]*conversion);
  if(res_claw[3] != 0)
  {
    fprintf(stderr,"Fail to set max velocity in motor 7");
    exit(1);
  }
  
  res_claw[4] = packetHandler->write4ByteTxRx(portHandler,ID_12, VELOCITY_LIMIT, (int)vel_max[7]*conversion);
  if(res_claw[4] != 0)
  {
    fprintf(stderr,"Fail to set max velocity in motor 8");
    exit(1);
  }

}


/**
 * @brief Sets the maximum allowable position limits for the Dynamixel claw motors.
 * @param pos_max Array containing the target maximum positions.
 */
void Manipulator :: set_max_Position(vec_motors pos_max)
{
  result res_claw;
  float conversion = 1/0.088;

  res_claw[0] = packetHandler->write4ByteTxRx(portHandler,ID_1, POSITION_LIMIT, (int)pos_max[3]*conversion);
  if(res_claw[0] != 0)
  {
    fprintf(stderr,"Fail to set max position in motor 4");
    exit(1);
  }
  
  res_claw[1] = packetHandler->write4ByteTxRx(portHandler,ID_3, POSITION_LIMIT, (int)pos_max[4]*conversion);
  if(res_claw[1] != 0)
  {
    fprintf(stderr,"Fail to set max position in motor 5");
    exit(1);
  }

  res_claw[2] = packetHandler->write4ByteTxRx(portHandler,ID_5, POSITION_LIMIT, (int)pos_max[5]*conversion);
  if(res_claw[2] != 0)
  {
    fprintf(stderr,"Fail to set max position in motor 6");
    exit(1);
  }

  res_claw[3] = packetHandler->write4ByteTxRx(portHandler,ID_2, POSITION_LIMIT, (int)pos_max[6]*conversion);
  if(res_claw[3] != 0)
  {
    fprintf(stderr,"Fail to set max position in motor 7");
    exit(1);
  }

  res_claw[4] = packetHandler->write4ByteTxRx(portHandler,ID_12, POSITION_LIMIT, (int)pos_max[7]*conversion);
  if(res_claw[4] != 0)
  {
    fprintf(stderr,"Fail to set max position in motor 8");
    exit(1);
  }

}


/**
 * @brief Initializes the Dynamixel motors for the robotic claw.
 * Opens the serial port, sets the baud rate, configures motors to velocity mode, and enables torque.
 */
void init_claw_motors(dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler)
{

  result res_claw_0;
  result res_claw_1;
  bool baud,port;

  //Open the port
  port = portHandler->openPort();
  if(!port)
  {
    fprintf(stderr,"Fail to open port");
    exit(1);
  }
  //Set baudrate
  baud = portHandler->setBaudRate(57600);
  if(!baud)
  {
    fprintf(stderr,"Fail to set baudrate");
    exit(1);
  }
  // Configure operating mode and enable torque for each motor
  
  //id 1
  res_claw_0[1] = packetHandler->write1ByteTxRx(portHandler, ID_1, OPERATING_MODE, VELOCITY_MODE);
  if(res_claw_0[1] != 0)
  {
    fprintf(stderr,"Fail to change operating mode of motor 5");
    exit(1);
  }
  res_claw_1[1] = packetHandler->write1ByteTxRx(portHandler, ID_1, TORQUE_ADDRESS, ON);
  if(res_claw_1[1] != 0)
  {
    fprintf(stderr,"Fail to activate torque of motor 5");
    exit(1);
  }

  //id 2
  res_claw_0[3] = packetHandler->write1ByteTxRx(portHandler, ID_2, OPERATING_MODE, VELOCITY_MODE);
  if(res_claw_0[3] != 0)
  {
    fprintf(stderr,"Fail to change operating mode of motor 7");
    exit(1);
  }
  res_claw_1[3] = packetHandler->write1ByteTxRx(portHandler, ID_2, TORQUE_ADDRESS, ON);
  if(res_claw_1[3] != 0)
  {
    fprintf(stderr,"Fail to activate torque of motor 7");
    exit(1);
  }

  //id 3
  res_claw_0[0] = packetHandler->write1ByteTxRx(portHandler, ID_3, OPERATING_MODE, VELOCITY_MODE);
  if(res_claw_0[0] != 0)
  {
    fprintf(stderr,"Fail to change operating mode of motor 4");
    exit(1);
  }
  res_claw_1[0] = packetHandler->write1ByteTxRx(portHandler, ID_3, TORQUE_ADDRESS, ON);
  if(res_claw_1[0] != 0)
  {
    fprintf(stderr,"Fail to activate torque of motor 4");
    exit(1);
  }

  //id 5
  res_claw_0[2] = packetHandler->write1ByteTxRx(portHandler, ID_5, OPERATING_MODE, VELOCITY_MODE);
  if(res_claw_0[2] != 0)
  {
    fprintf(stderr,"Fail to change operating mode of motor 6");
    exit(1);
  }
  res_claw_1[2] = packetHandler->write1ByteTxRx(portHandler, ID_5, TORQUE_ADDRESS, ON);
  if(res_claw_1[2] != 0)
  {
    fprintf(stderr,"Fail to activate torque of motor 6");
    exit(1);
  }

  //id 12
  res_claw_0[4] = packetHandler->write1ByteTxRx(portHandler, ID_12, OPERATING_MODE, VELOCITY_MODE);
  if(res_claw_0[4] != 0)
  {
    fprintf(stderr,"Fail to change operating mode of motor 8");
    exit(1);
  }
  res_claw_1[4] = packetHandler->write1ByteTxRx(portHandler, ID_12, TORQUE_ADDRESS, ON);
  if(res_claw_0[4] != 0)
  {
    fprintf(stderr,"Fail to change operating mode of motor 8");
    exit(1);
  }

}


/**
 * @brief Enables or disables the torque for all Dynamixel claw motors.
 * @param state Boolean value to set the torque state (true for ON, false for OFF).
 */
void Manipulator :: set_torque_state(bool state)
{
  result res_torq;
  //id 1
  res_torq[1] = packetHandler->write1ByteTxRx(portHandler, ID_1, TORQUE_ADDRESS, state);
  if(res_torq[1] != 0)
  {
    fprintf(stderr,"Fail to set torque of motor 5");
    exit(1);
  }
  //id 2
  res_torq[3] = packetHandler->write1ByteTxRx(portHandler, ID_2, TORQUE_ADDRESS, state);
  if(res_torq[3] != 0)
  {
    fprintf(stderr,"Fail to set torque of motor 7");
    exit(1);
  }
  //id 3
  res_torq[0] = packetHandler->write1ByteTxRx(portHandler, ID_3, TORQUE_ADDRESS, state);
  if(res_torq[0] != 0)
  {
    fprintf(stderr,"Fail to set torque of motor 4");
    exit(1);
  }
  //id 5
  res_torq[2] = packetHandler->write1ByteTxRx(portHandler, ID_5, TORQUE_ADDRESS, state);
  if(res_torq[2] != 0)
  {
    fprintf(stderr,"Fail to set torque of motor 6");
    exit(1);
  }
  //id 12
  res_torq[4] = packetHandler->write1ByteTxRx(portHandler, ID_12, TORQUE_ADDRESS, state);
  if(res_torq[1] != 0)
  {
    fprintf(stderr,"Fail to set torque of motor 8");
    exit(1);
  }
}


/**
 * @brief Reads the current torque enabled status from all Dynamixel claw motors.
 */
void Manipulator :: get_torque_state()
{

  uint8_t ids[5];
  result res_torq;
  //id 1
  res_torq[1] = packetHandler->read1ByteTxRx(portHandler, ID_1, TORQUE_ADDRESS, &ids[0]);
  if(res_torq[1] != 0)
  {
    fprintf(stderr,"Fail to get torque of motor 5");
    exit(1);
  }
  //id 2
  res_torq[3] = packetHandler->read1ByteTxRx(portHandler, ID_2, TORQUE_ADDRESS, &ids[1]);
  if(res_torq[3] != 0)
  {
    fprintf(stderr,"Fail to get torque of motor 7");
    exit(1);
  }
  //id 3
  res_torq[0] = packetHandler->read1ByteTxRx(portHandler, ID_3, TORQUE_ADDRESS, &ids[2]);
  if(res_torq[0] != 0)
  {
    fprintf(stderr,"Fail to get torque of motor 4");
    exit(1);
  }
  //id 5
  res_torq[2] = packetHandler->read1ByteTxRx(portHandler, ID_5, TORQUE_ADDRESS, &ids[3]);
  if(res_torq[2] != 0)
  {
    fprintf(stderr,"Fail to get torque of motor 3");
    exit(1);
  }
  //id 12
  res_torq[4] = packetHandler->read1ByteTxRx(portHandler, ID_12, TORQUE_ADDRESS, &ids[4]);
  if(res_torq[4] != 0)
  {
    fprintf(stderr,"Fail to get torque of motor 8");
    exit(1);
  }
  for(int i=0; i < 5; i++)
  {
    printf("Motor %d torque mode %d\n",i+4,(int)ids[i]);
  }
}


/**
 * @brief Retrieves the hardware IDs from the Dynamixel motors.
 */
void get_id_claw(dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler, ids_claw &id)
{
  result res_id;
  //id 3
  res_id[0] =  packetHandler->read1ByteTxRx(portHandler, ID_3,7, &id[0]);
  if(res_id[0] != 0)
  {
    fprintf(stderr,"Fail to get id of motor 4");
    exit(1);
  }
  
  //id 1
  res_id[1] = packetHandler->read1ByteTxRx(portHandler, ID_1, 7, &id[1]);
  if(res_id[1] != 0)
  {
    fprintf(stderr,"Fail to get id of motor 5");
    exit(1);
  }
  
  //id 5
  res_id[2] =  packetHandler->read1ByteTxRx(portHandler, ID_5, 7, &id[2]);
  if(res_id[2] != 0)
  {
    fprintf(stderr,"Fail to get id of motor 6");
    exit(1);
  }
  
  //id 12
  res_id[4] =  packetHandler->read1ByteTxRx(portHandler, ID_12, 7, &id[4]);
  if(res_id[4] != 0)
  {
    fprintf(stderr,"Fail to get id of motor 8");
    exit(1);
  }
  //id 2
  res_id[3] =  packetHandler->read1ByteTxRx(portHandler, ID_2, 7, &id[3]);
  if(res_id[3] != 0)
  {
    fprintf(stderr,"Fail to get id of motor 7");
    exit(1);
  }
}


/**
 * @brief Writes new hardware IDs to the Dynamixel motors.
 */
void set_id_claw(dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler, ids_claw id)
{
  result res_id;
  //id 1
  res_id[1] = packetHandler->write1ByteTxRx(portHandler, ID_1, 7, id[1]);
  if(res_id[1] != 0)
  {
    fprintf(stderr,"Fail to set id of motor 5");
    exit(1);
  }
  //id 2
  res_id[3] = packetHandler->write1ByteTxRx(portHandler, ID_2, 7, id[3]);
  if(res_id[3] != 0)
  {
    fprintf(stderr,"Fail to set id of motor 7");
    exit(1);
  }
  //id 3
  res_id[0] = packetHandler->write1ByteTxRx(portHandler, ID_3,7, id[0]);
  if(res_id[0] != 0)
  {
    fprintf(stderr,"Fail to set id of motor 4");
    exit(1);
  }
  //id 5
  res_id[2] = packetHandler->write1ByteTxRx(portHandler, ID_5, 7, id[2]);
  if(res_id[2] != 0)
  {
    fprintf(stderr,"Fail to set id of motor 6");
    exit(1);
  }
  //id 12
  res_id[4] = packetHandler->write1ByteTxRx(portHandler, ID_12, 7, id[4]);
  if(res_id[4] != 0)
  {
    fprintf(stderr,"Fail to set id of motor 8");
    exit(1);
  }
}





/**
 * @brief Commands velocities to the entire manipulator.
 * Implements software position limits for the Rozum arm motors to prevent over-travel.
 * Uses GroupSyncWrite for simultaneous command execution on Dynamixel motors.
 * @param vel Target velocity array for all 8 motors.
 */
void Manipulator :: set_velocity_claw()
{
    dynamixel::GroupSyncWrite groupSyncWrite(portHandler, packetHandler, GOAL_VELOCITY_ADDRESS, 4);
    uint8_t param_goal_velocity1[4];
    uint8_t param_goal_velocity3[4];
    uint8_t param_goal_velocity5[4];
    uint8_t param_goal_velocity2[4];
    uint8_t param_goal_velocity12[4];
    result res_claw;
    int tx;
    float conversion = 1/0.229;
    

    // --- Dynamixel Motors GroupSyncWrite Preparation ---
    // Splitting velocity values into 4-byte arrays
    param_goal_velocity1[0] = DXL_LOBYTE(DXL_LOWORD(vel[4]));
    param_goal_velocity1[1] = DXL_HIBYTE(DXL_LOWORD(vel[4]));
    param_goal_velocity1[2] = DXL_LOBYTE(DXL_HIWORD(vel[4]));
    param_goal_velocity1[3] = DXL_HIBYTE(DXL_HIWORD(vel[4]));
    
    param_goal_velocity3[0] = DXL_LOBYTE(DXL_LOWORD(vel[3]));
    param_goal_velocity3[1] = DXL_HIBYTE(DXL_LOWORD(vel[3]));
    param_goal_velocity3[2] = DXL_LOBYTE(DXL_HIWORD(vel[3]));
    param_goal_velocity3[3] = DXL_HIBYTE(DXL_HIWORD(vel[3]));

    param_goal_velocity5[0] = DXL_LOBYTE(DXL_LOWORD(vel[5]));
    param_goal_velocity5[1] = DXL_HIBYTE(DXL_LOWORD(vel[5]));
    param_goal_velocity5[2] = DXL_LOBYTE(DXL_HIWORD(vel[5]));
    param_goal_velocity5[3] = DXL_HIBYTE(DXL_HIWORD(vel[5]));


    param_goal_velocity2[0] = DXL_LOBYTE(DXL_LOWORD(vel[6]));
    param_goal_velocity2[1] = DXL_HIBYTE(DXL_LOWORD(vel[6]));
    param_goal_velocity2[2] = DXL_LOBYTE(DXL_HIWORD(vel[6]));
    param_goal_velocity2[3] = DXL_HIBYTE(DXL_HIWORD(vel[6]));
    
    param_goal_velocity12[0] = DXL_LOBYTE(DXL_LOWORD(vel[7]));
    param_goal_velocity12[1] = DXL_HIBYTE(DXL_LOWORD(vel[7]));
    param_goal_velocity12[2] = DXL_LOBYTE(DXL_HIWORD(vel[7]));
    param_goal_velocity12[3] = DXL_HIBYTE(DXL_HIWORD(vel[7]));
    
    res_claw[1] = groupSyncWrite.addParam(ID_1, param_goal_velocity1);
    if(res_claw[1] == 0)
    {
      fprintf(stderr,"Fail to set velocity of motor 5");
      exit(1);
    }
    res_claw[0] = groupSyncWrite.addParam(ID_3, param_goal_velocity3);
    if(res_claw[0] == 0)
    {
      fprintf(stderr,"Fail to set velocity of motor 4");
      exit(1);
    }
    res_claw[2] = groupSyncWrite.addParam(ID_5, param_goal_velocity5);
    if(res_claw[2] == 0)
    {
      fprintf(stderr,"Fail to set velocity of motor 6");
      exit(1);
    }
    res_claw[3] = groupSyncWrite.addParam(ID_2, param_goal_velocity2);
    if(res_claw[3] == 0)
    {
      fprintf(stderr,"Fail to set velocity of motor 7");
      exit(1);
    }
    res_claw[4] = groupSyncWrite.addParam(ID_12, param_goal_velocity12);
    if(res_claw[4] == 0)
    {
      fprintf(stderr,"Fail to set velocity of motor 8");
      exit(1);
    }
    tx = groupSyncWrite.txPacket();
    if(tx == COMM_NOT_AVAILABLE)
    {
      fprintf(stderr,"Fail to send the package");
      exit(1);
    }
    groupSyncWrite.clearParam();
}


void Manipulator :: set_velocity_raw()
{
    float vel_arm[3];
    float max_position[3];
    vel_arm[0] = vel[0];
    vel_arm[1] = vel[1];
    vel_arm[2] = vel[2];
    max_position[0] = max_pos[0];
    max_position[1] = max_pos[1];
    max_position[2] = max_pos[2];
  {
        std::lock_guard<std::mutex> lock(mtx_sincronizacion);
        comando_actual_ = tarea_dynamixel:: SEND_VELOCITY;
        tarea_completada_ = false;
  }
  cv_iniciar_tarea_.notify_one();
  set_velocity_arm(&arm,&var_m,vel_arm,max_position);
  {
        std::unique_lock<std::mutex> lock(mtx_sincronizacion);
        cv_tarea_terminada_.wait(lock, [this]{ return tarea_completada_; });
  }
}

/**
 * @brief Reads current position from all Dynamixel claw motors via GroupSyncRead.
 */
void read_pos_claw(Var_motors *var_m,dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler)
{
  dynamixel::GroupSyncRead groupSyncRead(portHandler, packetHandler, PRESENT_POSITION_ADDRESS, 4);
  result res_temp;
  float conversion = 0.087891;
  int rx;
  res_temp[1] = groupSyncRead.addParam(ID_1);
  if(res_temp[1] == 0)
  {
    fprintf(stderr,"Fail to read position of motor 5");
    exit(1);
  }
  res_temp[0] = groupSyncRead.addParam(ID_3);
  if(res_temp[0] == 0)
  {
    fprintf(stderr,"Fail to read position of motor 4");
    exit(1);
  }
  res_temp[2] = groupSyncRead.addParam(ID_5);
  if(res_temp[2] == 0)
  {
    fprintf(stderr,"Fail to read position of motor 6");
    exit(1);
  }
  res_temp[3] = groupSyncRead.addParam(ID_2);
  if(res_temp[3] == 0)
  {
    fprintf(stderr,"Fail to read position of motor 7");
    exit(1);
  }
  res_temp[4] = groupSyncRead.addParam(ID_12);
  if(res_temp[4] == 0)
  {
    fprintf(stderr,"Fail to read position of motor 8");
    exit(1);
  }
  rx = groupSyncRead.txRxPacket();
  if(rx == COMM_NOT_AVAILABLE)
  {
    fprintf(stderr,"Fail to receive the package of positions readings");
    exit(1);
  }
  var_m->p.motor_4 = (int32_t)groupSyncRead.getData(ID_3,PRESENT_POSITION_ADDRESS,4)*conversion;
  var_m->p.motor_5 = (int32_t)groupSyncRead.getData(ID_1,PRESENT_POSITION_ADDRESS,4)*conversion;
  var_m->p.motor_6 = (int32_t)groupSyncRead.getData(ID_5,PRESENT_POSITION_ADDRESS,4)*conversion;
  var_m->p.motor_7 = (int32_t)groupSyncRead.getData(ID_2,PRESENT_POSITION_ADDRESS,4)*conversion;
  var_m->p.motor_8 = (int32_t)groupSyncRead.getData(ID_12,PRESENT_POSITION_ADDRESS,4)*conversion;
}


/**
 * @brief Commands absolute positions to the entire manipulator.
 * Includes clamping for the Rozum arm motors to prevent exceeding physical limits.
 * Uses GroupSyncWrite for Dynamixel position commands.
 * @param pos Target position array for all 8 motors.
 */
void Manipulator :: set_position_claw()
{
    result res_claw;
    vec_motors pos_max;
    int tx;
    read_pos_claw(&var_m,portHandler,packetHandler);
    fill(&var_m,pos_max,'p');
    for (int i = 0; i < 5; i++)
    {
      if(max_pos[i+3] == 0)
      {
        pos[i+3] = pos_max[i];
      }
    }

    dynamixel::GroupSyncWrite groupSyncWrite(portHandler, packetHandler, GOAL_POSITION_ADDRESS, 4);
    uint8_t param_goal_pos1[4];
    uint8_t param_goal_pos3[4];
    uint8_t param_goal_pos5[4];
    uint8_t param_goal_pos2[4];
    uint8_t param_goal_pos12[4];

    // --- Dynamixel Motors GroupSyncWrite Preparation ---
    // Splitting position values into 4-byte arrays

    param_goal_pos1[0] = DXL_LOBYTE(DXL_LOWORD(pos[4]));
    param_goal_pos1[1] = DXL_HIBYTE(DXL_LOWORD(pos[4]));
    param_goal_pos1[2] = DXL_LOBYTE(DXL_HIWORD(pos[4]));
    param_goal_pos1[3] = DXL_HIBYTE(DXL_HIWORD(pos[4]));
    
    param_goal_pos3[0] = DXL_LOBYTE(DXL_LOWORD(pos[3]));
    param_goal_pos3[1] = DXL_HIBYTE(DXL_LOWORD(pos[3]));
    param_goal_pos3[2] = DXL_LOBYTE(DXL_HIWORD(pos[3]));
    param_goal_pos3[3] = DXL_HIBYTE(DXL_HIWORD(pos[3]));

    param_goal_pos5[0] = DXL_LOBYTE(DXL_LOWORD(pos[5]));
    param_goal_pos5[1] = DXL_HIBYTE(DXL_LOWORD(pos[5]));
    param_goal_pos5[2] = DXL_LOBYTE(DXL_HIWORD(pos[5]));
    param_goal_pos5[3] = DXL_HIBYTE(DXL_HIWORD(pos[5]));


    param_goal_pos2[0] = DXL_LOBYTE(DXL_LOWORD(pos[6]));
    param_goal_pos2[1] = DXL_HIBYTE(DXL_LOWORD(pos[6]));
    param_goal_pos2[2] = DXL_LOBYTE(DXL_HIWORD(pos[6]));
    param_goal_pos2[3] = DXL_HIBYTE(DXL_HIWORD(pos[6]));
    
    param_goal_pos12[0] = DXL_LOBYTE(DXL_LOWORD(pos[7]));
    param_goal_pos12[1] = DXL_HIBYTE(DXL_LOWORD(pos[7]));
    param_goal_pos12[2] = DXL_LOBYTE(DXL_HIWORD(pos[7]));
    param_goal_pos12[3] = DXL_HIBYTE(DXL_HIWORD(pos[7]));
    
    res_claw[1] = groupSyncWrite.addParam(ID_1, param_goal_pos1);
    if(res_claw[1] == 0)
    {
      fprintf(stderr,"Fail to set position of motor 5");
      exit(1);
    }
    res_claw[0] = groupSyncWrite.addParam(ID_3, param_goal_pos3);
    if(res_claw[0] == 0)
    {
      fprintf(stderr,"Fail to set position of motor 4");
      exit(1);
    }
    res_claw[2] = groupSyncWrite.addParam(ID_5, param_goal_pos5);
    if(res_claw[2] == 0)
    {
      fprintf(stderr,"Fail to set position of motor 6");
      exit(1);
    }
    res_claw[3] = groupSyncWrite.addParam(ID_2, param_goal_pos2);
    if(res_claw[3] == 0)
    {
      fprintf(stderr,"Fail to set position of motor 7");
      exit(1);
    }
    res_claw[4] = groupSyncWrite.addParam(ID_12, param_goal_pos12);
    if(res_claw[4] == 0)
    {
      fprintf(stderr,"Fail to set position of motor 8");
      exit(1);
    }
    tx = groupSyncWrite.txPacket();
    if(tx == COMM_NOT_AVAILABLE)
    {
      fprintf(stderr,"Fail to send the package");
      exit(1);
    }
    groupSyncWrite.clearParam();
}


void Manipulator :: set_position_raw()
{
  float pos_max_arm[3];
  float pos_arm[3];
  pos_arm[0] = pos[0];
  pos_max_arm[0] = max_pos[0];
  pos_arm[1] = pos[1];
  pos_max_arm[1] = max_pos[1];
  pos_arm[2] = pos[2];
  pos_max_arm[2] = max_pos[2];

  {
        std::lock_guard<std::mutex> lock(mtx_sincronizacion);
        comando_actual_ = tarea_dynamixel:: SEND_POSITION;
        tarea_completada_ = false;
  }
  cv_iniciar_tarea_.notify_one();
  set_position_arm(&arm,&var_m,pos_arm,pos_max_arm);
  {
        std::unique_lock<std::mutex> lock(mtx_sincronizacion);
        cv_tarea_terminada_.wait(lock, [this]{ return tarea_completada_; });
  }
}


/**
 * @brief Switches the operating mode of the Dynamixel claw motors.
 * @param mode Char selector: 'v'/'V' for Velocity mode, 'p'/'P' for Position mode.
 */
void Manipulator::set_Mode(char mode)
{
  result res_claw;

  if(mode == 'v' || mode == 'V')
  {
    //id 1
    res_claw[1] = packetHandler->write1ByteTxRx(portHandler, ID_1, OPERATING_MODE, VELOCITY_MODE);
    if(res_claw[1] != 0)
    {
      fprintf(stderr,"Fail to change operating mode to velocity mode of motor 5");
      exit(1);
    }
    //id 2
    res_claw[3] =  packetHandler->write1ByteTxRx(portHandler, ID_2, OPERATING_MODE, VELOCITY_MODE);
    if(res_claw[3] != 0)
    {
      fprintf(stderr,"Fail to change operating mode to velocity mode of motor 7");
      exit(1);
    }
    //id 3
    res_claw[0] =  packetHandler->write1ByteTxRx(portHandler, ID_3, OPERATING_MODE, VELOCITY_MODE);
    if(res_claw[0] != 0)
    {
      fprintf(stderr,"Fail to change operating mode to velocity mode of motor 4");
      exit(1);
    }
    //id 5
    res_claw[2] =  packetHandler->write1ByteTxRx(portHandler, ID_5, OPERATING_MODE, VELOCITY_MODE);
    if(res_claw[2] != 0)
    {
      fprintf(stderr,"Fail to change operating mode to velocity mode of motor 6");
      exit(1);
    }
    //id 12
    res_claw[4] =  packetHandler->write1ByteTxRx(portHandler, ID_12, OPERATING_MODE, VELOCITY_MODE);
    if(res_claw[1] != 0)
    {
      fprintf(stderr,"Fail to change operating mode to velocity mode of motor 8");
      exit(1);
    }
  }
  else if(mode == 'p' || mode == 'P')
  {
    //id 1
    res_claw[1] =  packetHandler->write1ByteTxRx(portHandler, ID_1, OPERATING_MODE, POSITION_MODE);
    if(res_claw[1] != 0)
    {
      fprintf(stderr,"Fail to change operating mode to position mode of motor 5");
      exit(1);
    }
    //id 2
    res_claw[3] =  packetHandler->write1ByteTxRx(portHandler, ID_2, OPERATING_MODE, POSITION_MODE);
    if(res_claw[3] != 0)
    {
      fprintf(stderr,"Fail to change operating mode to position mode of motor 7");
      exit(1);
    }
    //id 3
    res_claw[0] =  packetHandler->write1ByteTxRx(portHandler, ID_3, OPERATING_MODE, POSITION_MODE);
    if(res_claw[0] != 0)
    {
      fprintf(stderr,"Fail to change operating mode to position mode of motor 4");
      exit(1);
    }
    //id 5
    res_claw[2] =  packetHandler->write1ByteTxRx(portHandler, ID_5, OPERATING_MODE, POSITION_MODE);
    if(res_claw[2] != 0)
    {
      fprintf(stderr,"Fail to change operating mode to position mode of motor 6");
      exit(1);
    }
    //id 12
    res_claw[4] =  packetHandler->write1ByteTxRx(portHandler, ID_12, OPERATING_MODE, POSITION_MODE);
    if(res_claw[4] != 0)
    {
      fprintf(stderr,"Fail to change operating mode to position mode of motor 8");
      exit(1);
    }
  }
  else
  {
    fprintf(stderr, "Error: Modo invalido.\n");
    exit(1);
  }
}


/**
 * @brief Consolidates hardware IDs from both the Rozum arm and Dynamixel claw into a single array.
 */
void Manipulator:: getIDS(int ids[8])
{
  int id_arm[3];
  ids_claw id_claw;
  get_motors_arm_id(&arm,id_arm);
  get_id_claw(portHandler,packetHandler,id_claw);
  for (int i = 0; i < 3; i++)
  {
    ids[i] = id_arm[i];
  }
  for (int i = 0; i < 5; i++)
  {
    ids[i+3] = (int )id_claw[i];
  }
}


/**
 * @brief Sets hardware IDs for both the Rozum arm and Dynamixel claw from a single array.
 */
void Manipulator:: setIDS(int ids[8])
{
  int id_arm[3];
  ids_claw id_claw;
  for (int i = 0; i < 8; i++)
  {
    if(i < 3)
    {
      id_arm[i] = ids[i];
    }
    else
    {
      id_claw[i-3] = ids[i] ;
    }
  }
  set_motors_arm_id(&arm,id_arm);
  set_id_claw(portHandler,packetHandler,id_claw);  
}


/**
 * @brief Reads real-time temperature from all Dynamixel claw motors via GroupSyncRead.
 */
void read_temp_claw(Var_motors *var_m,dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler)
{
  dynamixel::GroupSyncRead groupSyncRead(portHandler, packetHandler, PRESENT_TEMPERATURE_ADDRESS, 4);
  result res_temp;
  int rx;
  res_temp[1] = groupSyncRead.addParam(ID_1);
  if(res_temp[1] == 0)
  {
    fprintf(stderr,"Fail to read temperature of motor 5");
    exit(1);
  }
  res_temp[0] = groupSyncRead.addParam(ID_3);
  if(res_temp[0] == 0)
  {
    fprintf(stderr,"Fail to read temperature of motor 4");
    exit(1);
  }
  res_temp[2] = groupSyncRead.addParam(ID_5);
  if(res_temp[2] == 0)
  {
    fprintf(stderr,"Fail to read temperature of motor 6");
    exit(1);
  }
  res_temp[3] = groupSyncRead.addParam(ID_2);
  if(res_temp[3] == 0)
  {
    fprintf(stderr,"Fail to read temperature of motor 7");
    exit(1);
  }
  res_temp[4] = groupSyncRead.addParam(ID_12);
  if(res_temp[4] == 0)
  {
    fprintf(stderr,"Fail to read temperature of motor 8");
    exit(1);
  }
  rx = groupSyncRead.txRxPacket();
  if(rx == COMM_NOT_AVAILABLE)
  {
    fprintf(stderr,"Fail to receive the package of temperatures readings");
    exit(1);
  }
  var_m->t.motor_4 = groupSyncRead.getData(ID_3,PRESENT_TEMPERATURE_ADDRESS,4);
  var_m->t.motor_5 = groupSyncRead.getData(ID_1,PRESENT_TEMPERATURE_ADDRESS,4);
  var_m->t.motor_6 = groupSyncRead.getData(ID_5,PRESENT_TEMPERATURE_ADDRESS,4);
  var_m->t.motor_7 = groupSyncRead.getData(ID_2,PRESENT_TEMPERATURE_ADDRESS,4)/10000000;
  var_m->t.motor_8 = groupSyncRead.getData(ID_12,PRESENT_TEMPERATURE_ADDRESS,4)/10000000;
}



/**
 * @brief Reads current velocity from all Dynamixel claw motors via GroupSyncRead.
 */
void read_vel_claw(Var_motors *var_m,dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler)
{
  dynamixel::GroupSyncRead groupSyncRead(portHandler, packetHandler, PRESENT_VELOCITY_ADDRESS, 4);
  result res_temp;
  int rx;
  res_temp[1] = groupSyncRead.addParam(ID_1);
  if(res_temp[1] == 0)
  {
    fprintf(stderr,"Fail to read velocity of motor 5");
    exit(1);
  }
  res_temp[0] = groupSyncRead.addParam(ID_3);
  if(res_temp[0] == 0)
  {
    fprintf(stderr,"Fail to read velocity of motor 4");
    exit(1);
  }
  res_temp[2] = groupSyncRead.addParam(ID_5);
  if(res_temp[2] == 0)
  {
    fprintf(stderr,"Fail to read velocity of motor 6");
    exit(1);
  }
  res_temp[3] = groupSyncRead.addParam(ID_2);
  if(res_temp[3] == 0)
  {
    fprintf(stderr,"Fail to read velocity of motor 7");
    exit(1);
  }
  res_temp[4] = groupSyncRead.addParam(ID_12);
  if(res_temp[4] == 0)
  {
    fprintf(stderr,"Fail to read velocity of motor 8");
    exit(1);
  }
  rx = groupSyncRead.txRxPacket();
  if(rx == COMM_NOT_AVAILABLE)
  {
    fprintf(stderr,"Fail to receive the package of velocities readings");
    exit(1);
  }
  var_m->v.motor_4 = groupSyncRead.getData(ID_3,PRESENT_VELOCITY_ADDRESS,4);
  var_m->v.motor_5 = groupSyncRead.getData(ID_1,PRESENT_VELOCITY_ADDRESS,4);
  var_m->v.motor_6 = groupSyncRead.getData(ID_5,PRESENT_VELOCITY_ADDRESS,4);
  var_m->v.motor_7 = groupSyncRead.getData(ID_2,PRESENT_VELOCITY_ADDRESS,4);
  var_m->v.motor_8 = groupSyncRead.getData(ID_12,PRESENT_VELOCITY_ADDRESS,4);
}


/**
 * @brief Reads current load/current from all Dynamixel claw motors via GroupSyncRead.
 */
void read_current_claw(Var_motors *var_m,dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler)
{
  dynamixel::GroupSyncRead groupSyncRead(portHandler, packetHandler, PRESENT_CURRENT_ADDRESS, 4);
  result res_temp;
  int rx;
  res_temp[1] = groupSyncRead.addParam(ID_1);
  if(res_temp[1] == 0)
  {
    fprintf(stderr,"Fail to read current of motor 5");
    exit(1);
  }
  res_temp[0] = groupSyncRead.addParam(ID_3);
  if(res_temp[0] == 0)
  {
    fprintf(stderr,"Fail to read current of motor 4");
    exit(1);
  }
  res_temp[2] = groupSyncRead.addParam(ID_5);
  if(res_temp[2] == 0)
  {
    fprintf(stderr,"Fail to read current of motor 6");
    exit(1);
  }
  res_temp[3] = groupSyncRead.addParam(ID_2);
  if(res_temp[3] == 0)
  {
    fprintf(stderr,"Fail to read current of motor 7");
    exit(1);
  }
  res_temp[4] = groupSyncRead.addParam(ID_12);
  if(res_temp[4] == 0)
  {
    fprintf(stderr,"Fail to read current of motor 8");
    exit(1);
  }
  rx = groupSyncRead.txRxPacket();
  if(rx == COMM_NOT_AVAILABLE)
  {
    fprintf(stderr,"Fail to receive the package of currents readings");
    exit(1);
  }
  var_m->c.motor_4 = groupSyncRead.getData(ID_3,PRESENT_CURRENT_ADDRESS,4);
  var_m->c.motor_5 = groupSyncRead.getData(ID_1,PRESENT_CURRENT_ADDRESS,4);
  var_m->c.motor_6 = groupSyncRead.getData(ID_5,PRESENT_CURRENT_ADDRESS,4);
  var_m->c.motor_7 = groupSyncRead.getData(ID_2,PRESENT_CURRENT_ADDRESS,4);
  var_m->c.motor_8 = groupSyncRead.getData(ID_12,PRESENT_CURRENT_ADDRESS,4);
}


/**
 * @brief Aggregates and prints temperature data from both Rozum arm and Dynamixel claw motors.
 */
void Manipulator :: read_temperature()
{
  vec_motors temperature;

  {
        std::lock_guard<std::mutex> lock(mtx_sincronizacion);
        comando_actual_ = tarea_dynamixel:: READ_TEMPERATURE;
        tarea_completada_ = false;
  }
  cv_iniciar_tarea_.notify_one();
  read_temp_arm(&arm,&var_m);
  //read_temp_claw(&var_m,portHandler,packetHandler);
  {
        std::unique_lock<std::mutex> lock(mtx_sincronizacion);
        cv_tarea_terminada_.wait(lock, [this]{ return tarea_completada_; });
  }
  fill(&var_m,temperature,'t');
  for (int i = 0; i < 8; i++)
  {
    printf("Motor %d Temperature: %f °C\n",i,temperature[i]);
  }
}


/**
 * @brief Aggregates and prints position data from both Rozum arm and Dynamixel claw motors.
 */
void Manipulator :: read_position()
{
  vec_motors position;
  
  
  {
        std::lock_guard<std::mutex> lock(mtx_sincronizacion);
        comando_actual_ = tarea_dynamixel:: READ_POSITION;
        tarea_completada_ = false;
  }
  cv_iniciar_tarea_.notify_one();

  read_pos_arm(&arm,&var_m);
  //read_pos_claw(&var_m,portHandler,packetHandler);
  {
        std::unique_lock<std::mutex> lock(mtx_sincronizacion);
        cv_tarea_terminada_.wait(lock, [this]{ return tarea_completada_; });
  }
  fill(&var_m,position,'p');
  for (int i = 0; i < 8; i++)
  {
    printf("Motor %d Position: %f °\n",i,position[i]);
  }
}


/**
 * @brief Aggregates and prints velocity data from both Rozum arm and Dynamixel claw motors.
 */
void Manipulator :: read_velocity()
{
  vec_motors velocity;
  {
        std::lock_guard<std::mutex> lock(mtx_sincronizacion);
        comando_actual_ = tarea_dynamixel:: READ_VELOCITY;
        tarea_completada_ = false;
  }
  cv_iniciar_tarea_.notify_one();
  read_vel_arm(&arm,&var_m);
  //read_vel_claw(&var_m,portHandler,packetHandler);
  {
        std::unique_lock<std::mutex> lock(mtx_sincronizacion);
        cv_tarea_terminada_.wait(lock, [this]{ return tarea_completada_; });
  }
  fill(&var_m,velocity,'v');
  for (int i = 0; i < 8; i++)
  {
    printf("Motor %d Velocity: %f m/s\n",i,velocity[i]);
  }
}


/**
 * @brief Aggregates and prints electrical current load from both Rozum arm and Dynamixel claw motors.
 */
void Manipulator :: read_current()
{
  vec_motors current;
  {
        std::lock_guard<std::mutex> lock(mtx_sincronizacion);
        comando_actual_ = tarea_dynamixel:: READ_CURRENT;
        tarea_completada_ = false;
  }
  cv_iniciar_tarea_.notify_one();
  read_current_arm(&arm,&var_m);
  //read_current_claw(&var_m,portHandler,packetHandler);
  {
        std::unique_lock<std::mutex> lock(mtx_sincronizacion);
        cv_tarea_terminada_.wait(lock, [this]{ return tarea_completada_; });
  }
  fill(&var_m,current,'c');
  for (int i = 0; i < 8; i++)
  {
    printf("Motor %d Current: %f mA\n",i,current[i]);
  }
}



void Manipulator::gestor_tareas() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx_sincronizacion);
        
        // 1. DORMIR hasta que el comando sea distinto de 'SLEEPING'
        cv_iniciar_tarea_.wait(lock, [this]{ 
            return comando_actual_ != tarea_dynamixel::SLEEPING; 
        });
        
        // Si se apagan los motores, rompemos el bucle infinito
        if (comando_actual_ == tarea_dynamixel::POWER_OFF) break;

        // 2. DESPERTAR Y TRABAJAR: Soltamos el candado para no bloquear
        tarea_dynamixel tarea_a_ejecutar = comando_actual_;
        lock.unlock(); 
        
        // --- LA MÁQUINA DE ESTADOS ---
        switch (tarea_a_ejecutar) {
            case tarea_dynamixel::READ_POSITION:
                read_pos_claw(&var_m, portHandler, packetHandler);
                break;
            case tarea_dynamixel::READ_VELOCITY:
                read_vel_claw(&var_m, portHandler, packetHandler);
                break;
            case tarea_dynamixel::READ_TEMPERATURE:
                read_temp_claw(&var_m, portHandler, packetHandler);
                break;
            case tarea_dynamixel::READ_CURRENT:
                read_current_claw(&var_m, portHandler, packetHandler);
                break;
            case tarea_dynamixel::SEND_VELOCITY:
                set_velocity_claw();
                break;
            case tarea_dynamixel::SEND_POSITION:
                set_position_claw();
                break;
            default:
                break;
        }
        
        // 3. AVISAR: Volvemos a coger el candado e informamos que terminamos
        lock.lock();
        comando_actual_ = tarea_dynamixel::SLEEPING; // Volvemos al estado base
        tarea_completada_ = true;
        cv_tarea_terminada_.notify_one();
    }
}


/**
 * @brief Master initialization function for the entire manipulator.
 * Boots up the Rozum base arm motors first, followed by the Dynamixel claw motors.
 */
void Manipulator::init_motors(RoboArm *arm,Var_motors *var_m,dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler)
{ 
    init_motors_arm(arm,var_m); // Start position motors (Rozum)

    init_claw_motors(portHandler,packetHandler); // Start orientation/claw motors (Dynamixel)

    for (int i = 0; i < 8; i++)
    {
      max_pos[i] = 0;
    }
    hilo_dynamixel = std::thread(&Manipulator::gestor_tareas,this);
    
}