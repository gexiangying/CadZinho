#include "gui_use.h"

int gui_spline_interactive(gui_obj *gui){
	if (gui->modal == SPLINE){
		static dxf_node *new_el;
		
		if (gui->step == 0){
			if (gui->ev & EV_ENTER){
				/* create a new DXF spline */
				new_el = (dxf_node *) dxf_new_lwpolyline (
					gui->step_x[gui->step], gui->step_y[gui->step], 0.0, /* pt1, */
					0.0,//gui->bulge, /* bulge */
					gui->color_idx, gui->drawing->layers[gui->layer_idx].name, /* color, layer */
					gui->drawing->ltypes[gui->ltypes_idx].name, dxf_lw[gui->lw_idx], /* line type, line weight */
					0, DWG_LIFE); /* paper space */
				dxf_lwpoly_append (new_el, gui->step_x[gui->step], gui->step_y[gui->step], 0.0, gui->bulge, DWG_LIFE);
				dxf_attr_change(new_el, 70, &gui->closed);
				gui->element = new_el;
				gui->step = 1;
				gui->en_distance = 1;
				gui->draw_tmp = 1;
				gui_next_step(gui);
			}
			else if (gui->ev & EV_CANCEL){
				gui_default_modal(gui);
			}
		}
		else{
			if (gui->ev & EV_ENTER){
				gui->step_x[gui->step - 1] = gui->step_x[gui->step];
				gui->step_y[gui->step - 1] = gui->step_y[gui->step];
				
				dxf_attr_change_i(new_el, 10, &gui->step_x[gui->step], -1);
				dxf_attr_change_i(new_el, 20, &gui->step_y[gui->step], -1);
				dxf_attr_change_i(new_el, 42, &gui->bulge, -1);
				
				new_el->obj.graphics = dxf_graph_parse(gui->drawing, new_el, 0 , 1);
				graph_list_change_patt(new_el->obj.graphics, (double[]){10/gui->zoom,-10/gui->zoom}, 2);
				
				dxf_lwpoly_append (new_el, gui->step_x[gui->step], gui->step_y[gui->step], 0.0, gui->bulge, DWG_LIFE);
				gui->step = 2;
				gui_next_step(gui);
			}
			else if (gui->ev & EV_CANCEL){
				gui->draw_tmp = 0;
				gui->draw_phanton = 0;
				if (gui->step == 2){
					dxf_lwpoly_remove (new_el, -1);
					
					if(gui->closed){
						dxf_attr_append(new_el, 42, (void *) &gui->bulge, DWG_LIFE);
					}
					
					dxf_node *spline =  dxf_new_spline (new_el, gui->sp_degree - 1,
						gui->color_idx, gui->drawing->layers[gui->layer_idx].name, /* color, layer */
						gui->drawing->ltypes[gui->ltypes_idx].name, dxf_lw[gui->lw_idx], /* line type, line weight */
						0, DWG_LIFE); /* paper space */
					
					if (spline){
						spline->obj.graphics = dxf_graph_parse(gui->drawing, spline, 0 , 0);
						drawing_ent_append(gui->drawing, spline);
						
						do_add_entry(&gui->list_do, "SPLINE");
						do_add_item(gui->list_do.current, NULL, spline);
					}
					gui->step = 0;
				}
				gui->element = NULL;
				gui_first_step(gui);
			}
			if (gui->ev & EV_MOTION){
				dxf_attr_change(new_el, 6, gui->drawing->ltypes[gui->ltypes_idx].name);
				dxf_attr_change(new_el, 8, gui->drawing->layers[gui->layer_idx].name);
				dxf_attr_change_i(new_el, 10, &gui->step_x[gui->step], -1);
				dxf_attr_change_i(new_el, 20, &gui->step_y[gui->step], -1);
				//dxf_attr_change_i(new_el, 42, &gui->bulge, -1);
				dxf_attr_change(new_el, 370, &dxf_lw[gui->lw_idx]);
				dxf_attr_change(new_el, 62, &gui->color_idx);
				dxf_attr_change(new_el, 70, &gui->closed);
				
				new_el->obj.graphics = dxf_graph_parse(gui->drawing, new_el, 0 , 1);
				graph_list_change_patt(new_el->obj.graphics, (double[]){10/gui->zoom,-10/gui->zoom}, 2);
				
			}
			dxf_node *spline =  dxf_new_spline (new_el, gui->sp_degree - 1,
				gui->color_idx, gui->drawing->layers[gui->layer_idx].name, /* color, layer */
				gui->drawing->ltypes[gui->ltypes_idx].name, dxf_lw[gui->lw_idx], /* line type, line weight */
				0, FRAME_LIFE); /* paper space */
			if (spline){
				gui->draw_phanton = 1;
				gui->phanton = dxf_graph_parse(gui->drawing, spline, 0 , FRAME_LIFE);
			}
			else{
				gui->draw_phanton = 0;
			}
	}
	}
	return 1;
}

int gui_spline_info (gui_obj *gui){
	if (gui->modal == SPLINE) {
		nk_layout_row_dynamic(gui->ctx, 20, 1);
		nk_label(gui->ctx, "Place a spline", NK_TEXT_LEFT);
		if (gui->step == 0){
			nk_label(gui->ctx, "Enter first point", NK_TEXT_LEFT);
		} else {
			nk_label(gui->ctx, "Enter next point", NK_TEXT_LEFT);
		}
		nk_property_int(gui->ctx, "Degree", 2, &gui->sp_degree, 15, 1, 0.1);
		nk_checkbox_label(gui->ctx, "Closed", &gui->closed);
		
	}
	return 1;
}