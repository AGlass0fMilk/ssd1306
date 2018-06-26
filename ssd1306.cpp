#include "ssd1306.h"

#include "platform/mbed_wait_api.h"

SSD1306::SSD1306(mbed::I2C* i2c, uint16_t width, uint16_t height) :
		_i2c(i2c), _width(width), _height(height)
{
}

void SSD1306::off()
{
	_send_command(0xAE);
}

void SSD1306::on()
{
	_send_command(0xAF);
}

void SSD1306::sleep()
{
	_send_command(0xAE);
}

void SSD1306::wake()
{
	_send_command(0xAF);
}

void SSD1306::set_inverse(uint8_t value)
{
	_send_command(value ? 0xA7 : 0xA6);
}

void SSD1306::set_display_offset(uint8_t value)
{
	uint8_t cmd[] = { 0xD3, (uint8_t) (value & 0x3F) };
	_send_command(cmd, 2);
}

void SSD1306::set_contrast(uint8_t value)
{
	_send_command(0x81);
	_send_command(value);
	uint8_t cmd[] = { 0xD3, (uint8_t) (value & 0x3F) };
	_send_command(cmd, 2);
}

void SSD1306::set_display_start_line(uint8_t value)
{
	_send_command(0x40 | value);
	uint8_t cmd[] = { 0xD3, (uint8_t)(value & 0x3F) };
	_send_command(cmd, 2);
}

void SSD1306::set_segment_remap(uint8_t value)
{
	_send_command(value ? 0xA1 : 0xA0);
}

void SSD1306::set_multiplex_ratio(uint8_t value)
{
	_send_command(0xA8);
	_send_command(value & 0x3F);
	uint8_t cmd[] = { 0xD3, (uint8_t)(value & 0x3F) };
	_send_command(cmd, 2);
}

void SSD1306::set_com_output_scan_direction(uint8_t value)
{
	_send_command(value ? 0xC8 : 0xC0);
}

void SSD1306::set_com_pins_hardware_configuration(uint8_t sequential,
		uint8_t lr_remap)
{
	uint8_t cmd[] = { 0xDA, (uint8_t)(0x02 | ((sequential & 1) << 4) | ((lr_remap & 1) << 5)) };
	_send_command(cmd, 2);
}

void SSD1306::start_horizontal_scroll(uint8_t direction, uint8_t start,
		uint8_t end, uint8_t interval)
{
	_send_command(direction ? 0x27 : 0x26);
	_send_command(0x00);
	_send_command(start & 0x07);

	switch (interval)
	{
	case 2:
		_send_command(0x07);
		break; // 111b
	case 3:
		_send_command(0x04);
		break; // 100b
	case 4:
		_send_command(0x05);
		break; // 101b
	case 5:
		_send_command(0x00);
		break; // 000b
	case 25:
		_send_command(0x06);
		break; // 110b
	case 64:
		_send_command(0x01);
		break; // 001b
	case 128:
		_send_command(0x02);
		break; // 010b
	case 256:
		_send_command(0x03);
		break; // 011b
	default:
		// default to 2 frame interval
		_send_command(0x07);
		break;
	}
	_send_command(end & 0x07);
	_send_command(0x00);
	_send_command(0xFF);

	// activate scroll
	_send_command(0x2F);

	//uint8_t cmd[] = { 0xD3, (uint8_t)(value & 0x3F) };
	//_send_command(cmd, 2);

}

void SSD1306::start_vertical_and_horizontal_scroll(uint8_t direction,
		uint8_t start, uint8_t end, uint8_t interval, uint8_t vertical_offset)
{
	_send_command(direction ? 0x2A : 0x29);
	_send_command(0x00);
	_send_command(start & 0x07);
	switch (interval)
	{
	case 2:
		_send_command(0x07);
		break; // 111b
	case 3:
		_send_command(0x04);
		break; // 100b
	case 4:
		_send_command(0x05);
		break; // 101b
	case 5:
		_send_command(0x00);
		break; // 000b
	case 25:
		_send_command(0x06);
		break; // 110b
	case 64:
		_send_command(0x01);
		break; // 001b
	case 128:
		_send_command(0x02);
		break; // 010b
	case 256:
		_send_command(0x03);
		break; // 011b
	default:
		// default to 2 frame interval
		_send_command(0x07);
		break;
	}
	_send_command(end & 0x07);
	_send_command(vertical_offset);

	// activate scroll
	_send_command(0x2F);

	//uint8_t cmd[] = { 0xD3, (uint8_t)(value & 0x3F) };
	//_send_command(cmd, 2);
}

void SSD1306::stop_scroll()
{
	// all scroll configurations are removed from the display when executing this command.
	_send_command(0x2E);
}

void SSD1306::pam_set_start_address(uint8_t address)
{
	// "Set Lower Column Start Address for Page Addressing Mode"
	//_send_command(address & 0x0F);

	// "Set Higher Column Start Address for Page Addressing Mode"
	//_send_command((address << 4) & 0x0F);

	uint8_t cmd[] = { (uint8_t)(address & 0x0F), (uint8_t)((address << 4) & 0x0F) };
	_send_command(cmd, 2);
}

void SSD1306::set_memory_addressing_mode(uint8_t mode)
{
	uint8_t cmd[] = { 0x20, (uint8_t)(mode & 0x03) };
	_send_command(cmd, 2);
}

void SSD1306::hv_set_column_address(uint8_t start, uint8_t end)
{
	uint8_t cmd[] = { 0x21, (uint8_t)(start & 0x7F), (uint8_t)(end & 0x7F) };
	_send_command(cmd, 3);
}

void SSD1306::hv_set_page_address(uint8_t start, uint8_t end)
{
	uint8_t cmd[] = { 0x22, (uint8_t)(start & 0x07), (uint8_t)(end & 0x07) };
	_send_command(cmd, 3);
}

void SSD1306::pam_set_page_start(uint8_t address)
{
	_send_command(0xB0 | (address & 0x07));
}

void SSD1306::set_display_clock_ratio_and_frequency(uint8_t ratio,
		uint8_t frequency)
{
	uint8_t cmd[] = { 0xD5, (uint8_t)((ratio & 0x0F) | ((frequency & 0x0F) << 4)) };
	_send_command(cmd, 2);
}

void SSD1306::set_precharge_period(uint8_t phase1, uint8_t phase2)
{
	uint8_t cmd[] = { 0xD9, (uint8_t)((phase1 & 0x0F) | ((phase2 & 0x0F) << 4)) };
	_send_command(cmd, 2);
}

void SSD1306::set_vcomh_deselect_level(uint8_t level)
{
	uint8_t cmd[] = { 0xDB, (uint8_t)((level & 0x03) << 4) };
	_send_command(cmd, 2);
}

void SSD1306::nop()
{
	_send_command(0xE3);
}

void SSD1306::set_charge_pump_enable(uint8_t enable)
{
	uint8_t cmd[] = { 0x8D, (uint8_t)(enable ? 0x14 : 0x10) };
	_send_command(cmd, 2);
}

void SSD1306::initialize()
{
	// Init
	//_reset = 1;
	//wait(0.01);
	//_reset = 0;
	//wait(0.10);
	//_reset = 1;

	off();

	set_display_clock_ratio_and_frequency(0, 8);
	set_multiplex_ratio(0x3F); // 1/64 duty
	set_precharge_period(0xF, 0x01);
	set_display_offset(0);
	set_display_start_line(0);
	set_charge_pump_enable(1);
	set_memory_addressing_mode(0); // horizontal addressing mode; across then down
	set_segment_remap(1);
	set_com_output_scan_direction(1);
	set_com_pins_hardware_configuration(1, 0);
	set_contrast(0xFF);
	set_vcomh_deselect_level(1);

	wake();
	set_inverse(0);

	hv_set_column_address(0, 127);
	hv_set_page_address(0, 7);

	pam_set_start_address(0);
	pam_set_page_start(0);

	// set_precharge_period(2, 2);
}

//void SSD1306::update()
//{
//	hv_set_column_address(0, 127);
//	hv_set_page_address(0, 7);
//
//	for (int i = 0; i < 1024; i++)
//		_send_data (_screen[i]);
//}

void SSD1306::set_pixel(int x, int y)
{

}

void SSD1306::clear_pixel(int x, int y)
{

}

void SSD1306::clear()
{

}

void SSD1306::test()
{
	_send_command(0xA5);
	wait_ms(3000);
	_send_command(0xA4);
}

void SSD1306::_send_command(uint8_t code)
{
	_send_command(&code, 1);
}

void SSD1306::_send_command(uint8_t* cmd, uint16_t len)
{
	_i2c->start();
	_i2c->write(0x3C << 1); // Write the SSD1306 slave address
	_i2c->write(0x00);		// Write the control byte

	// Write out the command
	for(int i = 0; i < len; i++)
	{
		_i2c->write(cmd[i]);
	}

	_i2c->stop();

}

void SSD1306::_send_data(uint8_t* data, uint16_t len)
{
	// TODO - Make this faster by allowing streamed data
	for(int i = 0; i < len; i++)
	{
		_i2c->start();
		_i2c->write(0x3C << 1); // Write out the SSD1306 slave address
		_i2c->write(0x40);		// Write out the control byte
		_i2c->write(data[i]);	// Write out the data byte
		_i2c->stop();
	}
}
