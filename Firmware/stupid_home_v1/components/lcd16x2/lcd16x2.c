#include "lcd16x2.h"
#include <string.h>
esp_err_t i2c_write_lcd(i2c_port_t i2c_num, uint8_t *data, int len)
{
    uint8_t tx[10] = {0};
    memcpy(tx, data, len);
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LCD_ADDRESS << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write(cmd, tx, len, ACK_CHECK_EN); 
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

void lcd_send_cmd(i2c_port_t i2c_num, char cmd)
{
    char data_u, data_l;
	char data_t[4] = {0};
	data_u = (cmd & 0xF0);
	data_l = ((cmd << 4) & 0xF0);
	data_t[0] = data_u | 0x0C;  
	data_t[1] = data_u | 0x08;  
	data_t[2] = data_l | 0x0C;  
	data_t[3] = data_l | 0x08;  
    i2c_write_lcd(i2c_num, (uint8_t*)data_t, strlen(data_t));
}

void lcd_send_data(i2c_port_t i2c_num, char data)
{
	char data_u, data_l;
	char data_t[4] = {0};
	data_u = (data & 0xf0);
	data_l = ((data << 4) & 0xF0);
	data_t[0] = data_u | 0x0D;  
	data_t[1] = data_u | 0x09;  
	data_t[2] = data_l | 0x0D;  
	data_t[3] = data_l | 0x09;  
	i2c_write_lcd(i2c_num, (uint8_t*)data_t, strlen(data_t));
}

void lcd_clear(i2c_port_t i2c_num)
{
	lcd_send_cmd(i2c_num, 0x00);
	for (int i = 0; i < 100; i++)
	{
		lcd_send_data(i2c_num, ' ');
	}
}

void lcd_init(i2c_port_t i2c_num)
{
	vTaskDelay(50 / portTICK_PERIOD_MS);  
	lcd_send_cmd(i2c_num, 0x30);
	vTaskDelay(5 / portTICK_PERIOD_MS);  
	lcd_send_cmd(i2c_num, 0x30);
	vTaskDelay(1 / portTICK_PERIOD_MS);  
	lcd_send_cmd(i2c_num, 0x30);
	vTaskDelay(10 / portTICK_PERIOD_MS);
	lcd_send_cmd(i2c_num, 0x20);  
	vTaskDelay(10 / portTICK_PERIOD_MS);
	lcd_send_cmd(i2c_num, 0x28); 
	vTaskDelay(1 / portTICK_PERIOD_MS);
	lcd_send_cmd(i2c_num, 0x08); 
	vTaskDelay(1 / portTICK_PERIOD_MS);
	lcd_send_cmd(i2c_num, 0x01); 
	vTaskDelay(2 / portTICK_PERIOD_MS);
	lcd_send_cmd(i2c_num, 0x06); 
	vTaskDelay(1 / portTICK_PERIOD_MS);
	lcd_send_cmd(i2c_num, 0x0C); 
}

void lcd_send_string(i2c_port_t i2c_num, char *str)
{
	while(*str) 
    {
        lcd_send_data(i2c_num, *str++);
    }
}