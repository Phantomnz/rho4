/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *          View this license at http://creativecommons.org/about/licenses/
 */

#include "spi.h"

void init_pot(void)
{
    init_spi_master();
    /* TODO: Code to initiliase interface */
}

void decXpot(void)
{
    spi_tx(0b00001000);
    /* TODO: Code to decrement first pot by 1 */
}

void incXpot(void)
{
    spi_tx(0b00000100);
    /* TODO: Code to increment first pot by 1 */
}

void decYpot(void)
{
    spi_tx(0b00011000);
    /* TODO: Code to decrement second pot by 1 */
}

void incYpot(void)
{
    spi_tx(0b00010100);
    /* TODO: Code to increment second pot by 1 */
}

void setXpot(uint8_t x)
{   
    spi_tx(0x00);
    spi_tx(x);
    /* TODO: Code to set first pot to a value between 0 and 255 */
}

void setYpot(uint8_t y)
{   
    spi_tx(0x10);
    spi_tx(y);
    /* TODO: Code to set second pot to a value between 0 and 255 */
}
