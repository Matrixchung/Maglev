#include "ICM20602.h"
uint8_t imu_id = 0;
int16_t imu_acc_x = 0, imu_acc_y = 0, imu_acc_z = 0;
int16_t imu_gyro_x = 0, imu_gyro_y = 0, imu_gyro_z = 0;
float imu_pitch = 0.0f, imu_roll = 0.0f;
void ICM20602_WriteByte(uint8_t cmd, uint8_t data)
{
    ICM20602_CS(0);
    cmd &= ICM20602_SPI_W;
    SPI_WriteReg8(ICM20602_SPIx, cmd, data);
    ICM20602_CS(1);
}
void ICM20602_ReadByte(uint8_t cmd, uint8_t *data)
{
    ICM20602_CS(0);
    cmd |= ICM20602_SPI_R;
    SPI_ReadReg8_2(ICM20602_SPIx, cmd, data);
    ICM20602_CS(1);
}
void ICM20602_ReadBytes(uint8_t cmd, uint8_t *data, uint8_t len)
{
    ICM20602_CS(0);
    cmd |= ICM20602_SPI_R;
    SPI_ReadReg8N(ICM20602_SPIx, cmd, data, len);
    ICM20602_CS(1);
}
uint8_t ICM20602_GetID(void)
{
    uint8_t data = 0;
    uint16_t timeout = 0;
    do
    {
        ICM20602_ReadByte(ICM20602_WHO_AM_I, &data);
        timeout++;
        if(timeout >= ICM20602_CONN_TIMEOUT) return 0;
    }while(!data);
    return data;
}
uint8_t ICM20602_SetSleep(uint8_t state)
{
    ICM20602_WriteByte(ICM20602_PWR_MGMT_1, state == 1 ? 0x41 : 0x01);
    uint8_t temp = 0;
    ICM20602_ReadByte(ICM20602_PWR_MGMT_1, &temp);
    return temp;
}
uint8_t ICM20602_Init(void)
{
    uint8_t temp = 0;
    ICM20602_CS(1);
    LL_SPI_Enable(SPI1);
    imu_id = ICM20602_GetID();
    if(imu_id != ICM20602_ID) return 0;
    ICM20602_WriteByte(ICM20602_I2C_IF, 0x40); // Disable I2C Slave module and put it in SPI mode only
    ICM20602_WriteByte(ICM20602_PWR_MGMT_1, 0x80); // Reset the device
    do
    {
        ICM20602_ReadByte(ICM20602_PWR_MGMT_1, &temp);
        delay_us(1);
    }while(0x41 != temp); // 0x41 means sleep mode
    delay_ms(50);
    ICM20602_WriteByte(ICM20602_I2C_IF, 0x40); // Disable I2C Slave module and put it in SPI mode only
    ICM20602_WriteByte(ICM20602_PWR_MGMT_1, 0x01); // Set clock, wake up
    delay_ms(50);
    ICM20602_WriteByte(ICM20602_PWR_MGMT_2, 0x00); // Enable all output
    ICM20602_WriteByte(ICM20602_SMPLRT_DIV, 0x00); // SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    ICM20602_WriteByte(ICM20602_CONFIG, DLPF_BW_20); // DLPF_CFG[2:0] = 0x01, 176Hz noise, 1KHz rate
    // ICM20602_GYRO_CONFIG寄存器
    // 设置为:0x00 陀螺仪量程为:±250 dps     获取到的陀螺仪数据除以131           可以转化为带物理单位的数据， 单位为：°/s
    // 设置为:0x08 陀螺仪量程为:±500 dps     获取到的陀螺仪数据除以65.5          可以转化为带物理单位的数据，单位为：°/s
    // 设置为:0x10 陀螺仪量程为:±1000dps     获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
    // 设置为:0x18 陀螺仪量程为:±2000dps     获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s
    ICM20602_WriteByte(ICM20602_GYRO_CONFIG, ICM20_GYRO_FS_500); // ±500 dps
    // ICM20602_ACCEL_CONFIG寄存器
    // 设置为:0x00 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)
    // 设置为:0x08 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
    // 设置为:0x10 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
    // 设置为:0x18 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)
    ICM20602_WriteByte(ICM20602_ACCEL_CONFIG, ICM20_ACCEL_FS_4G); // ±4g
    ICM20602_WriteByte(ICM20602_ACCEL_CONFIG_2, ACCEL_AVER_16|ACCEL_DLPF_BW_21); // Average 16 samples
    imu_id = ICM20602_GetID();
    if(imu_id != ICM20602_ID) return 0;
    return imu_id;
}
void ICM20602_UpdateData(void)
{
    uint8_t buffer[6] = {0};
    ICM20602_ReadBytes(ICM20602_ACCEL_XOUT_H, buffer, 6);
    imu_acc_x = (int16_t)(((uint16_t)buffer[0] << 8 | buffer[1]));
    imu_acc_y = (int16_t)(((uint16_t)buffer[2] << 8 | buffer[3]));
    imu_acc_z = (int16_t)(((uint16_t)buffer[4] << 8 | buffer[5]));
    ICM20602_ReadBytes(ICM20602_GYRO_XOUT_H, buffer, 6);
    imu_gyro_x = (int16_t)(((uint16_t)buffer[0] << 8 | buffer[1]));
    imu_gyro_y = (int16_t)(((uint16_t)buffer[2] << 8 | buffer[3]));
    imu_gyro_z = (int16_t)(((uint16_t)buffer[4] << 8 | buffer[5]));
}
void ICM20602_UpdateAHRS(void)
{
    MadgwickAHRSupdate_6((float)imu_gyro_x, (float)imu_gyro_y, (float)imu_gyro_z, (float)imu_acc_x, (float)imu_acc_y, (float)imu_acc_z, &imu_pitch, &imu_roll);
}
// void ICM20602_UpdateAcc(void)
// {
//     uint8_t buffer[6] = {0};
//     ICM20602_ReadBytes(ICM20602_ACCEL_XOUT_H, buffer, 6);
//     imu_acc_x = (int16_t)(((uint16_t)buffer[0] << 8 | buffer[1]));
//     imu_acc_y = (int16_t)(((uint16_t)buffer[2] << 8 | buffer[3]));
//     imu_acc_z = (int16_t)(((uint16_t)buffer[4] << 8 | buffer[5]));
// }
// void ICM20602_UpdateGyro(void)
// {
//     uint8_t buffer[6] = {0};
//     ICM20602_ReadBytes(ICM20602_GYRO_XOUT_H, buffer, 6);
//     imu_gyro_x = (int16_t)(((uint16_t)buffer[0] << 8 | buffer[1]));
//     imu_gyro_y = (int16_t)(((uint16_t)buffer[2] << 8 | buffer[3]));
//     imu_gyro_z = (int16_t)(((uint16_t)buffer[4] << 8 | buffer[5]));
// }