#include "common.h"

std::queue<Node*> free_queue;

Node::Node() {
	type	= NodeType::TYPE_NODE;
	name	= "Node";
	pos		= Vec3(0,0,0);
	size	= Vec3(1,1,1);
	rot		= Vec4(0,0,0,1);
	model	= 0;
	parent	= 0;
	update_transform();
}

void Node::update_transform() {
	local = translate(pos) * (quat_to_mat4(rot) * scale(size));

	if(parent)
		global = parent->global * local;
	else
		global = local;
	for(u32 i = 0; i < children.size(); i++)
		children.at(i)->update_transform();
}

Vec3 Node::get_pos()			{ return pos; }
Vec3 Node::get_scale()			{ return size; }
Vec4 Node::get_rot()			{ return rot; }
Mat4 Node::get_local()			{ return local; }
Mat4 Node::get_global()			{ return global; }
Node* Node::get_child(u32 idx)	{ return children.at(idx); }
u32 Node::get_child_count()		{ return children.size(); }
NodeType Node::get_type()		{ return type; }

void Node::set_pos(Vec3 p) {
	pos = p;
	update_transform();
}

void Node::set_scale(Vec3 s) {
	size = s;
	update_transform();
}

void Node::set_rot(Vec4 q) {
	rot = q;
	update_transform();
}

void Node::add_child(Node* node) {
	if(!node || node->parent)
		return;
	children.push_back(node);
	node->parent = this;
	node->update_transform();
}

Node* Node::remove_child(u32 idx) {
	if(idx >= children.size())
		return 0;
	Node* child = children[idx];
	children.erase(children.begin() + idx);
	child->parent = 0;
	return child;
}

void Node::queue_to_free() {
	// remove all children of this node and queue to free them.
	while(children.size() > 0)
		remove_child(0)->queue_to_free();

	// remove this node from its parent and queue to free this node
	if(parent != 0)
		for(u32 i = 0; i < parent->children.size(); i++)
			if(parent->children[i] == this) {
				parent->remove_child(i);
				break;
			}
	free_queue.push(this);
}

void Node::free_children() {
	for(u32 i = 0; i < children.size(); i++)
		children[i]->queue_to_free();
}

void node_cleanup() {
	while(free_queue.size() > 0) {
		Node* node = free_queue.front();
		free_queue.pop();
		switch(node->get_type()) {
			case TYPE_NODE:		delete (Node*)node;		break;
			case TYPE_CAMERA:	delete (Camera*)node;	break;
			case TYPE_PLAYER:	delete (Player*)node;	break;
		}
	}
}
