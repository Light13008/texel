#version 330 core
struct Light {
  vec3 ambient;
  vec3 diffuse;

  vec2 direction;
  vec2 position;

  float constant;
  float linear;
  float quadratic;

  float outerCutoff;
  float innerCutoff;
};

out vec4 color;

uniform sampler2D scene;
uniform vec2 resolution;
uniform Light light;

void main() {
  vec2 uv = gl_FragCoord.xy / resolution.xy;
  float aspectRatio = resolution.x / resolution.y;
  vec4 tex = texture(scene, uv);
  vec3 outColor = vec3(0., 0., 0.);
  vec2 lightDir = normalize(light.position - uv);
  float theta = dot(lightDir, -normalize(light.direction));
  // if(theta > cos(light.outerCutoff)) {
  //   color = vec4(vec3(8., 8., 8.) * tex.rgb, 1.);
  // } else {
  //   color = vec4(0.1, 0.1, 0.1, 1.);
  // }
  float innerCutoff = cos(light.innerCutoff);
  float outerCutoff = cos(light.outerCutoff);
  vec3 ambColor = light.ambient * tex.rgb;
  float dist = length(light.position - uv);
  float attenuation = 1.0;
  float epsilon = innerCutoff - outerCutoff;
  float intensity = smoothstep(0., 1., (theta - outerCutoff) / epsilon);
  vec3 diffColor = light.diffuse * intensity * attenuation * tex.rgb;
  outColor = ambColor;
  outColor += diffColor;
  color = vec4(outColor, 1.);
}