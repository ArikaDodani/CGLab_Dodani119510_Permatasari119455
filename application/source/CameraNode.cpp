#include "CameraNode.h"
#include "Node.h"


CameraNode::CameraNode()
{
}

CameraNode::~CameraNode()
{
}

bool CameraNode::getPerspective(){
	return isPerspective;
}
bool CameraNode::getEnabled(){

	return isEnabled;
}
void CameraNode::setEnabled(bool Enabled) {
	isEnabled = Enabled;

}
mat4 CameraNode::getProjectionMatrix() {
	
	return projectionMatrix;
}
mat4 CameraNode::setProjectionMatrix(mat4 projection_matrix) {

	projectionMatrix = projection_matrix;
	return projectionMatrix;
}