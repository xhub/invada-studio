/* 

    This LV2 extension provides lpf and hpf gui's

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

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gtk/gtk.h>
#include <lv2.h>
#include "lv2_ui.h"
#include "widgets/knob.h"
#include "widgets/meter.h"
#include "widgets/meter-phase.h"
#include "../plugin/inv_input.h"
#include "inv_input_gui.h"


static LV2UI_Descriptor *IInputGuiDescriptor = NULL;

typedef struct {
	GtkWidget	*windowContainer;
	GtkWidget	*heading;
	GtkWidget	*meterIn;
	GtkWidget	*meterOut;
	GtkWidget	*meterPhase;
	GtkWidget	*knobGain;
	GtkWidget	*knobPan;
	GtkWidget	*knobWidth;
//	GtkWidget	*switchandlightClip;

	gint		InChannels;
	gint		OutChannels;
	float		gain;
	float		pan;
	float		width;

	LV2UI_Write_Function 	write_function;
	LV2UI_Controller 	controller;

} IInputGui;



static LV2UI_Handle instantiateIInputGui(const struct _LV2UI_Descriptor* descriptor, const char* plugin_uri, const char* bundle_path, LV2UI_Write_Function write_function, LV2UI_Controller controller, LV2UI_Widget* widget, const LV2_Feature* const* features)
{

	IInputGui *pluginGui = (IInputGui *)malloc(sizeof(IInputGui));
	if(pluginGui==NULL)
		return NULL;

	pluginGui->write_function = write_function;
	pluginGui->controller = controller;

	GtkBuilder      *builder; 
	GtkWidget       *window;
	GtkWidget	*tempObject;

	GError *err = NULL;

	gtk_init (NULL,NULL);

	builder = gtk_builder_new ();
// TODO change this to use the supplied bundle path
	gtk_builder_add_from_file (builder, "/usr/local/lib/lv2/invada.lv2/gtk/inv_input_gui.xml", &err);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "input_window"));

	/* get pointers to some useful widgets from the design */
	pluginGui->windowContainer = GTK_WIDGET (gtk_builder_get_object (builder, "input_container"));
	pluginGui->heading = GTK_WIDGET (gtk_builder_get_object (builder, "label_heading"));

	/* add custom widgets */
	tempObject=GTK_WIDGET (gtk_builder_get_object (builder, "alignment_meter_in"));
	pluginGui->meterIn = inv_meter_new ();
	gtk_container_add (GTK_CONTAINER (tempObject), pluginGui->meterIn);

	tempObject=GTK_WIDGET (gtk_builder_get_object (builder, "alignment_meter_out"));
	pluginGui->meterOut = inv_meter_new ();
	gtk_container_add (GTK_CONTAINER (tempObject), pluginGui->meterOut);

	tempObject=GTK_WIDGET (gtk_builder_get_object (builder, "alignment_meter_phase"));
	pluginGui->meterPhase = inv_phase_meter_new ();
	gtk_container_add (GTK_CONTAINER (tempObject), pluginGui->meterPhase);

	tempObject=GTK_WIDGET (gtk_builder_get_object (builder, "alignment_gain_knob"));
	pluginGui->knobGain = inv_knob_new ();
	gtk_container_add (GTK_CONTAINER (tempObject), pluginGui->knobGain);

	tempObject=GTK_WIDGET (gtk_builder_get_object (builder, "alignment_pan_knob"));
	pluginGui->knobPan = inv_knob_new ();
	gtk_container_add (GTK_CONTAINER (tempObject), pluginGui->knobPan);

	tempObject=GTK_WIDGET (gtk_builder_get_object (builder, "alignment_width_knob"));
	pluginGui->knobWidth = inv_knob_new ();
	gtk_container_add (GTK_CONTAINER (tempObject), pluginGui->knobWidth);

	pluginGui->InChannels=2;
	pluginGui->OutChannels=2;
	pluginGui->gain=0.0;
	pluginGui->pan=0.0;
	pluginGui->width=0.0;

	inv_meter_set_channels(INV_METER (pluginGui->meterIn), pluginGui->InChannels);
	inv_meter_set_LdB(INV_METER (pluginGui->meterIn),-90);
	inv_meter_set_RdB(INV_METER (pluginGui->meterIn),-90);

	inv_meter_set_channels(INV_METER (pluginGui->meterOut), pluginGui->OutChannels);
	inv_meter_set_LdB(INV_METER (pluginGui->meterOut),-90);
	inv_meter_set_RdB(INV_METER (pluginGui->meterOut),-90);

	inv_phase_meter_set_phase(INV_PHASE_METER (pluginGui->meterPhase),0);

	inv_knob_set_size(INV_KNOB (pluginGui->knobGain), INV_KNOB_SIZE_MEDIUM);
	inv_knob_set_curve(INV_KNOB (pluginGui->knobGain), INV_KNOB_CURVE_LINEAR);
	inv_knob_set_markings(INV_KNOB (pluginGui->knobGain), INV_KNOB_MARKINGS_5);
	inv_knob_set_highlight(INV_KNOB (pluginGui->knobGain), INV_KNOB_HIGHLIGHT_L);
	inv_knob_set_units(INV_KNOB (pluginGui->knobGain), "dB");
	inv_knob_set_min(INV_KNOB (pluginGui->knobGain), -24.0);
	inv_knob_set_max(INV_KNOB (pluginGui->knobGain), 24.0);
	inv_knob_set_value(INV_KNOB (pluginGui->knobGain), pluginGui->gain);
	g_signal_connect_after(G_OBJECT(pluginGui->knobGain),"motion-notify-event",G_CALLBACK(on_inv_input_gain_knob_motion),pluginGui);

	inv_knob_set_size(INV_KNOB (pluginGui->knobPan), INV_KNOB_SIZE_MEDIUM);
	inv_knob_set_curve(INV_KNOB (pluginGui->knobPan), INV_KNOB_CURVE_LINEAR);
	inv_knob_set_markings(INV_KNOB (pluginGui->knobPan), INV_KNOB_MARKINGS_PAN); 
	inv_knob_set_highlight(INV_KNOB (pluginGui->knobGain), INV_KNOB_HIGHLIGHT_C);
	inv_knob_set_units(INV_KNOB (pluginGui->knobPan), "");
	inv_knob_set_min(INV_KNOB (pluginGui->knobPan), -1.0);
	inv_knob_set_max(INV_KNOB (pluginGui->knobPan), 1.0);
	inv_knob_set_value(INV_KNOB (pluginGui->knobPan), pluginGui->pan);
	g_signal_connect_after(G_OBJECT(pluginGui->knobPan),"motion-notify-event",G_CALLBACK(on_inv_input_pan_knob_motion),pluginGui);

	inv_knob_set_size(INV_KNOB (pluginGui->knobWidth), INV_KNOB_SIZE_MEDIUM);
	inv_knob_set_curve(INV_KNOB (pluginGui->knobWidth), INV_KNOB_CURVE_LINEAR);
	inv_knob_set_markings(INV_KNOB (pluginGui->knobWidth), INV_KNOB_MARKINGS_CUST10); 
	inv_knob_set_highlight(INV_KNOB (pluginGui->knobGain), INV_KNOB_HIGHLIGHT_C);
	inv_knob_set_units(INV_KNOB (pluginGui->knobWidth), "");
	inv_knob_set_custom(INV_KNOB (pluginGui->knobWidth), 0, "Mono");
	inv_knob_set_custom(INV_KNOB (pluginGui->knobWidth), 1, "Normal");
	inv_knob_set_custom(INV_KNOB (pluginGui->knobWidth), 2, "Stereo");
	inv_knob_set_min(INV_KNOB (pluginGui->knobWidth), -1.0);
	inv_knob_set_max(INV_KNOB (pluginGui->knobWidth), 1.0);
	inv_knob_set_value(INV_KNOB (pluginGui->knobWidth), pluginGui->width);
	g_signal_connect_after(G_OBJECT(pluginGui->knobWidth),"motion-notify-event",G_CALLBACK(on_inv_input_width_knob_motion),pluginGui);

	/* strip the parent window from the design so the host can attach its own */
	gtk_widget_ref(pluginGui->windowContainer);
	gtk_container_remove(GTK_CONTAINER(window), pluginGui->windowContainer);

	*widget = (LV2UI_Widget) pluginGui->windowContainer;

	g_object_unref (G_OBJECT (builder));
 
	/* return the instance */
	return pluginGui;
}


static void cleanupIInputGui(LV2UI_Handle ui)
{
	return;
}


static void port_eventIInputGui(LV2UI_Handle ui, uint32_t port, uint32_t buffer_size, uint32_t format, const void*  buffer)
{
	IInputGui *pluginGui = (IInputGui *)ui;

	float value;

	if(format==0) 
	{
		value=* (float *) buffer;
		switch(port)
		{
			case IINPUT_GAIN:
				pluginGui->gain=value;
				inv_knob_set_value(INV_KNOB (pluginGui->knobGain), pluginGui->gain);
				break;
			case IINPUT_PAN:
				pluginGui->pan=value;
				inv_knob_set_value(INV_KNOB (pluginGui->knobPan), pluginGui->pan);
				break;
			case IINPUT_WIDTH:
				pluginGui->width=value;
				inv_knob_set_value(INV_KNOB (pluginGui->knobWidth), pluginGui->width);
				break;
			case IINPUT_METER_INL:
				inv_meter_set_LdB(INV_METER (pluginGui->meterIn),value);
				break;
			case IINPUT_METER_INR:
				inv_meter_set_RdB(INV_METER (pluginGui->meterIn),value);
				break;
			case IINPUT_METER_OUTL:
				inv_meter_set_LdB(INV_METER (pluginGui->meterOut),value);
				break;
			case IINPUT_METER_OUTR:
				inv_meter_set_RdB(INV_METER (pluginGui->meterOut),value);
				break;
			case IINPUT_METER_PHASE:
				inv_phase_meter_set_phase(INV_PHASE_METER (pluginGui->meterPhase),value);
				break;
		}
	}
}


static void init()
{
	IInputGuiDescriptor =
	 (LV2UI_Descriptor *)malloc(sizeof(LV2UI_Descriptor));

	IInputGuiDescriptor->URI 		= IINPUT_GUI_URI;
	IInputGuiDescriptor->instantiate 	= instantiateIInputGui;
	IInputGuiDescriptor->cleanup		= cleanupIInputGui;
	IInputGuiDescriptor->port_event		= port_eventIInputGui;
	IInputGuiDescriptor->extension_data 	= NULL;

}


const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
	if (!IInputGuiDescriptor) init();

	switch (index) {
		case 0:
			return IInputGuiDescriptor;
	default:
		return NULL;
	}
}


/*****************************************************************************/


static void on_inv_input_gain_knob_motion(GtkWidget *widget, GdkEvent *event, gpointer data)
{

	IInputGui *pluginGui = (IInputGui *) data;

	pluginGui->gain=inv_knob_get_value(INV_KNOB (widget));
	(*pluginGui->write_function)(pluginGui->controller, IINPUT_GAIN, 4, 0, &pluginGui->gain);
	return;
}

static void on_inv_input_pan_knob_motion(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	IInputGui *pluginGui = (IInputGui *) data;

	pluginGui->pan=inv_knob_get_value(INV_KNOB (widget));
	(*pluginGui->write_function)(pluginGui->controller, IINPUT_PAN, 4, 0, &pluginGui->pan);
	return;
}

static void on_inv_input_width_knob_motion(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	IInputGui *pluginGui = (IInputGui *) data;

	pluginGui->width=inv_knob_get_value(INV_KNOB (widget));
	(*pluginGui->write_function)(pluginGui->controller, IINPUT_WIDTH, 4, 0, &pluginGui->width);
	return;
}

