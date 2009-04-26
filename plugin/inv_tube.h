/* 

    (c) Fraser Stuart 2009

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#define ITUBE_GUI_URI		"http://invadarecords.com/plugins/lv2/tube/gui"
#define ITUBE_MONO_URI		"http://invadarecords.com/plugins/lv2/tube/mono"
#define ITUBE_STEREO_URI	"http://invadarecords.com/plugins/lv2/tube/stereo"
#define ITUBE_DRIVE 		0
#define ITUBE_DCOFFSET		1
#define ITUBE_PHASE		2
#define ITUBE_MIX		3
#define ITUBE_METER_DRIVE 	4
#define ITUBE_METER_INL  	5
#define ITUBE_METER_OUTL 	6
#define ITUBE_AUDIO_INL  	7
#define ITUBE_AUDIO_OUTL 	8
#define ITUBE_METER_INR  	9  /* not used in mono mode */
#define ITUBE_METER_OUTR 	10 /* not used in mono mode */
#define ITUBE_AUDIO_INR  	11 /* not used in mono mode */
#define ITUBE_AUDIO_OUTR 	12 /* not used in mono mode */



/* control conversion function */
float convertParam(unsigned long param, float value, double sr);

