/*
 * COMP 3501 A FALL 2020: ASSIGNMENT 3: A small game
 * MODIFIED BY SHARJEEL ALI #101070889
 *
 * A program that demonstrates the world of space and nice RGB ringed planets.
 *
 */

#include <transformation.h>

Transformation::Transformation() {
	PushIdentityMatrix();
	PushMatrix();
}

Transformation::~Transformation() {
}

bool Transformation::Translate(glm::vec3 trans, bool new_matrix) {
	if (new_matrix)PushMatrix();
	transformations.top() = glm::translate(transformations.top(), trans);
	return true;
}

bool Transformation::Rotate(float scale, glm::vec3 normal, bool new_matrix) {
	glm::quat q = glm::angleAxis(scale*glm::pi<float>() / 180.0f, normal);
	return Rotate(q, new_matrix);

}

bool Transformation::Rotate(glm::quat q, bool new_matrix) {
	if (new_matrix)PushMatrix();
	transformations.top() = transformations.top()*glm::mat4_cast(q);

	return true;
}

bool Transformation::Scale(glm::vec3 scale, bool new_matrix) {
	if (new_matrix)PushMatrix();
	transformations.top() = glm::scale(transformations.top(), scale);
	return true;
}

glm::mat4 Transformation::Apply(bool pop_matrix) {
	glm::mat4 trans = transformations.top();
	if (pop_matrix)transformations.pop();
	return trans;
}

bool Transformation::PushMatrix() {
	transformations.push(transformations.top());
	return true;
}

bool Transformation::PushIdentityMatrix() {
	transformations.push(glm::mat4(1.0));
	return true;
}

bool Transformation::PopMatrix() {
	transformations.pop();

	return true;
}
glm::mat4 Transformation::SaveMatrix() {
	return transformations.top();
}