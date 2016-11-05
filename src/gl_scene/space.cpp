#include "space.h"
#include "scene.h"

#include "space_vert.h"
#include "space_frag.h"

using namespace std;
using namespace ppgso;

Space::Space() {
  offset = 0;
  // Z of 1 means back as there is no perspective projection applied during render
  position.z = 1;

  // Initialize static resources if needed
  if (!shader) shader = make_shared<Shader>(space_vert, space_frag);
  if (!texture) texture = make_shared<Texture>("stars.rgb", 512, 512);
  if (!mesh) mesh = make_shared<Mesh>(shader, "quad.obj");
}

Space::~Space() {
}

bool Space::Update(Scene &scene, float dt) {
  // Offset for UV mapping, creates illusion of scrolling
  offset -= dt/5;

  GenerateModelMatrix();
  return true;
}

void Space::Render(Scene &scene) {
  // NOTE: this object does not use camera, just renders the entire quad as is
  shader->Use();

  // Pass UV mapping offset to the shader
  shader->SetFloat(offset, "Offset");

  // Render mesh
  shader->SetMatrix(modelMatrix, "ModelMatrix");
  shader->SetTexture(texture, "Texture");
  mesh->Render();
}

// shared resources
shared_ptr<Mesh> Space::mesh;
shared_ptr<Shader> Space::shader;
shared_ptr<Texture> Space::texture;
