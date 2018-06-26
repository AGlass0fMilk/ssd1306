#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "drivers/I2C.h"

#define FONT_START          ' '  /* First character value in the font table */

/** SSD1306 Controller Driver
  *
  * This class provides a driver for the SSD1306 OLED controller.
  *
  * Information taken from the datasheet at:
  *   http://www.adafruit.com/datasheets/SSD1306.pdf
  *
  */
class SSD1306
{
public:
    /** Construct a new SSD1306 object.
     * @param[in] i2c I2C Handle for SSD1306
     * @param[in] width (optional) Width of display in pixels
     * @param[in] height (optional Height of display in pixels
     */
    SSD1306(mbed::I2C* i2c, uint16_t width = 128, uint16_t height = 64);

    // ----- HARDWARE CONTROL -----

    /** Initialize the display with defaults.*/
    void initialize();

    /** Turn the whole display off.  This will reset all configuration settings on the controller to their defaults. */
    void off();
    
    /** Turn the whole display on.  Used during initialisation. */
    void on();

    /** Sends the display to sleep, but leaves RAM intact. */
    void sleep();

    /** Wakes up this display following a sleep() call.
     *  @see sleep()
     */
    void wake();

    /** Set the display contrast.
     *  @param value The contrast, from 1 to 256.
     */
    void set_contrast(uint8_t value); // 1-256
    
    /** Set the display to normal or inverse.
     *  @param value 0 for normal mode, or 1 for inverse mode.
     */
    void set_inverse(uint8_t value); // 0 or 1
    
    /** Set the display start line.  This is the line at which the display will start rendering.
     *  @param value A value from 0 to 63 denoting the line to start at.
     */
    void set_display_start_line(uint8_t value); // 0-63
    
    /** Set the segment remap state.  This allows the module to be addressed as if flipped horizontally.
      * NOTE: Changing this setting has no effect on data already in the module's GDDRAM.
      * @param value 0 = column address 0 = segment 0 (the default), 1 = column address 127 = segment 0 (flipped).
      */
    void set_segment_remap(uint8_t value); // 0 or 1
    
    /** Set the vertical shift by COM.
      * @param value The number of rows to shift, from 0 - 63.
      */
    void set_display_offset(uint8_t value); // 0-63
    
    /** Set the multiplex ratio.
     *  @param value MUX will be set to (value+1). Valid values range from 15 to 63 - MUX 16 to 64.
     */
    void set_multiplex_ratio(uint8_t value); // 15-63 (value+1 mux)
    
    /** Set COM output scan direction.  If the display is active, this will immediately vertically
      * flip the display.
      * @param value 0 = Scan from COM0 (default), 1 = reversed (scan from COM[N-1]).
      */
    void set_com_output_scan_direction(uint8_t value); // 0 or 1
    
    /** Set COM pins hardware configuration.
      * @param sequential 0 = Sequental COM pin configuration, 1 = Alternative COM pin configuration (default).
      * @param lr_remap 0 = Disable COM left/right remap (default), 1 = enable COM left/right remap.
      */
    void set_com_pins_hardware_configuration(uint8_t sequential, uint8_t lr_remap); // 0 or 1 for both parametrs

    /** Set up and start a continuous horizontal scroll.
      * Once you have set up the scrolling, you can deactivate it with stop_scroll().
      * @param direction 0 for right, 1 for left.
      * @param start Start page address, 0 - 5.
      * @param end End page address, 0 - 5.
      * @param interval Interval in frame frequency.  Valid values are: 2, 3, 4, 5, 25, 64, 128, 256.
      * @see stop_scrol
      */
    void start_horizontal_scroll(uint8_t direction, uint8_t start, uint8_t end, uint8_t interval);

    /** Set up and start a continuous horizontal and vertical scroll.
      * NOTE: No continuous vertical scroll is available.
      * Once you have set up the scrolling, you can deactivate it with stop_scroll().
      * @param direction 0 for vertical and right horizontal scroll, 1 for vertical and left horizontal scroll.
      * @param start Start page address, 0 - 5.
      * @param end End page address, 0 - 5.
      * @param interval Interval in frame frequency.  Valid values are: 2, 3, 4, 5, 25, 64, 128, 256.
      * @param vertical_offset Offset of vertical scroll, 1 - 63.
      * @see stop_scroll
      */
    void start_vertical_and_horizontal_scroll(uint8_t direction, uint8_t start, uint8_t end, uint8_t interval, uint8_t vertical_offset);
    
    /** Deactivate the continuous scroll set up with start_horizontal_scroll() or 
      * start_vertical_and_horizontal_scroll().
      * @see set_horizontal_scroll, set_vertical_and_horizontal_scroll
      */
    void stop_scroll();
    
    // ----- ADDRESSING -----
    
    /** Set memory addressing mode to the given value.
      * @param mode 0 for Horizontal addressing mode, 1 for Vertical addressing mode, or 2 for Page addressing mode (PAM).  2 is the default.
      */
    void set_memory_addressing_mode(uint8_t mode);
    
    /** Page Addressing Mode: Set the column start address register for
      * page addressing mode.
      * @param address The address (full byte).
      */
    void pam_set_start_address(uint8_t address);
    
    /** Set the GDDRAM page start address for page addressing mode.
      * @param address The start page, 0 - 7.
      */
    void pam_set_page_start(uint8_t address);
    
    /** Set page start and end address for horizontal/vertical addressing mode.
      * @param start The start page, 0 - 7.
      * @param end The end page, 0 - 7.
      */
    void hv_set_page_address(uint8_t start, uint8_t end);
    
    /** Set column address range for horizontal/vertical addressing mode.
      * @param start Column start address, 0 - 127.
      * @param end Column end address, 0 - 127.
      */
    void hv_set_column_address(uint8_t start, uint8_t end);
    
    // ----- TIMING & DRIVING -----
    /** Set the display clock divide ratio and the oscillator frequency.
      * @param ratio The divide ratio, default is 0.
      * @param frequency The oscillator frequency, 0 - 127. Default is 8.  
      */
    void set_display_clock_ratio_and_frequency(uint8_t ratio, uint8_t frequency);
    
    /** Set the precharge period.
      * @param phase1 Phase 1 period in DCLK clocks.  1 - 15, default is 2.
      * @param phase2 Phase 2 period in DCLK clocks.  1 - 15, default is 2.
      */
    void set_precharge_period(uint8_t phase1, uint8_t phase2);
    
    /** Set the Vcomh deselect level.
      * @param level 0 = 0.65 x Vcc, 1 = 0.77 x Vcc (default), 2 = 0.83 x Vcc.
      */
    void set_vcomh_deselect_level(uint8_t level);
    
    /** Perform a "no operation".
      */
    void nop();
    
    /** Enable/disable charge pump.
      @param enable 0 to disable, 1 to enable the internal charge pump.
      */
    void set_charge_pump_enable(uint8_t enable);
    
    // ----- BUFFER EDITING -----

    void clear();
    void set_pixel(int x, int y);
    void clear_pixel(int x, int y);

    void test(void);

protected:

    void _send_command(uint8_t cmd);
    void _send_command(uint8_t* cmd, uint16_t len);
    void _send_data(uint8_t* data, uint16_t len);

protected:

    /** I2C instance connected to SSD1306 driver */
    mbed::I2C* _i2c;

    /** Width of underlying display */
    uint16_t _width;

    /** Height of underlying display */
    uint16_t _height;

};

#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64

#endif
