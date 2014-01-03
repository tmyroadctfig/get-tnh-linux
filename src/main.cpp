//==========================================================================
// Get reading from USB-TnH
//==========================================================================
//  Copyright (c) 2012 DogRatIan.  All rights reserved.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//==========================================================================
// Naming conventions
// ~~~~~~~~~~~~~~~~~~
//                Class : Leading C
//               Struct : Leading T
//             Constant : Leading K
//      Global Variable : Leading g
//    Function argument : Leading a
//       Local Variable : All lower case
//==========================================================================
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>

#if (SYSTEM_MSW)
#include <windows.h>
#endif

#include "debug.h"
#include "utils.h"
#include "main.h"
#include "serial.h"
#include "safe_string.h"

//==========================================================================
// Defines
//==========================================================================
// ms
#define TIMEOUT_WAIT_RESP       200

#define MIN_RELAY_IDX           1
#define MAX_RELAY_IDX           4

// relay state
enum
{
    RELAY_ASK = 0,
    RELAY_SET,
    RELAY_CLEAR,
};

//==========================================================================
// Prototypes
//==========================================================================
int Go (void);

//==========================================================================
// Variables
//==========================================================================
static int gArgCnt;
static char **gArgList;
static const char *gDevice = NULL;
static int gVerbose = 0;
static CSerial gSerial;
static bool gReadTemperature = true;
static bool gReadHumidity = false;
static int gLed = -1;
static const char *gName = NULL;

//==========================================================================
// Main Entry
//==========================================================================
int main (int aArgc, char *aArgv[])
{
    // Non buffered output
    setvbuf (stdout, NULL, _IONBF, 0);

    DEBUG_INIT ();

    // Start the main
    gArgCnt = aArgc;
    gArgList = aArgv;

    int ret;
    ret = Go ();

    DEBUG_EXIT ();

    return ret;
}

//==========================================================================
// Convert string to upper case
//==========================================================================
static void StringToUpper (char *aSrc)
{
    while (*aSrc)
    {
        if ((*aSrc >= 'a') && (*aSrc <= 'z'))
            *aSrc = *aSrc - 'a' + 'A';
        aSrc ++;
    }
}


//==========================================================================
// Show Program info
//==========================================================================
static void ShowProgramInfo (void)
{
    printf (STR_PROGNAME " " STR_VERSION);
    if (DEBUG)
        printf (" DEBUG");
    printf (" (build on " __DATE__ " " __TIME__ ")");
    printf (" " STR_COPYRIGHT "\n");
}

//==========================================================================
// Show help
//==========================================================================
static void ShowHelp (void)
{
    printf ("\n");
    printf ("Usage: %s [options]\n", FileNameOnly (gArgList[0]));
    printf ("Options:\n"
                 "  --help                          Show this help.\n"
                 "  -v, --verbose                   Verbose mode.\n"
                 "  -d DEVICE, --device=DEVICE      Set device / port.\n"
                 "  -t, --temperature               Read Temperature (default)\n"
                 "  -h, --humidity                  Read Humidity\n"
                 "  -a, --all                       Read both Temperature and Humidity\n"
                 "  -l ONOFF, --led=ONOFF           Set LED ON(1) or OFF(0).\n"
                 "  -n NAME, --name=NAME            Set sensor name. Use --name=? to read existing name.\n"
                 );
    printf ("\n");
    printf ("\n");
}

//==========================================================================
// Extract operation parameters from command line
//==========================================================================
static struct option KLongOptions[] =
{
    { "help",           no_argument,        NULL, '?'},
    { "verbose",        no_argument,        NULL, 'v'},
    { "device",         required_argument,  NULL, 'd'},
    { "temperature",    no_argument,        NULL, 't'},
    { "humidity",       no_argument,        NULL, 'h'},
    { "all",            no_argument,        NULL, 'a'},
    { "led",            required_argument,  NULL, 'l'},
    { "name",           required_argument,  NULL, 'n'},
    { NULL, 0, NULL, 0}
};

static int ExtractParam (void)
{
    int i;

    while (1)
    {
        i = getopt_long (gArgCnt, gArgList, "vd:thal:n:", KLongOptions, NULL);
        if (i == EOF)
            break;
        switch (i)
        {
            case 't':
                gReadTemperature    = true;
                gReadHumidity       = false;
                break;

            case 'h':
                gReadTemperature    = false;
                gReadHumidity       = true;
                break;

            case 'a':
                gReadTemperature    = true;
                gReadHumidity       = true;
                break;

            case 'd':
                gDevice = optarg;
                break;

            case 'l':
                gLed = atoi (optarg);
                break;

            case 'n':
                gName = optarg;
                break;

            case 'v':
                gVerbose = 1;
                break;

            default:
                return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

//==========================================================================
// Get Temperature
//==========================================================================
static int GetTemperature (void)
{
    unsigned char rx_buf[128];
    int rx_len;
    float f;

    gSerial.Write ((const unsigned char *)"\n", 1);
    gSerial.Flush ();

    if (gSerial.Write ((const unsigned char *)"GT\n", 3) < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Write\n");
        }
        else
            printf ("NaN\n");
        return -1;
    }

#if (SYSTEM_MSW)
        Sleep (1);
#else
        usleep (500000);
#endif

    rx_len = gSerial.Read (rx_buf, sizeof (rx_buf) - 1);
    if (rx_len < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Read\n");
        }
        else
            printf ("NaN\n");
        return -1;
    }

    if (rx_len == 0)
    {
        if (gVerbose)
            printf ("ERROR: No response.\n");
        else
            printf ("NaN\n");
        return -1;
    }
    rx_buf[rx_len] = 0;

    f = atof ((char *)rx_buf);

    printf ("%.2f\n", f);

    return 0;
}

//==========================================================================
// Get Humidity
//==========================================================================
static int GetHumidity (void)
{
    unsigned char rx_buf[128];
    int rx_len;
    float f;

    gSerial.Write ((const unsigned char *)"\n", 1);
    gSerial.Flush ();

    if (gSerial.Write ((const unsigned char *)"GH\n", 3) < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Write\n");
        }
        else
            printf ("NaN\n");
        return -1;
    }

#if (SYSTEM_MSW)
        Sleep (1);
#else
        usleep (500000);
#endif

    rx_len = gSerial.Read (rx_buf, sizeof (rx_buf) - 1);
    if (rx_len < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Read\n");
        }
        else
            printf ("NaN\n");
        return -1;
    }

    if (rx_len == 0)
    {
        if (gVerbose)
            printf ("ERROR: No response.\n");
        else
            printf ("NaN\n");
        return -1;
    }
    rx_buf[rx_len] = 0;

    f = atof ((char *)rx_buf);

    printf ("%.2f\n", f);

    return 0;
}

//==========================================================================
// Set LED
//==========================================================================
static int SetLed (int aState)
{
    unsigned char tx_buf[64];
    unsigned char rx_buf[128];
    int rx_len;

    gSerial.Write ((const unsigned char *)"\n", 1);
    gSerial.Flush ();

    if (aState)
        memcpy (tx_buf, "I=1\n", 4);
    else
        memcpy (tx_buf, "I=0\n", 4);

    if (gSerial.Write (tx_buf, 4) < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Write\n");
        }
        return -1;
    }

#if (SYSTEM_MSW)
        Sleep (1);
#else
        usleep (500000);
#endif

    rx_len = gSerial.Read (rx_buf, sizeof (rx_buf) - 1);
    if (rx_len < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Read\n");
        }
        return -1;
    }

    if (rx_len == 0)
    {
        if (gVerbose)
            printf ("ERROR: No response.\n");
        return -1;
    }

    if (gVerbose)
        printf ("Change LED state done.\n");

    return 0;
}

//==========================================================================
// Get Sensor Name
//==========================================================================
static int GetName (void)
{
    unsigned char rx_buf[128];
    int rx_len;
    unsigned char ch;

    gSerial.Write ((const unsigned char *)"\n", 1);
    gSerial.Flush ();

    if (gSerial.Write ((const unsigned char *)"GN\n", 3) < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Write\n");
        }
        else
            printf ("Unknown\n");
        return -1;
    }

#if (SYSTEM_MSW)
        Sleep (1);
#else
        usleep (500000);
#endif

    rx_len = gSerial.Read (rx_buf, sizeof (rx_buf) - 1);
    if (rx_len < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Read\n");
        }
        else
            printf ("Unknown\n");
        return -1;
    }

    if (rx_len == 0)
    {
        if (gVerbose)
            printf ("ERROR: No response.\n");
        else
            printf ("Unknown\n");
        return -1;
    }
    rx_buf[rx_len] = 0;

    // Trim tail
    do
    {
        ch = rx_buf[rx_len - 1];
        if ((ch == ' ') || (ch == '\n') || (ch == '\r') || (ch == '\t'))
            ch = 0;

        if (ch == rx_buf[rx_len - 1])
            break;

        rx_len --;
        rx_buf[rx_len] = ch;

    } while (rx_len > 1);


    printf ("%s\n", rx_buf);

    return 0;
}

//==========================================================================
// Set Name
//==========================================================================
static int SetName (void)
{
    const char *ptr;
    unsigned char tx_buf[64];
    unsigned char rx_buf[128];
    int rx_len;
    int tx_len;
    unsigned char ch;

    gSerial.Write ((const unsigned char *)"\n", 1);
    gSerial.Flush ();

    // Form TX buffer
    tx_buf[0] = 'N';
    tx_buf[1] = '=';

    ptr = gName;
    tx_len = 2;
    while ((*ptr) && (tx_len < 10))
    {
        ch = *ptr;
        if ((ch == ' ') || (ch == '\n') || (ch == '\r') || (ch == '\t'))
            break;

        tx_buf[tx_len] = ch;
        ptr ++;
        tx_len ++;
    }
    tx_buf[tx_len] = '\n';
    tx_len ++;

    if (gSerial.Write (tx_buf, tx_len) < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Write\n");
        }
        return -1;
    }

#if (SYSTEM_MSW)
        Sleep (1);
#else
        usleep (500000);
#endif

    rx_len = gSerial.Read (rx_buf, sizeof (rx_buf) - 1);
    if (rx_len < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Read\n");
        }
        return -1;
    }

    if (rx_len == 0)
    {
        if (gVerbose)
            printf ("ERROR: No response.\n");
        return -1;
    }

    printf ("Set sensor name to '%s' done.\n", gName);

    return 0;
}


//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
// Start of process
//==========================================================================
int Go (void)
{
    char buf[64];

    // Extract cmd-line parameter
    if (ExtractParam () == EXIT_FAILURE)
    {
        ShowProgramInfo ();
        ShowHelp ();
        return EXIT_FAILURE;
    }
    if (gDevice == NULL)
    {
        ShowProgramInfo ();
        ShowHelp ();
        return EXIT_FAILURE;
    }


    if (gVerbose)
        ShowProgramInfo ();

    // Open Serial port
    safe_strncpy (buf, gDevice, sizeof (buf));
    if (SYSTEM_MSW)
    {
        StringToUpper (buf);
    }
    if (gSerial.Open (buf, "115200 N81") < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("%s: %s\n", buf, gSerial.ErrorMessage);
            else
                printf ("%s: OpenPort fail.\n", buf);
        }
        else
            printf ("NaN\n");
        return EXIT_FAILURE;
    }

    // Set LED
    if (gLed == 0)
    {
        SetLed (0);
    }
    else if (gLed == 1)
    {
        SetLed (1);
    }

    // Sensor Name
    if (gName != NULL)
    {
        if (strcmp (gName, "?") == 0)
        {
            // Get Name
            if (GetName () < 0)
                return EXIT_FAILURE;
        }
        else
        {
            // Set Name
            if (SetName () < 0)
                return EXIT_FAILURE;
            else
                return EXIT_SUCCESS;
        }
    }

    // Read Temperature
    if (gReadTemperature)
    {
        if (GetTemperature () < 0)
            return EXIT_FAILURE;
    }

    // Read Humidity
    if (gReadHumidity)
    {
        if (GetHumidity () < 0)
            return EXIT_FAILURE;
    }

    //
    return EXIT_SUCCESS;
}
