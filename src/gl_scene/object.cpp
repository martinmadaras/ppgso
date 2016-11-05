#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "object.h"

using namespace std;
using namespace glm;

void Object::GenerateModelMatrix() {
  modelMatrix =
          glm::translate(mat4(1.0f), position)
          * glm::orientate4(rotation)
          * glm::scale(mat4(1.0f), scale);
}

float Object::Rand(float min, float max) {
  return ((max - min) * ((float) rand() / (float) RAND_MAX)) + min;
}
