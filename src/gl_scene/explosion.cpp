#include "scene.h"
#include "explosion.h"

#include "explosion_vert.h"
#include "explosion_frag.h"

using namespace std;
using namespace glm;
using namespace ppgso;

// static resources
shared_ptr<Mesh> Explosion::mesh;
shared_ptr<Texture> Explosion::texture;
shared_ptr<Shader> Explosion::shader;

Explosion::Explosion() {
  // Random rotation and momentum
  rotation = vec3{Rand(-PI, PI), Rand(-PI, PI), Rand(-PI, PI)};
  rotMomentum = vec3{Rand(-PI, PI), Rand(-PI, PI), Rand(-PI, PI)}*3.0f;
  speed = vec3{0.0f, 0.0f, 0.0f};

  // Initialize static resources if needed
  if (!shader) shader = make_shared<Shader>(explosion_vert, explosion_frag);
  if (!texture) texture = make_shared<Texture>("explosion.rgb", 512, 512);
  if (!mesh) mesh = make_shared<Mesh>(shader, "asteroid.obj");
}

Explosion::~Explosion() {
}

void Explosion::Render(Scene &scene) {
  shader->Use();

  // Transparency, interpolate from 1.0f -> 0.0f
  shader->SetFloat(1.0f-age/maxAge, "Transparency");

  // use camera
  shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
  shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");

  // render mesh
  shader->SetMatrix(modelMatrix, "ModelMatrix");
  shader->SetTexture(texture, "Texture");

  // Disable depth testing
  glDisable(GL_DEPTH_TEST);

  // Enable blending
  glEnable(GL_BLEND);
  // Additive blending
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  mesh->Render();

  // Disable blending
  glDisable(GL_BLEND);
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
}

bool Explosion::Update(Scene &scene, float dt) {
  // Update scale and rotation
  scale = scale * ( 1.0f + dt * 5.0f);
  rotation += rotMomentum * dt;
  position += speed * dt;

  // Die after reaching maxAge
  age += dt;
  if (age > maxAge) return false;

  GenerateModelMatrix();
  return true;
}
