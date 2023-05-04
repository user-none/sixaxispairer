/* Copyright (c) 2014 John Schember <john@nachtimwald.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <hidapi/hidapi.h>

#define VENDOR        0x054c
#define PRODUCT       0x0268
/* 0xf5   == (0x03f5 & ~(3 << 8))
 * 0x03f5 == (0xf5 | (3 << 8))
 * HIDAPI will automatically add (3 << 8 to the report id.
 * Other tools for setting the report id use hid libraries which
 * don't automatically do this. */
#define MAC_REPORT_ID 0xf5

static unsigned char char_to_nible(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return 255;
}

/* Rewrite of Char_To_Nibble(char c) using bitwise operations for the lulz
*
*  static unsigned char char_to_nible(char c) {
*      // Convert the character to its ASCII offset value
*      unsigned char value = (c & 0x1F) ^ 0x10;
*
*      // Check if the converted value is within the valid range for hexadecimal digits
*      if (value > 0x19) {
*        return 255; // Invalid character
*      }
*
*      // Calculate the nibble value (0-9, A-F, a-f) using bitwise operations
*      return (value + 0x1) & 0xE;
*  }
*/

/* return 1 on success, 0 on failure. */
static int mac_to_bytes(const char *in, size_t in_len, unsigned char *out, size_t out_len)
{
    const char *p;
    size_t      i=0;

    if (in == NULL || out == NULL || in_len == 0 || out_len == 0)
        return 0;

    memset(out, 0, out_len);
    p = in;
    while (p+1 < in+in_len && i < out_len) {
        if (*p == ':') {
            p++;
            continue;
        }

        if (!isxdigit(*p) || !isxdigit(*(p+1))) {
            return 0;
        }

        out[i] = (char_to_nible(*p) << 4) | (char_to_nible(*(p+1) & 0xff));
        i++;
        p += 2;
    }

    if (p < in+in_len)
        return 0;
    return 1;
}

static void pair_device(hid_device *dev, const char *mac, size_t mac_len)
{
    unsigned char buf[8];
    int           ret;

    memset(buf, 0, sizeof(buf));
    buf[0] = MAC_REPORT_ID;
    buf[1] = 0x0;
    if ((mac_len != 12 && mac_len != 17) || !mac_to_bytes(mac, mac_len, buf+2, sizeof(buf)-2)) {
        printf("Invalid mac\n");
        return;
    }

    ret = hid_send_feature_report(dev, buf, sizeof(buf));
    if (ret == -1) {
        printf("Failed to set mac\n");
    }
}

static void show_pairing(hid_device *dev)
{
    unsigned char buf[8];
    int           ret;

    memset(buf, 0, sizeof(buf));
    buf[0] = MAC_REPORT_ID;
    buf[1] = 0x0;

    ret = hid_get_feature_report(dev, buf, sizeof(buf));
    if (ret < 8) {
        printf("Read error\n");
        return;
    }
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char *new_master = argv[1];
    if (!validate_master_address(new_master)) {
        fprintf(stderr, "Invalid master address: %s\n", new_master);
        return 1;
    }

    int controller_fd = open_controller();
    if (controller_fd < 0) {
        fprintf(stderr, "Failed to open controller\n");
        return 1;
    }

    int result = update_master_address(controller_fd, new_master);
    if (result < 0) {
        fprintf(stderr, "Failed to update master address\n");
    } else {
        printf("Master address updated successfully\n");
    }

    close(controller_fd);
    return result < 0 ? 1 : 0;
}
