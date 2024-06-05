#include "common.h"

Node* tree_sel_node;
f32 trans_amt = 0.1, rot_amt = 10, scale_amt = 1.;
bool editor_open = true;

void tree_node_buttons(Node* n) {
	ImGui::SameLine();
	ImGui::Text(" (%d)", n->get_id());
	if(n != cur_scene->root) {
		ImGui::SameLine();
		if(ImGui::Button("Del")) {
			if(tree_sel_node == n)
				tree_sel_node = 0;
			n->queue_to_free();
		}
	}
	ImGui::SameLine();
	if(ImGui::Button("Edit"))
		tree_sel_node = n;
}

void transform_buttons(Node* n) {
	const char* tb[] = { "+x", "-x", "+y", "-y", "+z", "-z" };
	const char* rb[] = { "+x##1", "-x##1", "+y##1", "-y##1", "+z##1", "-z##1" };
	const char* sb[] = { "+x##2", "-x##2", "+y##2", "-y##2", "+z##2", "-z##2" };

	ImGui::Text("Translation: %f %f %f", n->get_pos().x, n->get_pos().y, n->get_pos().z);
	for(u32 i = 0; i < 6; i++) {
		f32 v[] = { 0,0,0 };
		v[i / 2] = trans_amt * (i%2 == 0 ? +1 : -1);
		if(ImGui::Button(tb[i]))
			n->set_pos(n->get_pos() + Vec3(v[0],v[1],v[2]));
		if(i < 5) ImGui::SameLine();
	}

	Vec3 e = quat_to_euler(n->get_rot());
	ImGui::Text("Rotation: %f %f %f", e.x, e.y, e.z);
	for(u32 i = 0; i < 6; i++) {
		f32 v[] = { 0,0,0 };
		v[i / 2] = torad(rot_amt * (i%2 == 0 ? +1 : -1));

		if(ImGui::Button(rb[i]))
			n->set_rot(quat_quat(n->get_rot(), euler_to_quat(v)));

		if(i < 5) ImGui::SameLine();
	}

	ImGui::Text("Scale: %f %f %f", n->get_scale().x, n->get_scale().y, n->get_scale().z);
	for(u32 i = 0; i < 6; i++) {
		f32 v[] = { 0,0,0 };
		v[i / 2] = scale_amt * (i%2 == 0 ? +1 : -1);
		if(ImGui::Button(sb[i]))
			n->set_scale(n->get_scale() + Vec3(v[0],v[1],v[2]));
		if(i < 5) ImGui::SameLine();
	}
}

void __tree_traverse(Node* n) {
	if(ImGui::TreeNode(n->name.c_str())) {
		tree_node_buttons(n);
		for(u32 i = 0; i < n->get_child_count(); i++)
			__tree_traverse(n->get_child(i));
		ImGui::TreePop();
	}
}

void editor_ui() {
	ImGui::Begin("Node Editor", &editor_open);

	if(tree_sel_node) {
		ImGui::Text("Selected: %s (%d)", tree_sel_node->name.c_str(), tree_sel_node->get_id());
		transform_buttons(tree_sel_node);
	}

	ImGui::SliderFloat("Translate", &trans_amt, .1, 1);
	ImGui::SliderFloat("Rotate", &rot_amt, .1, 90);
	ImGui::SliderFloat("Scale", &scale_amt, .1, 1);

	ImGui::Text("Scene Tree");
	__tree_traverse(cur_scene->root);

	ImGui::End();
}

void show_editor() {
	editor_open = true;
}

void hide_editor() {
	editor_open = false;
}
