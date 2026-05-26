#include "../include/robotic_arm.h"

void finish_motors_claw(dynamixel::PortHandler *portHandler,dynamixel::PacketHandler *packetHandler)
{
    /*Esta función se encarga de deshabilitar o apagar los motores dynamixel cuando el programa acabe
      (Solo existe esta función para los dynamixel dado que los motores de Rozum dejan de funcionar automaticamente cuando el programa acaba)*/
    
    //id 1
    packetHandler->write1ByteTxRx(portHandler, ID_1, TORQUE_ADDRESS, OFF);
    //id 2
    packetHandler->write1ByteTxRx(portHandler, ID_2, TORQUE_ADDRESS, OFF);

    //id 3
    packetHandler->write1ByteTxRx(portHandler, ID_3, TORQUE_ADDRESS, OFF);

    //id 5
    packetHandler->write1ByteTxRx(portHandler, ID_5, TORQUE_ADDRESS, OFF);

    //id 12
    packetHandler->write1ByteTxRx(portHandler, ID_12, TORQUE_ADDRESS, OFF);

    portHandler->closePort();
}

void Manipulator :: init()
{
    init_motors(&arm,&var_m,portHandler,packetHandler);
}

void Manipulator :: deinit()
{
    finish_motors_arm(&arm);
    finish_motors_claw(portHandler,packetHandler);
}