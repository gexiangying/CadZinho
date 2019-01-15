#include "gui.h"

int tstyles_mng (gui_obj *gui){
	int i, show_tstyle_mng = 1;
	static int show_color_pick = 0, show_name = 0;
	static int sel_t_sty, t_sty_idx, sel_font;
	
	static struct sort_by_idx sort_t_sty[DXF_MAX_LAYERS];
	dxf_tstyle *t_sty = gui->drawing->text_styles;
	
	gui->next_win_x += gui->next_win_w + 3;
	//gui->next_win_y += gui->next_win_h + 3;
	gui->next_win_w = 810;
	gui->next_win_h = 310;
	
	enum tstyle_op {
		TSTYLE_OP_NONE,
		TSTYLE_OP_CREATE,
		TSTYLE_OP_RENAME,
		TSTYLE_OP_UPDATE
	};
	static int tstyle_change = TSTYLE_OP_UPDATE;
	
	//if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "Info", NK_WINDOW_CLOSABLE, nk_rect(310, 50, 200, 300))){
	if (nk_begin(gui->ctx, "Text Styles Manager", nk_rect(gui->next_win_x, gui->next_win_y, gui->next_win_w, gui->next_win_h),
	NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
	NK_WINDOW_CLOSABLE|NK_WINDOW_TITLE)){
		static char sty_name[DXF_MAX_CHARS] = "";
		
		t_sty = gui->drawing->text_styles;
		
		nk_layout_row_dynamic(gui->ctx, 32, 1);
		if (nk_group_begin(gui->ctx, "tstyle_head", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
			nk_layout_row(gui->ctx, NK_STATIC, 22, 8, (float[]){175, 175, 175, 25, 25, 50, 50, 50});
			if (nk_button_label(gui->ctx, "Name")){
				
			}
			if (nk_button_label(gui->ctx, "File")){
				
			}
			if (nk_button_label(gui->ctx, "Subst")){
				
			}
			if (nk_button_label(gui->ctx, "F1")){
				
			}
			if (nk_button_label(gui->ctx, "F2")){
				
			}
			if (nk_button_label(gui->ctx, "W")){
				
			}
			if (nk_button_label(gui->ctx, "FH")){
				
			}
			if (nk_button_label(gui->ctx, "A")){
				
			}
			
			nk_group_end(gui->ctx);
		}
		nk_layout_row_dynamic(gui->ctx, 200, 1);
		if (nk_group_begin(gui->ctx, "tstyle_prop", NK_WINDOW_BORDER)) {
			
			nk_layout_row(gui->ctx, NK_STATIC, 22, 8, (float[]){175, 175, 175, 25, 25, 50, 50, 50});
			int num_tstyles = gui->drawing->num_tstyles;
			char txt[DXF_MAX_CHARS];
				
			for (i = 0; i < num_tstyles; i++){
				
				//t_sty_idx = sort_t_sty[i].idx;
				t_sty_idx = i;
				if (sel_t_sty == t_sty_idx){
					if (nk_button_label_styled(gui->ctx, &gui->b_icon_sel, t_sty[t_sty_idx].name)){
						sel_t_sty = -1;
					}
				}
				else {
					if (nk_button_label_styled(gui->ctx,&gui->b_icon_unsel, t_sty[t_sty_idx].name)){
						sel_t_sty = t_sty_idx;
					}
				}
				
				//nk_button_label_styled(gui->ctx, &gui->b_icon_unsel, t_sty[t_sty_idx].file);
				//list_node *list_get_idx(list_node * list, int idx)
				
				sel_font = -1;
				if (nk_combo_begin_label(gui->ctx,  t_sty[t_sty_idx].file, nk_vec2(200,200))){
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					int j = 0;
					list_node *curr_node = NULL;
					while (curr_node = list_get_idx(gui->font_list, j)){
						
						if (!(curr_node->data)) continue;
						struct tfont *curr_font = curr_node->data;
						
						if (nk_button_label(gui->ctx, curr_font->name)){
							sel_font = j;
							nk_combo_close(gui->ctx);
						}
						j++;
					}
					nk_combo_end(gui->ctx);
				}
				
				if (sel_font >= 0){
					list_node *curr_node = list_get_idx(gui->font_list, sel_font);
					if (curr_node) {
						if (curr_node->data) {
							struct tfont *curr_font = curr_node->data;
							strncpy(t_sty[t_sty_idx].file, curr_font->name, DXF_MAX_CHARS);
						
							dxf_attr_change(t_sty[t_sty_idx].obj, 3, curr_font->name);
						}
					}
				}
				
				
				nk_button_label_styled(gui->ctx, &gui->b_icon_unsel, t_sty[t_sty_idx].subst_file);
				
				snprintf(txt, DXF_MAX_CHARS, "%d", t_sty[t_sty_idx].flags1);
				nk_button_label_styled(gui->ctx, &gui->b_icon_unsel, txt);
				
				snprintf(txt, DXF_MAX_CHARS, "%d", t_sty[t_sty_idx].flags2);
				nk_button_label_styled(gui->ctx, &gui->b_icon_unsel, txt);
				
				snprintf(txt, DXF_MAX_CHARS, "%0.2f", t_sty[t_sty_idx].width_f);
				nk_button_label_styled(gui->ctx, &gui->b_icon_unsel, txt);
				
				snprintf(txt, DXF_MAX_CHARS, "%0.2f", t_sty[t_sty_idx].fixed_h);
				nk_button_label_styled(gui->ctx, &gui->b_icon_unsel, txt);
				
				snprintf(txt, DXF_MAX_CHARS, "%0.2f", t_sty[t_sty_idx].oblique);
				nk_button_label_styled(gui->ctx, &gui->b_icon_unsel, txt);
				
				
			}
			nk_group_end(gui->ctx);
		}
		
		nk_layout_row_dynamic(gui->ctx, 20, 3);
		if (nk_button_label(gui->ctx, "Create")){
			
			show_name = 1;
			sty_name[0] = 0;
			tstyle_change = TSTYLE_OP_CREATE;
		}
		if ((nk_button_label(gui->ctx, "Rename")) && (sel_t_sty >= 0)){
			/*show_lay_name = 1;
			strncpy(lay_name, layers[sel_lay].name, DXF_MAX_CHARS);
			lay_change = LAY_OP_RENAME;*/
			
		}
		if ((nk_button_label(gui->ctx, "Remove")) && (sel_t_sty >= 0)){
			/*if (layers[sel_lay].num_el){
				snprintf(gui->log_msg, 63, "Error: Don't remove Layer in use");
			}
			else{
				
				layer_use(gui->drawing);
				dxf_obj_subst(layers[sel_lay].obj, NULL);
				sel_lay = -1;
				dxf_layer_assemb (gui->drawing);
				lay_change = LAY_OP_UPDATE;
			}*/
		}
		
		if ((show_name)){
			if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "Style Name", NK_WINDOW_CLOSABLE, nk_rect(10, 20, 220, 100))){
				
				nk_layout_row_dynamic(gui->ctx, 20, 1);
				//nk_label(gui->ctx, "Layer Name:",  NK_TEXT_LEFT);
				/* set focus to edit */
				nk_edit_focus(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER|NK_EDIT_SELECTABLE|NK_EDIT_AUTO_SELECT);
				nk_edit_string_zero_terminated(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER|NK_EDIT_SELECTABLE|NK_EDIT_AUTO_SELECT, sty_name, DXF_MAX_CHARS, nk_filter_default);
				
				nk_layout_row_dynamic(gui->ctx, 20, 2);
				if (nk_button_label(gui->ctx, "OK")){
					if (tstyle_change == TSTYLE_OP_CREATE){
						if (!dxf_new_tstyle (gui->drawing, sty_name)){
							snprintf(gui->log_msg, 63, "Error: Text Style already exists");
						}
						else {
							nk_popup_close(gui->ctx);
							show_name = 0;
							tstyle_change = TSTYLE_OP_UPDATE;
						}
					}
					
					#if (0)
					else if ((lay_change == LAY_OP_RENAME) && (sel_lay >= 0)){
						/* verify if is not name of existing layer*/
						lay_exist = 0;
						for (i = 0; i < num_layers; i++){
							if (i != sel_lay){ /*except current layer*/
								if(strcmp(layers[i].name, lay_name) == 0){
									lay_exist = 1;
									break;
								}
							}
						}
						if (!lay_exist){
							//layer_rename(gui->drawing, sel_lay, lay_name);
							
							
							
							nk_popup_close(gui->ctx);
							show_name = 0;
							tstyle_change = TSTYLE_OP_UPDATE;
						}
						else snprintf(gui->log_msg, 63, "Error: exists Text Style with same name");
					}
					#endif
					else {
						nk_popup_close(gui->ctx);
						show_name = 0;
						tstyle_change = TSTYLE_OP_NONE;
					}
				}
				if (nk_button_label(gui->ctx, "Cancel")){
					nk_popup_close(gui->ctx);
					show_name = 0;
					tstyle_change = TSTYLE_OP_NONE;
				}
				nk_popup_end(gui->ctx);
			} else {
				show_name = 0;
				tstyle_change = TSTYLE_OP_NONE;
			}
		}
		
	} else {
		show_tstyle_mng = 0;
		tstyle_change = TSTYLE_OP_UPDATE;
	}
	nk_end(gui->ctx);
	
	return show_tstyle_mng;
}